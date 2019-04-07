#include "debugCreator.hpp"

#include <cctype>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../../../util/contains.hpp"
#include "../../../util/join.hpp"
#include "../../../util/map.hpp"
#include "../dispatch.hpp"
#include "../trace.hpp"

namespace rn::vki::context {

bool isSpecIdentifier(const char *text) {
	if (!text || *text == '\0') {
		return false;
	}

	while (*text != '\0') {
		if (!(std::isupper(*text) || std::isdigit(*text) || *text == '_')) {
			return false;
		}

		text++;
	}

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(VkDebugReportFlagsEXT flags, [[maybe_unused]] VkDebugReportObjectTypeEXT objectType, [[maybe_unused]] uint64_t object, [[maybe_unused]] size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, [[maybe_unused]] void *pUserData) {
	std::stringstream ss;

	if ((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)) {
		ss << "(INFORMATION) ";
	}

	if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)) {
		ss << "(WARNING) ";
	}

	if ((flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)) {
		ss << "(PERFORMANCE WARNING) ";
	}

	if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)) {
		ss << "(ERROR) ";
	}

	if ((flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)) {
		ss << "(DEBUG) ";
	}

	ss << "{" << pLayerPrefix << ":" << messageCode << "}: " << pMessage;

	const auto &lastCall = rn::vki::traceLastCall();
	ngn::log::debug("rn::vki::context::DebugCreator::debugReportCallback() => {} | Last call [{}:{}]: {}", ss.str(), std::string{lastCall.file}, lastCall.line, std::string{lastCall.code});

	return VK_FALSE;
}

VkBool32 debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, [[maybe_unused]] void *pUserData) {
	std::stringstream ss;
	ss << "(";

	if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)) {
		ss << "ERROR";
	} else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)) {
		ss << "WARNING";
	} else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)) {
		ss << "INFO";
	} else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)) {
		ss << "LOG";
	}

	std::vector<std::string_view> types{};

	if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)) {
		types.push_back("GENERAL");
	}
	if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)) {
		types.push_back("VALIDATION");
	}
	if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)) {
		types.push_back("PERFORMANCE");
	}

	if (types.empty()) {
		types.push_back("UNKNOWN");
	}

	ss << " " << util::join(types, " | ") << ") ";

	ss << "[" << pCallbackData->messageIdNumber << ":" << pCallbackData->pMessageIdName << "]: " << pCallbackData->pMessage;
	if (isSpecIdentifier(pCallbackData->pMessageIdName)) {
		ss << " (spec: https://www.khronos.org/registry/vulkan/specs/1.1/html/vkspec.html#" << pCallbackData->pMessageIdName << ")";
	}

	const auto &lastCall = rn::vki::traceLastCall();
	ngn::log::debug("rn::vki::context::DebugCreator::debugUtilsMessengerCallback() => {} | Last call [{}:{}]: {}", ss.str(), std::string{lastCall.file}, lastCall.line, std::string{lastCall.code});

	return VK_FALSE;
}

rn::vki::context::Debug DebugCreator::create(rn::vki::HandleInstance &&instance, ngn::config::Config &config) {
	std::vector<std::string> availableExtensions = util::map(RN_VKI_TRACE(vk::enumerateInstanceExtensionProperties()), [] (auto &properties) {
		return std::string{properties.extensionName};
	});

	if (util::contains(availableExtensions, "VK_EXT_debug_utils")) {
		ngn::log::debug("rn::vki::context::DebugCreator::create() => using VK_EXT_debug_utils");

		vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity{};

		switch (config.core.debug.vki.logLevel) {
			case 0: {
				messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
				[[fallthrough]];
			}
			case 1: {
				messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
				[[fallthrough]];
			}
			case 2: {
				[[fallthrough]];
			}
			case 3: {
				messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
				[[fallthrough]];
			}
			case 4: {
				messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
			}
		}

		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
		debugUtilsMessengerCreateInfo.messageSeverity = messageSeverity;
		debugUtilsMessengerCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
		debugUtilsMessengerCreateInfo.pfnUserCallback = debugUtilsMessengerCallback;
		debugUtilsMessengerCreateInfo.pUserData = &config;

		return {
			/*.debugReportCallback=*/ {},
			/*.debugUtilsMessenger=*/ {
				RN_VKI_TRACE(instance->createDebugUtilsMessengerEXTUnique(debugUtilsMessengerCreateInfo, nullptr, instance.table())),
				instance.table()
			},
		};
	} else if (util::contains(availableExtensions, "VK_EXT_debug_report")) {
		ngn::log::debug("rn::vki::context::DebugCreator::create() => using VK_EXT_debug_report");

		vk::DebugReportFlagsEXT flags{};

		switch (config.core.debug.vki.logLevel) {
			case 0: {
				flags |= vk::DebugReportFlagBitsEXT::eDebug;
				[[fallthrough]];
			}
			case 1: {
				flags |= vk::DebugReportFlagBitsEXT::eInformation;
				[[fallthrough]];
			}
			case 2: {
				flags |= vk::DebugReportFlagBitsEXT::eWarning;
				[[fallthrough]];
			}
			case 3: {
				flags |= vk::DebugReportFlagBitsEXT::ePerformanceWarning;
				[[fallthrough]];
			}
			case 4: {
				flags |= vk::DebugReportFlagBitsEXT::eError;
			}
		}

		vk::DebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo{};
		debugReportCallbackCreateInfo.flags = flags;
		debugReportCallbackCreateInfo.pfnCallback = debugReportCallback;
		debugReportCallbackCreateInfo.pUserData = &config;

		return {
			/*.debugReportCallback=*/ {
				RN_VKI_TRACE(instance->createDebugReportCallbackEXTUnique(debugReportCallbackCreateInfo, nullptr, instance.table())),
				instance.table()
			},
			/*.debugUtilsMessenger=*/ {},
		};
	} else {
		return {};
	}
}

std::vector<std::string> DebugCreator::selectLayers(ngn::config::Config &config, std::vector<std::string> &availableLayers, std::vector<std::string> selectedLayers) {
	if (util::contains(availableLayers, "VK_LAYER_LUNARG_standard_validation")) {
		selectedLayers.push_back("VK_LAYER_LUNARG_standard_validation");

		if (config.core.debug.vki.useRenderDoc) {
			if (util::contains(availableLayers, "VK_LAYER_RENDERDOC_Capture")) {
				selectedLayers.push_back("VK_LAYER_RENDERDOC_Capture");
			} else {
				ngn::log::warn("rn::vki::context::DebugCreator::selectLayers() => Vulkan debugging might be limited, missing \"{}\" layer", "VK_LAYER_RENDERDOC_Capture");
			}
		}
	} else {
		ngn::log::warn("rn::vki::context::DebugCreator::selectLayers() => Vulkan debugging disabled, missing \"{}\" layer", "VK_LAYER_LUNARG_standard_validation");
	}

	return selectedLayers;
}

std::vector<std::string> DebugCreator::selectExtensions([[maybe_unused]] ngn::config::Config &config, std::vector<std::string> &availableExtensions, std::vector<std::string> selectedExtensions) {
	if (util::contains(availableExtensions, "VK_EXT_debug_utils")) {
		selectedExtensions.push_back("VK_EXT_debug_utils");
	} else if (util::contains(availableExtensions, "VK_EXT_debug_report")) {
		selectedExtensions.push_back("VK_EXT_debug_report");
	} else {
		ngn::log::warn("rn::vki::context::DebugCreator::selectExtensions() => Vulkan debugging disabled, missing \"{}\" extension", "VK_EXT_debug_report");
	}

	return selectedExtensions;
}

} // rn::vki::context
