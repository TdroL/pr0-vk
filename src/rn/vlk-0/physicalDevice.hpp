#pragma once

#include <vulkan/vulkan.h>

namespace rn {

namespace vlk {

class Context;

class PhysicalDevice {
public:
	VkPhysicalDevice handle = VK_NULL_HANDLE;
	VkPhysicalDeviceMemoryProperties memoryProperties{};
};

} // vlk

} // rn