#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <future>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../memory/handle.hpp"
#include "payload.hpp"

namespace rn::vlk {

class Context;

} // rn::vlk

namespace rn::vlk::db {

class Mesh;

} // rn::vlk::db

namespace rn::vlk::db::mesh {

class Transferrer {
public:
	std::thread workerThread{};
	std::mutex tasksMutex{};
	std::condition_variable tasksCond{};
	std::atomic<bool> shouldStop{false};
	std::deque<Payload> tasks{};

	vk::UniqueCommandPool commandPool{};
	bool forceSynchronusLoading = ngn::config::core.db.mesh.forceSynchronusLoading();

	rn::vlk::Context &context;
	rn::vlk::db::Mesh &db;

	explicit Transferrer(rn::vlk::Context &context, rn::vlk::db::Mesh &db);

	~Transferrer();

	void run(std::vector<Payload> &&payloads);

	void transfer(Payload &&payload);
};

} // rn::vlk::db::mesh
