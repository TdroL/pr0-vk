#include "surfaceFormatSelector.hpp"

#include <cassert>
#include <vector>

#include "../../../ngn/config.hpp"
#include "../../../util/fold.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"

namespace rn::vki {

vk::SurfaceFormatKHR SurfaceFormatSelector::select(Context &context) {
	vk::SurfaceKHR surface = context.surface.handle;
	vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;

	assert(surface);
	assert(physicalDevice);

	std::vector<vk::SurfaceFormatKHR> surfaceFormats = RN_VLK_TRACE(physicalDevice.getSurfaceFormatsKHR(surface));

	if (surfaceFormats.empty()) {
		throw std::runtime_error{"Vulkan surface formats not available"};
	}

	vk::Format desiredFormat = std::visit(util::fold{
		[] (rn::PixelFormat value) {
			return fromCore(value);
		},
		[] (uint32_t value) {
			static_assert(sizeof(vk::Format) == sizeof(value));
			return static_cast<vk::Format>(value);
		}
	}, ngn::config::core.window.surfaceFormat());

	// if format is undefined, then it will accept any valid
	if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined) {
		return {
			desiredFormat,
			surfaceFormats[0].colorSpace
		};
	}

	// try to match both format and color space
	for (const auto& surfaceFormat : surfaceFormats) {
		if (surfaceFormat.format == desiredFormat) {
			bool matchesColorSpace = std::visit(util::fold{
				[colorSpace = surfaceFormat.colorSpace] (rn::ColorSpace value) {
					switch (value) {
						default:
						case rn::ColorSpace::LDR: {
							return colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
						}
						case rn::ColorSpace::HDR: {
							// compare with known HDR formats
							switch (colorSpace) {
								case vk::ColorSpaceKHR::eDisplayP3NonlinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eDciP3LinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eDciP3NonlinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eBt709LinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eBt709NonlinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eBt2020LinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eHdr10St2084EXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eDolbyvisionEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eHdr10HlgEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eAdobergbLinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eAdobergbNonlinearEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::ePassThroughEXT: [[fallthrough]];
								case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT: return true;
								default: return false;
							}
						}
					}
				},
				[colorSpace = surfaceFormat.colorSpace] (uint32_t value) {
					static_assert(sizeof(vk::ColorSpaceKHR) == sizeof(value));
					return colorSpace == static_cast<vk::ColorSpaceKHR>(value);
				}
			}, ngn::config::core.window.surfaceColorSpace());

			if (matchesColorSpace) {
				return surfaceFormat;
			}
		}
	}

	// try to match format with any color space
	for (const auto& surfaceFormat : surfaceFormats) {
		if (surfaceFormat.format == desiredFormat) {
			return surfaceFormat;
		}
	}

	// matched nothing, use first available format
	return surfaceFormats[0];
}

} // rn::vki
