#pragma once

#include <cassert>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"
#include "../../../ngn/log.hpp"

namespace rn::vlk {

class SurfaceImagesSelector {
public:
	std::vector<vk::Image> select(Context &context) {
		vk::Device device = context.device;
		vk::SwapchainKHR swapchain = context.swapchain;

		assert(device);
		assert(swapchain);

		return device.getSwapchainImagesKHR(swapchain);
	}
};

} // rn::vlk
