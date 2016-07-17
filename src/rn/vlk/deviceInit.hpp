#pragma once

#include <cassert>
#include <vector>

#include <vulkan/vulkan.h>

#include "context.hpp"

namespace rn {

namespace vlk {

class DeviceInit {
public:
	std::vector<const char *> desiredDeviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	std::vector<const char *> deviceExtensions{};

	void appendDesiredDeviceExtensions(std::vector<const char*> additionalDeviceExtensions) {
		desiredDeviceExtensions.insert(end(desiredDeviceExtensions), begin(additionalDeviceExtensions), end(additionalDeviceExtensions));
	}

	VkResult init(Context &context) {
		return VK_SUCCESS;
	}
};

} // vlk

} // rn
