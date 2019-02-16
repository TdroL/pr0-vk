#include "surfaceImagesSelector.hpp"

#include "../trace.hpp"

namespace rn::vki::context {

std::vector<vk::Image> SurfaceImagesSelector::select(rn::vki::HandleSwapchainKHR &&swapchain, rn::vki::HandleDevice &&device) {
	return RN_VKI_TRACE(device->getSwapchainImagesKHR(swapchain.get(), device.table()));
}

} // rn::vki::context
