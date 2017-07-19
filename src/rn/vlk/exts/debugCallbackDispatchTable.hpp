#pragma once

#include <vulkan/vulkan.hpp>

// #include "../instanceOwner.hpp"

namespace rn {

namespace vlk {

struct DebugCallbackDispatchTable {
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{};
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT{};
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT{};
};

size_t initDebugCallbackDispatchTable(const vk::Instance &instance);
size_t releaseDebugCallbackDispatchTable(const vk::Instance &instance);
DebugCallbackDispatchTable & getDebugCallbackDispatchTable(const vk::Instance &instance);

} // vlk

} // rn
