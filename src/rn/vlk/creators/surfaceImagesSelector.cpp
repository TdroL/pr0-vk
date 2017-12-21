#include "surfaceImagesSelector.hpp"

#include <cassert>

#include "../trace.hpp"

namespace rn::vlk {

std::vector<vk::Image> SurfaceImagesSelector::select(Context &context) {
	vk::Device device = context.device;
	vk::SwapchainKHR swapchain = context.swapchain;

	assert(device);
	assert(swapchain);

	return RN_VLK_TRACE(device.getSwapchainImagesKHR(swapchain));
}

} // rn::vlk
