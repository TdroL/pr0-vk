#include "reader.hpp"

#include <algorithm>
#include <cctype>

#include "../../../../ngn/fs.hpp"
#include "../../../../ngn/log.hpp"
#include "../../../../ngn/prof.hpp"
#include "../../context.hpp"
#include "../mesh.hpp"

namespace rn::vlk::db::mesh {

Reader::Reader(rn::vlk::Context &context, rn::vlk::db::Mesh &db) :
	context(context),
	db(db)
{
	workerThread = std::thread{[&] () {
		while (true) {
			Payload payload{};

			{
				std::unique_lock lock{tasksMutex};

				tasksCond.wait(lock, [&] () {
					return shouldStop.load() || ! tasks.empty();
				});


				if (shouldStop.load()) {
					if (tasks.empty()) {
						return;
					}

					db.map([&] () {
						for (auto &payload : tasks) {
							db.entries[payload.index].status = MeshEntryStatus::CANCELLED;
						}
					});

					for (auto &payload : tasks) {
						payload.promise.set_value(false);
					}

					ngn::log::debug("rn::vlk::db::mesh::Reader::workerThread[] => {} tasks cancelled", tasks.size());

					return;
				}

				payload = std::move(tasks.front());
				tasks.pop_front();
			}

			run(std::move(payload));
		}
	}};
}

Reader::~Reader() {
	shouldStop.store(true);
	tasksCond.notify_one();

	if (workerThread.joinable()) {
		workerThread.join();
	}
}

void Reader::run(Payload &&payload) {
	try {
		NGN_PROF_SCOPE("Texture::Reader");

		/*
		work
		*/
	} catch (std::runtime_error const &e) {
		ngn::log::debug("rn::vlk::db::mesh::Reader::workerThread[?] => error {}", e.what());

		if ( ! payload.source.empty()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = MeshEntryStatus::READER_FAILED;
			});

			payload.promise.set_value(false);
		}
	} catch (...) {
		ngn::log::debug("rn::vlk::db::mesh::Reader::workerThread[?] => unknown error");

		if ( ! payload.source.empty()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = MeshEntryStatus::READER_FAILED;
			});

			payload.promise.set_value(false);
		}
	}
}

void Reader::read(Payload &&payload) {
	uint32_t index = payload.index;
	std::string source = std::string{payload.source};

	{
		std::unique_lock lock{tasksMutex};

		if (shouldStop.load()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = MeshEntryStatus::CANCELLED;
			});

			payload.promise.set_value(false);

			ngn::log::debug("rn::vlk::db::mesh::Reader::read({}, {}) => cancelled", index, source);

			return;
		}

		if (forceSynchronusLoading) {
			lock.unlock();

			run(std::move(payload));

			return;
		}

		tasks.push_back(std::move(payload));

		ngn::log::debug("rn::vlk::db::mesh::Reader::read({}, {}) => item {}", index, source, tasks.size() - 1);
	}

	tasksCond.notify_one();
}

} // rn::vlk::db::mesh
