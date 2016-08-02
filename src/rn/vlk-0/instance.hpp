#pragma once

#include <vulkan/vulkan.h>

namespace rn {

namespace vlk {

class Context;

class Instance {
public:
	Context *context = nullptr;
	VkInstance handle = VK_NULL_HANDLE;

	VkDebugReportCallbackEXT debugReportCallback = VK_NULL_HANDLE;
};

} // vlk

} // rn