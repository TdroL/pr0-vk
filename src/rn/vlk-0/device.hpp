#pragma once

#include <vulkan/vulkan.h>

namespace rn {

namespace vlk {

class Context;

class Device {
public:
	VkDevice handle = VK_NULL_HANDLE;
};

} // vlk

} // rn