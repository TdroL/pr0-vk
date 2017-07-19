#include "debugCallbackDispatchTable.hpp"

#include "../../../ngn/log.hpp"

#include <unordered_map>
#include <sstream>

namespace rn {

namespace vlk {

struct DebugCallbackDispatchTableCounted {
	size_t refCount = 0;
	DebugCallbackDispatchTable dispatch{};
};

std::unordered_map<VkInstance, DebugCallbackDispatchTableCounted> dispatchMap{};

DebugCallbackDispatchTable loadDebugCallbackDispatchTable(const vk::Instance &instance) {
	if ( ! instance) {
		throw std::runtime_error{"rn::vlk::debugCallbackDispatchTable/loadDebugCallbackDispatchTable() - missing required instance"};
	}

	DebugCallbackDispatchTable dispatch{};
	#define LOAD(name) do { \
		dispatch.name = reinterpret_cast<PFN_##name>(instance.getProcAddr(#name)); \
		if (dispatch.name == nullptr) { \
			throw std::runtime_error{"rn::vlk::debugCallbackDispatchTable/loadDebugCallbackDispatchTable() - function " #name " not available"}; \
		} \
	} while (false)

	LOAD(vkCreateDebugReportCallbackEXT);
	LOAD(vkDestroyDebugReportCallbackEXT);
	LOAD(vkDebugReportMessageEXT);
	#undef LOAD

	return dispatch;
}

size_t initDebugCallbackDispatchTable(const vk::Instance &instance) {
	auto it = dispatchMap.find(instance);

	if (it != std::end(dispatchMap)) {
		it->second.refCount++;
		return it->second.refCount;
	}

	DebugCallbackDispatchTableCounted entry {
		1,
		loadDebugCallbackDispatchTable(instance)
	};

	dispatchMap.insert_or_assign(instance, std::move(entry));

	return entry.refCount;
}

size_t releaseDebugCallbackDispatchTable(const vk::Instance &instance) {
	auto it = dispatchMap.find(instance);

	if (it == std::end(dispatchMap)) {
		return 0;
	}

	it->second.refCount--;

	size_t refCount = it->second.refCount;

	if ( ! refCount) {
		dispatchMap.erase(it);
	}

	return refCount;
}

DebugCallbackDispatchTable & getDebugCallbackDispatchTable(const vk::Instance &instance) {
	auto it = dispatchMap.find(instance);

	if (it == std::end(dispatchMap)) {
		initDebugCallbackDispatchTable(instance);

		it = dispatchMap.find(instance);
	}

	return it->second.dispatch;
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
