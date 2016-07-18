#include "interface.hpp"
#include <cassert>

namespace rn {

namespace vlk {

bool Interface::loadGlobalProcs(void (*GetInstanceProcAddr)()) {
	this->GetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetInstanceProcAddr);

	#define LOAD_GLOBAL_PROC(X) do { getGlobalProc(X, "vk"#X); assert(X); if (X == nullptr) { return false; } } while (0)
	#define LOAD_OPT_GLOBAL_PROC(X) do { getGlobalProc(X, "vk"#X); } while (0)

	LOAD_GLOBAL_PROC(CreateInstance);
	LOAD_GLOBAL_PROC(EnumerateInstanceExtensionProperties);
	LOAD_GLOBAL_PROC(EnumerateInstanceLayerProperties);

	#undef LOAD_GLOBAL_PROC
	#undef LOAD_OPT_GLOBAL_PROC

	return true;
}

void Interface::unloadInstanceProcs() {
	currentInstance = VK_NULL_HANDLE;

	#define UNLOAD_INSTANCE_PROC(X) X = nullptr

	UNLOAD_INSTANCE_PROC(CreateDevice);
	UNLOAD_INSTANCE_PROC(DestroyInstance);
	UNLOAD_INSTANCE_PROC(EnumerateDeviceExtensionProperties);
	UNLOAD_INSTANCE_PROC(EnumeratePhysicalDevices);
	UNLOAD_INSTANCE_PROC(GetDeviceProcAddr);
	UNLOAD_INSTANCE_PROC(GetInstanceProcAddr);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceFeatures);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceProperties);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceQueueFamilyProperties);
	UNLOAD_INSTANCE_PROC(BeginCommandBuffer);
	UNLOAD_INSTANCE_PROC(CmdBeginQuery);
	UNLOAD_INSTANCE_PROC(CmdBeginRenderPass);
	UNLOAD_INSTANCE_PROC(CmdBindDescriptorSets);
	UNLOAD_INSTANCE_PROC(CmdBindIndexBuffer);
	UNLOAD_INSTANCE_PROC(CmdBindPipeline);
	UNLOAD_INSTANCE_PROC(CmdBindVertexBuffers);
	UNLOAD_INSTANCE_PROC(CmdBlitImage);
	UNLOAD_INSTANCE_PROC(CmdClearAttachments);
	UNLOAD_INSTANCE_PROC(CmdClearColorImage);
	UNLOAD_INSTANCE_PROC(CmdClearDepthStencilImage);
	UNLOAD_INSTANCE_PROC(CmdCopyBuffer);
	UNLOAD_INSTANCE_PROC(CmdCopyBufferToImage);
	UNLOAD_INSTANCE_PROC(CmdCopyImage);
	UNLOAD_INSTANCE_PROC(CmdCopyImageToBuffer);
	UNLOAD_INSTANCE_PROC(CmdCopyQueryPoolResults);
	UNLOAD_INSTANCE_PROC(CmdDispatch);
	UNLOAD_INSTANCE_PROC(CmdDispatchIndirect);
	UNLOAD_INSTANCE_PROC(CmdDraw);
	UNLOAD_INSTANCE_PROC(CmdDrawIndexed);
	UNLOAD_INSTANCE_PROC(CmdDrawIndexedIndirect);
	UNLOAD_INSTANCE_PROC(CmdDrawIndirect);
	UNLOAD_INSTANCE_PROC(CmdEndQuery);
	UNLOAD_INSTANCE_PROC(CmdEndRenderPass);
	UNLOAD_INSTANCE_PROC(CmdExecuteCommands);
	UNLOAD_INSTANCE_PROC(CmdFillBuffer);
	UNLOAD_INSTANCE_PROC(CmdNextSubpass);
	UNLOAD_INSTANCE_PROC(CmdPipelineBarrier);
	UNLOAD_INSTANCE_PROC(CmdPushConstants);
	UNLOAD_INSTANCE_PROC(CmdResetEvent);
	UNLOAD_INSTANCE_PROC(CmdResetQueryPool);
	UNLOAD_INSTANCE_PROC(CmdResolveImage);
	UNLOAD_INSTANCE_PROC(CmdSetBlendConstants);
	UNLOAD_INSTANCE_PROC(CmdSetDepthBias);
	UNLOAD_INSTANCE_PROC(CmdSetDepthBounds);
	UNLOAD_INSTANCE_PROC(CmdSetEvent);
	UNLOAD_INSTANCE_PROC(CmdSetLineWidth);
	UNLOAD_INSTANCE_PROC(CmdSetScissor);
	UNLOAD_INSTANCE_PROC(CmdSetStencilCompareMask);
	UNLOAD_INSTANCE_PROC(CmdSetStencilReference);
	UNLOAD_INSTANCE_PROC(CmdSetStencilWriteMask);
	UNLOAD_INSTANCE_PROC(CmdSetViewport);
	UNLOAD_INSTANCE_PROC(CmdUpdateBuffer);
	UNLOAD_INSTANCE_PROC(CmdWaitEvents);
	UNLOAD_INSTANCE_PROC(CmdWriteTimestamp);
	UNLOAD_INSTANCE_PROC(EndCommandBuffer);
	UNLOAD_INSTANCE_PROC(EnumerateDeviceLayerProperties);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceFormatProperties);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceImageFormatProperties);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceMemoryProperties);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceSparseImageFormatProperties);
	UNLOAD_INSTANCE_PROC(QueueBindSparse);
	UNLOAD_INSTANCE_PROC(QueueSubmit);
	UNLOAD_INSTANCE_PROC(QueueWaitIdle);
	UNLOAD_INSTANCE_PROC(ResetCommandBuffer);

	UNLOAD_INSTANCE_PROC(DestroySurfaceKHR);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceSurfaceSupportKHR);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceSurfaceCapabilitiesKHR);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceSurfaceFormatsKHR);
	UNLOAD_INSTANCE_PROC(GetPhysicalDeviceSurfacePresentModesKHR);

	UNLOAD_INSTANCE_PROC(CreateDebugReportCallbackEXT);
	UNLOAD_INSTANCE_PROC(DestroyDebugReportCallbackEXT);
	UNLOAD_INSTANCE_PROC(DebugReportMessageEXT);

	UNLOAD_INSTANCE_PROC(DebugMarkerSetObjectTagEXT);
	UNLOAD_INSTANCE_PROC(DebugMarkerSetObjectNameEXT);
	UNLOAD_INSTANCE_PROC(CmdDebugMarkerBeginEXT);
	UNLOAD_INSTANCE_PROC(CmdDebugMarkerEndEXT);
	UNLOAD_INSTANCE_PROC(CmdDebugMarkerInsertEXT);

	#undef UNLOAD_INSTANCE_PROC
}

void Interface::unloadDeviceProcs() {
	currentDevice = VK_NULL_HANDLE;

	#define UNLOAD_DEVICE_PROC(X) X = nullptr

	UNLOAD_DEVICE_PROC(DestroyDevice);
	UNLOAD_DEVICE_PROC(GetDeviceQueue);
	UNLOAD_DEVICE_PROC(DeviceWaitIdle);
	UNLOAD_DEVICE_PROC(AllocateMemory);
	UNLOAD_DEVICE_PROC(FreeMemory);
	UNLOAD_DEVICE_PROC(MapMemory);
	UNLOAD_DEVICE_PROC(UnmapMemory);
	UNLOAD_DEVICE_PROC(FlushMappedMemoryRanges);
	UNLOAD_DEVICE_PROC(InvalidateMappedMemoryRanges);
	UNLOAD_DEVICE_PROC(GetDeviceMemoryCommitment);
	UNLOAD_DEVICE_PROC(BindBufferMemory);
	UNLOAD_DEVICE_PROC(BindImageMemory);
	UNLOAD_DEVICE_PROC(GetBufferMemoryRequirements);
	UNLOAD_DEVICE_PROC(GetImageMemoryRequirements);
	UNLOAD_DEVICE_PROC(GetImageSparseMemoryRequirements);
	UNLOAD_DEVICE_PROC(CreateFence);
	UNLOAD_DEVICE_PROC(DestroyFence);
	UNLOAD_DEVICE_PROC(ResetFences);
	UNLOAD_DEVICE_PROC(GetFenceStatus);
	UNLOAD_DEVICE_PROC(WaitForFences);
	UNLOAD_DEVICE_PROC(CreateSemaphore);
	UNLOAD_DEVICE_PROC(DestroySemaphore);
	UNLOAD_DEVICE_PROC(CreateEvent);
	UNLOAD_DEVICE_PROC(DestroyEvent);
	UNLOAD_DEVICE_PROC(GetEventStatus);
	UNLOAD_DEVICE_PROC(SetEvent);
	UNLOAD_DEVICE_PROC(ResetEvent);
	UNLOAD_DEVICE_PROC(CreateQueryPool);
	UNLOAD_DEVICE_PROC(DestroyQueryPool);
	UNLOAD_DEVICE_PROC(GetQueryPoolResults);
	UNLOAD_DEVICE_PROC(CreateBuffer);
	UNLOAD_DEVICE_PROC(DestroyBuffer);
	UNLOAD_DEVICE_PROC(CreateBufferView);
	UNLOAD_DEVICE_PROC(DestroyBufferView);
	UNLOAD_DEVICE_PROC(CreateImage);
	UNLOAD_DEVICE_PROC(DestroyImage);
	UNLOAD_DEVICE_PROC(GetImageSubresourceLayout);
	UNLOAD_DEVICE_PROC(CreateImageView);
	UNLOAD_DEVICE_PROC(DestroyImageView);
	UNLOAD_DEVICE_PROC(CreateShaderModule);
	UNLOAD_DEVICE_PROC(DestroyShaderModule);
	UNLOAD_DEVICE_PROC(CreatePipelineCache);
	UNLOAD_DEVICE_PROC(DestroyPipelineCache);
	UNLOAD_DEVICE_PROC(GetPipelineCacheData);
	UNLOAD_DEVICE_PROC(MergePipelineCaches);
	UNLOAD_DEVICE_PROC(CreateGraphicsPipelines);
	UNLOAD_DEVICE_PROC(CreateComputePipelines);
	UNLOAD_DEVICE_PROC(DestroyPipeline);
	UNLOAD_DEVICE_PROC(CreatePipelineLayout);
	UNLOAD_DEVICE_PROC(DestroyPipelineLayout);
	UNLOAD_DEVICE_PROC(CreateSampler);
	UNLOAD_DEVICE_PROC(DestroySampler);
	UNLOAD_DEVICE_PROC(CreateDescriptorSetLayout);
	UNLOAD_DEVICE_PROC(DestroyDescriptorSetLayout);
	UNLOAD_DEVICE_PROC(CreateDescriptorPool);
	UNLOAD_DEVICE_PROC(DestroyDescriptorPool);
	UNLOAD_DEVICE_PROC(ResetDescriptorPool);
	UNLOAD_DEVICE_PROC(AllocateDescriptorSets);
	UNLOAD_DEVICE_PROC(FreeDescriptorSets);
	UNLOAD_DEVICE_PROC(UpdateDescriptorSets);
	UNLOAD_DEVICE_PROC(CreateFramebuffer);
	UNLOAD_DEVICE_PROC(DestroyFramebuffer);
	UNLOAD_DEVICE_PROC(CreateRenderPass);
	UNLOAD_DEVICE_PROC(DestroyRenderPass);
	UNLOAD_DEVICE_PROC(GetRenderAreaGranularity);

	UNLOAD_DEVICE_PROC(CreateCommandPool);
	UNLOAD_DEVICE_PROC(DestroyCommandPool);
	UNLOAD_DEVICE_PROC(ResetCommandPool);
	UNLOAD_DEVICE_PROC(AllocateCommandBuffers);
	UNLOAD_DEVICE_PROC(FreeCommandBuffers);

	UNLOAD_DEVICE_PROC(QueuePresentKHR);
	UNLOAD_DEVICE_PROC(CreateSwapchainKHR);
	UNLOAD_DEVICE_PROC(DestroySwapchainKHR);
	UNLOAD_DEVICE_PROC(GetSwapchainImagesKHR);
	UNLOAD_DEVICE_PROC(AcquireNextImageKHR);

	#undef UNLOAD_DEVICE_PROC
}

bool Interface::loadInstanceProcs(VkInstance instance) {
	if (currentInstance != VK_NULL_HANDLE && currentInstance != instance) {
		unloadInstanceProcs();
	}

	currentInstance = instance;

	#define LOAD_INSTANCE_PROC(X) do { getInstanceProc(instance, X, "vk"#X); assert(X); if (X == nullptr) { return false; } } while (0)
	#define LOAD_OPT_INSTANCE_PROC(X) do { getInstanceProc(instance, X, "vk"#X); } while (0)

	LOAD_INSTANCE_PROC(CreateDevice);
	LOAD_INSTANCE_PROC(DestroyInstance);
	LOAD_INSTANCE_PROC(EnumerateDeviceExtensionProperties);
	LOAD_INSTANCE_PROC(EnumeratePhysicalDevices);
	LOAD_INSTANCE_PROC(GetDeviceProcAddr);
	LOAD_INSTANCE_PROC(GetInstanceProcAddr);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceFeatures);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceProperties);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceQueueFamilyProperties);
	LOAD_INSTANCE_PROC(BeginCommandBuffer);
	LOAD_INSTANCE_PROC(CmdBeginQuery);
	LOAD_INSTANCE_PROC(CmdBeginRenderPass);
	LOAD_INSTANCE_PROC(CmdBindDescriptorSets);
	LOAD_INSTANCE_PROC(CmdBindIndexBuffer);
	LOAD_INSTANCE_PROC(CmdBindPipeline);
	LOAD_INSTANCE_PROC(CmdBindVertexBuffers);
	LOAD_INSTANCE_PROC(CmdBlitImage);
	LOAD_INSTANCE_PROC(CmdClearAttachments);
	LOAD_INSTANCE_PROC(CmdClearColorImage);
	LOAD_INSTANCE_PROC(CmdClearDepthStencilImage);
	LOAD_INSTANCE_PROC(CmdCopyBuffer);
	LOAD_INSTANCE_PROC(CmdCopyBufferToImage);
	LOAD_INSTANCE_PROC(CmdCopyImage);
	LOAD_INSTANCE_PROC(CmdCopyImageToBuffer);
	LOAD_INSTANCE_PROC(CmdCopyQueryPoolResults);
	LOAD_INSTANCE_PROC(CmdDispatch);
	LOAD_INSTANCE_PROC(CmdDispatchIndirect);
	LOAD_INSTANCE_PROC(CmdDraw);
	LOAD_INSTANCE_PROC(CmdDrawIndexed);
	LOAD_INSTANCE_PROC(CmdDrawIndexedIndirect);
	LOAD_INSTANCE_PROC(CmdDrawIndirect);
	LOAD_INSTANCE_PROC(CmdEndQuery);
	LOAD_INSTANCE_PROC(CmdEndRenderPass);
	LOAD_INSTANCE_PROC(CmdExecuteCommands);
	LOAD_INSTANCE_PROC(CmdFillBuffer);
	LOAD_INSTANCE_PROC(CmdNextSubpass);
	LOAD_INSTANCE_PROC(CmdPipelineBarrier);
	LOAD_INSTANCE_PROC(CmdPushConstants);
	LOAD_INSTANCE_PROC(CmdResetEvent);
	LOAD_INSTANCE_PROC(CmdResetQueryPool);
	LOAD_INSTANCE_PROC(CmdResolveImage);
	LOAD_INSTANCE_PROC(CmdSetBlendConstants);
	LOAD_INSTANCE_PROC(CmdSetDepthBias);
	LOAD_INSTANCE_PROC(CmdSetDepthBounds);
	LOAD_INSTANCE_PROC(CmdSetEvent);
	LOAD_INSTANCE_PROC(CmdSetLineWidth);
	LOAD_INSTANCE_PROC(CmdSetScissor);
	LOAD_INSTANCE_PROC(CmdSetStencilCompareMask);
	LOAD_INSTANCE_PROC(CmdSetStencilReference);
	LOAD_INSTANCE_PROC(CmdSetStencilWriteMask);
	LOAD_INSTANCE_PROC(CmdSetViewport);
	LOAD_INSTANCE_PROC(CmdUpdateBuffer);
	LOAD_INSTANCE_PROC(CmdWaitEvents);
	LOAD_INSTANCE_PROC(CmdWriteTimestamp);
	LOAD_INSTANCE_PROC(EndCommandBuffer);
	LOAD_INSTANCE_PROC(EnumerateDeviceLayerProperties);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceFormatProperties);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceImageFormatProperties);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceMemoryProperties);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceSparseImageFormatProperties);
	LOAD_INSTANCE_PROC(QueueBindSparse);
	LOAD_INSTANCE_PROC(QueueSubmit);
	LOAD_INSTANCE_PROC(QueueWaitIdle);
	LOAD_INSTANCE_PROC(ResetCommandBuffer);

	LOAD_INSTANCE_PROC(DestroySurfaceKHR);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceSurfaceSupportKHR);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceSurfaceCapabilitiesKHR);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceSurfaceFormatsKHR);
	LOAD_INSTANCE_PROC(GetPhysicalDeviceSurfacePresentModesKHR);

	LOAD_OPT_INSTANCE_PROC(CreateDebugReportCallbackEXT);
	LOAD_OPT_INSTANCE_PROC(DestroyDebugReportCallbackEXT);
	LOAD_OPT_INSTANCE_PROC(DebugReportMessageEXT);

	LOAD_OPT_INSTANCE_PROC(DebugMarkerSetObjectTagEXT);
	LOAD_OPT_INSTANCE_PROC(DebugMarkerSetObjectNameEXT);
	LOAD_OPT_INSTANCE_PROC(CmdDebugMarkerBeginEXT);
	LOAD_OPT_INSTANCE_PROC(CmdDebugMarkerEndEXT);
	LOAD_OPT_INSTANCE_PROC(CmdDebugMarkerInsertEXT);

	#undef LOAD_INSTANCE_PROC
	#undef LOAD_OPT_INSTANCE_PROC

	return true;
}

bool Interface::loadDeviceProcs(VkDevice device) {
	if (currentDevice != VK_NULL_HANDLE && currentDevice != device) {
		unloadDeviceProcs();
	}

	currentDevice = device;

	#define LOAD_DEVICE_PROC(X) do { getDeviceProc(device, X, "vk"#X); assert(X); if (X == nullptr) { return false; } } while (0)
	#define LOAD_OPT_DEVICE_PROC(X) do { getDeviceProc(device, X, "vk"#X); } while (0)

	LOAD_DEVICE_PROC(DestroyDevice);
	LOAD_DEVICE_PROC(GetDeviceQueue);
	LOAD_DEVICE_PROC(DeviceWaitIdle);
	LOAD_DEVICE_PROC(AllocateMemory);
	LOAD_DEVICE_PROC(FreeMemory);
	LOAD_DEVICE_PROC(MapMemory);
	LOAD_DEVICE_PROC(UnmapMemory);
	LOAD_DEVICE_PROC(FlushMappedMemoryRanges);
	LOAD_DEVICE_PROC(InvalidateMappedMemoryRanges);
	LOAD_DEVICE_PROC(GetDeviceMemoryCommitment);
	LOAD_DEVICE_PROC(BindBufferMemory);
	LOAD_DEVICE_PROC(BindImageMemory);
	LOAD_DEVICE_PROC(GetBufferMemoryRequirements);
	LOAD_DEVICE_PROC(GetImageMemoryRequirements);
	LOAD_DEVICE_PROC(GetImageSparseMemoryRequirements);
	LOAD_DEVICE_PROC(CreateFence);
	LOAD_DEVICE_PROC(DestroyFence);
	LOAD_DEVICE_PROC(ResetFences);
	LOAD_DEVICE_PROC(GetFenceStatus);
	LOAD_DEVICE_PROC(WaitForFences);
	LOAD_DEVICE_PROC(CreateSemaphore);
	LOAD_DEVICE_PROC(DestroySemaphore);
	LOAD_DEVICE_PROC(CreateEvent);
	LOAD_DEVICE_PROC(DestroyEvent);
	LOAD_DEVICE_PROC(GetEventStatus);
	LOAD_DEVICE_PROC(SetEvent);
	LOAD_DEVICE_PROC(ResetEvent);
	LOAD_DEVICE_PROC(CreateQueryPool);
	LOAD_DEVICE_PROC(DestroyQueryPool);
	LOAD_DEVICE_PROC(GetQueryPoolResults);
	LOAD_DEVICE_PROC(CreateBuffer);
	LOAD_DEVICE_PROC(DestroyBuffer);
	LOAD_DEVICE_PROC(CreateBufferView);
	LOAD_DEVICE_PROC(DestroyBufferView);
	LOAD_DEVICE_PROC(CreateImage);
	LOAD_DEVICE_PROC(DestroyImage);
	LOAD_DEVICE_PROC(GetImageSubresourceLayout);
	LOAD_DEVICE_PROC(CreateImageView);
	LOAD_DEVICE_PROC(DestroyImageView);
	LOAD_DEVICE_PROC(CreateShaderModule);
	LOAD_DEVICE_PROC(DestroyShaderModule);
	LOAD_DEVICE_PROC(CreatePipelineCache);
	LOAD_DEVICE_PROC(DestroyPipelineCache);
	LOAD_DEVICE_PROC(GetPipelineCacheData);
	LOAD_DEVICE_PROC(MergePipelineCaches);
	LOAD_DEVICE_PROC(CreateGraphicsPipelines);
	LOAD_DEVICE_PROC(CreateComputePipelines);
	LOAD_DEVICE_PROC(DestroyPipeline);
	LOAD_DEVICE_PROC(CreatePipelineLayout);
	LOAD_DEVICE_PROC(DestroyPipelineLayout);
	LOAD_DEVICE_PROC(CreateSampler);
	LOAD_DEVICE_PROC(DestroySampler);
	LOAD_DEVICE_PROC(CreateDescriptorSetLayout);
	LOAD_DEVICE_PROC(DestroyDescriptorSetLayout);
	LOAD_DEVICE_PROC(CreateDescriptorPool);
	LOAD_DEVICE_PROC(DestroyDescriptorPool);
	LOAD_DEVICE_PROC(ResetDescriptorPool);
	LOAD_DEVICE_PROC(AllocateDescriptorSets);
	LOAD_DEVICE_PROC(FreeDescriptorSets);
	LOAD_DEVICE_PROC(UpdateDescriptorSets);
	LOAD_DEVICE_PROC(CreateFramebuffer);
	LOAD_DEVICE_PROC(DestroyFramebuffer);
	LOAD_DEVICE_PROC(CreateRenderPass);
	LOAD_DEVICE_PROC(DestroyRenderPass);
	LOAD_DEVICE_PROC(GetRenderAreaGranularity);

	LOAD_DEVICE_PROC(CreateCommandPool);
	LOAD_DEVICE_PROC(DestroyCommandPool);
	LOAD_DEVICE_PROC(ResetCommandPool);
	LOAD_DEVICE_PROC(AllocateCommandBuffers);
	LOAD_DEVICE_PROC(FreeCommandBuffers);

	LOAD_DEVICE_PROC(QueuePresentKHR);
	LOAD_DEVICE_PROC(CreateSwapchainKHR);
	LOAD_DEVICE_PROC(DestroySwapchainKHR);
	LOAD_DEVICE_PROC(GetSwapchainImagesKHR);
	LOAD_DEVICE_PROC(AcquireNextImageKHR);

	#undef LOAD_DEVICE_PROC
	#undef LOAD_OPT_DEVICE_PROC

	return true;
}

Interface vk{};

} // vlk

} // rn