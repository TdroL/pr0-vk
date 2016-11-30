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
	explicit DebugCallbackOwner(const vk::Instance &instance, vk::DebugReportCallbackEXT &&debugCallback)
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

		return *this;
	}

	void destroy() {
		if (handle) {
			instance.destroyDebugReportCallbackEXT(handle);
			handle = vk::DebugReportCallbackEXT{};
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
