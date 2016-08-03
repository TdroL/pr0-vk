#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class DebugCallbackOwner {
public:
	vk::DebugReportCallbackEXT handle{};

	DebugCallbackOwner() = default;
	explicit DebugCallbackOwner(vk::DebugReportCallbackEXT &&debugCallback)
		: handle{std::move(debugCallback)} {
		debugCallback = vk::DebugReportCallbackEXT{};
		if (handle) {
			std::cout << "creating vk::DebugCallback" << std::endl;
		}
	}
	DebugCallbackOwner(DebugCallbackOwner &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = vk::DebugReportCallbackEXT{};
		if (handle) {
			std::cout << "creating vk::DebugCallback" << std::endl;
		}
	}

	DebugCallbackOwner & operator=(DebugCallbackOwner &&rhs) {
		handle = std::move(rhs.handle);
		rhs.handle = vk::DebugReportCallbackEXT{};

		return *this;
	}

	DebugCallbackOwner(const DebugCallbackOwner &rhs) = delete;
	DebugCallbackOwner & operator=(const DebugCallbackOwner &rhs) = delete;

	~DebugCallbackOwner() {
		if (handle) {
			std::cout << "destroying vk::DebugCallback" << std::endl;
		}
	}
};

} // vlk

} // rn
