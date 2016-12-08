#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class DebugCallbackOwner {
public:
	vk::DebugReportCallbackEXT handle{};
	vk::Instance instance{};

	DebugCallbackOwner() = default;
	DebugCallbackOwner(vk::DebugReportCallbackEXT &&debugCallback, const vk::Instance &instance)
		: handle{std::move(debugCallback)}, instance{instance} {
		debugCallback = vk::DebugReportCallbackEXT{};
	}
	DebugCallbackOwner(DebugCallbackOwner &&rhs)
		: handle{std::move(rhs.handle)}, instance{rhs.instance} {
		rhs.handle = vk::DebugReportCallbackEXT{};
	}

	DebugCallbackOwner & operator=(DebugCallbackOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		instance = rhs.instance;

		rhs.handle = vk::DebugReportCallbackEXT{};
		rhs.instance = vk::Instance{};

		return *this;
	}

	void destroy() {
		if (handle) {
			instance.destroyDebugReportCallbackEXT(handle);

			handle = vk::DebugReportCallbackEXT{};
			instance = vk::Instance{};
		}
	}

	DebugCallbackOwner(const DebugCallbackOwner &rhs) = delete;
	DebugCallbackOwner & operator=(const DebugCallbackOwner &rhs) = delete;

	~DebugCallbackOwner() {
		destroy();
	}
};

} // vlk

} // rn
