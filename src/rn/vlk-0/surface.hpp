#pragma once

#include <vulkan/vulkan.h>

namespace rn {

namespace vlk {

class Context;

class Surface {
public:
	VkSurfaceKHR handle = VK_NULL_HANDLE;
	VkExtent2D extent{};
	VkSurfaceFormatKHR format{};
};

} // vlk

} // rn