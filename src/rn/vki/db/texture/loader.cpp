#include "loader.hpp"

#include <optional>

#include "../../../ngn/log.hpp"
#include "../../context.hpp"
#include "../texture.hpp"

namespace rn::vki::db::texture {

Loader::Loader(rn::vki::Context &context, rn::vki::db::Texture &db) :
	db(db),
	reader{context, db},
	transferrer{context, db}
{
	if (forceSynchronusLoading) {
		ngn::log::warn("rn::vki::db::texture::Loader::Loader() => forcing synchronus loading");
	}

	readerThread = std::thread{[&] () {
		while (true) {
			Payload payload{};

			{
				std::unique_lock lock{readerMutex};

				readerCond.wait(lock, [&] () {
					return shouldStop.load() || ! readerPayloads.empty();
				});

				if (shouldStop.load()) {
					db.map([&] () {
						for (auto &payload : readerPayloads) {
							db.entries[payload.index].status = TextureEntryStatus::CANCELLED;
						}
					});

					for (auto &payload : readerPayloads) {
						payload.promise.set_value(false);
					}

					readerPayloads.clear();

					ngn::log::debug("rn::vki::db::texture::Loader::readerThread[] => {} task(s) canceled", readerPayloads.size());

					return;
				}

				payload = std::move(readerPayloads.front());
				readerPayloads.pop_front();
			}

			uint32_t index = payload.index;
			std::string source = payload.source;

			std::optional<Payload> result = reader.run(std::move(payload));

			if (result) {
				ngn::log::debug("rn::vki::db::texture::Loader::readerThread[] => reading <{}> \"{}\" done", index, source);

				{
					std::unique_lock lock{transferrerMutex};

					transferrerPayloads.push_back(std::move(*result));
				}

				transferrerCond.notify_one();

			} else {
				ngn::log::debug("rn::vki::db::texture::Loader::readerThread[] => reading <{}> \"{}\" failed", index, source);
			}
		}
	}};

	transferrerThread = std::thread{[&] () {
		while (true) {
			std::vector<Payload> payloads{};

			{
				std::unique_lock lock{transferrerMutex};

				transferrerCond.wait(lock, [&] () {
					return shouldStop.load() || ! transferrerPayloads.empty();
				});

				if (shouldStop.load()) {
					db.map([&] () {
						for (auto &payload : transferrerPayloads) {
							db.entries[payload.index].status = TextureEntryStatus::CANCELLED;
						}
					});

					for (auto &payload : transferrerPayloads) {
						payload.promise.set_value(false);
					}

					transferrerPayloads.clear();

					ngn::log::debug("rn::vki::db::texture::Loader::transferrerThread[] => {} task(s) canceled", transferrerPayloads.size());

					return;
				}

				ngn::log::debug("rn::vki::db::texture::Loader::transferrerThread[] => got {} task(s)", transferrerPayloads.size());

				payloads.reserve(transferrerPayloads.size());
				for (auto &payload : transferrerPayloads) {
					payloads.push_back(std::move(payload));
				}

				transferrerPayloads.clear();
			}

			transferrer.run(std::move(payloads));
		}
	}};
}

Loader::~Loader() {
	shouldStop.store(true);

	readerCond.notify_one();
	if (readerThread.joinable()) {
		readerThread.join();
	}

	transferrerCond.notify_one();
	if (transferrerThread.joinable()) {
		transferrerThread.join();
	}
}

std::future<bool> Loader::load(Handle &handle) {
	uint32_t index = handle.index;
	std::string source = handle.db->map(index, [] (auto &entry) {
		return entry.source;
	});

	std::promise<bool> promise{};
	std::future<bool> future = promise.get_future();

	Payload payload{
		std::move(index),
		std::move(source),
		std::move(promise),
	};

	if (forceSynchronusLoading) {
		std::optional<Payload> result = reader.run(std::move(payload));

		if (result) {
			std::vector<Payload> payloads{};
			payloads.push_back(std::move(*result));

			transferrer.run(std::move(payloads));
		} else {
			ngn::log::debug("rn::vki::db::texture::Loader::load({}, {}) => transferring canceled", index, source);
		}
	} else {
		uint32_t index = payload.index;
		std::string source = std::string{payload.source};

		if (shouldStop.load()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = TextureEntryStatus::CANCELLED;
			});

			payload.promise.set_value(false);

			ngn::log::debug("rn::vki::db::texture::Loader::load({}, {}) => reading canceled", index, source);
		} else {
			size_t enqueueIndex;
			{
				std::unique_lock lock{readerMutex};

				readerPayloads.push_back(std::move(payload));

				enqueueIndex = readerPayloads.size() - 1;
			}

			readerCond.notify_one();

			ngn::log::debug("rn::vki::db::texture::Loader::load({}, {}) => item {}", index, source, enqueueIndex);
		}
	}

	return future;
}

} // rn::vki::db::texture
