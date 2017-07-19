#include "debugCallbackCreator.hpp"

#include <sstream>

// #include "../instanceOwner.hpp"
#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"

namespace rn {

namespace vlk {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void */*pUserData*/) {

	int logLevel = ngn::config::core.debug.vulkanLogLevel();

	switch (logLevel) {
		case 4:
			if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
				return VK_FALSE;
			}
			[[fallthrough]];
		case 3:
			if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
				return VK_FALSE;
			}
			[[fallthrough]];
		case 2:
			if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
				return VK_FALSE;
			}
			[[fallthrough]];
		case 1:
			if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
				return VK_FALSE;
			}
			[[fallthrough]];
		default: {
			if (logLevel > 4) {
				return VK_FALSE;
			}
		}
	}

	std::stringstream ss;

	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		ss << "(INFORMATION) ";
	}

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		ss << "(WARNING) ";
	}

	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		ss << "(PERFORMANCE WARNING) ";
	}

	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		ss << "(ERROR) ";
	}

	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		ss << "(DEBUG) ";
	}

	ss << "{" << pLayerPrefix << ":" << messageCode << "}: " << pMessage;

	ngn::log::debug("Vulkan: {}", ss.str());

	return VK_FALSE;
}

} // vlk

} // rn
