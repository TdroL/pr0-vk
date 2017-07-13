#pragma once

#include <vector>
#include <cassert>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/log.hpp"

namespace rn {

namespace vlk {

class SurfaceImagesSelector {
public:
	std::vector<vk::Image> select(vk::UniqueDevice &deviceOwner, vk::UniqueSwapchainKHR &swapchainOwner) {
		vk::Device device = deviceOwner.get();
		vk::SwapchainKHR swapchain = swapchainOwner.get();

		assert(device);
		assert(swapchain);

		return device.getSwapchainImagesKHR(swapchain);
	}
};

} // vlk

} // rn
