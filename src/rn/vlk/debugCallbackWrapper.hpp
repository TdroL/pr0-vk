#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class DebugCallbackWrapper {
public:
	vk::DebugReportCallbackEXT handle{};

	DebugCallbackWrapper() = default;
	explicit DebugCallbackWrapper(vk::DebugReportCallbackEXT &&debugCallback)
		: handle{std::move(debugCallback)} {
		debugCallback = vk::DebugReportCallbackEXT{};
	}
	DebugCallbackWrapper(DebugCallbackWrapper &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = vk::DebugReportCallbackEXT{};
	}

	DebugCallbackWrapper & operator=(DebugCallbackWrapper &&rhs) {
		handle = std::move(rhs.handle);
		rhs.handle = vk::DebugReportCallbackEXT{};

		return *this;
	}

	DebugCallbackWrapper(const DebugCallbackWrapper &rhs) = delete;
	DebugCallbackWrapper & operator=(const DebugCallbackWrapper &rhs) = delete;

	~DebugCallbackWrapper() {
		if (handle) {
			std::cout << "destroying vk::DebugCallback" << std::endl;
		}
	}
};

} // vlk

} // rn
