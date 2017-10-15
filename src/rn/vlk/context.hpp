#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "memory/pool.hpp"

namespace rn::vlk {

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

	vk::Instance instance{};

	struct PhysicalDevice {
		vk::PhysicalDevice handle{};
		vk::PhysicalDeviceProperties properties{};
		vk::PhysicalDeviceMemoryProperties memoryProperties{};
		vk::PhysicalDeviceFeatures availableFeatures{};
		vk::PhysicalDeviceFeatures requiredFeatures{};
	} physicalDevice{};

	struct Queue {
		vk::Queue presentation{};
		vk::Queue graphic{};
		vk::Queue compute{};
		vk::Queue transfer{};
	} queue{};

	struct Family {
		uint32_t presentation{};
		uint32_t graphic{};
		uint32_t compute{};
		uint32_t transfer{};
	} family{};

	vk::Device device{};

	vk::SwapchainKHR swapchain{};

	struct Surface {
		vk::SurfaceKHR handle{};
		vk::Extent2D extent{};
		vk::SurfaceFormatKHR format{};
		std::vector<vk::Image> images{};
		std::vector<vk::ImageView> imageViews{};
	} surface{};

	struct Allocator {
		memory::Pool frameRT{};
		memory::Pool squareRT{};
		memory::Pool small{};
		memory::Pool huge{};
	} allocator{};
};

} // rn::vlk
