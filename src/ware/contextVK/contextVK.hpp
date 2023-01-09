#pragma once

#include <vulkan/vulkan.hpp>

#include <util/uniqueResource.hpp>

#include "../config/config.hpp"
#include "../contextGLFW/contextGLFW.hpp"
#include "../windowGLFW/windowGLFW.hpp"
#include "vma.hpp"

namespace ware::contextVK {

struct State {
	vk::UniqueInstance instance;
	vk::UniqueDebugUtilsMessengerEXT debugUtilsMessanger;
	vk::UniqueSurfaceKHR surface;
	vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features> &features;
	vk::PhysicalDevice physicalDevice;
	vk::PhysicalDeviceProperties2 physicalDeviceProperties2;
	vk::PhysicalDeviceDriverProperties physicalDeviceDriverProperties;
	vk::PhysicalDeviceMemoryProperties2 physicalDeviceMemoryProperties2;
	std::vector<vk::QueueFamilyProperties2> queueFamilyProperties2;
	vk::UniqueDevice device;
	vk::Queue presentationQueue;
	uint32_t presentationQueueFamily;
	uint32_t presentationQueueIndex;
	vk::Queue graphicQueue;
	uint32_t graphicQueueFamily;
	uint32_t graphicQueueIndex;
	vk::Queue computeQueue;
	uint32_t computeQueueFamily;
	uint32_t computeQueueIndex;
	vk::Queue transferQueue;
	uint32_t transferQueueFamily;
	uint32_t transferQueueIndex;
	util::UniqueResource<VmaAllocator> allocator;
	vk::UniquePipelineCache pipelineCache;
	bool requestedWaitIdle;
};

struct BufferState {
	vk::Buffer buffer;
	vk::DeviceMemory memory;
	vk::DeviceSize offset;
	vk::DeviceSize size;
	void *mappedData;
	VmaAllocation allocation;
	VmaAllocator &allocator;
};

using UniqueBuffer = util::UniqueResource<BufferState, void (*)(BufferState &)>;

void requestWaitIdle(State &context);

UniqueBuffer createBuffer(State &context, vk::BufferCreateInfo &bufferCreateInfo, VmaAllocationCreateInfo &allocationCreateInfo);

State setup(ware::config::State &config, ware::contextGLFW::State &glfw, ware::windowGLFW::State &window);

void refresh(State &state);

void process(State &state);

} // ware::contextVK
