#pragma once

#include <vulkan/vulkan.hpp>

namespace rn::vki {

extern vk::DispatchLoaderDynamic staticDispatchTable;

template<class H>
class HandleDispatch {
public:
	H staticHandle{};
	vk::DispatchLoaderDynamic *dispatchTable = nullptr;

	HandleDispatch() = default;

	HandleDispatch(H staticHandle, vk::DispatchLoaderDynamic &dispatchTable) noexcept :
		staticHandle{staticHandle},
		dispatchTable{&dispatchTable}
	{}

	HandleDispatch(const HandleDispatch<H> &other) = default;
	HandleDispatch(HandleDispatch<H> &&other) = default;

	HandleDispatch<H> & operator=(const HandleDispatch<H> &other) = default;
	HandleDispatch<H> & operator=(HandleDispatch<H> &&other) = default;

	void reset() {
		staticHandle = H{};
		dispatchTable = nullptr;
	}

	explicit operator bool() const {
		return staticHandle.operator bool();
	}

	const H * operator->() const {
		return &staticHandle;
	}

	H * operator->() {
		return &staticHandle;
	}

	const H & operator*() const {
		return staticHandle;
	}

	H & operator*() {
		return staticHandle;
	}
	const H & get() const {
		return staticHandle;
	}

	H & get() {
		return staticHandle;
	}

	HandleDispatch<H> handle() {
		return HandleDispatch<H>{ staticHandle, table() };
	}

	vk::DispatchLoaderDynamic & table() {
		return *dispatchTable;
	}

	const vk::DispatchLoaderDynamic & table() const {
		return *dispatchTable;
	}
};

template<class H>
class UniqueDispatch {
public:
	vk::UniqueHandle<H, vk::DispatchLoaderDynamic> uniqueHandle{};
	vk::DispatchLoaderDynamic *dispatchTable = nullptr;

	UniqueDispatch() = default;

	UniqueDispatch(vk::UniqueHandle<H, vk::DispatchLoaderStatic> &&uniqueHandle, vk::DispatchLoaderDynamic &dispatchTable) = delete;

	UniqueDispatch(vk::UniqueHandle<H, vk::DispatchLoaderDynamic> &&uniqueHandle, vk::DispatchLoaderDynamic &dispatchTable) noexcept :
		uniqueHandle{std::move(uniqueHandle)},
		dispatchTable{&dispatchTable}
	{}

	UniqueDispatch(const UniqueDispatch<H> &other) = delete;
	UniqueDispatch(UniqueDispatch<H> &&other) = default;

	UniqueDispatch<H> & operator=(const UniqueDispatch<H> &other) = delete;
	UniqueDispatch<H> & operator=(UniqueDispatch<H> &&other) = default;

	void reset() {
		uniqueHandle.reset();
		dispatchTable = nullptr;
	}

	explicit operator bool() const {
		return uniqueHandle->operator bool();
	}

	const H * operator->() const {
		return &uniqueHandle.get();
	}

	H * operator->() {
		return &uniqueHandle.get();
	}

	const H & operator*() const {
		return uniqueHandle.get();
	}

	H & operator*() {
		return uniqueHandle.get();
	}

	const H & get() const {
		return uniqueHandle.get();
	}

	H & get() {
		return uniqueHandle.get();
	}

	HandleDispatch<H> handle() {
		return HandleDispatch<H>{ uniqueHandle.get(), table() };
	}

	vk::DispatchLoaderDynamic & table() {
		return *dispatchTable;
	}

	const vk::DispatchLoaderDynamic & table() const {
		return *dispatchTable;
	}
};

template<class H>
void loadExtensions([[maybe_unused]] H &handle) {}

template<class H>
class UniqueTableDispatch {
public:
	vk::UniqueHandle<H, vk::DispatchLoaderStatic> uniqueHandle{};
	std::unique_ptr<vk::DispatchLoaderDynamic> dispatchTable{};

	UniqueTableDispatch() = default;

	UniqueTableDispatch(vk::UniqueHandle<H, vk::DispatchLoaderDynamic> &&uniqueHandle, std::unique_ptr<vk::DispatchLoaderDynamic> &&dispatchTable) = delete;
	UniqueTableDispatch(vk::UniqueHandle<H, vk::DispatchLoaderStatic> &&uniqueHandle, std::unique_ptr<vk::DispatchLoaderDynamic> &&dispatchTable) noexcept :
		uniqueHandle{std::move(uniqueHandle)},
		dispatchTable{std::move(dispatchTable)}
	{
		loadExtensions(this->uniqueHandle);
	}

	UniqueTableDispatch(UniqueDispatch<H> &&uniqueDispatch, std::unique_ptr<vk::DispatchLoaderDynamic> &&dispatchTable) noexcept :
		uniqueHandle{std::move(uniqueDispatch.uniqueHandle)},
		dispatchTable{std::move(dispatchTable)}
	{
		loadExtensions(this->uniqueHandle);
	}

	UniqueTableDispatch(const UniqueTableDispatch<H> &other) = delete;
	UniqueTableDispatch(UniqueTableDispatch<H> &&other) noexcept :
		uniqueHandle{std::move(other.uniqueHandle)},
		dispatchTable{std::move(other.dispatchTable)}
	{}

	UniqueTableDispatch<H> & operator=(const UniqueTableDispatch<H> &other) = delete;
	UniqueTableDispatch<H> & operator=(UniqueTableDispatch<H> &&other) {
		uniqueHandle.reset(other.uniqueHandle.release());
		dispatchTable.reset(other.dispatchTable.release());

		return *this;
	}

	void reset() {
		uniqueHandle.reset();
		dispatchTable.reset();
	}

	explicit operator bool() const {
		return uniqueHandle.operator bool();
	}

	const H * operator->() const {
		return &uniqueHandle.get();
	}

	H * operator->() {
		return &uniqueHandle.get();
	}

	const H & operator*() const {
		return uniqueHandle.get();
	}

	H & operator*() {
		return uniqueHandle.get();
	}

	const H & get() const {
		return uniqueHandle.get();
	}

	H & get() {
		return uniqueHandle.get();
	}

	HandleDispatch<H> handle() {
		return HandleDispatch<H>{ uniqueHandle.get(), table() };
	}

	vk::DispatchLoaderDynamic & table() {
		return *dispatchTable;
	}

	const vk::DispatchLoaderDynamic & table() const {
		return *dispatchTable;
	}
};

using HandleAccelerationStructureNV = HandleDispatch<vk::AccelerationStructureNV>;
using HandleBuffer = HandleDispatch<vk::Buffer>;
using HandleBufferView = HandleDispatch<vk::BufferView>;
using HandleCommandBuffer = HandleDispatch<vk::CommandBuffer>;
using HandleCommandPool = HandleDispatch<vk::CommandPool>;
using HandleDebugReportCallbackEXT = HandleDispatch<vk::DebugReportCallbackEXT>;
using HandleDebugUtilsMessengerEXT = HandleDispatch<vk::DebugUtilsMessengerEXT>;
using HandleDescriptorPool = HandleDispatch<vk::DescriptorPool>;
using HandleDescriptorSet = HandleDispatch<vk::DescriptorSet>;
using HandleDescriptorSetLayout = HandleDispatch<vk::DescriptorSetLayout>;
using HandleDescriptorUpdateTemplate = HandleDispatch<vk::DescriptorUpdateTemplate>;
using HandleDevice = HandleDispatch<vk::Device>;
using HandleDeviceMemory = HandleDispatch<vk::DeviceMemory>;
using HandleDisplayKHR = HandleDispatch<vk::DisplayKHR>;
using HandleDisplayModeKHR = HandleDispatch<vk::DisplayModeKHR>;
using HandleEvent = HandleDispatch<vk::Event>;
using HandleFence = HandleDispatch<vk::Fence>;
using HandleFramebuffer = HandleDispatch<vk::Framebuffer>;
using HandleImage = HandleDispatch<vk::Image>;
using HandleImageView = HandleDispatch<vk::ImageView>;
using HandleIndirectCommandsLayoutNVX = HandleDispatch<vk::IndirectCommandsLayoutNVX>;
using HandleInstance = HandleDispatch<vk::Instance>;
using HandleObjectTableNVX = HandleDispatch<vk::ObjectTableNVX>;
using HandlePhysicalDevice = HandleDispatch<vk::PhysicalDevice>;
using HandlePipeline = HandleDispatch<vk::Pipeline>;
using HandlePipelineCache = HandleDispatch<vk::PipelineCache>;
using HandlePipelineLayout = HandleDispatch<vk::PipelineLayout>;
using HandleQueryPool = HandleDispatch<vk::QueryPool>;
using HandleQueue = HandleDispatch<vk::Queue>;
using HandleRenderPass = HandleDispatch<vk::RenderPass>;
using HandleSampler = HandleDispatch<vk::Sampler>;
using HandleSamplerYcbcrConversion = HandleDispatch<vk::SamplerYcbcrConversion>;
using HandleSemaphore = HandleDispatch<vk::Semaphore>;
using HandleShaderModule = HandleDispatch<vk::ShaderModule>;
using HandleSurfaceKHR = HandleDispatch<vk::SurfaceKHR>;
using HandleSwapchainKHR = HandleDispatch<vk::SwapchainKHR>;
using HandleValidationCacheEXT = HandleDispatch<vk::ValidationCacheEXT>;

using UniqueAccelerationStructureNV = UniqueDispatch<vk::AccelerationStructureNV>;
using UniqueBuffer = UniqueDispatch<vk::Buffer>;
using UniqueBufferView = UniqueDispatch<vk::BufferView>;
using UniqueCommandBuffer = UniqueDispatch<vk::CommandBuffer>;
using UniqueCommandPool = UniqueDispatch<vk::CommandPool>;
using UniqueDescriptorPool = UniqueDispatch<vk::DescriptorPool>;
using UniqueDescriptorSet = UniqueDispatch<vk::DescriptorSet>;
using UniqueDescriptorSetLayout = UniqueDispatch<vk::DescriptorSetLayout>;
using UniqueDescriptorUpdateTemplate = UniqueDispatch<vk::DescriptorUpdateTemplate>;
using UniqueDeviceMemory = UniqueDispatch<vk::DeviceMemory>;
using UniqueEvent = UniqueDispatch<vk::Event>;
using UniqueFence = UniqueDispatch<vk::Fence>;
using UniqueFramebuffer = UniqueDispatch<vk::Framebuffer>;
using UniqueImage = UniqueDispatch<vk::Image>;
using UniqueImageView = UniqueDispatch<vk::ImageView>;
using UniqueIndirectCommandsLayoutNVX = UniqueDispatch<vk::IndirectCommandsLayoutNVX>;
using UniqueObjectTableNVX = UniqueDispatch<vk::ObjectTableNVX>;
using UniquePipeline = UniqueDispatch<vk::Pipeline>;
using UniquePipelineCache = UniqueDispatch<vk::PipelineCache>;
using UniquePipelineLayout = UniqueDispatch<vk::PipelineLayout>;
using UniqueQueryPool = UniqueDispatch<vk::QueryPool>;
using UniqueRenderPass = UniqueDispatch<vk::RenderPass>;
using UniqueSampler = UniqueDispatch<vk::Sampler>;
using UniqueSamplerYcbcrConversion = UniqueDispatch<vk::SamplerYcbcrConversion>;
using UniqueSemaphore = UniqueDispatch<vk::Semaphore>;
using UniqueShaderModule = UniqueDispatch<vk::ShaderModule>;
using UniqueSwapchainKHR = UniqueDispatch<vk::SwapchainKHR>;
using UniqueValidationCacheEXT = UniqueDispatch<vk::ValidationCacheEXT>;
using UniqueDevice = UniqueDispatch<vk::Device>;
using UniqueDebugReportCallbackEXT = UniqueDispatch<vk::DebugReportCallbackEXT>;
using UniqueDebugUtilsMessengerEXT = UniqueDispatch<vk::DebugUtilsMessengerEXT>;
using UniqueSurfaceKHR = UniqueDispatch<vk::SurfaceKHR>;
using UniqueInstance = UniqueDispatch<vk::Instance>;

using UniqueTableDevice = UniqueTableDispatch<vk::Device>;
using UniqueTableInstance = UniqueTableDispatch<vk::Instance>;


} // rn::vki
