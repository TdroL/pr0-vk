#pragma once

#ifndef VK_NO_PROTOTYPES
	#error "rn::vlk::Interface - VK_NO_PROTOTYPES must be defined"
#endif

#include <vulkan/vulkan.h>

namespace rn {

namespace vlk {

class Interface {
public:
	VkInstance currentInstance = VK_NULL_HANDLE;
	VkDevice currentDevice = VK_NULL_HANDLE;

	PFN_vkCreateInstance CreateInstance = nullptr;
	PFN_vkDestroyInstance DestroyInstance = nullptr;
	PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices = nullptr;
	PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures = nullptr;
	PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties = nullptr;
	PFN_vkGetPhysicalDeviceImageFormatProperties GetPhysicalDeviceImageFormatProperties = nullptr;
	PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties = nullptr;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties = nullptr;
	PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties = nullptr;
	PFN_vkGetInstanceProcAddr GetInstanceProcAddr = nullptr;
	PFN_vkGetDeviceProcAddr GetDeviceProcAddr = nullptr;
	PFN_vkCreateDevice CreateDevice = nullptr;
	PFN_vkDestroyDevice DestroyDevice = nullptr;
	PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties = nullptr;
	PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties = nullptr;
	PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties = nullptr;
	PFN_vkEnumerateDeviceLayerProperties EnumerateDeviceLayerProperties = nullptr;
	PFN_vkGetDeviceQueue GetDeviceQueue = nullptr;
	PFN_vkQueueSubmit QueueSubmit = nullptr;
	PFN_vkQueueWaitIdle QueueWaitIdle = nullptr;
	PFN_vkDeviceWaitIdle DeviceWaitIdle = nullptr;
	PFN_vkAllocateMemory AllocateMemory = nullptr;
	PFN_vkFreeMemory FreeMemory = nullptr;
	PFN_vkMapMemory MapMemory = nullptr;
	PFN_vkUnmapMemory UnmapMemory = nullptr;
	PFN_vkFlushMappedMemoryRanges FlushMappedMemoryRanges = nullptr;
	PFN_vkInvalidateMappedMemoryRanges InvalidateMappedMemoryRanges = nullptr;
	PFN_vkGetDeviceMemoryCommitment GetDeviceMemoryCommitment = nullptr;
	PFN_vkBindBufferMemory BindBufferMemory = nullptr;
	PFN_vkBindImageMemory BindImageMemory = nullptr;
	PFN_vkGetBufferMemoryRequirements GetBufferMemoryRequirements = nullptr;
	PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements = nullptr;
	PFN_vkGetImageSparseMemoryRequirements GetImageSparseMemoryRequirements = nullptr;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties GetPhysicalDeviceSparseImageFormatProperties = nullptr;
	PFN_vkQueueBindSparse QueueBindSparse = nullptr;
	PFN_vkCreateFence CreateFence = nullptr;
	PFN_vkDestroyFence DestroyFence = nullptr;
	PFN_vkResetFences ResetFences = nullptr;
	PFN_vkGetFenceStatus GetFenceStatus = nullptr;
	PFN_vkWaitForFences WaitForFences = nullptr;
	PFN_vkCreateSemaphore CreateSemaphore = nullptr;
	PFN_vkDestroySemaphore DestroySemaphore = nullptr;
	PFN_vkCreateEvent CreateEvent = nullptr;
	PFN_vkDestroyEvent DestroyEvent = nullptr;
	PFN_vkGetEventStatus GetEventStatus = nullptr;
	PFN_vkSetEvent SetEvent = nullptr;
	PFN_vkResetEvent ResetEvent = nullptr;
	PFN_vkCreateQueryPool CreateQueryPool = nullptr;
	PFN_vkDestroyQueryPool DestroyQueryPool = nullptr;
	PFN_vkGetQueryPoolResults GetQueryPoolResults = nullptr;
	PFN_vkCreateBuffer CreateBuffer = nullptr;
	PFN_vkDestroyBuffer DestroyBuffer = nullptr;
	PFN_vkCreateBufferView CreateBufferView = nullptr;
	PFN_vkDestroyBufferView DestroyBufferView = nullptr;
	PFN_vkCreateImage CreateImage = nullptr;
	PFN_vkDestroyImage DestroyImage = nullptr;
	PFN_vkGetImageSubresourceLayout GetImageSubresourceLayout = nullptr;
	PFN_vkCreateImageView CreateImageView = nullptr;
	PFN_vkDestroyImageView DestroyImageView = nullptr;
	PFN_vkCreateShaderModule CreateShaderModule = nullptr;
	PFN_vkDestroyShaderModule DestroyShaderModule = nullptr;
	PFN_vkCreatePipelineCache CreatePipelineCache = nullptr;
	PFN_vkDestroyPipelineCache DestroyPipelineCache = nullptr;
	PFN_vkGetPipelineCacheData GetPipelineCacheData = nullptr;
	PFN_vkMergePipelineCaches MergePipelineCaches = nullptr;
	PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines = nullptr;
	PFN_vkCreateComputePipelines CreateComputePipelines = nullptr;
	PFN_vkDestroyPipeline DestroyPipeline = nullptr;
	PFN_vkCreatePipelineLayout CreatePipelineLayout = nullptr;
	PFN_vkDestroyPipelineLayout DestroyPipelineLayout = nullptr;
	PFN_vkCreateSampler CreateSampler = nullptr;
	PFN_vkDestroySampler DestroySampler = nullptr;
	PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout = nullptr;
	PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout = nullptr;
	PFN_vkCreateDescriptorPool CreateDescriptorPool = nullptr;
	PFN_vkDestroyDescriptorPool DestroyDescriptorPool = nullptr;
	PFN_vkResetDescriptorPool ResetDescriptorPool = nullptr;
	PFN_vkAllocateDescriptorSets AllocateDescriptorSets = nullptr;
	PFN_vkFreeDescriptorSets FreeDescriptorSets = nullptr;
	PFN_vkUpdateDescriptorSets UpdateDescriptorSets = nullptr;
	PFN_vkCreateFramebuffer CreateFramebuffer = nullptr;
	PFN_vkDestroyFramebuffer DestroyFramebuffer = nullptr;
	PFN_vkCreateRenderPass CreateRenderPass = nullptr;
	PFN_vkDestroyRenderPass DestroyRenderPass = nullptr;
	PFN_vkGetRenderAreaGranularity GetRenderAreaGranularity = nullptr;
	PFN_vkCreateCommandPool CreateCommandPool = nullptr;
	PFN_vkDestroyCommandPool DestroyCommandPool = nullptr;
	PFN_vkResetCommandPool ResetCommandPool = nullptr;
	PFN_vkAllocateCommandBuffers AllocateCommandBuffers = nullptr;
	PFN_vkFreeCommandBuffers FreeCommandBuffers = nullptr;
	PFN_vkBeginCommandBuffer BeginCommandBuffer = nullptr;
	PFN_vkEndCommandBuffer EndCommandBuffer = nullptr;
	PFN_vkResetCommandBuffer ResetCommandBuffer = nullptr;
	PFN_vkCmdBindPipeline CmdBindPipeline = nullptr;
	PFN_vkCmdSetViewport CmdSetViewport = nullptr;
	PFN_vkCmdSetScissor CmdSetScissor = nullptr;
	PFN_vkCmdSetLineWidth CmdSetLineWidth = nullptr;
	PFN_vkCmdSetDepthBias CmdSetDepthBias = nullptr;
	PFN_vkCmdSetBlendConstants CmdSetBlendConstants = nullptr;
	PFN_vkCmdSetDepthBounds CmdSetDepthBounds = nullptr;
	PFN_vkCmdSetStencilCompareMask CmdSetStencilCompareMask = nullptr;
	PFN_vkCmdSetStencilWriteMask CmdSetStencilWriteMask = nullptr;
	PFN_vkCmdSetStencilReference CmdSetStencilReference = nullptr;
	PFN_vkCmdBindDescriptorSets CmdBindDescriptorSets = nullptr;
	PFN_vkCmdBindIndexBuffer CmdBindIndexBuffer = nullptr;
	PFN_vkCmdBindVertexBuffers CmdBindVertexBuffers = nullptr;
	PFN_vkCmdDraw CmdDraw = nullptr;
	PFN_vkCmdDrawIndexed CmdDrawIndexed = nullptr;
	PFN_vkCmdDrawIndirect CmdDrawIndirect = nullptr;
	PFN_vkCmdDrawIndexedIndirect CmdDrawIndexedIndirect = nullptr;
	PFN_vkCmdDispatch CmdDispatch = nullptr;
	PFN_vkCmdDispatchIndirect CmdDispatchIndirect = nullptr;
	PFN_vkCmdCopyBuffer CmdCopyBuffer = nullptr;
	PFN_vkCmdCopyImage CmdCopyImage = nullptr;
	PFN_vkCmdBlitImage CmdBlitImage = nullptr;
	PFN_vkCmdCopyBufferToImage CmdCopyBufferToImage = nullptr;
	PFN_vkCmdCopyImageToBuffer CmdCopyImageToBuffer = nullptr;
	PFN_vkCmdUpdateBuffer CmdUpdateBuffer = nullptr;
	PFN_vkCmdFillBuffer CmdFillBuffer = nullptr;
	PFN_vkCmdClearColorImage CmdClearColorImage = nullptr;
	PFN_vkCmdClearDepthStencilImage CmdClearDepthStencilImage = nullptr;
	PFN_vkCmdClearAttachments CmdClearAttachments = nullptr;
	PFN_vkCmdResolveImage CmdResolveImage = nullptr;
	PFN_vkCmdSetEvent CmdSetEvent = nullptr;
	PFN_vkCmdResetEvent CmdResetEvent = nullptr;
	PFN_vkCmdWaitEvents CmdWaitEvents = nullptr;
	PFN_vkCmdPipelineBarrier CmdPipelineBarrier = nullptr;
	PFN_vkCmdBeginQuery CmdBeginQuery = nullptr;
	PFN_vkCmdEndQuery CmdEndQuery = nullptr;
	PFN_vkCmdResetQueryPool CmdResetQueryPool = nullptr;
	PFN_vkCmdWriteTimestamp CmdWriteTimestamp = nullptr;
	PFN_vkCmdCopyQueryPoolResults CmdCopyQueryPoolResults = nullptr;
	PFN_vkCmdPushConstants CmdPushConstants = nullptr;
	PFN_vkCmdBeginRenderPass CmdBeginRenderPass = nullptr;
	PFN_vkCmdNextSubpass CmdNextSubpass = nullptr;
	PFN_vkCmdEndRenderPass CmdEndRenderPass = nullptr;
	PFN_vkCmdExecuteCommands CmdExecuteCommands = nullptr;

	// VK_KHR_SURFACE_EXTENSION_NAME
	PFN_vkDestroySurfaceKHR DestroySurfaceKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR = nullptr;
	PFN_vkQueuePresentKHR QueuePresentKHR = nullptr;
	PFN_vkCreateSwapchainKHR CreateSwapchainKHR = nullptr;
	PFN_vkDestroySwapchainKHR DestroySwapchainKHR = nullptr;
	PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR = nullptr;
	PFN_vkAcquireNextImageKHR AcquireNextImageKHR = nullptr;

	// VK_EXT_debug_report
	PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT = nullptr;
	PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT = nullptr;
	PFN_vkDebugReportMessageEXT DebugReportMessageEXT = nullptr;

	// VK_EXT_debug_marker
	PFN_vkDebugMarkerSetObjectTagEXT DebugMarkerSetObjectTagEXT = nullptr;
	PFN_vkDebugMarkerSetObjectNameEXT DebugMarkerSetObjectNameEXT = nullptr;
	PFN_vkCmdDebugMarkerBeginEXT CmdDebugMarkerBeginEXT = nullptr;
	PFN_vkCmdDebugMarkerEndEXT CmdDebugMarkerEndEXT = nullptr;
	PFN_vkCmdDebugMarkerInsertEXT CmdDebugMarkerInsertEXT = nullptr;

	template<typename T>
	void getGlobalProc(T &func, const char *name) {
		if (this->GetInstanceProcAddr == nullptr) {
			func = nullptr;
			return;
		}

		func = reinterpret_cast<T>(this->GetInstanceProcAddr(VK_NULL_HANDLE, name));
	}

	template<typename T>
	void getInstanceProc(VkInstance instance, T &func, const char *name) {
		if (this->GetInstanceProcAddr == nullptr) {
			func = nullptr;
			return;
		}

		func = reinterpret_cast<T>(this->GetInstanceProcAddr(instance, name));
	}

	template<typename T>
	void getDeviceProc(VkDevice device, T &func, const char *name) {
		if (device == VK_NULL_HANDLE) {
			func = nullptr;
			return;
		}

		if (this->GetDeviceProcAddr == nullptr) {
			func = nullptr;
			return;
		}

		func = reinterpret_cast<T>(this->GetDeviceProcAddr(device, name));
	}

	void unloadInstanceProcs();
	void unloadDeviceProcs();

	bool loadGlobalProcs(void (*GetInstanceProcAddr)());
	bool loadInstanceProcs(VkInstance instance);
	bool loadDeviceProcs(VkDevice device);
};

extern Interface vk;

#define vkCreateInstance ::rn::vlk::vk.CreateInstance
#define vkDestroyInstance ::rn::vlk::vk.DestroyInstance
#define vkEnumeratePhysicalDevices ::rn::vlk::vk.EnumeratePhysicalDevices
#define vkGetPhysicalDeviceFeatures ::rn::vlk::vk.GetPhysicalDeviceFeatures
#define vkGetPhysicalDeviceFormatProperties ::rn::vlk::vk.GetPhysicalDeviceFormatProperties
#define vkGetPhysicalDeviceImageFormatProperties ::rn::vlk::vk.GetPhysicalDeviceImageFormatProperties
#define vkGetPhysicalDeviceProperties ::rn::vlk::vk.GetPhysicalDeviceProperties
#define vkGetPhysicalDeviceQueueFamilyProperties ::rn::vlk::vk.GetPhysicalDeviceQueueFamilyProperties
#define vkGetPhysicalDeviceMemoryProperties ::rn::vlk::vk.GetPhysicalDeviceMemoryProperties
#define vkGetInstanceProcAddr ::rn::vlk::vk.GetInstanceProcAddr
#define vkGetDeviceProcAddr ::rn::vlk::vk.GetDeviceProcAddr
#define vkCreateDevice ::rn::vlk::vk.CreateDevice
#define vkDestroyDevice ::rn::vlk::vk.DestroyDevice
#define vkEnumerateInstanceExtensionProperties ::rn::vlk::vk.EnumerateInstanceExtensionProperties
#define vkEnumerateDeviceExtensionProperties ::rn::vlk::vk.EnumerateDeviceExtensionProperties
#define vkEnumerateInstanceLayerProperties ::rn::vlk::vk.EnumerateInstanceLayerProperties
#define vkEnumerateDeviceLayerProperties ::rn::vlk::vk.EnumerateDeviceLayerProperties
#define vkGetDeviceQueue ::rn::vlk::vk.GetDeviceQueue
#define vkQueueSubmit ::rn::vlk::vk.QueueSubmit
#define vkQueueWaitIdle ::rn::vlk::vk.QueueWaitIdle
#define vkDeviceWaitIdle ::rn::vlk::vk.DeviceWaitIdle
#define vkAllocateMemory ::rn::vlk::vk.AllocateMemory
#define vkFreeMemory ::rn::vlk::vk.FreeMemory
#define vkMapMemory ::rn::vlk::vk.MapMemory
#define vkUnmapMemory ::rn::vlk::vk.UnmapMemory
#define vkFlushMappedMemoryRanges ::rn::vlk::vk.FlushMappedMemoryRanges
#define vkInvalidateMappedMemoryRanges ::rn::vlk::vk.InvalidateMappedMemoryRanges
#define vkGetDeviceMemoryCommitment ::rn::vlk::vk.GetDeviceMemoryCommitment
#define vkBindBufferMemory ::rn::vlk::vk.BindBufferMemory
#define vkBindImageMemory ::rn::vlk::vk.BindImageMemory
#define vkGetBufferMemoryRequirements ::rn::vlk::vk.GetBufferMemoryRequirements
#define vkGetImageMemoryRequirements ::rn::vlk::vk.GetImageMemoryRequirements
#define vkGetImageSparseMemoryRequirements ::rn::vlk::vk.GetImageSparseMemoryRequirements
#define vkGetPhysicalDeviceSparseImageFormatProperties ::rn::vlk::vk.GetPhysicalDeviceSparseImageFormatProperties
#define vkQueueBindSparse ::rn::vlk::vk.QueueBindSparse
#define vkCreateFence ::rn::vlk::vk.CreateFence
#define vkDestroyFence ::rn::vlk::vk.DestroyFence
#define vkResetFences ::rn::vlk::vk.ResetFences
#define vkGetFenceStatus ::rn::vlk::vk.GetFenceStatus
#define vkWaitForFences ::rn::vlk::vk.WaitForFences
#define vkCreateSemaphore ::rn::vlk::vk.CreateSemaphore
#define vkDestroySemaphore ::rn::vlk::vk.DestroySemaphore
#define vkCreateEvent ::rn::vlk::vk.CreateEvent
#define vkDestroyEvent ::rn::vlk::vk.DestroyEvent
#define vkGetEventStatus ::rn::vlk::vk.GetEventStatus
#define vkSetEvent ::rn::vlk::vk.SetEvent
#define vkResetEvent ::rn::vlk::vk.ResetEvent
#define vkCreateQueryPool ::rn::vlk::vk.CreateQueryPool
#define vkDestroyQueryPool ::rn::vlk::vk.DestroyQueryPool
#define vkGetQueryPoolResults ::rn::vlk::vk.GetQueryPoolResults
#define vkCreateBuffer ::rn::vlk::vk.CreateBuffer
#define vkDestroyBuffer ::rn::vlk::vk.DestroyBuffer
#define vkCreateBufferView ::rn::vlk::vk.CreateBufferView
#define vkDestroyBufferView ::rn::vlk::vk.DestroyBufferView
#define vkCreateImage ::rn::vlk::vk.CreateImage
#define vkDestroyImage ::rn::vlk::vk.DestroyImage
#define vkGetImageSubresourceLayout ::rn::vlk::vk.GetImageSubresourceLayout
#define vkCreateImageView ::rn::vlk::vk.CreateImageView
#define vkDestroyImageView ::rn::vlk::vk.DestroyImageView
#define vkCreateShaderModule ::rn::vlk::vk.CreateShaderModule
#define vkDestroyShaderModule ::rn::vlk::vk.DestroyShaderModule
#define vkCreatePipelineCache ::rn::vlk::vk.CreatePipelineCache
#define vkDestroyPipelineCache ::rn::vlk::vk.DestroyPipelineCache
#define vkGetPipelineCacheData ::rn::vlk::vk.GetPipelineCacheData
#define vkMergePipelineCaches ::rn::vlk::vk.MergePipelineCaches
#define vkCreateGraphicsPipelines ::rn::vlk::vk.CreateGraphicsPipelines
#define vkCreateComputePipelines ::rn::vlk::vk.CreateComputePipelines
#define vkDestroyPipeline ::rn::vlk::vk.DestroyPipeline
#define vkCreatePipelineLayout ::rn::vlk::vk.CreatePipelineLayout
#define vkDestroyPipelineLayout ::rn::vlk::vk.DestroyPipelineLayout
#define vkCreateSampler ::rn::vlk::vk.CreateSampler
#define vkDestroySampler ::rn::vlk::vk.DestroySampler
#define vkCreateDescriptorSetLayout ::rn::vlk::vk.CreateDescriptorSetLayout
#define vkDestroyDescriptorSetLayout ::rn::vlk::vk.DestroyDescriptorSetLayout
#define vkCreateDescriptorPool ::rn::vlk::vk.CreateDescriptorPool
#define vkDestroyDescriptorPool ::rn::vlk::vk.DestroyDescriptorPool
#define vkResetDescriptorPool ::rn::vlk::vk.ResetDescriptorPool
#define vkAllocateDescriptorSets ::rn::vlk::vk.AllocateDescriptorSets
#define vkFreeDescriptorSets ::rn::vlk::vk.FreeDescriptorSets
#define vkUpdateDescriptorSets ::rn::vlk::vk.UpdateDescriptorSets
#define vkCreateFramebuffer ::rn::vlk::vk.CreateFramebuffer
#define vkDestroyFramebuffer ::rn::vlk::vk.DestroyFramebuffer
#define vkCreateRenderPass ::rn::vlk::vk.CreateRenderPass
#define vkDestroyRenderPass ::rn::vlk::vk.DestroyRenderPass
#define vkGetRenderAreaGranularity ::rn::vlk::vk.GetRenderAreaGranularity
#define vkCreateCommandPool ::rn::vlk::vk.CreateCommandPool
#define vkDestroyCommandPool ::rn::vlk::vk.DestroyCommandPool
#define vkResetCommandPool ::rn::vlk::vk.ResetCommandPool
#define vkAllocateCommandBuffers ::rn::vlk::vk.AllocateCommandBuffers
#define vkFreeCommandBuffers ::rn::vlk::vk.FreeCommandBuffers
#define vkBeginCommandBuffer ::rn::vlk::vk.BeginCommandBuffer
#define vkEndCommandBuffer ::rn::vlk::vk.EndCommandBuffer
#define vkResetCommandBuffer ::rn::vlk::vk.ResetCommandBuffer
#define vkCmdBindPipeline ::rn::vlk::vk.CmdBindPipeline
#define vkCmdSetViewport ::rn::vlk::vk.CmdSetViewport
#define vkCmdSetScissor ::rn::vlk::vk.CmdSetScissor
#define vkCmdSetLineWidth ::rn::vlk::vk.CmdSetLineWidth
#define vkCmdSetDepthBias ::rn::vlk::vk.CmdSetDepthBias
#define vkCmdSetBlendConstants ::rn::vlk::vk.CmdSetBlendConstants
#define vkCmdSetDepthBounds ::rn::vlk::vk.CmdSetDepthBounds
#define vkCmdSetStencilCompareMask ::rn::vlk::vk.CmdSetStencilCompareMask
#define vkCmdSetStencilWriteMask ::rn::vlk::vk.CmdSetStencilWriteMask
#define vkCmdSetStencilReference ::rn::vlk::vk.CmdSetStencilReference
#define vkCmdBindDescriptorSets ::rn::vlk::vk.CmdBindDescriptorSets
#define vkCmdBindIndexBuffer ::rn::vlk::vk.CmdBindIndexBuffer
#define vkCmdBindVertexBuffers ::rn::vlk::vk.CmdBindVertexBuffers
#define vkCmdDraw ::rn::vlk::vk.CmdDraw
#define vkCmdDrawIndexed ::rn::vlk::vk.CmdDrawIndexed
#define vkCmdDrawIndirect ::rn::vlk::vk.CmdDrawIndirect
#define vkCmdDrawIndexedIndirect ::rn::vlk::vk.CmdDrawIndexedIndirect
#define vkCmdDispatch ::rn::vlk::vk.CmdDispatch
#define vkCmdDispatchIndirect ::rn::vlk::vk.CmdDispatchIndirect
#define vkCmdCopyBuffer ::rn::vlk::vk.CmdCopyBuffer
#define vkCmdCopyImage ::rn::vlk::vk.CmdCopyImage
#define vkCmdBlitImage ::rn::vlk::vk.CmdBlitImage
#define vkCmdCopyBufferToImage ::rn::vlk::vk.CmdCopyBufferToImage
#define vkCmdCopyImageToBuffer ::rn::vlk::vk.CmdCopyImageToBuffer
#define vkCmdUpdateBuffer ::rn::vlk::vk.CmdUpdateBuffer
#define vkCmdFillBuffer ::rn::vlk::vk.CmdFillBuffer
#define vkCmdClearColorImage ::rn::vlk::vk.CmdClearColorImage
#define vkCmdClearDepthStencilImage ::rn::vlk::vk.CmdClearDepthStencilImage
#define vkCmdClearAttachments ::rn::vlk::vk.CmdClearAttachments
#define vkCmdResolveImage ::rn::vlk::vk.CmdResolveImage
#define vkCmdSetEvent ::rn::vlk::vk.CmdSetEvent
#define vkCmdResetEvent ::rn::vlk::vk.CmdResetEvent
#define vkCmdWaitEvents ::rn::vlk::vk.CmdWaitEvents
#define vkCmdPipelineBarrier ::rn::vlk::vk.CmdPipelineBarrier
#define vkCmdBeginQuery ::rn::vlk::vk.CmdBeginQuery
#define vkCmdEndQuery ::rn::vlk::vk.CmdEndQuery
#define vkCmdResetQueryPool ::rn::vlk::vk.CmdResetQueryPool
#define vkCmdWriteTimestamp ::rn::vlk::vk.CmdWriteTimestamp
#define vkCmdCopyQueryPoolResults ::rn::vlk::vk.CmdCopyQueryPoolResults
#define vkCmdPushConstants ::rn::vlk::vk.CmdPushConstants
#define vkCmdBeginRenderPass ::rn::vlk::vk.CmdBeginRenderPass
#define vkCmdNextSubpass ::rn::vlk::vk.CmdNextSubpass
#define vkCmdEndRenderPass ::rn::vlk::vk.CmdEndRenderPass
#define vkCmdExecuteCommands ::rn::vlk::vk.CmdExecuteCommands

// VK_KHR_SURFACE_EXTENSION_NAME
#define vkDestroySurfaceKHR ::rn::vlk::vk.DestroySurfaceKHR
#define vkGetPhysicalDeviceSurfaceSupportKHR ::rn::vlk::vk.GetPhysicalDeviceSurfaceSupportKHR
#define vkGetPhysicalDeviceSurfaceCapabilitiesKHR ::rn::vlk::vk.GetPhysicalDeviceSurfaceCapabilitiesKHR
#define vkGetPhysicalDeviceSurfaceFormatsKHR ::rn::vlk::vk.GetPhysicalDeviceSurfaceFormatsKHR
#define vkGetPhysicalDeviceSurfacePresentModesKHR ::rn::vlk::vk.GetPhysicalDeviceSurfacePresentModesKHR
#define vkQueuePresentKHR ::rn::vlk::vk.QueuePresentKHR
#define vkCreateSwapchainKHR ::rn::vlk::vk.CreateSwapchainKHR
#define vkDestroySwapchainKHR ::rn::vlk::vk.DestroySwapchainKHR
#define vkGetSwapchainImagesKHR ::rn::vlk::vk.GetSwapchainImagesKHR
#define vkAcquireNextImageKHR ::rn::vlk::vk.AcquireNextImageKHR

// VK_EXT_debug_report
#define vkCreateDebugReportCallbackEXT ::rn::vlk::vk.CreateDebugReportCallbackEXT
#define vkDestroyDebugReportCallbackEXT ::rn::vlk::vk.DestroyDebugReportCallbackEXT
#define vkDebugReportMessageEXT ::rn::vlk::vk.DebugReportMessageEXT

// VK_EXT_debug_marker
#define vkDebugMarkerSetObjectTagEXT ::rn::vlk::vk.DebugMarkerSetObjectTagEXT
#define vkDebugMarkerSetObjectNameEXT ::rn::vlk::vk.DebugMarkerSetObjectNameEXT
#define vkCmdDebugMarkerBeginEXT ::rn::vlk::vk.CmdDebugMarkerBeginEXT
#define vkCmdDebugMarkerEndEXT ::rn::vlk::vk.CmdDebugMarkerEndEXT
#define vkCmdDebugMarkerInsertEXT ::rn::vlk::vk.CmdDebugMarkerInsertEXT

} // vlk

} // rn
