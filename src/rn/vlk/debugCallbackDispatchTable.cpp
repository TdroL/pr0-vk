#include "debugCallbackCreator.hpp"
#include "instanceOwner.hpp"
#include "../../ngn/log.hpp"

#include <unordered_map>

namespace rn {

namespace vlk {

struct DebugCallbackDispatchTableCounter {
	size_t refCount = 0;
	DebugCallbackDispatchTable dispatch{};
};

std::unordered_map<VkInstance, DebugCallbackDispatchTableCounter> dispatchCountners{};

DebugCallbackDispatchTable loadDebugCallbackDispatchTableCounter(VkInstance instance) {
	if ( ! instance) {
		throw std::runtime_error{"rn::vlk::debugCallbackDispatchTable/loadDebugCallbackDispatchTableCounter() - instance is null"};
	}

	DebugCallbackDispatchTable dispatch{};
	#define LOAD(name) do { \
		dispatch.name = reinterpret_cast<PFN_name>(vkGetInstanceProcAddr(instance, #name)); \
		if (dispatch.name == nullptr) { \
			throw std::runtime_error{"rn::vlk::debugCallbackDispatchTable/loadDebugCallbackDispatchTableCounter() - function " #name " not available"}; \
		} \
	} while (false)

	LOAD(vkCreateDebugReportCallbackEXT);
	LOAD(vkDestroyDebugReportCallbackEXT);
	LOAD(vkDebugReportMessageEXT);
	#undef LOAD

	return dispatch;
}

void initDebugCallbackDispatchTable(vk::Instance &instance) {

}

void deinitDebugCallbackDispatchTable(vk::Instance &instance) {

}

DebugCallbackDispatchTable & getDebugCallbackDispatchTable(vk::Instance &instance) {
	return DebugCallbackDispatchTable{};
}

} // vlk

} // rn

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
	VkInstance                                  instance,
	const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDebugReportCallbackEXT*                   pCallback) {
	const rn::vlk::DebugCallbackDispatchTable &dispatch = rn::vlk::getDebugCallbackDispatchTable(instance);
	return dispatch.vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
	VkInstance                                  instance,
	VkDebugReportCallbackEXT                    callback,
	const VkAllocationCallbacks*                pAllocator) {
	const rn::vlk::DebugCallbackDispatchTable &dispatch = rn::vlk::getDebugCallbackDispatchTable(instance);
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
	const rn::vlk::DebugCallbackDispatchTable &dispatch = rn::vlk::getDebugCallbackDispatchTable(instance);
	dispatch.vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}