#pragma once

#include <stdexcept>

#include <vulkan/vulkan.hpp>

// #include "instanceOwner.hpp"
// #include "surfaceOwner.hpp"
// #include "debugCallbackOwner.hpp"
// #include "physicalDeviceOwner.hpp"
// #include "deviceOwner.hpp"
// #include "queuesOwner.hpp"
// #include "swapchainOwner.hpp"

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


	// shortcuts
	// vk::Device device{};

	// struct Queues {
	// 	vk::Queue presentation{};
	// 	vk::Queue graphic{};
	// 	vk::Queue compute{};
	// 	vk::Queue transfer{};
	// } queues;

	// vk::SwapchainKHR swapchain{};
};

} // vlk

} // rn
