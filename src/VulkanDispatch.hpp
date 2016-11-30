#pragma once

#include <cassert>
#include <functional>

#include <vulkan/vulkan.h>

struct VulkanDispatch {
	PFN_vkCreateInstance vkCreateInstance = nullptr;
	PFN_vkDestroyInstance vkDestroyInstance = nullptr;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
	PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
	PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = nullptr;
	PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = nullptr;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
	PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;
	PFN_vkCreateDevice vkCreateDevice = nullptr;
	PFN_vkDestroyDevice vkDestroyDevice = nullptr;
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
	PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
	PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
	PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties = nullptr;
	PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
	PFN_vkQueueSubmit vkQueueSubmit = nullptr;
	PFN_vkQueueWaitIdle vkQueueWaitIdle = nullptr;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle = nullptr;
	PFN_vkAllocateMemory vkAllocateMemory = nullptr;
	PFN_vkFreeMemory vkFreeMemory = nullptr;
	PFN_vkMapMemory vkMapMemory = nullptr;
	PFN_vkUnmapMemory vkUnmapMemory = nullptr;
	PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges = nullptr;
	PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = nullptr;
	PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment = nullptr;
	PFN_vkBindBufferMemory vkBindBufferMemory = nullptr;
	PFN_vkBindImageMemory vkBindImageMemory = nullptr;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
	PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements = nullptr;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
	PFN_vkQueueBindSparse vkQueueBindSparse = nullptr;
	PFN_vkCreateFence vkCreateFence = nullptr;
	PFN_vkDestroyFence vkDestroyFence = nullptr;
	PFN_vkResetFences vkResetFences = nullptr;
	PFN_vkGetFenceStatus vkGetFenceStatus = nullptr;
	PFN_vkWaitForFences vkWaitForFences = nullptr;
	PFN_vkCreateSemaphore vkCreateSemaphore = nullptr;
	PFN_vkDestroySemaphore vkDestroySemaphore = nullptr;
	PFN_vkCreateEvent vkCreateEvent = nullptr;
	PFN_vkDestroyEvent vkDestroyEvent = nullptr;
	PFN_vkGetEventStatus vkGetEventStatus = nullptr;
	PFN_vkSetEvent vkSetEvent = nullptr;
	PFN_vkResetEvent vkResetEvent = nullptr;
	PFN_vkCreateQueryPool vkCreateQueryPool = nullptr;
	PFN_vkDestroyQueryPool vkDestroyQueryPool = nullptr;
	PFN_vkGetQueryPoolResults vkGetQueryPoolResults = nullptr;
	PFN_vkCreateBuffer vkCreateBuffer = nullptr;
	PFN_vkDestroyBuffer vkDestroyBuffer = nullptr;
	PFN_vkCreateBufferView vkCreateBufferView = nullptr;
	PFN_vkDestroyBufferView vkDestroyBufferView = nullptr;
	PFN_vkCreateImage vkCreateImage = nullptr;
	PFN_vkDestroyImage vkDestroyImage = nullptr;
	PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout = nullptr;
	PFN_vkCreateImageView vkCreateImageView = nullptr;
	PFN_vkDestroyImageView vkDestroyImageView = nullptr;
	PFN_vkCreateShaderModule vkCreateShaderModule = nullptr;
	PFN_vkDestroyShaderModule vkDestroyShaderModule = nullptr;
	PFN_vkCreatePipelineCache vkCreatePipelineCache = nullptr;
	PFN_vkDestroyPipelineCache vkDestroyPipelineCache = nullptr;
	PFN_vkGetPipelineCacheData vkGetPipelineCacheData = nullptr;
	PFN_vkMergePipelineCaches vkMergePipelineCaches = nullptr;
	PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = nullptr;
	PFN_vkCreateComputePipelines vkCreateComputePipelines = nullptr;
	PFN_vkDestroyPipeline vkDestroyPipeline = nullptr;
	PFN_vkCreatePipelineLayout vkCreatePipelineLayout = nullptr;
	PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout = nullptr;
	PFN_vkCreateSampler vkCreateSampler = nullptr;
	PFN_vkDestroySampler vkDestroySampler = nullptr;
	PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout = nullptr;
	PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = nullptr;
	PFN_vkCreateDescriptorPool vkCreateDescriptorPool = nullptr;
	PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool = nullptr;
	PFN_vkResetDescriptorPool vkResetDescriptorPool = nullptr;
	PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets = nullptr;
	PFN_vkFreeDescriptorSets vkFreeDescriptorSets = nullptr;
	PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets = nullptr;
	PFN_vkCreateFramebuffer vkCreateFramebuffer = nullptr;
	PFN_vkDestroyFramebuffer vkDestroyFramebuffer = nullptr;
	PFN_vkCreateRenderPass vkCreateRenderPass = nullptr;
	PFN_vkDestroyRenderPass vkDestroyRenderPass = nullptr;
	PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity = nullptr;
	PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
	PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
	PFN_vkResetCommandPool vkResetCommandPool = nullptr;
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers = nullptr;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer = nullptr;
	PFN_vkEndCommandBuffer vkEndCommandBuffer = nullptr;
	PFN_vkResetCommandBuffer vkResetCommandBuffer = nullptr;
	PFN_vkCmdBindPipeline vkCmdBindPipeline = nullptr;
	PFN_vkCmdSetViewport vkCmdSetViewport = nullptr;
	PFN_vkCmdSetScissor vkCmdSetScissor = nullptr;
	PFN_vkCmdSetLineWidth vkCmdSetLineWidth = nullptr;
	PFN_vkCmdSetDepthBias vkCmdSetDepthBias = nullptr;
	PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants = nullptr;
	PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds = nullptr;
	PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask = nullptr;
	PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask = nullptr;
	PFN_vkCmdSetStencilReference vkCmdSetStencilReference = nullptr;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = nullptr;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = nullptr;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = nullptr;
	PFN_vkCmdDraw vkCmdDraw = nullptr;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed = nullptr;
	PFN_vkCmdDrawIndirect vkCmdDrawIndirect = nullptr;
	PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect = nullptr;
	PFN_vkCmdDispatch vkCmdDispatch = nullptr;
	PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect = nullptr;
	PFN_vkCmdCopyBuffer vkCmdCopyBuffer = nullptr;
	PFN_vkCmdCopyImage vkCmdCopyImage = nullptr;
	PFN_vkCmdBlitImage vkCmdBlitImage = nullptr;
	PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage = nullptr;
	PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer = nullptr;
	PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer = nullptr;
	PFN_vkCmdFillBuffer vkCmdFillBuffer = nullptr;
	PFN_vkCmdClearColorImage vkCmdClearColorImage = nullptr;
	PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage = nullptr;
	PFN_vkCmdClearAttachments vkCmdClearAttachments = nullptr;
	PFN_vkCmdResolveImage vkCmdResolveImage = nullptr;
	PFN_vkCmdSetEvent vkCmdSetEvent = nullptr;
	PFN_vkCmdResetEvent vkCmdResetEvent = nullptr;
	PFN_vkCmdWaitEvents vkCmdWaitEvents = nullptr;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier = nullptr;
	PFN_vkCmdBeginQuery vkCmdBeginQuery = nullptr;
	PFN_vkCmdEndQuery vkCmdEndQuery = nullptr;
	PFN_vkCmdResetQueryPool vkCmdResetQueryPool = nullptr;
	PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp = nullptr;
	PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults = nullptr;
	PFN_vkCmdPushConstants vkCmdPushConstants = nullptr;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass = nullptr;
	PFN_vkCmdNextSubpass vkCmdNextSubpass = nullptr;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass = nullptr;
	PFN_vkCmdExecuteCommands vkCmdExecuteCommands = nullptr;

	// VK_KHR_SURFACE_EXTENSION_NAME
	PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
	PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = nullptr;

	// VK_EXT_debug_report
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = nullptr;

	explicit VulkanDispatch(std::function<void *()> vkGetInstanceProcAddrLoader) {
		vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(vkGetInstanceProcAddrLoader());
	}

	template<typename T>
	void loadGlobalProc(T &func, const char *name) {
		if (this->vkGetInstanceProcAddr == nullptr) {
			func = nullptr;
			return;
		}

		func = reinterpret_cast<T>(this->vkGetInstanceProcAddr(nullptr, name));
	}

	template<typename T>
	void loadInstanceProc(VkInstance instance, T &func, const char *name) {
		if (this->vkGetInstanceProcAddr == nullptr) {
			func = nullptr;
			return;
		}

		func = reinterpret_cast<T>(this->vkGetInstanceProcAddr(instance, name));
	}

	template<typename T>
	void loadDeviceProc(VkDevice device, T &func, const char *name) {
		if (device == VK_NULL_HANDLE) {
			func = nullptr;
			return;
		}

		if (this->vkGetDeviceProcAddr == nullptr) {
			func = nullptr;
			return;
		}

		func = reinterpret_cast<T>(this->vkGetDeviceProcAddr(device, name));
	}

	void loadGlobalVulkanFunctions() {
		loadGlobalProc(this->vkCreateInstance, "vkCreateInstance");
		loadGlobalProc(this->vkEnumerateInstanceExtensionProperties, "vkEnumerateInstanceExtensionProperties");
		loadGlobalProc(this->vkEnumerateInstanceLayerProperties, "vkEnumerateInstanceLayerProperties");

		assert(this->vkCreateInstance);
		assert(this->vkEnumerateInstanceExtensionProperties);
		assert(this->vkEnumerateInstanceLayerProperties);
	}

	void loadInstanceVulkanFunctions(VkInstance instance) {
		loadInstanceProc(instance, this->vkCreateDevice, "vkCreateDevice");
		loadInstanceProc(instance, this->vkDestroyInstance, "vkDestroyInstance");
		loadInstanceProc(instance, this->vkEnumerateDeviceExtensionProperties, "vkEnumerateDeviceExtensionProperties");
		loadInstanceProc(instance, this->vkEnumeratePhysicalDevices, "vkEnumeratePhysicalDevices");
		loadInstanceProc(instance, this->vkGetDeviceProcAddr, "vkGetDeviceProcAddr");
		loadInstanceProc(instance, this->vkGetInstanceProcAddr, "vkGetInstanceProcAddr");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceFeatures, "vkGetPhysicalDeviceFeatures");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceProperties, "vkGetPhysicalDeviceProperties");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceQueueFamilyProperties, "vkGetPhysicalDeviceQueueFamilyProperties");
		loadInstanceProc(instance, this->vkBeginCommandBuffer, "vkBeginCommandBuffer");
		loadInstanceProc(instance, this->vkCmdBeginQuery, "vkCmdBeginQuery");
		loadInstanceProc(instance, this->vkCmdBeginRenderPass, "vkCmdBeginRenderPass");
		loadInstanceProc(instance, this->vkCmdBindDescriptorSets, "vkCmdBindDescriptorSets");
		loadInstanceProc(instance, this->vkCmdBindIndexBuffer, "vkCmdBindIndexBuffer");
		loadInstanceProc(instance, this->vkCmdBindPipeline, "vkCmdBindPipeline");
		loadInstanceProc(instance, this->vkCmdBindVertexBuffers, "vkCmdBindVertexBuffers");
		loadInstanceProc(instance, this->vkCmdBlitImage, "vkCmdBlitImage");
		loadInstanceProc(instance, this->vkCmdClearAttachments, "vkCmdClearAttachments");
		loadInstanceProc(instance, this->vkCmdClearColorImage, "vkCmdClearColorImage");
		loadInstanceProc(instance, this->vkCmdClearDepthStencilImage, "vkCmdClearDepthStencilImage");
		loadInstanceProc(instance, this->vkCmdCopyBuffer, "vkCmdCopyBuffer");
		loadInstanceProc(instance, this->vkCmdCopyBufferToImage, "vkCmdCopyBufferToImage");
		loadInstanceProc(instance, this->vkCmdCopyImage, "vkCmdCopyImage");
		loadInstanceProc(instance, this->vkCmdCopyImageToBuffer, "vkCmdCopyImageToBuffer");
		loadInstanceProc(instance, this->vkCmdCopyQueryPoolResults, "vkCmdCopyQueryPoolResults");
		loadInstanceProc(instance, this->vkCmdDispatch, "vkCmdDispatch");
		loadInstanceProc(instance, this->vkCmdDispatchIndirect, "vkCmdDispatchIndirect");
		loadInstanceProc(instance, this->vkCmdDraw, "vkCmdDraw");
		loadInstanceProc(instance, this->vkCmdDrawIndexed, "vkCmdDrawIndexed");
		loadInstanceProc(instance, this->vkCmdDrawIndexedIndirect, "vkCmdDrawIndexedIndirect");
		loadInstanceProc(instance, this->vkCmdDrawIndirect, "vkCmdDrawIndirect");
		loadInstanceProc(instance, this->vkCmdEndQuery, "vkCmdEndQuery");
		loadInstanceProc(instance, this->vkCmdEndRenderPass, "vkCmdEndRenderPass");
		loadInstanceProc(instance, this->vkCmdExecuteCommands, "vkCmdExecuteCommands");
		loadInstanceProc(instance, this->vkCmdFillBuffer, "vkCmdFillBuffer");
		loadInstanceProc(instance, this->vkCmdNextSubpass, "vkCmdNextSubpass");
		loadInstanceProc(instance, this->vkCmdPipelineBarrier, "vkCmdPipelineBarrier");
		loadInstanceProc(instance, this->vkCmdPushConstants, "vkCmdPushConstants");
		loadInstanceProc(instance, this->vkCmdResetEvent, "vkCmdResetEvent");
		loadInstanceProc(instance, this->vkCmdResetQueryPool, "vkCmdResetQueryPool");
		loadInstanceProc(instance, this->vkCmdResolveImage, "vkCmdResolveImage");
		loadInstanceProc(instance, this->vkCmdSetBlendConstants, "vkCmdSetBlendConstants");
		loadInstanceProc(instance, this->vkCmdSetDepthBias, "vkCmdSetDepthBias");
		loadInstanceProc(instance, this->vkCmdSetDepthBounds, "vkCmdSetDepthBounds");
		loadInstanceProc(instance, this->vkCmdSetEvent, "vkCmdSetEvent");
		loadInstanceProc(instance, this->vkCmdSetLineWidth, "vkCmdSetLineWidth");
		loadInstanceProc(instance, this->vkCmdSetScissor, "vkCmdSetScissor");
		loadInstanceProc(instance, this->vkCmdSetStencilCompareMask, "vkCmdSetStencilCompareMask");
		loadInstanceProc(instance, this->vkCmdSetStencilReference, "vkCmdSetStencilReference");
		loadInstanceProc(instance, this->vkCmdSetStencilWriteMask, "vkCmdSetStencilWriteMask");
		loadInstanceProc(instance, this->vkCmdSetViewport, "vkCmdSetViewport");
		loadInstanceProc(instance, this->vkCmdUpdateBuffer, "vkCmdUpdateBuffer");
		loadInstanceProc(instance, this->vkCmdWaitEvents, "vkCmdWaitEvents");
		loadInstanceProc(instance, this->vkCmdWriteTimestamp, "vkCmdWriteTimestamp");
		loadInstanceProc(instance, this->vkEndCommandBuffer, "vkEndCommandBuffer");
		loadInstanceProc(instance, this->vkEnumerateDeviceLayerProperties, "vkEnumerateDeviceLayerProperties");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceFormatProperties, "vkGetPhysicalDeviceFormatProperties");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceImageFormatProperties, "vkGetPhysicalDeviceImageFormatProperties");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceMemoryProperties, "vkGetPhysicalDeviceMemoryProperties");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceSparseImageFormatProperties, "vkGetPhysicalDeviceSparseImageFormatProperties");
		loadInstanceProc(instance, this->vkQueueBindSparse, "vkQueueBindSparse");
		loadInstanceProc(instance, this->vkQueueSubmit, "vkQueueSubmit");
		loadInstanceProc(instance, this->vkQueueWaitIdle, "vkQueueWaitIdle");
		loadInstanceProc(instance, this->vkResetCommandBuffer, "vkResetCommandBuffer");

		loadInstanceProc(instance, this->vkDestroySurfaceKHR, "vkDestroySurfaceKHR");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceSurfaceSupportKHR, "vkGetPhysicalDeviceSurfaceSupportKHR");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceSurfaceFormatsKHR, "vkGetPhysicalDeviceSurfaceFormatsKHR");
		loadInstanceProc(instance, this->vkGetPhysicalDeviceSurfacePresentModesKHR, "vkGetPhysicalDeviceSurfacePresentModesKHR");

		loadInstanceProc(instance, this->vkCreateDebugReportCallbackEXT, "vkCreateDebugReportCallbackEXT");
		loadInstanceProc(instance, this->vkDestroyDebugReportCallbackEXT, "vkDestroyDebugReportCallbackEXT");
		loadInstanceProc(instance, this->vkDebugReportMessageEXT, "vkDebugReportMessageEXT");

		assert(this->vkCreateDevice);
		assert(this->vkDestroyInstance);
		assert(this->vkEnumerateDeviceExtensionProperties);
		assert(this->vkEnumeratePhysicalDevices);
		assert(this->vkGetDeviceProcAddr);
		assert(this->vkGetInstanceProcAddr);
		assert(this->vkGetPhysicalDeviceFeatures);
		assert(this->vkGetPhysicalDeviceProperties);
		assert(this->vkGetPhysicalDeviceQueueFamilyProperties);
		assert(this->vkBeginCommandBuffer);
		assert(this->vkCmdBeginQuery);
		assert(this->vkCmdBeginRenderPass);
		assert(this->vkCmdBindDescriptorSets);
		assert(this->vkCmdBindIndexBuffer);
		assert(this->vkCmdBindPipeline);
		assert(this->vkCmdBindVertexBuffers);
		assert(this->vkCmdBlitImage);
		assert(this->vkCmdClearAttachments);
		assert(this->vkCmdClearColorImage);
		assert(this->vkCmdClearDepthStencilImage);
		assert(this->vkCmdCopyBuffer);
		assert(this->vkCmdCopyBufferToImage);
		assert(this->vkCmdCopyImage);
		assert(this->vkCmdCopyImageToBuffer);
		assert(this->vkCmdCopyQueryPoolResults);
		assert(this->vkCmdDispatch);
		assert(this->vkCmdDispatchIndirect);
		assert(this->vkCmdDraw);
		assert(this->vkCmdDrawIndexed);
		assert(this->vkCmdDrawIndexedIndirect);
		assert(this->vkCmdDrawIndirect);
		assert(this->vkCmdEndQuery);
		assert(this->vkCmdEndRenderPass);
		assert(this->vkCmdExecuteCommands);
		assert(this->vkCmdFillBuffer);
		assert(this->vkCmdNextSubpass);
		assert(this->vkCmdPipelineBarrier);
		assert(this->vkCmdPushConstants);
		assert(this->vkCmdResetEvent);
		assert(this->vkCmdResetQueryPool);
		assert(this->vkCmdResolveImage);
		assert(this->vkCmdSetBlendConstants);
		assert(this->vkCmdSetDepthBias);
		assert(this->vkCmdSetDepthBounds);
		assert(this->vkCmdSetEvent);
		assert(this->vkCmdSetLineWidth);
		assert(this->vkCmdSetScissor);
		assert(this->vkCmdSetStencilCompareMask);
		assert(this->vkCmdSetStencilReference);
		assert(this->vkCmdSetStencilWriteMask);
		assert(this->vkCmdSetViewport);
		assert(this->vkCmdUpdateBuffer);
		assert(this->vkCmdWaitEvents);
		assert(this->vkCmdWriteTimestamp);
		assert(this->vkEndCommandBuffer);
		assert(this->vkEnumerateDeviceLayerProperties);
		assert(this->vkGetPhysicalDeviceFormatProperties);
		assert(this->vkGetPhysicalDeviceImageFormatProperties);
		assert(this->vkGetPhysicalDeviceMemoryProperties);
		assert(this->vkGetPhysicalDeviceSparseImageFormatProperties);
		assert(this->vkQueueBindSparse);
		assert(this->vkQueueSubmit);
		assert(this->vkQueueWaitIdle);
		assert(this->vkResetCommandBuffer);

		assert(this->vkDestroySurfaceKHR);
		assert(this->vkGetPhysicalDeviceSurfaceSupportKHR);
		assert(this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		assert(this->vkGetPhysicalDeviceSurfaceFormatsKHR);
		assert(this->vkGetPhysicalDeviceSurfacePresentModesKHR);

		assert(this->vkCreateDebugReportCallbackEXT);
		assert(this->vkDestroyDebugReportCallbackEXT);
		assert(this->vkDebugReportMessageEXT);
	}

	void unloadInstanceVulkanFunctions() {
		this->vkCreateDevice = nullptr;
		this->vkDestroyInstance = nullptr;
		this->vkEnumerateDeviceExtensionProperties = nullptr;
		this->vkEnumeratePhysicalDevices = nullptr;
		this->vkGetDeviceProcAddr = nullptr;
		this->vkGetInstanceProcAddr = nullptr;
		this->vkGetPhysicalDeviceFeatures = nullptr;
		this->vkGetPhysicalDeviceProperties = nullptr;
		this->vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
		this->vkBeginCommandBuffer = nullptr;
		this->vkCmdBeginQuery = nullptr;
		this->vkCmdBeginRenderPass = nullptr;
		this->vkCmdBindDescriptorSets = nullptr;
		this->vkCmdBindIndexBuffer = nullptr;
		this->vkCmdBindPipeline = nullptr;
		this->vkCmdBindVertexBuffers = nullptr;
		this->vkCmdBlitImage = nullptr;
		this->vkCmdClearAttachments = nullptr;
		this->vkCmdClearColorImage = nullptr;
		this->vkCmdClearDepthStencilImage = nullptr;
		this->vkCmdCopyBuffer = nullptr;
		this->vkCmdCopyBufferToImage = nullptr;
		this->vkCmdCopyImage = nullptr;
		this->vkCmdCopyImageToBuffer = nullptr;
		this->vkCmdCopyQueryPoolResults = nullptr;
		this->vkCmdDispatch = nullptr;
		this->vkCmdDispatchIndirect = nullptr;
		this->vkCmdDraw = nullptr;
		this->vkCmdDrawIndexed = nullptr;
		this->vkCmdDrawIndexedIndirect = nullptr;
		this->vkCmdDrawIndirect = nullptr;
		this->vkCmdEndQuery = nullptr;
		this->vkCmdEndRenderPass = nullptr;
		this->vkCmdExecuteCommands = nullptr;
		this->vkCmdFillBuffer = nullptr;
		this->vkCmdNextSubpass = nullptr;
		this->vkCmdPipelineBarrier = nullptr;
		this->vkCmdPushConstants = nullptr;
		this->vkCmdResetEvent = nullptr;
		this->vkCmdResetQueryPool = nullptr;
		this->vkCmdResolveImage = nullptr;
		this->vkCmdSetBlendConstants = nullptr;
		this->vkCmdSetDepthBias = nullptr;
		this->vkCmdSetDepthBounds = nullptr;
		this->vkCmdSetEvent = nullptr;
		this->vkCmdSetLineWidth = nullptr;
		this->vkCmdSetScissor = nullptr;
		this->vkCmdSetStencilCompareMask = nullptr;
		this->vkCmdSetStencilReference = nullptr;
		this->vkCmdSetStencilWriteMask = nullptr;
		this->vkCmdSetViewport = nullptr;
		this->vkCmdUpdateBuffer = nullptr;
		this->vkCmdWaitEvents = nullptr;
		this->vkCmdWriteTimestamp = nullptr;
		this->vkEndCommandBuffer = nullptr;
		this->vkEnumerateDeviceLayerProperties = nullptr;
		this->vkGetPhysicalDeviceFormatProperties = nullptr;
		this->vkGetPhysicalDeviceImageFormatProperties = nullptr;
		this->vkGetPhysicalDeviceMemoryProperties = nullptr;
		this->vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
		this->vkQueueBindSparse = nullptr;
		this->vkQueueSubmit = nullptr;
		this->vkQueueWaitIdle = nullptr;
		this->vkResetCommandBuffer = nullptr;

		this->vkDestroySurfaceKHR = nullptr;
		this->vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
		this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
		this->vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
		this->vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;

		this->vkCreateDebugReportCallbackEXT = nullptr;
		this->vkDestroyDebugReportCallbackEXT = nullptr;
		this->vkDebugReportMessageEXT = nullptr;
	}

	void loadDeviceVulkanFunctions(VkDevice device) {
		loadDeviceProc(device, this->vkDestroyDevice, "vkDestroyDevice");
		loadDeviceProc(device, this->vkGetDeviceQueue, "vkGetDeviceQueue");
		loadDeviceProc(device, this->vkDeviceWaitIdle, "vkDeviceWaitIdle");
		loadDeviceProc(device, this->vkAllocateMemory, "vkAllocateMemory");
		loadDeviceProc(device, this->vkFreeMemory, "vkFreeMemory");
		loadDeviceProc(device, this->vkMapMemory, "vkMapMemory");
		loadDeviceProc(device, this->vkUnmapMemory, "vkUnmapMemory");
		loadDeviceProc(device, this->vkFlushMappedMemoryRanges, "vkFlushMappedMemoryRanges");
		loadDeviceProc(device, this->vkInvalidateMappedMemoryRanges, "vkInvalidateMappedMemoryRanges");
		loadDeviceProc(device, this->vkGetDeviceMemoryCommitment, "vkGetDeviceMemoryCommitment");
		loadDeviceProc(device, this->vkBindBufferMemory, "vkBindBufferMemory");
		loadDeviceProc(device, this->vkBindImageMemory, "vkBindImageMemory");
		loadDeviceProc(device, this->vkGetBufferMemoryRequirements, "vkGetBufferMemoryRequirements");
		loadDeviceProc(device, this->vkGetImageMemoryRequirements, "vkGetImageMemoryRequirements");
		loadDeviceProc(device, this->vkGetImageSparseMemoryRequirements, "vkGetImageSparseMemoryRequirements");
		loadDeviceProc(device, this->vkCreateFence, "vkCreateFence");
		loadDeviceProc(device, this->vkDestroyFence, "vkDestroyFence");
		loadDeviceProc(device, this->vkResetFences, "vkResetFences");
		loadDeviceProc(device, this->vkGetFenceStatus, "vkGetFenceStatus");
		loadDeviceProc(device, this->vkWaitForFences, "vkWaitForFences");
		loadDeviceProc(device, this->vkCreateSemaphore, "vkCreateSemaphore");
		loadDeviceProc(device, this->vkDestroySemaphore, "vkDestroySemaphore");
		loadDeviceProc(device, this->vkCreateEvent, "vkCreateEvent");
		loadDeviceProc(device, this->vkDestroyEvent, "vkDestroyEvent");
		loadDeviceProc(device, this->vkGetEventStatus, "vkGetEventStatus");
		loadDeviceProc(device, this->vkSetEvent, "vkSetEvent");
		loadDeviceProc(device, this->vkResetEvent, "vkResetEvent");
		loadDeviceProc(device, this->vkCreateQueryPool, "vkCreateQueryPool");
		loadDeviceProc(device, this->vkDestroyQueryPool, "vkDestroyQueryPool");
		loadDeviceProc(device, this->vkGetQueryPoolResults, "vkGetQueryPoolResults");
		loadDeviceProc(device, this->vkCreateBuffer, "vkCreateBuffer");
		loadDeviceProc(device, this->vkDestroyBuffer, "vkDestroyBuffer");
		loadDeviceProc(device, this->vkCreateBufferView, "vkCreateBufferView");
		loadDeviceProc(device, this->vkDestroyBufferView, "vkDestroyBufferView");
		loadDeviceProc(device, this->vkCreateImage, "vkCreateImage");
		loadDeviceProc(device, this->vkDestroyImage, "vkDestroyImage");
		loadDeviceProc(device, this->vkGetImageSubresourceLayout, "vkGetImageSubresourceLayout");
		loadDeviceProc(device, this->vkCreateImageView, "vkCreateImageView");
		loadDeviceProc(device, this->vkDestroyImageView, "vkDestroyImageView");
		loadDeviceProc(device, this->vkCreateShaderModule, "vkCreateShaderModule");
		loadDeviceProc(device, this->vkDestroyShaderModule, "vkDestroyShaderModule");
		loadDeviceProc(device, this->vkCreatePipelineCache, "vkCreatePipelineCache");
		loadDeviceProc(device, this->vkDestroyPipelineCache, "vkDestroyPipelineCache");
		loadDeviceProc(device, this->vkGetPipelineCacheData, "vkGetPipelineCacheData");
		loadDeviceProc(device, this->vkMergePipelineCaches, "vkMergePipelineCaches");
		loadDeviceProc(device, this->vkCreateGraphicsPipelines, "vkCreateGraphicsPipelines");
		loadDeviceProc(device, this->vkCreateComputePipelines, "vkCreateComputePipelines");
		loadDeviceProc(device, this->vkDestroyPipeline, "vkDestroyPipeline");
		loadDeviceProc(device, this->vkCreatePipelineLayout, "vkCreatePipelineLayout");
		loadDeviceProc(device, this->vkDestroyPipelineLayout, "vkDestroyPipelineLayout");
		loadDeviceProc(device, this->vkCreateSampler, "vkCreateSampler");
		loadDeviceProc(device, this->vkDestroySampler, "vkDestroySampler");
		loadDeviceProc(device, this->vkCreateDescriptorSetLayout, "vkCreateDescriptorSetLayout");
		loadDeviceProc(device, this->vkDestroyDescriptorSetLayout, "vkDestroyDescriptorSetLayout");
		loadDeviceProc(device, this->vkCreateDescriptorPool, "vkCreateDescriptorPool");
		loadDeviceProc(device, this->vkDestroyDescriptorPool, "vkDestroyDescriptorPool");
		loadDeviceProc(device, this->vkResetDescriptorPool, "vkResetDescriptorPool");
		loadDeviceProc(device, this->vkAllocateDescriptorSets, "vkAllocateDescriptorSets");
		loadDeviceProc(device, this->vkFreeDescriptorSets, "vkFreeDescriptorSets");
		loadDeviceProc(device, this->vkUpdateDescriptorSets, "vkUpdateDescriptorSets");
		loadDeviceProc(device, this->vkCreateFramebuffer, "vkCreateFramebuffer");
		loadDeviceProc(device, this->vkDestroyFramebuffer, "vkDestroyFramebuffer");
		loadDeviceProc(device, this->vkCreateRenderPass, "vkCreateRenderPass");
		loadDeviceProc(device, this->vkDestroyRenderPass, "vkDestroyRenderPass");
		loadDeviceProc(device, this->vkGetRenderAreaGranularity, "vkGetRenderAreaGranularity");

		loadDeviceProc(device, this->vkCreateCommandPool, "vkCreateCommandPool");
		loadDeviceProc(device, this->vkDestroyCommandPool, "vkDestroyCommandPool");
		loadDeviceProc(device, this->vkResetCommandPool, "vkResetCommandPool");
		loadDeviceProc(device, this->vkAllocateCommandBuffers, "vkAllocateCommandBuffers");
		loadDeviceProc(device, this->vkFreeCommandBuffers, "vkFreeCommandBuffers");

		loadDeviceProc(device, this->vkQueuePresentKHR, "vkQueuePresentKHR");
		loadDeviceProc(device, this->vkCreateSwapchainKHR, "vkCreateSwapchainKHR");
		loadDeviceProc(device, this->vkDestroySwapchainKHR, "vkDestroySwapchainKHR");
		loadDeviceProc(device, this->vkGetSwapchainImagesKHR, "vkGetSwapchainImagesKHR");
		loadDeviceProc(device, this->vkAcquireNextImageKHR, "vkAcquireNextImageKHR");

		assert(this->vkDestroyDevice);
		assert(this->vkGetDeviceQueue);
		assert(this->vkDeviceWaitIdle);
		assert(this->vkAllocateMemory);
		assert(this->vkFreeMemory);
		assert(this->vkMapMemory);
		assert(this->vkUnmapMemory);
		assert(this->vkFlushMappedMemoryRanges);
		assert(this->vkInvalidateMappedMemoryRanges);
		assert(this->vkGetDeviceMemoryCommitment);
		assert(this->vkBindBufferMemory);
		assert(this->vkBindImageMemory);
		assert(this->vkGetBufferMemoryRequirements);
		assert(this->vkGetImageMemoryRequirements);
		assert(this->vkGetImageSparseMemoryRequirements);
		assert(this->vkCreateFence);
		assert(this->vkDestroyFence);
		assert(this->vkResetFences);
		assert(this->vkGetFenceStatus);
		assert(this->vkWaitForFences);
		assert(this->vkCreateSemaphore);
		assert(this->vkDestroySemaphore);
		assert(this->vkCreateEvent);
		assert(this->vkDestroyEvent);
		assert(this->vkGetEventStatus);
		assert(this->vkSetEvent);
		assert(this->vkResetEvent);
		assert(this->vkCreateQueryPool);
		assert(this->vkDestroyQueryPool);
		assert(this->vkGetQueryPoolResults);
		assert(this->vkCreateBuffer);
		assert(this->vkDestroyBuffer);
		assert(this->vkCreateBufferView);
		assert(this->vkDestroyBufferView);
		assert(this->vkCreateImage);
		assert(this->vkDestroyImage);
		assert(this->vkGetImageSubresourceLayout);
		assert(this->vkCreateImageView);
		assert(this->vkDestroyImageView);
		assert(this->vkCreateShaderModule);
		assert(this->vkDestroyShaderModule);
		assert(this->vkCreatePipelineCache);
		assert(this->vkDestroyPipelineCache);
		assert(this->vkGetPipelineCacheData);
		assert(this->vkMergePipelineCaches);
		assert(this->vkCreateGraphicsPipelines);
		assert(this->vkCreateComputePipelines);
		assert(this->vkDestroyPipeline);
		assert(this->vkCreatePipelineLayout);
		assert(this->vkDestroyPipelineLayout);
		assert(this->vkCreateSampler);
		assert(this->vkDestroySampler);
		assert(this->vkCreateDescriptorSetLayout);
		assert(this->vkDestroyDescriptorSetLayout);
		assert(this->vkCreateDescriptorPool);
		assert(this->vkDestroyDescriptorPool);
		assert(this->vkResetDescriptorPool);
		assert(this->vkAllocateDescriptorSets);
		assert(this->vkFreeDescriptorSets);
		assert(this->vkUpdateDescriptorSets);
		assert(this->vkCreateFramebuffer);
		assert(this->vkDestroyFramebuffer);
		assert(this->vkCreateRenderPass);
		assert(this->vkDestroyRenderPass);
		assert(this->vkGetRenderAreaGranularity);
		assert(this->vkCreateCommandPool);
		assert(this->vkDestroyCommandPool);
		assert(this->vkResetCommandPool);
		assert(this->vkAllocateCommandBuffers);
		assert(this->vkFreeCommandBuffers);

		assert(this->vkQueuePresentKHR);
		assert(this->vkCreateSwapchainKHR);
		assert(this->vkDestroySwapchainKHR);
		assert(this->vkGetSwapchainImagesKHR);
		assert(this->vkAcquireNextImageKHR);
	}

	void unloadDeviceVulkanFunctions() {
		this->vkDestroyDevice = nullptr;
		this->vkGetDeviceQueue = nullptr;
		this->vkDeviceWaitIdle = nullptr;
		this->vkAllocateMemory = nullptr;
		this->vkFreeMemory = nullptr;
		this->vkMapMemory = nullptr;
		this->vkUnmapMemory = nullptr;
		this->vkFlushMappedMemoryRanges = nullptr;
		this->vkInvalidateMappedMemoryRanges = nullptr;
		this->vkGetDeviceMemoryCommitment = nullptr;
		this->vkBindBufferMemory = nullptr;
		this->vkBindImageMemory = nullptr;
		this->vkGetBufferMemoryRequirements = nullptr;
		this->vkGetImageMemoryRequirements = nullptr;
		this->vkGetImageSparseMemoryRequirements = nullptr;
		this->vkCreateFence = nullptr;
		this->vkDestroyFence = nullptr;
		this->vkResetFences = nullptr;
		this->vkGetFenceStatus = nullptr;
		this->vkWaitForFences = nullptr;
		this->vkCreateSemaphore = nullptr;
		this->vkDestroySemaphore = nullptr;
		this->vkCreateEvent = nullptr;
		this->vkDestroyEvent = nullptr;
		this->vkGetEventStatus = nullptr;
		this->vkSetEvent = nullptr;
		this->vkResetEvent = nullptr;
		this->vkCreateQueryPool = nullptr;
		this->vkDestroyQueryPool = nullptr;
		this->vkGetQueryPoolResults = nullptr;
		this->vkCreateBuffer = nullptr;
		this->vkDestroyBuffer = nullptr;
		this->vkCreateBufferView = nullptr;
		this->vkDestroyBufferView = nullptr;
		this->vkCreateImage = nullptr;
		this->vkDestroyImage = nullptr;
		this->vkGetImageSubresourceLayout = nullptr;
		this->vkCreateImageView = nullptr;
		this->vkDestroyImageView = nullptr;
		this->vkCreateShaderModule = nullptr;
		this->vkDestroyShaderModule = nullptr;
		this->vkCreatePipelineCache = nullptr;
		this->vkDestroyPipelineCache = nullptr;
		this->vkGetPipelineCacheData = nullptr;
		this->vkMergePipelineCaches = nullptr;
		this->vkCreateGraphicsPipelines = nullptr;
		this->vkCreateComputePipelines = nullptr;
		this->vkDestroyPipeline = nullptr;
		this->vkCreatePipelineLayout = nullptr;
		this->vkDestroyPipelineLayout = nullptr;
		this->vkCreateSampler = nullptr;
		this->vkDestroySampler = nullptr;
		this->vkCreateDescriptorSetLayout = nullptr;
		this->vkDestroyDescriptorSetLayout = nullptr;
		this->vkCreateDescriptorPool = nullptr;
		this->vkDestroyDescriptorPool = nullptr;
		this->vkResetDescriptorPool = nullptr;
		this->vkAllocateDescriptorSets = nullptr;
		this->vkFreeDescriptorSets = nullptr;
		this->vkUpdateDescriptorSets = nullptr;
		this->vkCreateFramebuffer = nullptr;
		this->vkDestroyFramebuffer = nullptr;
		this->vkCreateRenderPass = nullptr;
		this->vkDestroyRenderPass = nullptr;
		this->vkGetRenderAreaGranularity = nullptr;
		this->vkCreateCommandPool = nullptr;
		this->vkDestroyCommandPool = nullptr;
		this->vkResetCommandPool = nullptr;
		this->vkAllocateCommandBuffers = nullptr;
		this->vkFreeCommandBuffers = nullptr;

		this->vkQueuePresentKHR = nullptr;
		this->vkCreateSwapchainKHR = nullptr;
		this->vkDestroySwapchainKHR = nullptr;
		this->vkGetSwapchainImagesKHR = nullptr;
		this->vkAcquireNextImageKHR = nullptr;
	}

};