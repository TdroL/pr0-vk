#pragma once

#include <vulkan/vulkan.h>

#include "instanceWrap.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	Instance instance{};

	// VkInstance instance = VK_NULL_HANDLE;

	// VkDevice device = VK_NULL_HANDLE;

	// VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	// VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};

	// VkDebugReportCallbackEXT debugReportCallback = VK_NULL_HANDLE;
};

} // vlk

} // rn
