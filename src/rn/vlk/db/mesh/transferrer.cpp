#include "reader.hpp"

#include <algorithm>

#include "../../../../ngn/log.hpp"
#include "../../../../ngn/prof.hpp"
#include "../../../../util/implode.hpp"
#include "../../../../util/map.hpp"
#include "../../mapping.hpp"
#include "../../context.hpp"
#include "../mesh.hpp"

namespace rn::vlk::db::mesh {

Transferrer::Transferrer(rn::vlk::Context &context, rn::vlk::db::Mesh &db) :
	context(context),
	db(db)
{
	vk::CommandPoolCreateInfo commandPoolCreateInfo{
		/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
		/*.queueFamilyIndex=*/ context.family.transfer,
	};
	commandPool = context.device.createCommandPoolUnique(commandPoolCreateInfo);

	workerThread = std::thread{[&] () {
		while (true) {
			std::vector<Payload> payloads{};

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

					ngn::log::debug("rn::vlk::db::mesh::Transferrer::workerThread[] => {} tasks cancelled", tasks.size());

					return;
				}

				ngn::log::debug("rn::vlk::db::mesh::Transferrer::workerThread[] => got {} tasks", tasks.size());

				payloads.reserve(tasks.size());
				for (auto &task : tasks) {
					payloads.push_back(std::move(task));
				}

				tasks.clear();
			}

			run(std::move(payloads));
		}
	}};
}

Transferrer::~Transferrer() {
	shouldStop.store(true);
	tasksCond.notify_one();

	if (workerThread.joinable()) {
		workerThread.join();
	}
}

void Transferrer::run(std::vector<Payload> &&payloads) {
	try {
		NGN_PROF_SCOPE("Mesh::Transferrer");

		context.device.resetCommandPool(commandPool.get(), vk::CommandPoolResetFlagBits::eReleaseResources);

		// std::string jobNames = util::implode(util::map(jobs, [] (const JobData &job) {
		// 	return "{" + std::to_string(job.payload.index) + "} \"" + std::string{job.source} + "\"";
		// }));
		// ngn::log::debug("rn::vlk::db::mesh::Transferrer::workerThread[{}] => transferring {} meshes", jobNames, jobs.size());

		/*
		work
		*/
	} catch (std::runtime_error const &e) {
		ngn::log::debug("rn::vlk::db::mesh::Transferrer::workerThread[?] => error {}", e.what());

		db.map([&] () {
			for (auto &payload : payloads) {
				db.entries[payload.index].status = MeshEntryStatus::TRANSFERRER_FAILED;
			}
		});

		for (auto &payload : payloads) {
			payload.promise.set_value(false);
		}
	} catch (...) {
		ngn::log::debug("rn::vlk::db::mesh::Transferrer::workerThread[?] => unknown error");

		db.map([&] () {
			for (auto &payload : payloads) {
				db.entries[payload.index].status = MeshEntryStatus::TRANSFERRER_FAILED;
			}
		});

		for (auto &payload : payloads) {
			payload.promise.set_value(false);
		}
	}
}

void Transferrer::transfer(Payload &&payload) {
	uint32_t index = payload.index;
	std::string source = std::string{payload.source};

	{
		std::unique_lock lock{tasksMutex};

		if (shouldStop.load()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = MeshEntryStatus::CANCELLED;
			});

			payload.promise.set_value(false);

			ngn::log::debug("rn::vlk::db::mesh::Transferrer::transfer({}, {}) => cancelled", index, source);

			return;
		}

		if (forceSynchronusLoading) {
			lock.unlock();

			std::vector<Payload> payloads{};
			payloads.push_back(std::move(payload));
			run(std::move(payloads));

			return;
		}

		tasks.push_back(std::move(payload));

		ngn::log::debug("rn::vlk::db::mesh::Transferrer::transfer({}, {}) => item {}", index, source, tasks.size() - 1);
	}

	tasksCond.notify_one();
}

} // rn::vlk::db::mesh
