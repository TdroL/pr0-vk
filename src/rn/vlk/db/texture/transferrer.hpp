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

class Texture;

} // rn::vlk::db

namespace rn::vlk::db::texture {

class Transferrer {
public:
	enum class TransitionType {
		UNDEFINED_TO_TRANSFERABLE,
		TRANSFERABLE_TO_READABLE,
	};

	struct ImageData {
		vk::Image image{};
		uint32_t levels;
		uint32_t layers;
	};

	std::thread workerThread{};
	std::mutex tasksMutex{};
	std::condition_variable tasksCond{};
	std::atomic<bool> shouldStop{false};
	std::deque<Payload> tasks{};

	vk::UniqueCommandPool commandPool{};
	bool forceSynchronusLoading = ngn::config::core.db.texture.forceSynchronusLoading();

	rn::vlk::Context &context;
	rn::vlk::db::Texture &db;

	explicit Transferrer(rn::vlk::Context &context, rn::vlk::db::Texture &db);

	~Transferrer();

	void run(std::vector<Payload> &&payloads);

	void transfer(Payload &&payload);

	void transitionImages(TransitionType type, vk::CommandBuffer commandBuffer, const std::vector<ImageData> &imageDatas);
};

} // rn::vlk::db::texture
