#pragma once

#include <cassert>
#include <vulkan/vulkan.hpp>

static_assert( VK_HEADER_VERSION ==  73 , "Wrong VK_HEADER_VERSION!" );

namespace vk {

class DeviceDispatch
{
public:
	DeviceDispatch() = default;

	explicit DeviceDispatch(Device &device, Instance &instance) noexcept :
		device{device},
		dispatch{instance, device}
	{}

	PFN_vkVoidFunction getProcAddr(const char* pName) const {
		return device.getProcAddr(pName, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	PFN_vkVoidFunction getProcAddr(const std::string & name) const {
		return device.getProcAddr(name, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(const AllocationCallbacks* pAllocator) const {
		return device.destroy(pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue) const {
		return device.getQueue(queueFamilyIndex, queueIndex, pQueue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Queue getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const {
		return device.getQueue(queueFamilyIndex, queueIndex, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result waitIdle() const {
		return device.waitIdle(dispatch);
	}
#else
	ResultValueType<void>::type waitIdle() const {
		return device.waitIdle(dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result allocateMemory(const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory) const {
		return device.allocateMemory(pAllocateInfo, pAllocator, pMemory, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DeviceMemory>::type allocateMemory(const MemoryAllocateInfo & allocateInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.allocateMemory(allocateInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueDeviceMemory>::type allocateMemoryUnique(const MemoryAllocateInfo & allocateInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.allocateMemoryUnique(allocateInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void freeMemory(DeviceMemory memory, const AllocationCallbacks* pAllocator) const {
		return device.freeMemory(memory, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void freeMemory(DeviceMemory memory, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.freeMemory(memory, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void free(DeviceMemory memory, const AllocationCallbacks* pAllocator) const {
		return device.free(memory, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void free(DeviceMemory memory, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.free(memory, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result mapMemory(DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData) const {
		return device.mapMemory(memory, offset, size, flags, ppData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void*>::type mapMemory(DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags = MemoryMapFlags()) const {
		return device.mapMemory(memory, offset, size, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void unmapMemory(DeviceMemory memory) const {
		return device.unmapMemory(memory, dispatch);
	}

	Result flushMappedMemoryRanges(uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges) const {
		return device.flushMappedMemoryRanges(memoryRangeCount, pMemoryRanges, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type flushMappedMemoryRanges(ArrayProxy<const MappedMemoryRange> memoryRanges) const {
		return device.flushMappedMemoryRanges(memoryRanges, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result invalidateMappedMemoryRanges(uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges) const {
		return device.invalidateMappedMemoryRanges(memoryRangeCount, pMemoryRanges, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type invalidateMappedMemoryRanges(ArrayProxy<const MappedMemoryRange> memoryRanges) const {
		return device.invalidateMappedMemoryRanges(memoryRanges, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getMemoryCommitment(DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes) const {
		return device.getMemoryCommitment(memory, pCommittedMemoryInBytes, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	DeviceSize getMemoryCommitment(DeviceMemory memory) const {
		return device.getMemoryCommitment(memory, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getBufferMemoryRequirements(Buffer buffer, MemoryRequirements* pMemoryRequirements) const {
		return device.getBufferMemoryRequirements(buffer, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	MemoryRequirements getBufferMemoryRequirements(Buffer buffer) const {
		return device.getBufferMemoryRequirements(buffer, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result bindBufferMemory(Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset) const {
		return device.bindBufferMemory(buffer, memory, memoryOffset, dispatch);
	}
#else
	ResultValueType<void>::type bindBufferMemory(Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset) const {
		return device.bindBufferMemory(buffer, memory, memoryOffset, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageMemoryRequirements(Image image, MemoryRequirements* pMemoryRequirements) const {
		return device.getImageMemoryRequirements(image, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	MemoryRequirements getImageMemoryRequirements(Image image) const {
		return device.getImageMemoryRequirements(image, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result bindImageMemory(Image image, DeviceMemory memory, DeviceSize memoryOffset) const {
		return device.bindImageMemory(image, memory, memoryOffset, dispatch);
	}
#else
	ResultValueType<void>::type bindImageMemory(Image image, DeviceMemory memory, DeviceSize memoryOffset) const {
		return device.bindImageMemory(image, memory, memoryOffset, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageSparseMemoryRequirements(Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements) const {
		return device.getImageSparseMemoryRequirements(image, pSparseMemoryRequirementCount, pSparseMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<SparseImageMemoryRequirements>>
	std::vector<SparseImageMemoryRequirements,Allocator> getImageSparseMemoryRequirements(Image image) const {
		return device.getImageSparseMemoryRequirements(image, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createFence(const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence) const {
		return device.createFence(pCreateInfo, pAllocator, pFence, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Fence>::type createFence(const FenceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createFence(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueFence>::type createFenceUnique(const FenceCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createFenceUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyFence(Fence fence, const AllocationCallbacks* pAllocator) const {
		return device.destroyFence(fence, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyFence(Fence fence, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyFence(fence, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Fence fence, const AllocationCallbacks* pAllocator) const {
		return device.destroy(fence, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Fence fence, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(fence, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result resetFences(uint32_t fenceCount, const Fence* pFences) const {
		return device.resetFences(fenceCount, pFences, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type resetFences(ArrayProxy<const Fence> fences) const {
		return device.resetFences(fences, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getFenceStatus(Fence fence) const {
		return device.getFenceStatus(fence, dispatch);
	}

	Result waitForFences(uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout) const {
		return device.waitForFences(fenceCount, pFences, waitAll, timeout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result waitForFences(ArrayProxy<const Fence> fences, Bool32 waitAll, uint64_t timeout) const {
		return device.waitForFences(fences, waitAll, timeout, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createSemaphore(const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore) const {
		return device.createSemaphore(pCreateInfo, pAllocator, pSemaphore, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Semaphore>::type createSemaphore(const SemaphoreCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSemaphore(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueSemaphore>::type createSemaphoreUnique(const SemaphoreCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSemaphoreUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroySemaphore(Semaphore semaphore, const AllocationCallbacks* pAllocator) const {
		return device.destroySemaphore(semaphore, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroySemaphore(Semaphore semaphore, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroySemaphore(semaphore, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Semaphore semaphore, const AllocationCallbacks* pAllocator) const {
		return device.destroy(semaphore, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Semaphore semaphore, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(semaphore, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createEvent(const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent) const {
		return device.createEvent(pCreateInfo, pAllocator, pEvent, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Event>::type createEvent(const EventCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createEvent(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueEvent>::type createEventUnique(const EventCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createEventUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyEvent(Event event, const AllocationCallbacks* pAllocator) const {
		return device.destroyEvent(event, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyEvent(Event event, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyEvent(event, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Event event, const AllocationCallbacks* pAllocator) const {
		return device.destroy(event, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Event event, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(event, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getEventStatus(Event event) const {
		return device.getEventStatus(event, dispatch);
	}

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result setEvent(Event event) const {
		return device.setEvent(event, dispatch);
	}
#else
	ResultValueType<void>::type setEvent(Event event) const {
		return device.setEvent(event, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result resetEvent(Event event) const {
		return device.resetEvent(event, dispatch);
	}
#else
	ResultValueType<void>::type resetEvent(Event event) const {
		return device.resetEvent(event, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createQueryPool(const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool) const {
		return device.createQueryPool(pCreateInfo, pAllocator, pQueryPool, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<QueryPool>::type createQueryPool(const QueryPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createQueryPool(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueQueryPool>::type createQueryPoolUnique(const QueryPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createQueryPoolUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyQueryPool(QueryPool queryPool, const AllocationCallbacks* pAllocator) const {
		return device.destroyQueryPool(queryPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyQueryPool(QueryPool queryPool, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyQueryPool(queryPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(QueryPool queryPool, const AllocationCallbacks* pAllocator) const {
		return device.destroy(queryPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(QueryPool queryPool, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(queryPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getQueryPoolResults(QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags) const {
		return device.getQueryPoolResults(queryPool, firstQuery, queryCount, dataSize, pData, stride, flags, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename T>
	Result getQueryPoolResults(QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, ArrayProxy<T> data, DeviceSize stride, QueryResultFlags flags) const {
		return device.getQueryPoolResults(queryPool, firstQuery, queryCount, data, stride, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createBuffer(const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer) const {
		return device.createBuffer(pCreateInfo, pAllocator, pBuffer, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Buffer>::type createBuffer(const BufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createBuffer(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueBuffer>::type createBufferUnique(const BufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createBufferUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyBuffer(Buffer buffer, const AllocationCallbacks* pAllocator) const {
		return device.destroyBuffer(buffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyBuffer(Buffer buffer, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyBuffer(buffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Buffer buffer, const AllocationCallbacks* pAllocator) const {
		return device.destroy(buffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Buffer buffer, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(buffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createBufferView(const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView) const {
		return device.createBufferView(pCreateInfo, pAllocator, pView, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<BufferView>::type createBufferView(const BufferViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createBufferView(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueBufferView>::type createBufferViewUnique(const BufferViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createBufferViewUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyBufferView(BufferView bufferView, const AllocationCallbacks* pAllocator) const {
		return device.destroyBufferView(bufferView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyBufferView(BufferView bufferView, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyBufferView(bufferView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(BufferView bufferView, const AllocationCallbacks* pAllocator) const {
		return device.destroy(bufferView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(BufferView bufferView, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(bufferView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createImage(const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage) const {
		return device.createImage(pCreateInfo, pAllocator, pImage, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Image>::type createImage(const ImageCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createImage(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueImage>::type createImageUnique(const ImageCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createImageUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyImage(Image image, const AllocationCallbacks* pAllocator) const {
		return device.destroyImage(image, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyImage(Image image, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyImage(image, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Image image, const AllocationCallbacks* pAllocator) const {
		return device.destroy(image, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Image image, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(image, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageSubresourceLayout(Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout) const {
		return device.getImageSubresourceLayout(image, pSubresource, pLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	SubresourceLayout getImageSubresourceLayout(Image image, const ImageSubresource & subresource) const {
		return device.getImageSubresourceLayout(image, subresource, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createImageView(const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView) const {
		return device.createImageView(pCreateInfo, pAllocator, pView, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<ImageView>::type createImageView(const ImageViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createImageView(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueImageView>::type createImageViewUnique(const ImageViewCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createImageViewUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyImageView(ImageView imageView, const AllocationCallbacks* pAllocator) const {
		return device.destroyImageView(imageView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyImageView(ImageView imageView, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyImageView(imageView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(ImageView imageView, const AllocationCallbacks* pAllocator) const {
		return device.destroy(imageView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(ImageView imageView, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(imageView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createShaderModule(const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule) const {
		return device.createShaderModule(pCreateInfo, pAllocator, pShaderModule, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<ShaderModule>::type createShaderModule(const ShaderModuleCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createShaderModule(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueShaderModule>::type createShaderModuleUnique(const ShaderModuleCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createShaderModuleUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyShaderModule(ShaderModule shaderModule, const AllocationCallbacks* pAllocator) const {
		return device.destroyShaderModule(shaderModule, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyShaderModule(ShaderModule shaderModule, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyShaderModule(shaderModule, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(ShaderModule shaderModule, const AllocationCallbacks* pAllocator) const {
		return device.destroy(shaderModule, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(ShaderModule shaderModule, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(shaderModule, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createPipelineCache(const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache) const {
		return device.createPipelineCache(pCreateInfo, pAllocator, pPipelineCache, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<PipelineCache>::type createPipelineCache(const PipelineCacheCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createPipelineCache(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniquePipelineCache>::type createPipelineCacheUnique(const PipelineCacheCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createPipelineCacheUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyPipelineCache(PipelineCache pipelineCache, const AllocationCallbacks* pAllocator) const {
		return device.destroyPipelineCache(pipelineCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyPipelineCache(PipelineCache pipelineCache, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyPipelineCache(pipelineCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(PipelineCache pipelineCache, const AllocationCallbacks* pAllocator) const {
		return device.destroy(pipelineCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(PipelineCache pipelineCache, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(pipelineCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getPipelineCacheData(PipelineCache pipelineCache, size_t* pDataSize, void* pData) const {
		return device.getPipelineCacheData(pipelineCache, pDataSize, pData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<uint8_t>>
	typename ResultValueType<std::vector<uint8_t,Allocator>>::type getPipelineCacheData(PipelineCache pipelineCache) const {
		return device.getPipelineCacheData(pipelineCache, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result mergePipelineCaches(PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches) const {
		return device.mergePipelineCaches(dstCache, srcCacheCount, pSrcCaches, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type mergePipelineCaches(PipelineCache dstCache, ArrayProxy<const PipelineCache> srcCaches) const {
		return device.mergePipelineCaches(dstCache, srcCaches, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createGraphicsPipelines(PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines) const {
		return device.createGraphicsPipelines(pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<Pipeline>>
	typename ResultValueType<std::vector<Pipeline,Allocator>>::type createGraphicsPipelines(PipelineCache pipelineCache, ArrayProxy<const GraphicsPipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createGraphicsPipelines(pipelineCache, createInfos, allocator, dispatch);
	}
	template <typename Allocator = std::allocator<Pipeline>>
	ResultValueType<Pipeline>::type createGraphicsPipeline(PipelineCache pipelineCache, const GraphicsPipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createGraphicsPipeline(pipelineCache, createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template <typename Allocator = std::allocator<UniquePipeline>>
	typename ResultValueType<std::vector<UniquePipeline,Allocator>>::type createGraphicsPipelinesUnique(PipelineCache pipelineCache, ArrayProxy<const GraphicsPipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createGraphicsPipelinesUnique(pipelineCache, createInfos, allocator, dispatch);
	}
	template <typename Allocator = std::allocator<UniquePipeline>>
	ResultValueType<UniquePipeline>::type createGraphicsPipelineUnique(PipelineCache pipelineCache, const GraphicsPipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createGraphicsPipelineUnique(pipelineCache, createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createComputePipelines(PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines) const {
		return device.createComputePipelines(pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<Pipeline>>
	typename ResultValueType<std::vector<Pipeline,Allocator>>::type createComputePipelines(PipelineCache pipelineCache, ArrayProxy<const ComputePipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createComputePipelines(pipelineCache, createInfos, allocator, dispatch);
	}
	template <typename Allocator = std::allocator<Pipeline>>
	ResultValueType<Pipeline>::type createComputePipeline(PipelineCache pipelineCache, const ComputePipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createComputePipeline(pipelineCache, createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template <typename Allocator = std::allocator<UniquePipeline>>
	typename ResultValueType<std::vector<UniquePipeline,Allocator>>::type createComputePipelinesUnique(PipelineCache pipelineCache, ArrayProxy<const ComputePipelineCreateInfo> createInfos, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createComputePipelinesUnique(pipelineCache, createInfos, allocator, dispatch);
	}
	template <typename Allocator = std::allocator<UniquePipeline>>
	ResultValueType<UniquePipeline>::type createComputePipelineUnique(PipelineCache pipelineCache, const ComputePipelineCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createComputePipelineUnique(pipelineCache, createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyPipeline(Pipeline pipeline, const AllocationCallbacks* pAllocator) const {
		return device.destroyPipeline(pipeline, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyPipeline(Pipeline pipeline, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyPipeline(pipeline, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Pipeline pipeline, const AllocationCallbacks* pAllocator) const {
		return device.destroy(pipeline, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Pipeline pipeline, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(pipeline, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createPipelineLayout(const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout) const {
		return device.createPipelineLayout(pCreateInfo, pAllocator, pPipelineLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<PipelineLayout>::type createPipelineLayout(const PipelineLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createPipelineLayout(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniquePipelineLayout>::type createPipelineLayoutUnique(const PipelineLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createPipelineLayoutUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyPipelineLayout(PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator) const {
		return device.destroyPipelineLayout(pipelineLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyPipelineLayout(PipelineLayout pipelineLayout, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyPipelineLayout(pipelineLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator) const {
		return device.destroy(pipelineLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(PipelineLayout pipelineLayout, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(pipelineLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createSampler(const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler) const {
		return device.createSampler(pCreateInfo, pAllocator, pSampler, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Sampler>::type createSampler(const SamplerCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSampler(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueSampler>::type createSamplerUnique(const SamplerCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSamplerUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroySampler(Sampler sampler, const AllocationCallbacks* pAllocator) const {
		return device.destroySampler(sampler, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroySampler(Sampler sampler, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroySampler(sampler, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Sampler sampler, const AllocationCallbacks* pAllocator) const {
		return device.destroy(sampler, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Sampler sampler, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(sampler, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createDescriptorSetLayout(const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout) const {
		return device.createDescriptorSetLayout(pCreateInfo, pAllocator, pSetLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DescriptorSetLayout>::type createDescriptorSetLayout(const DescriptorSetLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorSetLayout(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueDescriptorSetLayout>::type createDescriptorSetLayoutUnique(const DescriptorSetLayoutCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorSetLayoutUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyDescriptorSetLayout(DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator) const {
		return device.destroyDescriptorSetLayout(descriptorSetLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyDescriptorSetLayout(DescriptorSetLayout descriptorSetLayout, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyDescriptorSetLayout(descriptorSetLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator) const {
		return device.destroy(descriptorSetLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(DescriptorSetLayout descriptorSetLayout, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(descriptorSetLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createDescriptorPool(const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool) const {
		return device.createDescriptorPool(pCreateInfo, pAllocator, pDescriptorPool, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DescriptorPool>::type createDescriptorPool(const DescriptorPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorPool(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueDescriptorPool>::type createDescriptorPoolUnique(const DescriptorPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorPoolUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyDescriptorPool(DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator) const {
		return device.destroyDescriptorPool(descriptorPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyDescriptorPool(DescriptorPool descriptorPool, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyDescriptorPool(descriptorPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator) const {
		return device.destroy(descriptorPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(DescriptorPool descriptorPool, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(descriptorPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result resetDescriptorPool(DescriptorPool descriptorPool, DescriptorPoolResetFlags flags = DescriptorPoolResetFlags()) const {
		return device.resetDescriptorPool(descriptorPool, flags, dispatch);
	}
#else
	ResultValueType<void>::type resetDescriptorPool(DescriptorPool descriptorPool, DescriptorPoolResetFlags flags = DescriptorPoolResetFlags()) const {
		return device.resetDescriptorPool(descriptorPool, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result allocateDescriptorSets(const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets) const {
		return device.allocateDescriptorSets(pAllocateInfo, pDescriptorSets, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<DescriptorSet>>
	typename ResultValueType<std::vector<DescriptorSet,Allocator>>::type allocateDescriptorSets(const DescriptorSetAllocateInfo & allocateInfo) const {
		return device.allocateDescriptorSets(allocateInfo, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template <typename Allocator = std::allocator<UniqueDescriptorSet>>
	typename ResultValueType<std::vector<UniqueDescriptorSet,Allocator>>::type allocateDescriptorSetsUnique(const DescriptorSetAllocateInfo & allocateInfo) const {
		return device.allocateDescriptorSetsUnique(allocateInfo, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result freeDescriptorSets(DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets) const {
		return device.freeDescriptorSets(descriptorPool, descriptorSetCount, pDescriptorSets, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type freeDescriptorSets(DescriptorPool descriptorPool, ArrayProxy<const DescriptorSet> descriptorSets) const {
		return device.freeDescriptorSets(descriptorPool, descriptorSets, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result free(DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets) const {
		return device.free(descriptorPool, descriptorSetCount, pDescriptorSets, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type free(DescriptorPool descriptorPool, ArrayProxy<const DescriptorSet> descriptorSets) const {
		return device.free(descriptorPool, descriptorSets, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void updateDescriptorSets(uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies) const {
		return device.updateDescriptorSets(descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void updateDescriptorSets(ArrayProxy<const WriteDescriptorSet> descriptorWrites, ArrayProxy<const CopyDescriptorSet> descriptorCopies) const {
		return device.updateDescriptorSets(descriptorWrites, descriptorCopies, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createFramebuffer(const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer) const {
		return device.createFramebuffer(pCreateInfo, pAllocator, pFramebuffer, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Framebuffer>::type createFramebuffer(const FramebufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createFramebuffer(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueFramebuffer>::type createFramebufferUnique(const FramebufferCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createFramebufferUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyFramebuffer(Framebuffer framebuffer, const AllocationCallbacks* pAllocator) const {
		return device.destroyFramebuffer(framebuffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyFramebuffer(Framebuffer framebuffer, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyFramebuffer(framebuffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(Framebuffer framebuffer, const AllocationCallbacks* pAllocator) const {
		return device.destroy(framebuffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(Framebuffer framebuffer, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(framebuffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createRenderPass(const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass) const {
		return device.createRenderPass(pCreateInfo, pAllocator, pRenderPass, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<RenderPass>::type createRenderPass(const RenderPassCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createRenderPass(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueRenderPass>::type createRenderPassUnique(const RenderPassCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createRenderPassUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyRenderPass(RenderPass renderPass, const AllocationCallbacks* pAllocator) const {
		return device.destroyRenderPass(renderPass, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyRenderPass(RenderPass renderPass, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyRenderPass(renderPass, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(RenderPass renderPass, const AllocationCallbacks* pAllocator) const {
		return device.destroy(renderPass, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(RenderPass renderPass, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(renderPass, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getRenderAreaGranularity(RenderPass renderPass, Extent2D* pGranularity) const {
		return device.getRenderAreaGranularity(renderPass, pGranularity, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Extent2D getRenderAreaGranularity(RenderPass renderPass) const {
		return device.getRenderAreaGranularity(renderPass, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createCommandPool(const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool) const {
		return device.createCommandPool(pCreateInfo, pAllocator, pCommandPool, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<CommandPool>::type createCommandPool(const CommandPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createCommandPool(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueCommandPool>::type createCommandPoolUnique(const CommandPoolCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createCommandPoolUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyCommandPool(CommandPool commandPool, const AllocationCallbacks* pAllocator) const {
		return device.destroyCommandPool(commandPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyCommandPool(CommandPool commandPool, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyCommandPool(commandPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(CommandPool commandPool, const AllocationCallbacks* pAllocator) const {
		return device.destroy(commandPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(CommandPool commandPool, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(commandPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Result resetCommandPool(CommandPool commandPool, CommandPoolResetFlags flags) const {
		return device.resetCommandPool(commandPool, flags, dispatch);
	}
#else
	ResultValueType<void>::type resetCommandPool(CommandPool commandPool, CommandPoolResetFlags flags) const {
		return device.resetCommandPool(commandPool, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result allocateCommandBuffers(const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers) const {
		return device.allocateCommandBuffers(pAllocateInfo, pCommandBuffers, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<CommandBuffer>>
	typename ResultValueType<std::vector<CommandBuffer,Allocator>>::type allocateCommandBuffers(const CommandBufferAllocateInfo & allocateInfo) const {
		return device.allocateCommandBuffers(allocateInfo, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template <typename Allocator = std::allocator<UniqueCommandBuffer>>
	typename ResultValueType<std::vector<UniqueCommandBuffer,Allocator>>::type allocateCommandBuffersUnique(const CommandBufferAllocateInfo & allocateInfo) const {
		return device.allocateCommandBuffersUnique(allocateInfo, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void freeCommandBuffers(CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers) const {
		return device.freeCommandBuffers(commandPool, commandBufferCount, pCommandBuffers, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void freeCommandBuffers(CommandPool commandPool, ArrayProxy<const CommandBuffer> commandBuffers) const {
		return device.freeCommandBuffers(commandPool, commandBuffers, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void free(CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers) const {
		return device.free(commandPool, commandBufferCount, pCommandBuffers, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void free(CommandPool commandPool, ArrayProxy<const CommandBuffer> commandBuffers) const {
		return device.free(commandPool, commandBuffers, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createSharedSwapchainsKHR(uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains) const {
		return device.createSharedSwapchainsKHR(swapchainCount, pCreateInfos, pAllocator, pSwapchains, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<SwapchainKHR>>
	typename ResultValueType<std::vector<SwapchainKHR,Allocator>>::type createSharedSwapchainsKHR(ArrayProxy<const SwapchainCreateInfoKHR> createInfos, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSharedSwapchainsKHR(createInfos, allocator, dispatch);
	}
	template <typename Allocator = std::allocator<SwapchainKHR>>
	ResultValueType<SwapchainKHR>::type createSharedSwapchainKHR(const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSharedSwapchainKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template <typename Allocator = std::allocator<UniqueSwapchainKHR>>
	typename ResultValueType<std::vector<UniqueSwapchainKHR,Allocator>>::type createSharedSwapchainsKHRUnique(ArrayProxy<const SwapchainCreateInfoKHR> createInfos, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSharedSwapchainsKHRUnique(createInfos, allocator, dispatch);
	}
	template <typename Allocator = std::allocator<UniqueSwapchainKHR>>
	ResultValueType<UniqueSwapchainKHR>::type createSharedSwapchainKHRUnique(const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSharedSwapchainKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createSwapchainKHR(const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain) const {
		return device.createSwapchainKHR(pCreateInfo, pAllocator, pSwapchain, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<SwapchainKHR>::type createSwapchainKHR(const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSwapchainKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueSwapchainKHR>::type createSwapchainKHRUnique(const SwapchainCreateInfoKHR & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSwapchainKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroySwapchainKHR(SwapchainKHR swapchain, const AllocationCallbacks* pAllocator) const {
		return device.destroySwapchainKHR(swapchain, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroySwapchainKHR(SwapchainKHR swapchain, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroySwapchainKHR(swapchain, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(SwapchainKHR swapchain, const AllocationCallbacks* pAllocator) const {
		return device.destroy(swapchain, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(SwapchainKHR swapchain, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(swapchain, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getSwapchainImagesKHR(SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages) const {
		return device.getSwapchainImagesKHR(swapchain, pSwapchainImageCount, pSwapchainImages, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<Image>>
	typename ResultValueType<std::vector<Image,Allocator>>::type getSwapchainImagesKHR(SwapchainKHR swapchain) const {
		return device.getSwapchainImagesKHR(swapchain, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result acquireNextImageKHR(SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex) const {
		return device.acquireNextImageKHR(swapchain, timeout, semaphore, fence, pImageIndex, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValue<uint32_t> acquireNextImageKHR(SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence) const {
		return device.acquireNextImageKHR(swapchain, timeout, semaphore, fence, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result debugMarkerSetObjectNameEXT(const DebugMarkerObjectNameInfoEXT* pNameInfo) const {
		return device.debugMarkerSetObjectNameEXT(pNameInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type debugMarkerSetObjectNameEXT(const DebugMarkerObjectNameInfoEXT & nameInfo) const {
		return device.debugMarkerSetObjectNameEXT(nameInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result debugMarkerSetObjectTagEXT(const DebugMarkerObjectTagInfoEXT* pTagInfo) const {
		return device.debugMarkerSetObjectTagEXT(pTagInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type debugMarkerSetObjectTagEXT(const DebugMarkerObjectTagInfoEXT & tagInfo) const {
		return device.debugMarkerSetObjectTagEXT(tagInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_NV
	Result getMemoryWin32HandleNV(DeviceMemory memory, ExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) const {
		return device.getMemoryWin32HandleNV(memory, handleType, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<HANDLE>::type getMemoryWin32HandleNV(DeviceMemory memory, ExternalMemoryHandleTypeFlagsNV handleType) const {
		return device.getMemoryWin32HandleNV(memory, handleType, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_NV*/

	Result createIndirectCommandsLayoutNVX(const IndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const AllocationCallbacks* pAllocator, IndirectCommandsLayoutNVX* pIndirectCommandsLayout) const {
		return device.createIndirectCommandsLayoutNVX(pCreateInfo, pAllocator, pIndirectCommandsLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<IndirectCommandsLayoutNVX>::type createIndirectCommandsLayoutNVX(const IndirectCommandsLayoutCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createIndirectCommandsLayoutNVX(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueIndirectCommandsLayoutNVX>::type createIndirectCommandsLayoutNVXUnique(const IndirectCommandsLayoutCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createIndirectCommandsLayoutNVXUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyIndirectCommandsLayoutNVX(IndirectCommandsLayoutNVX indirectCommandsLayout, const AllocationCallbacks* pAllocator) const {
		return device.destroyIndirectCommandsLayoutNVX(indirectCommandsLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyIndirectCommandsLayoutNVX(IndirectCommandsLayoutNVX indirectCommandsLayout, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyIndirectCommandsLayoutNVX(indirectCommandsLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(IndirectCommandsLayoutNVX indirectCommandsLayout, const AllocationCallbacks* pAllocator) const {
		return device.destroy(indirectCommandsLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(IndirectCommandsLayoutNVX indirectCommandsLayout, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(indirectCommandsLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createObjectTableNVX(const ObjectTableCreateInfoNVX* pCreateInfo, const AllocationCallbacks* pAllocator, ObjectTableNVX* pObjectTable) const {
		return device.createObjectTableNVX(pCreateInfo, pAllocator, pObjectTable, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<ObjectTableNVX>::type createObjectTableNVX(const ObjectTableCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createObjectTableNVX(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueObjectTableNVX>::type createObjectTableNVXUnique(const ObjectTableCreateInfoNVX & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createObjectTableNVXUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyObjectTableNVX(ObjectTableNVX objectTable, const AllocationCallbacks* pAllocator) const {
		return device.destroyObjectTableNVX(objectTable, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyObjectTableNVX(ObjectTableNVX objectTable, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyObjectTableNVX(objectTable, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(ObjectTableNVX objectTable, const AllocationCallbacks* pAllocator) const {
		return device.destroy(objectTable, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(ObjectTableNVX objectTable, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(objectTable, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result registerObjectsNVX(ObjectTableNVX objectTable, uint32_t objectCount, const ObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices) const {
		return device.registerObjectsNVX(objectTable, objectCount, ppObjectTableEntries, pObjectIndices, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type registerObjectsNVX(ObjectTableNVX objectTable, ArrayProxy<const ObjectTableEntryNVX* const> pObjectTableEntries, ArrayProxy<const uint32_t> objectIndices) const {
		return device.registerObjectsNVX(objectTable, pObjectTableEntries, objectIndices, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result unregisterObjectsNVX(ObjectTableNVX objectTable, uint32_t objectCount, const ObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices) const {
		return device.unregisterObjectsNVX(objectTable, objectCount, pObjectEntryTypes, pObjectIndices, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type unregisterObjectsNVX(ObjectTableNVX objectTable, ArrayProxy<const ObjectEntryTypeNVX> objectEntryTypes, ArrayProxy<const uint32_t> objectIndices) const {
		return device.unregisterObjectsNVX(objectTable, objectEntryTypes, objectIndices, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void trimCommandPool(CommandPool commandPool, CommandPoolTrimFlags flags = CommandPoolTrimFlags()) const {
		return device.trimCommandPool(commandPool, flags, dispatch);
	}

	void trimCommandPoolKHR(CommandPool commandPool, CommandPoolTrimFlags flags = CommandPoolTrimFlags()) const {
		return device.trimCommandPoolKHR(commandPool, flags, dispatch);
	}

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Result getMemoryWin32HandleKHR(const MemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const {
		return device.getMemoryWin32HandleKHR(pGetWin32HandleInfo, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<HANDLE>::type getMemoryWin32HandleKHR(const MemoryGetWin32HandleInfoKHR & getWin32HandleInfo) const {
		return device.getMemoryWin32HandleKHR(getWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Result getMemoryWin32HandlePropertiesKHR(ExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, MemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) const {
		return device.getMemoryWin32HandlePropertiesKHR(handleType, handle, pMemoryWin32HandleProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<MemoryWin32HandlePropertiesKHR>::type getMemoryWin32HandlePropertiesKHR(ExternalMemoryHandleTypeFlagBits handleType, HANDLE handle) const {
		return device.getMemoryWin32HandlePropertiesKHR(handleType, handle, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

	Result getMemoryFdKHR(const MemoryGetFdInfoKHR* pGetFdInfo, int* pFd) const {
		return device.getMemoryFdKHR(pGetFdInfo, pFd, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<int>::type getMemoryFdKHR(const MemoryGetFdInfoKHR & getFdInfo) const {
		return device.getMemoryFdKHR(getFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getMemoryFdPropertiesKHR(ExternalMemoryHandleTypeFlagBits handleType, int fd, MemoryFdPropertiesKHR* pMemoryFdProperties) const {
		return device.getMemoryFdPropertiesKHR(handleType, fd, pMemoryFdProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<MemoryFdPropertiesKHR>::type getMemoryFdPropertiesKHR(ExternalMemoryHandleTypeFlagBits handleType, int fd) const {
		return device.getMemoryFdPropertiesKHR(handleType, fd, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Result getSemaphoreWin32HandleKHR(const SemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const {
		return device.getSemaphoreWin32HandleKHR(pGetWin32HandleInfo, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<HANDLE>::type getSemaphoreWin32HandleKHR(const SemaphoreGetWin32HandleInfoKHR & getWin32HandleInfo) const {
		return device.getSemaphoreWin32HandleKHR(getWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Result importSemaphoreWin32HandleKHR(const ImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) const {
		return device.importSemaphoreWin32HandleKHR(pImportSemaphoreWin32HandleInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type importSemaphoreWin32HandleKHR(const ImportSemaphoreWin32HandleInfoKHR & importSemaphoreWin32HandleInfo) const {
		return device.importSemaphoreWin32HandleKHR(importSemaphoreWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

	Result getSemaphoreFdKHR(const SemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) const {
		return device.getSemaphoreFdKHR(pGetFdInfo, pFd, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<int>::type getSemaphoreFdKHR(const SemaphoreGetFdInfoKHR & getFdInfo) const {
		return device.getSemaphoreFdKHR(getFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result importSemaphoreFdKHR(const ImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) const {
		return device.importSemaphoreFdKHR(pImportSemaphoreFdInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type importSemaphoreFdKHR(const ImportSemaphoreFdInfoKHR & importSemaphoreFdInfo) const {
		return device.importSemaphoreFdKHR(importSemaphoreFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Result getFenceWin32HandleKHR(const FenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const {
		return device.getFenceWin32HandleKHR(pGetWin32HandleInfo, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<HANDLE>::type getFenceWin32HandleKHR(const FenceGetWin32HandleInfoKHR & getWin32HandleInfo) const {
		return device.getFenceWin32HandleKHR(getWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Result importFenceWin32HandleKHR(const ImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) const {
		return device.importFenceWin32HandleKHR(pImportFenceWin32HandleInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type importFenceWin32HandleKHR(const ImportFenceWin32HandleInfoKHR & importFenceWin32HandleInfo) const {
		return device.importFenceWin32HandleKHR(importFenceWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

	Result getFenceFdKHR(const FenceGetFdInfoKHR* pGetFdInfo, int* pFd) const {
		return device.getFenceFdKHR(pGetFdInfo, pFd, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<int>::type getFenceFdKHR(const FenceGetFdInfoKHR & getFdInfo) const {
		return device.getFenceFdKHR(getFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result importFenceFdKHR(const ImportFenceFdInfoKHR* pImportFenceFdInfo) const {
		return device.importFenceFdKHR(pImportFenceFdInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type importFenceFdKHR(const ImportFenceFdInfoKHR & importFenceFdInfo) const {
		return device.importFenceFdKHR(importFenceFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result displayPowerControlEXT(DisplayKHR display, const DisplayPowerInfoEXT* pDisplayPowerInfo) const {
		return device.displayPowerControlEXT(display, pDisplayPowerInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type displayPowerControlEXT(DisplayKHR display, const DisplayPowerInfoEXT & displayPowerInfo) const {
		return device.displayPowerControlEXT(display, displayPowerInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result registerEventEXT(const DeviceEventInfoEXT* pDeviceEventInfo, const AllocationCallbacks* pAllocator, Fence* pFence) const {
		return device.registerEventEXT(pDeviceEventInfo, pAllocator, pFence, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Fence>::type registerEventEXT(const DeviceEventInfoEXT & deviceEventInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.registerEventEXT(deviceEventInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result registerDisplayEventEXT(DisplayKHR display, const DisplayEventInfoEXT* pDisplayEventInfo, const AllocationCallbacks* pAllocator, Fence* pFence) const {
		return device.registerDisplayEventEXT(display, pDisplayEventInfo, pAllocator, pFence, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<Fence>::type registerDisplayEventEXT(DisplayKHR display, const DisplayEventInfoEXT & displayEventInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.registerDisplayEventEXT(display, displayEventInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getSwapchainCounterEXT(SwapchainKHR swapchain, SurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) const {
		return device.getSwapchainCounterEXT(swapchain, counter, pCounterValue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<uint64_t>::type getSwapchainCounterEXT(SwapchainKHR swapchain, SurfaceCounterFlagBitsEXT counter) const {
		return device.getSwapchainCounterEXT(swapchain, counter, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, PeerMemoryFeatureFlags* pPeerMemoryFeatures) const {
		return device.getGroupPeerMemoryFeatures(heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	PeerMemoryFeatureFlags getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex) const {
		return device.getGroupPeerMemoryFeatures(heapIndex, localDeviceIndex, remoteDeviceIndex, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, PeerMemoryFeatureFlags* pPeerMemoryFeatures) const {
		return device.getGroupPeerMemoryFeaturesKHR(heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	PeerMemoryFeatureFlags getGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex) const {
		return device.getGroupPeerMemoryFeaturesKHR(heapIndex, localDeviceIndex, remoteDeviceIndex, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result bindBufferMemory2(uint32_t bindInfoCount, const BindBufferMemoryInfo* pBindInfos) const {
		return device.bindBufferMemory2(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type bindBufferMemory2(ArrayProxy<const BindBufferMemoryInfo> bindInfos) const {
		return device.bindBufferMemory2(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result bindBufferMemory2KHR(uint32_t bindInfoCount, const BindBufferMemoryInfo* pBindInfos) const {
		return device.bindBufferMemory2KHR(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type bindBufferMemory2KHR(ArrayProxy<const BindBufferMemoryInfo> bindInfos) const {
		return device.bindBufferMemory2KHR(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result bindImageMemory2(uint32_t bindInfoCount, const BindImageMemoryInfo* pBindInfos) const {
		return device.bindImageMemory2(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type bindImageMemory2(ArrayProxy<const BindImageMemoryInfo> bindInfos) const {
		return device.bindImageMemory2(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result bindImageMemory2KHR(uint32_t bindInfoCount, const BindImageMemoryInfo* pBindInfos) const {
		return device.bindImageMemory2KHR(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type bindImageMemory2KHR(ArrayProxy<const BindImageMemoryInfo> bindInfos) const {
		return device.bindImageMemory2KHR(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getGroupPresentCapabilitiesKHR(DeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) const {
		return device.getGroupPresentCapabilitiesKHR(pDeviceGroupPresentCapabilities, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DeviceGroupPresentCapabilitiesKHR>::type getGroupPresentCapabilitiesKHR() const {
		return device.getGroupPresentCapabilitiesKHR(dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getGroupSurfacePresentModesKHR(SurfaceKHR surface, DeviceGroupPresentModeFlagsKHR* pModes) const {
		return device.getGroupSurfacePresentModesKHR(surface, pModes, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DeviceGroupPresentModeFlagsKHR>::type getGroupSurfacePresentModesKHR(SurfaceKHR surface) const {
		return device.getGroupSurfacePresentModesKHR(surface, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result acquireNextImage2KHR(const AcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) const {
		return device.acquireNextImage2KHR(pAcquireInfo, pImageIndex, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValue<uint32_t> acquireNextImage2KHR(const AcquireNextImageInfoKHR & acquireInfo) const {
		return device.acquireNextImage2KHR(acquireInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createDescriptorUpdateTemplate(const DescriptorUpdateTemplateCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorUpdateTemplate* pDescriptorUpdateTemplate) const {
		return device.createDescriptorUpdateTemplate(pCreateInfo, pAllocator, pDescriptorUpdateTemplate, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DescriptorUpdateTemplate>::type createDescriptorUpdateTemplate(const DescriptorUpdateTemplateCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorUpdateTemplate(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueDescriptorUpdateTemplate>::type createDescriptorUpdateTemplateUnique(const DescriptorUpdateTemplateCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorUpdateTemplateUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createDescriptorUpdateTemplateKHR(const DescriptorUpdateTemplateCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorUpdateTemplate* pDescriptorUpdateTemplate) const {
		return device.createDescriptorUpdateTemplateKHR(pCreateInfo, pAllocator, pDescriptorUpdateTemplate, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<DescriptorUpdateTemplate>::type createDescriptorUpdateTemplateKHR(const DescriptorUpdateTemplateCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorUpdateTemplateKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueDescriptorUpdateTemplate>::type createDescriptorUpdateTemplateKHRUnique(const DescriptorUpdateTemplateCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createDescriptorUpdateTemplateKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyDescriptorUpdateTemplate(DescriptorUpdateTemplate descriptorUpdateTemplate, const AllocationCallbacks* pAllocator) const {
		return device.destroyDescriptorUpdateTemplate(descriptorUpdateTemplate, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyDescriptorUpdateTemplate(DescriptorUpdateTemplate descriptorUpdateTemplate, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyDescriptorUpdateTemplate(descriptorUpdateTemplate, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(DescriptorUpdateTemplate descriptorUpdateTemplate, const AllocationCallbacks* pAllocator) const {
		return device.destroy(descriptorUpdateTemplate, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(DescriptorUpdateTemplate descriptorUpdateTemplate, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(descriptorUpdateTemplate, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyDescriptorUpdateTemplateKHR(DescriptorUpdateTemplate descriptorUpdateTemplate, const AllocationCallbacks* pAllocator) const {
		return device.destroyDescriptorUpdateTemplateKHR(descriptorUpdateTemplate, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyDescriptorUpdateTemplateKHR(DescriptorUpdateTemplate descriptorUpdateTemplate, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyDescriptorUpdateTemplateKHR(descriptorUpdateTemplate, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void updateDescriptorSetWithTemplate(DescriptorSet descriptorSet, DescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const {
		return device.updateDescriptorSetWithTemplate(descriptorSet, descriptorUpdateTemplate, pData, dispatch);
	}

	void updateDescriptorSetWithTemplateKHR(DescriptorSet descriptorSet, DescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const {
		return device.updateDescriptorSetWithTemplateKHR(descriptorSet, descriptorUpdateTemplate, pData, dispatch);
	}

	void setHdrMetadataEXT(uint32_t swapchainCount, const SwapchainKHR* pSwapchains, const HdrMetadataEXT* pMetadata) const {
		return device.setHdrMetadataEXT(swapchainCount, pSwapchains, pMetadata, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void setHdrMetadataEXT(ArrayProxy<const SwapchainKHR> swapchains, ArrayProxy<const HdrMetadataEXT> metadata) const {
		return device.setHdrMetadataEXT(swapchains, metadata, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getSwapchainStatusKHR(SwapchainKHR swapchain) const {
		return device.getSwapchainStatusKHR(swapchain, dispatch);
	}

	Result getRefreshCycleDurationGOOGLE(SwapchainKHR swapchain, RefreshCycleDurationGOOGLE* pDisplayTimingProperties) const {
		return device.getRefreshCycleDurationGOOGLE(swapchain, pDisplayTimingProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<RefreshCycleDurationGOOGLE>::type getRefreshCycleDurationGOOGLE(SwapchainKHR swapchain) const {
		return device.getRefreshCycleDurationGOOGLE(swapchain, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getPastPresentationTimingGOOGLE(SwapchainKHR swapchain, uint32_t* pPresentationTimingCount, PastPresentationTimingGOOGLE* pPresentationTimings) const {
		return device.getPastPresentationTimingGOOGLE(swapchain, pPresentationTimingCount, pPresentationTimings, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<PastPresentationTimingGOOGLE>>
	typename ResultValueType<std::vector<PastPresentationTimingGOOGLE,Allocator>>::type getPastPresentationTimingGOOGLE(SwapchainKHR swapchain) const {
		return device.getPastPresentationTimingGOOGLE(swapchain, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getBufferMemoryRequirements2(const BufferMemoryRequirementsInfo2* pInfo, MemoryRequirements2* pMemoryRequirements) const {
		return device.getBufferMemoryRequirements2(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	MemoryRequirements2 getBufferMemoryRequirements2(const BufferMemoryRequirementsInfo2 & info) const {
		return device.getBufferMemoryRequirements2<MemoryRequirements2>(info, dispatch).get<MemoryRequirements2>();
	}
	template <typename ...T>
	StructureChain<T...> getBufferMemoryRequirements2(const BufferMemoryRequirementsInfo2 & info) const {
		return device.getBufferMemoryRequirements2<T...>(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getBufferMemoryRequirements2KHR(const BufferMemoryRequirementsInfo2* pInfo, MemoryRequirements2* pMemoryRequirements) const {
		return device.getBufferMemoryRequirements2KHR(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	MemoryRequirements2 getBufferMemoryRequirements2KHR(const BufferMemoryRequirementsInfo2 & info) const {
		return device.getBufferMemoryRequirements2KHR<MemoryRequirements2>(info, dispatch).get<MemoryRequirements2>();
	}
	template <typename ...T>
	StructureChain<T...> getBufferMemoryRequirements2KHR(const BufferMemoryRequirementsInfo2 & info) const {
		return device.getBufferMemoryRequirements2KHR<T...>(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageMemoryRequirements2(const ImageMemoryRequirementsInfo2* pInfo, MemoryRequirements2* pMemoryRequirements) const {
		return device.getImageMemoryRequirements2(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	MemoryRequirements2 getImageMemoryRequirements2(const ImageMemoryRequirementsInfo2 & info) const {
		return device.getImageMemoryRequirements2<MemoryRequirements2>(info, dispatch).get<MemoryRequirements2>();
	}
	template <typename ...T>
	StructureChain<T...> getImageMemoryRequirements2(const ImageMemoryRequirementsInfo2 & info) const {
		return device.getImageMemoryRequirements2<T...>(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageMemoryRequirements2KHR(const ImageMemoryRequirementsInfo2* pInfo, MemoryRequirements2* pMemoryRequirements) const {
		return device.getImageMemoryRequirements2KHR(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	MemoryRequirements2 getImageMemoryRequirements2KHR(const ImageMemoryRequirementsInfo2 & info) const {
		return device.getImageMemoryRequirements2KHR<MemoryRequirements2>(info, dispatch).get<MemoryRequirements2>();
	}
	template <typename ...T>
	StructureChain<T...> getImageMemoryRequirements2KHR(const ImageMemoryRequirementsInfo2 & info) const {
		return device.getImageMemoryRequirements2KHR<T...>(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageSparseMemoryRequirements2(const ImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements2* pSparseMemoryRequirements) const {
		return device.getImageSparseMemoryRequirements2(pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<SparseImageMemoryRequirements2>>
	std::vector<SparseImageMemoryRequirements2,Allocator> getImageSparseMemoryRequirements2(const ImageSparseMemoryRequirementsInfo2 & info) const {
		return device.getImageSparseMemoryRequirements2(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getImageSparseMemoryRequirements2KHR(const ImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements2* pSparseMemoryRequirements) const {
		return device.getImageSparseMemoryRequirements2KHR(pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<SparseImageMemoryRequirements2>>
	std::vector<SparseImageMemoryRequirements2,Allocator> getImageSparseMemoryRequirements2KHR(const ImageSparseMemoryRequirementsInfo2 & info) const {
		return device.getImageSparseMemoryRequirements2KHR(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createSamplerYcbcrConversion(const SamplerYcbcrConversionCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, SamplerYcbcrConversion* pYcbcrConversion) const {
		return device.createSamplerYcbcrConversion(pCreateInfo, pAllocator, pYcbcrConversion, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<SamplerYcbcrConversion>::type createSamplerYcbcrConversion(const SamplerYcbcrConversionCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSamplerYcbcrConversion(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueSamplerYcbcrConversion>::type createSamplerYcbcrConversionUnique(const SamplerYcbcrConversionCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSamplerYcbcrConversionUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createSamplerYcbcrConversionKHR(const SamplerYcbcrConversionCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, SamplerYcbcrConversion* pYcbcrConversion) const {
		return device.createSamplerYcbcrConversionKHR(pCreateInfo, pAllocator, pYcbcrConversion, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<SamplerYcbcrConversion>::type createSamplerYcbcrConversionKHR(const SamplerYcbcrConversionCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSamplerYcbcrConversionKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueSamplerYcbcrConversion>::type createSamplerYcbcrConversionKHRUnique(const SamplerYcbcrConversionCreateInfo & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createSamplerYcbcrConversionKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroySamplerYcbcrConversion(SamplerYcbcrConversion ycbcrConversion, const AllocationCallbacks* pAllocator) const {
		return device.destroySamplerYcbcrConversion(ycbcrConversion, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroySamplerYcbcrConversion(SamplerYcbcrConversion ycbcrConversion, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroySamplerYcbcrConversion(ycbcrConversion, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(SamplerYcbcrConversion ycbcrConversion, const AllocationCallbacks* pAllocator) const {
		return device.destroy(ycbcrConversion, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(SamplerYcbcrConversion ycbcrConversion, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(ycbcrConversion, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroySamplerYcbcrConversionKHR(SamplerYcbcrConversion ycbcrConversion, const AllocationCallbacks* pAllocator) const {
		return device.destroySamplerYcbcrConversionKHR(ycbcrConversion, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroySamplerYcbcrConversionKHR(SamplerYcbcrConversion ycbcrConversion, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroySamplerYcbcrConversionKHR(ycbcrConversion, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getQueue2(const DeviceQueueInfo2* pQueueInfo, Queue* pQueue) const {
		return device.getQueue2(pQueueInfo, pQueue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	Queue getQueue2(const DeviceQueueInfo2 & queueInfo) const {
		return device.getQueue2(queueInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result createValidationCacheEXT(const ValidationCacheCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, ValidationCacheEXT* pValidationCache) const {
		return device.createValidationCacheEXT(pCreateInfo, pAllocator, pValidationCache, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<ValidationCacheEXT>::type createValidationCacheEXT(const ValidationCacheCreateInfoEXT & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createValidationCacheEXT(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	ResultValueType<UniqueValidationCacheEXT>::type createValidationCacheEXTUnique(const ValidationCacheCreateInfoEXT & createInfo, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.createValidationCacheEXTUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroyValidationCacheEXT(ValidationCacheEXT validationCache, const AllocationCallbacks* pAllocator) const {
		return device.destroyValidationCacheEXT(validationCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroyValidationCacheEXT(ValidationCacheEXT validationCache, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroyValidationCacheEXT(validationCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void destroy(ValidationCacheEXT validationCache, const AllocationCallbacks* pAllocator) const {
		return device.destroy(validationCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	void destroy(ValidationCacheEXT validationCache, Optional<const AllocationCallbacks> allocator = nullptr) const {
		return device.destroy(validationCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getValidationCacheDataEXT(ValidationCacheEXT validationCache, size_t* pDataSize, void* pData) const {
		return device.getValidationCacheDataEXT(validationCache, pDataSize, pData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<uint8_t>>
	typename ResultValueType<std::vector<uint8_t,Allocator>>::type getValidationCacheDataEXT(ValidationCacheEXT validationCache) const {
		return device.getValidationCacheDataEXT(validationCache, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result mergeValidationCachesEXT(ValidationCacheEXT dstCache, uint32_t srcCacheCount, const ValidationCacheEXT* pSrcCaches) const {
		return device.mergeValidationCachesEXT(dstCache, srcCacheCount, pSrcCaches, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type mergeValidationCachesEXT(ValidationCacheEXT dstCache, ArrayProxy<const ValidationCacheEXT> srcCaches) const {
		return device.mergeValidationCachesEXT(dstCache, srcCaches, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getDescriptorSetLayoutSupport(const DescriptorSetLayoutCreateInfo* pCreateInfo, DescriptorSetLayoutSupport* pSupport) const {
		return device.getDescriptorSetLayoutSupport(pCreateInfo, pSupport, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	DescriptorSetLayoutSupport getDescriptorSetLayoutSupport(const DescriptorSetLayoutCreateInfo & createInfo) const {
		return device.getDescriptorSetLayoutSupport<DescriptorSetLayoutSupport>(createInfo, dispatch).get<DescriptorSetLayoutSupport>();
	}
	template <typename ...T>
	StructureChain<T...> getDescriptorSetLayoutSupport(const DescriptorSetLayoutCreateInfo & createInfo) const {
		return device.getDescriptorSetLayoutSupport<T...>(createInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	void getDescriptorSetLayoutSupportKHR(const DescriptorSetLayoutCreateInfo* pCreateInfo, DescriptorSetLayoutSupport* pSupport) const {
		return device.getDescriptorSetLayoutSupportKHR(pCreateInfo, pSupport, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	DescriptorSetLayoutSupport getDescriptorSetLayoutSupportKHR(const DescriptorSetLayoutCreateInfo & createInfo) const {
		return device.getDescriptorSetLayoutSupportKHR<DescriptorSetLayoutSupport>(createInfo, dispatch).get<DescriptorSetLayoutSupport>();
	}
	template <typename ...T>
	StructureChain<T...> getDescriptorSetLayoutSupportKHR(const DescriptorSetLayoutCreateInfo & createInfo) const {
		return device.getDescriptorSetLayoutSupportKHR<T...>(createInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getShaderInfoAMD(Pipeline pipeline, ShaderStageFlagBits shaderStage, ShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) const {
		return device.getShaderInfoAMD(pipeline, shaderStage, infoType, pInfoSize, pInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template <typename Allocator = std::allocator<uint8_t>>
	typename ResultValueType<std::vector<uint8_t,Allocator>>::type getShaderInfoAMD(Pipeline pipeline, ShaderStageFlagBits shaderStage, ShaderInfoTypeAMD infoType) const {
		return device.getShaderInfoAMD(pipeline, shaderStage, infoType, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result setDebugUtilsObjectNameEXT(const DebugUtilsObjectNameInfoEXT* pNameInfo) const {
		return device.setDebugUtilsObjectNameEXT(pNameInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type setDebugUtilsObjectNameEXT(const DebugUtilsObjectNameInfoEXT & nameInfo) const {
		return device.setDebugUtilsObjectNameEXT(nameInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result setDebugUtilsObjectTagEXT(const DebugUtilsObjectTagInfoEXT* pTagInfo) const {
		return device.setDebugUtilsObjectTagEXT(pTagInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<void>::type setDebugUtilsObjectTagEXT(const DebugUtilsObjectTagInfoEXT & tagInfo) const {
		return device.setDebugUtilsObjectTagEXT(tagInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

	Result getMemoryHostPointerPropertiesEXT(ExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, MemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) const {
		return device.getMemoryHostPointerPropertiesEXT(handleType, pHostPointer, pMemoryHostPointerProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<MemoryHostPointerPropertiesEXT>::type getMemoryHostPointerPropertiesEXT(ExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer) const {
		return device.getMemoryHostPointerPropertiesEXT(handleType, pHostPointer, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	Result getAndroidHardwareBufferPropertiesANDROID(const struct AHardwareBuffer* buffer, AndroidHardwareBufferPropertiesANDROID* pProperties) const {
		return device.getAndroidHardwareBufferPropertiesANDROID(buffer, pProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<AndroidHardwareBufferPropertiesANDROID>::type getAndroidHardwareBufferPropertiesANDROID(const struct AHardwareBuffer & buffer) const {
		return device.getAndroidHardwareBufferPropertiesANDROID(buffer, dispatch);
	}
	template <typename ...T>
	typename ResultValueType<StructureChain<T...>>::type getAndroidHardwareBufferPropertiesANDROID(const struct AHardwareBuffer & buffer) const {
		return device.getAndroidHardwareBufferPropertiesANDROID(buffer, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/

#ifdef VK_USE_PLATFORM_ANDROID_ANDROID
	Result getMemoryAndroidHardwareBufferANDROID(const MemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) const {
		return device.getMemoryAndroidHardwareBufferANDROID(pInfo, pBuffer, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	ResultValueType<struct AHardwareBuffer*>::type getMemoryAndroidHardwareBufferANDROID(const MemoryGetAndroidHardwareBufferInfoANDROID & info) const {
		return device.getMemoryAndroidHardwareBufferANDROID(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_ANDROID_ANDROID*/

	VULKAN_HPP_TYPESAFE_EXPLICIT operator VkDevice() const
	{
		return device;
	}

	explicit operator bool() const
	{
		return device;
	}

	bool operator!() const
	{
		return !device;
	}

private:
	Device device{};
	DispatchLoaderDynamic dispatch{};
};

} // vk

namespace rn::vki {

using Device = vk::DeviceDispatch;

} // rn::vki
