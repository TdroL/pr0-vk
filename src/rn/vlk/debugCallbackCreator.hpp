#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "instanceOwner.hpp"
#include "debugCallbackOwner.hpp"

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
	void* pUserData
);

void initDebugReportCallbackDispatch(vk::Instance &instance);

class DebugCallbackCreator {
public:
	bool enabled = true;
	bool isAvailable = false;

	int logLevel = 0;

	std::vector<std::string> optionalExtensions {
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	std::vector<std::string> optionalLayers {
		"VK_LAYER_LUNARG_standard_validation"
	};

	DebugCallbackOwner create(InstanceOwner &instanceOwner) {
		if ( ! isAvailable) {
			return DebugCallbackOwner{};
		}

		vk::Instance &instance = instanceOwner.handle;

		initDebugReportCallbackDispatch(instance);

		vk::DebugReportFlagsEXT flags{};

		switch (logLevel) {
			case 0: flags = flags | vk::DebugReportFlagBitsEXT::eDebug;
			case 1: flags = flags | vk::DebugReportFlagBitsEXT::eInformation;
			case 2: flags = flags | vk::DebugReportFlagBitsEXT::eWarning;
			case 3: flags = flags | vk::DebugReportFlagBitsEXT::ePerformanceWarning;
			case 4: flags = flags | vk::DebugReportFlagBitsEXT::eError;
		}

		vk::DebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo{};
		debugReportCallbackCreateInfo.flags       = flags;
		debugReportCallbackCreateInfo.pfnCallback = debugCallback;

		return DebugCallbackOwner{instance.createDebugReportCallbackEXT(debugReportCallbackCreateInfo), instance};
	}
};

} // vlk

} // rn
