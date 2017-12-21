#include "surfaceFormatSelector.hpp"

#include <cassert>
#include <vector>

#include "../../../ngn/config.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"

namespace rn::vlk {

vk::SurfaceFormatKHR SurfaceFormatSelector::select(Context &context) {
	vk::SurfaceKHR surface = context.surface.handle;
	vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;

	assert(surface);
	assert(physicalDevice);

	std::vector<vk::SurfaceFormatKHR> surfaceFormats = RN_VLK_TRACE(physicalDevice.getSurfaceFormatsKHR(surface));

	if (surfaceFormats.empty()) {
		throw std::runtime_error{"Vulkan surface formats not available"};
	}

	vk::SurfaceFormatKHR desiredSurfaceFormat{
		fromConfig(ngn::config::core.window.surfaceFormat()),
		fromConfig(ngn::config::core.window.surfaceColorSpace())
	};

	if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined) {
		return desiredSurfaceFormat;
	}

	for (const auto& surfaceFormat : surfaceFormats) {
		if (surfaceFormat.format == desiredSurfaceFormat.format && surfaceFormat.colorSpace == desiredSurfaceFormat.colorSpace) {
			return desiredSurfaceFormat;
		}
	}

	return surfaceFormats[0];
}

} // rn::vlk
