#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

#include <vulkan/vulkan.hpp>
// #include "../instanceOwner.hpp"
// #include "../debugCallbackOwner.hpp"
#include "../exts/debugCallbackDispatchTable.hpp"

#include "../../../ngn/config.hpp"

namespace rn {

namespace vlk {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char *pLayerPrefix,
	const char *pMessage,
	void *pUserData
);

class DebugCallbackCreator {
public:
	bool enabled = true;
	bool isAvailable = false;

	int initialLogLevel = 0;

	std::vector<std::string> optionalExtensions {
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	std::vector<std::string> optionalLayers {
		"VK_LAYER_LUNARG_standard_validation"
	};

	vk::UniqueDebugReportCallbackEXT create(vk::UniqueInstance &instanceOwner) {
		if ( ! isAvailable) {
			return vk::UniqueDebugReportCallbackEXT{};
		}

		vk::Instance instance = instanceOwner.get();

		assert(instance);

		initDebugCallbackDispatchTable(instance);

		vk::DebugReportFlagsEXT flags{};

		switch (ngn::config::core.debug.vulkanLogLevel()) {
			case 0: flags = flags | vk::DebugReportFlagBitsEXT::eDebug;
			case 1: flags = flags | vk::DebugReportFlagBitsEXT::eInformation;
			case 2: flags = flags | vk::DebugReportFlagBitsEXT::eWarning;
			case 3: flags = flags | vk::DebugReportFlagBitsEXT::ePerformanceWarning;
			case 4: flags = flags | vk::DebugReportFlagBitsEXT::eError;
		}

		vk::DebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo{};
		debugReportCallbackCreateInfo.flags       = flags;
		debugReportCallbackCreateInfo.pfnCallback = debugCallback;

		vk::UniqueDebugReportCallbackEXT debugCallbackOwner{instance.createDebugReportCallbackEXTUnique(debugReportCallbackCreateInfo)};

		if ( ! debugCallbackOwner) {
			throw std::runtime_error{"Vulkan debug callback could not be created"};
		}

		return debugCallbackOwner;
	}
};

} // vlk

} // rn
