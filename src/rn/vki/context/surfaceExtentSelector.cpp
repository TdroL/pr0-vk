#include "surfaceExtentSelector.hpp"

#include "../../../ngn/config.hpp"
#include "../trace.hpp"

namespace rn::vki::context {

vk::Extent2D SurfaceExtentSelector::select(rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandlePhysicalDevice &&physicalDevice, ngn::config::Config &config) {
	vk::SurfaceCapabilitiesKHR surfaceCapabilities = RN_VKI_TRACE(physicalDevice->getSurfaceCapabilitiesKHR(surface.get(), physicalDevice.table()));

	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return surfaceCapabilities.currentExtent;
	} else {
		vk::Extent2D desiredExtent{
			config.core.window.width,
			config.core.window.height
		};

		desiredExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, desiredExtent.width));
		desiredExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, desiredExtent.height));

		return desiredExtent;
	}
}

} // rn::vki::context
