#pragma once

#include <mutex>

#include "../memory/pool.hpp"
#include "../dispatch.hpp"

namespace rn::vki::context {

struct Debug {
	rn::vki::UniqueDebugReportCallbackEXT debugReportCallback{};
	rn::vki::UniqueDebugUtilsMessengerEXT debugUtilsMessenger{};
};

struct PhysicalDeviceDescription {
	vk::PhysicalDeviceProperties2 properties{};
	vk::PhysicalDeviceMemoryProperties2 memoryProperties{};
	vk::PhysicalDeviceFeatures2 availableFeatures{};
	vk::PhysicalDeviceFeatures2 requiredFeatures{};
	std::vector<vk::QueueFamilyProperties2> familyProperties{};
};

struct QueueFamilyIndex {
	struct FamilyIndex {
		uint32_t family = 0;
		uint32_t index = 0;
	};

	FamilyIndex presentation{};
	FamilyIndex graphic{};
	FamilyIndex compute{};
	FamilyIndex transfer{};
};

struct Queue {
	vk::Queue presentation{};
	vk::Queue graphic{};
	vk::Queue compute{};
	vk::Queue transfer{};
};

struct SurfaceDescription {
	vk::Extent2D extent{};
	vk::SurfaceFormatKHR format{};
	std::vector<vk::Image> images{};
	std::vector<vk::ImageView> imageViews{};
	uint32_t imageIndex = 0;
};

struct Allocator {
	rn::vki::memory::Pool buffer{};
	rn::vki::memory::Pool texture{};
	rn::vki::memory::Pool staging{};
};

struct CommandPoolGroups {
	std::vector<std::vector<rn::vki::UniqueCommandPool>> graphicGroup{};
	std::vector<std::vector<rn::vki::UniqueCommandPool>> computeGroup{};
	std::vector<std::vector<rn::vki::UniqueCommandPool>> transferGroup{};
};

struct Synchronization {
	struct RenderingResources {
		rn::vki::UniqueSemaphore acquireSemaphore{};
		std::array<rn::vki::UniqueSemaphore, 2> graphicSemaphores{};

		rn::vki::UniqueFence graphicFence{};
		rn::vki::UniqueFence computeFence{};
	};

	std::vector<RenderingResources> renderingResources{};
	size_t renderingIndex = 0;
	size_t renderingCounter = 0;

	std::vector<rn::vki::UniqueFence> transferFences{};
	size_t resolvedIndex = 0;
	size_t resolvedCounter = 0;
	size_t pendingIndex = 0;
	size_t pendingCounter = 0;
};

} // rn::vki::context
