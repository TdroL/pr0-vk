#include "extensionsStaticLoader.hpp"

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

struct ExtensionDispatchTable {
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
	PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
	PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
	PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
	PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
	PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
	PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
	PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
	PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
	PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
	PFN_vkCmdProcessCommandsNVX vkCmdProcessCommandsNVX;
	PFN_vkCmdReserveSpaceForCommandsNVX vkCmdReserveSpaceForCommandsNVX;
	PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
	PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
	PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
	PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
#ifdef VK_USE_PLATFORM_IOS_MVK
	PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif /*VK_USE_PLATFORM_IOS_MVK*/
	PFN_vkCreateIndirectCommandsLayoutNVX vkCreateIndirectCommandsLayoutNVX;
#ifdef VK_USE_PLATFORM_MACOS_MVK
	PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif /*VK_USE_PLATFORM_MACOS_MVK*/
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
	PFN_vkCreateObjectTableNVX vkCreateObjectTableNVX;
	PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
#ifdef VK_USE_PLATFORM_VI_NN
	PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif /*VK_USE_PLATFORM_VI_NN*/
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
	PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
	PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
	PFN_vkDestroyIndirectCommandsLayoutNVX vkDestroyIndirectCommandsLayoutNVX;
	PFN_vkDestroyObjectTableNVX vkDestroyObjectTableNVX;
	PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
	PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
	PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_NV
	PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /*VK_USE_PLATFORM_WIN32_NV*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
	PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
	PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX;
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
	PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
	PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
	PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
	PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
	PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
	PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
	PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
	PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
	PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
	PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
	PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
	PFN_vkRegisterObjectsNVX vkRegisterObjectsNVX;
	PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
	PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
	PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
	PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
	PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
	PFN_vkUnregisterObjectsNVX vkUnregisterObjectsNVX;
} table{};

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
VkResult vkAcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display) {
	return table.vkAcquireXlibDisplayEXT(physicalDevice, dpy, display);
}
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
void vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
	return table.vkCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
void vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
	return table.vkCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}
void vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
	return table.vkCmdDebugMarkerEndEXT(commandBuffer);
}
void vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
	return table.vkCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}
void vkCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
	return table.vkCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void vkCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
	return table.vkCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
void vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
	return table.vkCmdEndDebugUtilsLabelEXT(commandBuffer);
}
void vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
	return table.vkCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
void vkCmdProcessCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo) {
	return table.vkCmdProcessCommandsNVX(commandBuffer, pProcessCommandsInfo);
}
void vkCmdReserveSpaceForCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo) {
	return table.vkCmdReserveSpaceForCommandsNVX(commandBuffer, pReserveSpaceInfo);
}
void vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) {
	return table.vkCmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}
void vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
	return table.vkCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}
void vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) {
	return table.vkCmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}
void vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
	return table.vkCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
VkResult vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	return table.vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}
VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) {
	return table.vkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}
#ifdef VK_USE_PLATFORM_IOS_MVK
VkResult vkCreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	return table.vkCreateIOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
}
#endif /*VK_USE_PLATFORM_IOS_MVK*/
VkResult vkCreateIndirectCommandsLayoutNVX(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout) {
	return table.vkCreateIndirectCommandsLayoutNVX(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}
#ifdef VK_USE_PLATFORM_MACOS_MVK
VkResult vkCreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	return table.vkCreateMacOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
}
#endif /*VK_USE_PLATFORM_MACOS_MVK*/
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
VkResult vkCreateObjectTableNVX(VkDevice device, const VkObjectTableCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable) {
	return table.vkCreateObjectTableNVX(device, pCreateInfo, pAllocator, pObjectTable);
}
VkResult vkCreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) {
	return table.vkCreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
}
#ifdef VK_USE_PLATFORM_VI_NN
VkResult vkCreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	return table.vkCreateViSurfaceNN(instance, pCreateInfo, pAllocator, pSurface);
}
#endif /*VK_USE_PLATFORM_VI_NN*/
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
VkResult vkDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) {
	return table.vkDebugMarkerSetObjectNameEXT(device, pNameInfo);
}
VkResult vkDebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) {
	return table.vkDebugMarkerSetObjectTagEXT(device, pTagInfo);
}
void vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) {
	return table.vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}
void vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	return table.vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}
void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) {
	return table.vkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}
void vkDestroyIndirectCommandsLayoutNVX(VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) {
	return table.vkDestroyIndirectCommandsLayoutNVX(device, indirectCommandsLayout, pAllocator);
}
void vkDestroyObjectTableNVX(VkDevice device, VkObjectTableNVX objectTable, const VkAllocationCallbacks* pAllocator) {
	return table.vkDestroyObjectTableNVX(device, objectTable, pAllocator);
}
void vkDestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) {
	return table.vkDestroyValidationCacheEXT(device, validationCache, pAllocator);
}
VkResult vkDisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) {
	return table.vkDisplayPowerControlEXT(device, display, pDisplayPowerInfo);
}
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
VkResult vkGetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) {
	return table.vkGetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
}
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
VkResult vkGetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) {
	return table.vkGetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
}
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
VkResult vkGetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) {
	return table.vkGetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_NV
VkResult vkGetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) {
	return table.vkGetMemoryWin32HandleNV(device, memory, handleType, pHandle);
}
#endif /*VK_USE_PLATFORM_WIN32_NV*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
VkResult vkGetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) {
	return table.vkGetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}
VkResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) {
	return table.vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}
void vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(VkPhysicalDevice physicalDevice, VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits) {
	return table.vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(physicalDevice, pFeatures, pLimits);
}
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) {
	return table.vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}
VkResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) {
	return table.vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
VkResult vkGetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) {
	return table.vkGetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
}
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
VkResult vkGetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) {
	return table.vkGetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
VkResult vkGetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) {
	return table.vkGetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}
VkResult vkGetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) {
	return table.vkGetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
}
VkResult vkGetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) {
	return table.vkGetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
}
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
VkResult vkMergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) {
	return table.vkMergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
}
void vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) {
	return table.vkQueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}
void vkQueueEndDebugUtilsLabelEXT(VkQueue queue) {
	return table.vkQueueEndDebugUtilsLabelEXT(queue);
}
void vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) {
	return table.vkQueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}
VkResult vkRegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) {
	return table.vkRegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
}
VkResult vkRegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) {
	return table.vkRegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
}
VkResult vkRegisterObjectsNVX(VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const*    ppObjectTableEntries, const uint32_t* pObjectIndices) {
	return table.vkRegisterObjectsNVX(device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
}
VkResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) {
	return table.vkReleaseDisplayEXT(physicalDevice, display);
}
VkResult vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) {
	return table.vkSetDebugUtilsObjectNameEXT(device, pNameInfo);
}
VkResult vkSetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) {
	return table.vkSetDebugUtilsObjectTagEXT(device, pTagInfo);
}
void vkSetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) {
	return table.vkSetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
}
void vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
	return table.vkSubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
}
VkResult vkUnregisterObjectsNVX(VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices) {
	return table.vkUnregisterObjectsNVX(device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
}

namespace rn::vki {

void ExtensionsStaticLoader::loadInstance(Context &context) {
	const auto instance = context.instance;

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
	table.vkAcquireXlibDisplayEXT = PFN_vkAcquireXlibDisplayEXT(instance.getProcAddr("vkAcquireXlibDisplayEXT"));
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
	table.vkCmdBeginDebugUtilsLabelEXT = PFN_vkCmdBeginDebugUtilsLabelEXT(instance.getProcAddr("vkCmdBeginDebugUtilsLabelEXT"));
	table.vkCmdDebugMarkerBeginEXT = PFN_vkCmdDebugMarkerBeginEXT(instance.getProcAddr("vkCmdDebugMarkerBeginEXT"));
	table.vkCmdDebugMarkerEndEXT = PFN_vkCmdDebugMarkerEndEXT(instance.getProcAddr("vkCmdDebugMarkerEndEXT"));
	table.vkCmdDebugMarkerInsertEXT = PFN_vkCmdDebugMarkerInsertEXT(instance.getProcAddr("vkCmdDebugMarkerInsertEXT"));
	table.vkCmdDrawIndexedIndirectCountAMD = PFN_vkCmdDrawIndexedIndirectCountAMD(instance.getProcAddr("vkCmdDrawIndexedIndirectCountAMD"));
	table.vkCmdDrawIndirectCountAMD = PFN_vkCmdDrawIndirectCountAMD(instance.getProcAddr("vkCmdDrawIndirectCountAMD"));
	table.vkCmdEndDebugUtilsLabelEXT = PFN_vkCmdEndDebugUtilsLabelEXT(instance.getProcAddr("vkCmdEndDebugUtilsLabelEXT"));
	table.vkCmdInsertDebugUtilsLabelEXT = PFN_vkCmdInsertDebugUtilsLabelEXT(instance.getProcAddr("vkCmdInsertDebugUtilsLabelEXT"));
	table.vkCmdProcessCommandsNVX = PFN_vkCmdProcessCommandsNVX(instance.getProcAddr("vkCmdProcessCommandsNVX"));
	table.vkCmdReserveSpaceForCommandsNVX = PFN_vkCmdReserveSpaceForCommandsNVX(instance.getProcAddr("vkCmdReserveSpaceForCommandsNVX"));
	table.vkCmdSetDiscardRectangleEXT = PFN_vkCmdSetDiscardRectangleEXT(instance.getProcAddr("vkCmdSetDiscardRectangleEXT"));
	table.vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT(instance.getProcAddr("vkCmdSetSampleLocationsEXT"));
	table.vkCmdSetViewportWScalingNV = PFN_vkCmdSetViewportWScalingNV(instance.getProcAddr("vkCmdSetViewportWScalingNV"));
	table.vkCmdWriteBufferMarkerAMD = PFN_vkCmdWriteBufferMarkerAMD(instance.getProcAddr("vkCmdWriteBufferMarkerAMD"));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
	table.vkCreateDebugReportCallbackEXT = PFN_vkCreateDebugReportCallbackEXT(instance.getProcAddr("vkCreateDebugReportCallbackEXT"));
	table.vkCreateDebugUtilsMessengerEXT = PFN_vkCreateDebugUtilsMessengerEXT(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
#ifdef VK_USE_PLATFORM_IOS_MVK
	table.vkCreateIOSSurfaceMVK = PFN_vkCreateIOSSurfaceMVK(instance.getProcAddr("vkCreateIOSSurfaceMVK"));
#endif /*VK_USE_PLATFORM_IOS_MVK*/
	table.vkCreateIndirectCommandsLayoutNVX = PFN_vkCreateIndirectCommandsLayoutNVX(instance.getProcAddr("vkCreateIndirectCommandsLayoutNVX"));
#ifdef VK_USE_PLATFORM_MACOS_MVK
	table.vkCreateMacOSSurfaceMVK = PFN_vkCreateMacOSSurfaceMVK(instance.getProcAddr("vkCreateMacOSSurfaceMVK"));
#endif /*VK_USE_PLATFORM_MACOS_MVK*/
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
	table.vkCreateObjectTableNVX = PFN_vkCreateObjectTableNVX(instance.getProcAddr("vkCreateObjectTableNVX"));
	table.vkCreateValidationCacheEXT = PFN_vkCreateValidationCacheEXT(instance.getProcAddr("vkCreateValidationCacheEXT"));
#ifdef VK_USE_PLATFORM_VI_NN
	table.vkCreateViSurfaceNN = PFN_vkCreateViSurfaceNN(instance.getProcAddr("vkCreateViSurfaceNN"));
#endif /*VK_USE_PLATFORM_VI_NN*/
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
	table.vkDebugMarkerSetObjectNameEXT = PFN_vkDebugMarkerSetObjectNameEXT(instance.getProcAddr("vkDebugMarkerSetObjectNameEXT"));
	table.vkDebugMarkerSetObjectTagEXT = PFN_vkDebugMarkerSetObjectTagEXT(instance.getProcAddr("vkDebugMarkerSetObjectTagEXT"));
	table.vkDebugReportMessageEXT = PFN_vkDebugReportMessageEXT(instance.getProcAddr("vkDebugReportMessageEXT"));
	table.vkDestroyDebugReportCallbackEXT = PFN_vkDestroyDebugReportCallbackEXT(instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));
	table.vkDestroyDebugUtilsMessengerEXT = PFN_vkDestroyDebugUtilsMessengerEXT(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
	table.vkDestroyIndirectCommandsLayoutNVX = PFN_vkDestroyIndirectCommandsLayoutNVX(instance.getProcAddr("vkDestroyIndirectCommandsLayoutNVX"));
	table.vkDestroyObjectTableNVX = PFN_vkDestroyObjectTableNVX(instance.getProcAddr("vkDestroyObjectTableNVX"));
	table.vkDestroyValidationCacheEXT = PFN_vkDestroyValidationCacheEXT(instance.getProcAddr("vkDestroyValidationCacheEXT"));
	table.vkDisplayPowerControlEXT = PFN_vkDisplayPowerControlEXT(instance.getProcAddr("vkDisplayPowerControlEXT"));
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	table.vkGetAndroidHardwareBufferPropertiesANDROID = PFN_vkGetAndroidHardwareBufferPropertiesANDROID(instance.getProcAddr("vkGetAndroidHardwareBufferPropertiesANDROID"));
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	table.vkGetMemoryAndroidHardwareBufferANDROID = PFN_vkGetMemoryAndroidHardwareBufferANDROID(instance.getProcAddr("vkGetMemoryAndroidHardwareBufferANDROID"));
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
	table.vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT(instance.getProcAddr("vkGetMemoryHostPointerPropertiesEXT"));
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_NV
	table.vkGetMemoryWin32HandleNV = PFN_vkGetMemoryWin32HandleNV(instance.getProcAddr("vkGetMemoryWin32HandleNV"));
#endif /*VK_USE_PLATFORM_WIN32_NV*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	table.vkGetPastPresentationTimingGOOGLE = PFN_vkGetPastPresentationTimingGOOGLE(instance.getProcAddr("vkGetPastPresentationTimingGOOGLE"));
	table.vkGetPhysicalDeviceExternalImageFormatPropertiesNV = PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(instance.getProcAddr("vkGetPhysicalDeviceExternalImageFormatPropertiesNV"));
	table.vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX = PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(instance.getProcAddr("vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX"));
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
	table.vkGetPhysicalDeviceMultisamplePropertiesEXT = PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT(instance.getProcAddr("vkGetPhysicalDeviceMultisamplePropertiesEXT"));
	table.vkGetPhysicalDeviceSurfaceCapabilities2EXT = PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT(instance.getProcAddr("vkGetPhysicalDeviceSurfaceCapabilities2EXT"));
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
	table.vkGetRandROutputDisplayEXT = PFN_vkGetRandROutputDisplayEXT(instance.getProcAddr("vkGetRandROutputDisplayEXT"));
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
	table.vkGetRefreshCycleDurationGOOGLE = PFN_vkGetRefreshCycleDurationGOOGLE(instance.getProcAddr("vkGetRefreshCycleDurationGOOGLE"));
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	table.vkGetShaderInfoAMD = PFN_vkGetShaderInfoAMD(instance.getProcAddr("vkGetShaderInfoAMD"));
	table.vkGetSwapchainCounterEXT = PFN_vkGetSwapchainCounterEXT(instance.getProcAddr("vkGetSwapchainCounterEXT"));
	table.vkGetValidationCacheDataEXT = PFN_vkGetValidationCacheDataEXT(instance.getProcAddr("vkGetValidationCacheDataEXT"));
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	table.vkMergeValidationCachesEXT = PFN_vkMergeValidationCachesEXT(instance.getProcAddr("vkMergeValidationCachesEXT"));
	table.vkQueueBeginDebugUtilsLabelEXT = PFN_vkQueueBeginDebugUtilsLabelEXT(instance.getProcAddr("vkQueueBeginDebugUtilsLabelEXT"));
	table.vkQueueEndDebugUtilsLabelEXT = PFN_vkQueueEndDebugUtilsLabelEXT(instance.getProcAddr("vkQueueEndDebugUtilsLabelEXT"));
	table.vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT(instance.getProcAddr("vkQueueInsertDebugUtilsLabelEXT"));
	table.vkRegisterDeviceEventEXT = PFN_vkRegisterDeviceEventEXT(instance.getProcAddr("vkRegisterDeviceEventEXT"));
	table.vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT(instance.getProcAddr("vkRegisterDisplayEventEXT"));
	table.vkRegisterObjectsNVX = PFN_vkRegisterObjectsNVX(instance.getProcAddr("vkRegisterObjectsNVX"));
	table.vkReleaseDisplayEXT = PFN_vkReleaseDisplayEXT(instance.getProcAddr("vkReleaseDisplayEXT"));
	table.vkSetDebugUtilsObjectNameEXT = PFN_vkSetDebugUtilsObjectNameEXT(instance.getProcAddr("vkSetDebugUtilsObjectNameEXT"));
	table.vkSetDebugUtilsObjectTagEXT = PFN_vkSetDebugUtilsObjectTagEXT(instance.getProcAddr("vkSetDebugUtilsObjectTagEXT"));
	table.vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT(instance.getProcAddr("vkSetHdrMetadataEXT"));
	table.vkSubmitDebugUtilsMessageEXT = PFN_vkSubmitDebugUtilsMessageEXT(instance.getProcAddr("vkSubmitDebugUtilsMessageEXT"));
	table.vkUnregisterObjectsNVX = PFN_vkUnregisterObjectsNVX(instance.getProcAddr("vkUnregisterObjectsNVX"));
}

void ExtensionsStaticLoader::loadDevice(Context &context) {
	const auto instance = context.instance;
	const auto device = context.device;

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
	table.vkAcquireXlibDisplayEXT = PFN_vkAcquireXlibDisplayEXT(device ? device.getProcAddr("vkAcquireXlibDisplayEXT") : instance.getProcAddr("vkAcquireXlibDisplayEXT"));
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
	table.vkCmdBeginDebugUtilsLabelEXT = PFN_vkCmdBeginDebugUtilsLabelEXT(device ? device.getProcAddr("vkCmdBeginDebugUtilsLabelEXT") : instance.getProcAddr("vkCmdBeginDebugUtilsLabelEXT"));
	table.vkCmdDebugMarkerBeginEXT = PFN_vkCmdDebugMarkerBeginEXT(device ? device.getProcAddr("vkCmdDebugMarkerBeginEXT") : instance.getProcAddr("vkCmdDebugMarkerBeginEXT"));
	table.vkCmdDebugMarkerEndEXT = PFN_vkCmdDebugMarkerEndEXT(device ? device.getProcAddr("vkCmdDebugMarkerEndEXT") : instance.getProcAddr("vkCmdDebugMarkerEndEXT"));
	table.vkCmdDebugMarkerInsertEXT = PFN_vkCmdDebugMarkerInsertEXT(device ? device.getProcAddr("vkCmdDebugMarkerInsertEXT") : instance.getProcAddr("vkCmdDebugMarkerInsertEXT"));
	table.vkCmdDrawIndexedIndirectCountAMD = PFN_vkCmdDrawIndexedIndirectCountAMD(device ? device.getProcAddr("vkCmdDrawIndexedIndirectCountAMD") : instance.getProcAddr("vkCmdDrawIndexedIndirectCountAMD"));
	table.vkCmdDrawIndirectCountAMD = PFN_vkCmdDrawIndirectCountAMD(device ? device.getProcAddr("vkCmdDrawIndirectCountAMD") : instance.getProcAddr("vkCmdDrawIndirectCountAMD"));
	table.vkCmdEndDebugUtilsLabelEXT = PFN_vkCmdEndDebugUtilsLabelEXT(device ? device.getProcAddr("vkCmdEndDebugUtilsLabelEXT") : instance.getProcAddr("vkCmdEndDebugUtilsLabelEXT"));
	table.vkCmdInsertDebugUtilsLabelEXT = PFN_vkCmdInsertDebugUtilsLabelEXT(device ? device.getProcAddr("vkCmdInsertDebugUtilsLabelEXT") : instance.getProcAddr("vkCmdInsertDebugUtilsLabelEXT"));
	table.vkCmdProcessCommandsNVX = PFN_vkCmdProcessCommandsNVX(device ? device.getProcAddr("vkCmdProcessCommandsNVX") : instance.getProcAddr("vkCmdProcessCommandsNVX"));
	table.vkCmdReserveSpaceForCommandsNVX = PFN_vkCmdReserveSpaceForCommandsNVX(device ? device.getProcAddr("vkCmdReserveSpaceForCommandsNVX") : instance.getProcAddr("vkCmdReserveSpaceForCommandsNVX"));
	table.vkCmdSetDiscardRectangleEXT = PFN_vkCmdSetDiscardRectangleEXT(device ? device.getProcAddr("vkCmdSetDiscardRectangleEXT") : instance.getProcAddr("vkCmdSetDiscardRectangleEXT"));
	table.vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT(device ? device.getProcAddr("vkCmdSetSampleLocationsEXT") : instance.getProcAddr("vkCmdSetSampleLocationsEXT"));
	table.vkCmdSetViewportWScalingNV = PFN_vkCmdSetViewportWScalingNV(device ? device.getProcAddr("vkCmdSetViewportWScalingNV") : instance.getProcAddr("vkCmdSetViewportWScalingNV"));
	table.vkCmdWriteBufferMarkerAMD = PFN_vkCmdWriteBufferMarkerAMD(device ? device.getProcAddr("vkCmdWriteBufferMarkerAMD") : instance.getProcAddr("vkCmdWriteBufferMarkerAMD"));
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
#ifdef VK_USE_PLATFORM_IOS_MVK
#endif /*VK_USE_PLATFORM_IOS_MVK*/
	table.vkCreateIndirectCommandsLayoutNVX = PFN_vkCreateIndirectCommandsLayoutNVX(device ? device.getProcAddr("vkCreateIndirectCommandsLayoutNVX") : instance.getProcAddr("vkCreateIndirectCommandsLayoutNVX"));
#ifdef VK_USE_PLATFORM_MACOS_MVK
#endif /*VK_USE_PLATFORM_MACOS_MVK*/
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
	table.vkCreateObjectTableNVX = PFN_vkCreateObjectTableNVX(device ? device.getProcAddr("vkCreateObjectTableNVX") : instance.getProcAddr("vkCreateObjectTableNVX"));
	table.vkCreateValidationCacheEXT = PFN_vkCreateValidationCacheEXT(device ? device.getProcAddr("vkCreateValidationCacheEXT") : instance.getProcAddr("vkCreateValidationCacheEXT"));
#ifdef VK_USE_PLATFORM_VI_NN
#endif /*VK_USE_PLATFORM_VI_NN*/
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
	table.vkDebugMarkerSetObjectNameEXT = PFN_vkDebugMarkerSetObjectNameEXT(device ? device.getProcAddr("vkDebugMarkerSetObjectNameEXT") : instance.getProcAddr("vkDebugMarkerSetObjectNameEXT"));
	table.vkDebugMarkerSetObjectTagEXT = PFN_vkDebugMarkerSetObjectTagEXT(device ? device.getProcAddr("vkDebugMarkerSetObjectTagEXT") : instance.getProcAddr("vkDebugMarkerSetObjectTagEXT"));
	table.vkDestroyIndirectCommandsLayoutNVX = PFN_vkDestroyIndirectCommandsLayoutNVX(device ? device.getProcAddr("vkDestroyIndirectCommandsLayoutNVX") : instance.getProcAddr("vkDestroyIndirectCommandsLayoutNVX"));
	table.vkDestroyObjectTableNVX = PFN_vkDestroyObjectTableNVX(device ? device.getProcAddr("vkDestroyObjectTableNVX") : instance.getProcAddr("vkDestroyObjectTableNVX"));
	table.vkDestroyValidationCacheEXT = PFN_vkDestroyValidationCacheEXT(device ? device.getProcAddr("vkDestroyValidationCacheEXT") : instance.getProcAddr("vkDestroyValidationCacheEXT"));
	table.vkDisplayPowerControlEXT = PFN_vkDisplayPowerControlEXT(device ? device.getProcAddr("vkDisplayPowerControlEXT") : instance.getProcAddr("vkDisplayPowerControlEXT"));
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	table.vkGetAndroidHardwareBufferPropertiesANDROID = PFN_vkGetAndroidHardwareBufferPropertiesANDROID(device ? device.getProcAddr("vkGetAndroidHardwareBufferPropertiesANDROID") : instance.getProcAddr("vkGetAndroidHardwareBufferPropertiesANDROID"));
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	table.vkGetMemoryAndroidHardwareBufferANDROID = PFN_vkGetMemoryAndroidHardwareBufferANDROID(device ? device.getProcAddr("vkGetMemoryAndroidHardwareBufferANDROID") : instance.getProcAddr("vkGetMemoryAndroidHardwareBufferANDROID"));
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/
	table.vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT(device ? device.getProcAddr("vkGetMemoryHostPointerPropertiesEXT") : instance.getProcAddr("vkGetMemoryHostPointerPropertiesEXT"));
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_NV
	table.vkGetMemoryWin32HandleNV = PFN_vkGetMemoryWin32HandleNV(device ? device.getProcAddr("vkGetMemoryWin32HandleNV") : instance.getProcAddr("vkGetMemoryWin32HandleNV"));
#endif /*VK_USE_PLATFORM_WIN32_NV*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	table.vkGetPastPresentationTimingGOOGLE = PFN_vkGetPastPresentationTimingGOOGLE(device ? device.getProcAddr("vkGetPastPresentationTimingGOOGLE") : instance.getProcAddr("vkGetPastPresentationTimingGOOGLE"));
	table.vkGetPhysicalDeviceExternalImageFormatPropertiesNV = PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(device ? device.getProcAddr("vkGetPhysicalDeviceExternalImageFormatPropertiesNV") : instance.getProcAddr("vkGetPhysicalDeviceExternalImageFormatPropertiesNV"));
	table.vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX = PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(device ? device.getProcAddr("vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX") : instance.getProcAddr("vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX"));
#ifdef VK_USE_PLATFORM_MIR_KHR
#endif /*VK_USE_PLATFORM_MIR_KHR*/
	table.vkGetPhysicalDeviceMultisamplePropertiesEXT = PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT(device ? device.getProcAddr("vkGetPhysicalDeviceMultisamplePropertiesEXT") : instance.getProcAddr("vkGetPhysicalDeviceMultisamplePropertiesEXT"));
	table.vkGetPhysicalDeviceSurfaceCapabilities2EXT = PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT(device ? device.getProcAddr("vkGetPhysicalDeviceSurfaceCapabilities2EXT") : instance.getProcAddr("vkGetPhysicalDeviceSurfaceCapabilities2EXT"));
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_XCB_KHR
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_KHR
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_NV
	table.vkGetRandROutputDisplayEXT = PFN_vkGetRandROutputDisplayEXT(device ? device.getProcAddr("vkGetRandROutputDisplayEXT") : instance.getProcAddr("vkGetRandROutputDisplayEXT"));
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_NV*/
	table.vkGetRefreshCycleDurationGOOGLE = PFN_vkGetRefreshCycleDurationGOOGLE(device ? device.getProcAddr("vkGetRefreshCycleDurationGOOGLE") : instance.getProcAddr("vkGetRefreshCycleDurationGOOGLE"));
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	table.vkGetShaderInfoAMD = PFN_vkGetShaderInfoAMD(device ? device.getProcAddr("vkGetShaderInfoAMD") : instance.getProcAddr("vkGetShaderInfoAMD"));
	table.vkGetSwapchainCounterEXT = PFN_vkGetSwapchainCounterEXT(device ? device.getProcAddr("vkGetSwapchainCounterEXT") : instance.getProcAddr("vkGetSwapchainCounterEXT"));
	table.vkGetValidationCacheDataEXT = PFN_vkGetValidationCacheDataEXT(device ? device.getProcAddr("vkGetValidationCacheDataEXT") : instance.getProcAddr("vkGetValidationCacheDataEXT"));
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
	table.vkMergeValidationCachesEXT = PFN_vkMergeValidationCachesEXT(device ? device.getProcAddr("vkMergeValidationCachesEXT") : instance.getProcAddr("vkMergeValidationCachesEXT"));
	table.vkQueueBeginDebugUtilsLabelEXT = PFN_vkQueueBeginDebugUtilsLabelEXT(device ? device.getProcAddr("vkQueueBeginDebugUtilsLabelEXT") : instance.getProcAddr("vkQueueBeginDebugUtilsLabelEXT"));
	table.vkQueueEndDebugUtilsLabelEXT = PFN_vkQueueEndDebugUtilsLabelEXT(device ? device.getProcAddr("vkQueueEndDebugUtilsLabelEXT") : instance.getProcAddr("vkQueueEndDebugUtilsLabelEXT"));
	table.vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT(device ? device.getProcAddr("vkQueueInsertDebugUtilsLabelEXT") : instance.getProcAddr("vkQueueInsertDebugUtilsLabelEXT"));
	table.vkRegisterDeviceEventEXT = PFN_vkRegisterDeviceEventEXT(device ? device.getProcAddr("vkRegisterDeviceEventEXT") : instance.getProcAddr("vkRegisterDeviceEventEXT"));
	table.vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT(device ? device.getProcAddr("vkRegisterDisplayEventEXT") : instance.getProcAddr("vkRegisterDisplayEventEXT"));
	table.vkRegisterObjectsNVX = PFN_vkRegisterObjectsNVX(device ? device.getProcAddr("vkRegisterObjectsNVX") : instance.getProcAddr("vkRegisterObjectsNVX"));
	table.vkReleaseDisplayEXT = PFN_vkReleaseDisplayEXT(device ? device.getProcAddr("vkReleaseDisplayEXT") : instance.getProcAddr("vkReleaseDisplayEXT"));
	table.vkSetDebugUtilsObjectNameEXT = PFN_vkSetDebugUtilsObjectNameEXT(device ? device.getProcAddr("vkSetDebugUtilsObjectNameEXT") : instance.getProcAddr("vkSetDebugUtilsObjectNameEXT"));
	table.vkSetDebugUtilsObjectTagEXT = PFN_vkSetDebugUtilsObjectTagEXT(device ? device.getProcAddr("vkSetDebugUtilsObjectTagEXT") : instance.getProcAddr("vkSetDebugUtilsObjectTagEXT"));
	table.vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT(device ? device.getProcAddr("vkSetHdrMetadataEXT") : instance.getProcAddr("vkSetHdrMetadataEXT"));
	table.vkUnregisterObjectsNVX = PFN_vkUnregisterObjectsNVX(device ? device.getProcAddr("vkUnregisterObjectsNVX") : instance.getProcAddr("vkUnregisterObjectsNVX"));
}

} // rn::vki
