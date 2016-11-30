#include "debugCallbackCreator.hpp"
#include "instanceOwner.hpp"
#include "../../ngn/log.hpp"

#include <unordered_map>
#include <sstream>

struct VkDebugDispatchTable {
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{};
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT{};
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT{};
};

std::unordered_map<VkInstance, VkDebugDispatchTable> dispatchTables{};

ngn::Listener<rn::vlk::EventInstanceDestroy> eventInstanceDestroyListener{[&] (const rn::vlk::EventInstanceDestroy &event) {
	dispatchTables.erase(static_cast<VkInstance>(event.instance));
}};

const VkDebugDispatchTable & loaderGetInstanceDispatch(VkInstance instance) {
	if ( ! instance) {
		throw std::runtime_error{"rn::vlk::debugCallbackCreator/loaderGetInstanceDispatch() - instance is null"};
	}

	auto found = dispatchTables.find(instance);

	if (found != std::end(dispatchTables)) {
		return found->second;
	}

	auto &dispatch = dispatchTables.emplace(instance, VkDebugDispatchTable{}).first->second;
	dispatch.vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
	dispatch.vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
	dispatch.vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT"));

	if (dispatch.vkCreateDebugReportCallbackEXT == nullptr) {
		throw std::runtime_error{"vkCreateDebugReportCallbackEXT not available"};
	}

	if (dispatch.vkDestroyDebugReportCallbackEXT == nullptr) {
		throw std::runtime_error{"vkDestroyDebugReportCallbackEXT not available"};
	}

	if (dispatch.vkDebugReportMessageEXT == nullptr) {
		throw std::runtime_error{"vkDebugReportMessageEXT not available"};
	}

	return dispatch;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
	VkInstance                                  instance,
	const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDebugReportCallbackEXT*                   pCallback) {
	const VkDebugDispatchTable &dispatch = loaderGetInstanceDispatch(instance);
	return dispatch.vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
	VkInstance                                  instance,
	VkDebugReportCallbackEXT                    callback,
	const VkAllocationCallbacks*                pAllocator) {
	const VkDebugDispatchTable &dispatch = loaderGetInstanceDispatch(instance);
	dispatch.vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(
	VkInstance                                  instance,
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage) {
	const VkDebugDispatchTable &dispatch = loaderGetInstanceDispatch(instance);
	dispatch.vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

namespace rn {

namespace vlk {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void* /*pUserData*/) {
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

void initDebugReportCallbackDispatch(vk::Instance &instance) {
	loaderGetInstanceDispatch(instance);
}

} // vlk

} // rn