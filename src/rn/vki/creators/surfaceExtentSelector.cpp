#include "surfaceExtentSelector.hpp"

#include <cassert>
#include <vector>

#include "../../../ngn/config.hpp"
#include "../trace.hpp"

namespace rn::vki {

vk::Extent2D SurfaceExtentSelector::select(Context &context) {
	vk::SurfaceKHR surface = context.surface.handle;
	vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;

	assert(surface);
	assert(physicalDevice);

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = RN_VLK_TRACE(physicalDevice.getSurfaceCapabilitiesKHR(surface));

	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return surfaceCapabilities.currentExtent;
	} else {
		vk::Extent2D desiredExtent{
			ngn::config::core.window.width(),
			ngn::config::core.window.height()
		};

		desiredExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, desiredExtent.width));
		desiredExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, desiredExtent.height));

		return desiredExtent;
	}
}

} // rn::vki
