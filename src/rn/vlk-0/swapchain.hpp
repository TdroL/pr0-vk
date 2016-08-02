#pragma once

#include <vulkan/vulkan.h>

namespace rn {

namespace vlk {

class Context;

class Swapchain {
public:
	VkSwapchainKHR handle = VK_NULL_HANDLE;
};

} // vlk

} // rn