#include "debugCallbackCreator.hpp"

#include <sstream>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../trace.hpp"

namespace rn::vlk {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, [[maybe_unused]] VkDebugReportObjectTypeEXT objectType, [[maybe_unused]] uint64_t object, [[maybe_unused]] size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, [[maybe_unused]] void *pUserData) {

	int logLevel = ngn::config::core.debug.vk.logLevel();

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

	ngn::log::debug("Vulkan: {} | Last call [{}:{}]: {}", ss.str(), std::string{rn::vlk::lastCall().file}, rn::vlk::lastCall().line, std::string{rn::vlk::lastCall().code});

	return VK_FALSE;
}

} // rn::vlk
