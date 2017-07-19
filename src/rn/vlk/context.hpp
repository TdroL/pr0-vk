#pragma once

#include <vulkan/vulkan.hpp>

#include "physicalDeviceHandle.hpp"
#include "queuesHandle.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	struct Owners {
		vk::UniqueInstance instance{};
		vk::UniqueDebugReportCallbackEXT debugCallback{};
		vk::UniqueSurfaceKHR surface{};
		vk::UniqueDevice device{};
		vk::UniqueSwapchainKHR swapchain{};
		std::vector<vk::UniqueImageView> surfaceImageViews{};
	} owners{};

	struct Handles {
		PhysicalDeviceHandle physicalDevice{};
		QueuesHandle queues{};
		vk::Extent2D surfaceExtent{};
		vk::SurfaceFormatKHR surfaceFormat{};
		std::vector<vk::Image> surfaceImages{};
	} handles{};
};

} // vlk

} // rn
