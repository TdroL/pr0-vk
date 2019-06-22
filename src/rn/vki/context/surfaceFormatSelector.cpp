#include "surfaceFormatSelector.hpp"

#include <algorithm>
#include <cassert>
#include <variant>
#include <vector>

#include <fmt/format.h>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../../../util/contains.hpp"
// #include "../../../util/format.hpp"
#include "../../../util/join.hpp"
#include "../../../util/map.hpp"
#include "../id.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"

namespace rn::vki::context {

vk::SurfaceFormatKHR SurfaceFormatSelector::select(rn::vki::HandlePhysicalDevice &&physicalDevice, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config) {
	std::vector<vk::SurfaceFormatKHR> surfaceFormats = RN_VKI_TRACE(physicalDevice->getSurfaceFormatsKHR(surface.get(), physicalDevice.table()));

	if (surfaceFormats.empty()) {
		throw std::runtime_error{"Vulkan surface formats not available"};
	}

	ngn::log::debug("rn::vki::context::SurfaceFormatSelector::select({:#x}, {:#x}) => available surface formats: [{}]", rn::vki::id(surface.get()), rn::vki::id(physicalDevice.get()), util::join(util::map(surfaceFormats, [] (const auto &surfaceFormat) {
		return fmt::format("format={} ({}) colorSpace={} ({})", vk::to_string(surfaceFormat.format), static_cast<uint32_t>(surfaceFormat.format), vk::to_string(surfaceFormat.colorSpace), static_cast<uint32_t>(surfaceFormat.colorSpace));
	})));

	vk::Format desiredFormat = vk::Format::eUndefined;
	if (auto value = std::get_if<rn::PixelFormat>(&config.core.window.surfaceFormat)) {
		desiredFormat = fromCommon(*value);
	} else if (auto value = std::get_if<uint32_t>(&config.core.window.surfaceFormat)) {
		static_assert(sizeof(vk::Format) == sizeof(*value));
		desiredFormat = static_cast<vk::Format>(*value);
	} else {
		assert("config.core.window.surfaceFormat holds unknown alternative");
	}

	// if undefined format exists, then any format is accepted
	bool hasUndefinedFormat = util::containsIf(surfaceFormats, [] (auto &surfaceFormat) {
		return surfaceFormat.format == vk::Format::eUndefined;
	});
	if (surfaceFormats.size() > 0 && hasUndefinedFormat) {
		return {
			desiredFormat,
			surfaceFormats[0].colorSpace
		};
	}

	// try to match both format and color space
	if (auto value = std::get_if<rn::ColorSpace>(&config.core.window.surfaceColorSpace)) {
		if (*value == rn::ColorSpace::LDR) {
			const auto foundFormatLDR = std::find_if(std::begin(surfaceFormats), std::end(surfaceFormats), [=] (const auto &surfaceFormat) {
				return surfaceFormat.format == desiredFormat && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
			});

			if (foundFormatLDR != std::end(surfaceFormats)) {
				return *foundFormatLDR;
			}
		} else if (*value == rn::ColorSpace::HDR) {
			const auto foundFormatHDR = std::find_if(std::begin(surfaceFormats), std::end(surfaceFormats), [=] (const auto &surfaceFormat) {
				if (surfaceFormat.format == desiredFormat) {
					// switch (surfaceFormat.colorSpace) {
					// 	case vk::ColorSpaceKHR::eDisplayP3NonlinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eDciP3LinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eDciP3NonlinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eBt709LinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eBt709NonlinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eBt2020LinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eHdr10St2084EXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eDolbyvisionEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eHdr10HlgEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eAdobergbLinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eAdobergbNonlinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::ePassThroughEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT: [[fallthrough]];
					// 	case vk::ColorSpaceKHR::eDisplayNativeAMD: {
					// 		return true;
					// 	}
					// 	default: {
					// 		return false;
					// 	}
					// }

					return surfaceFormat.colorSpace != vk::ColorSpaceKHR::eSrgbNonlinear;
				}

				return false;
			});

			if (foundFormatHDR != std::end(surfaceFormats)) {
				return *foundFormatHDR;
			}
		}
	} else if (auto value = std::get_if<uint32_t>(&config.core.window.surfaceColorSpace)) {
		static_assert(sizeof(vk::ColorSpaceKHR) == sizeof(*value));

		const auto foundFormatSpec = std::find_if(std::begin(surfaceFormats), std::end(surfaceFormats), [=, desiredColorSpace = static_cast<vk::ColorSpaceKHR>(*value)] (const auto &surfaceFormat) {
			return surfaceFormat.format == desiredFormat && surfaceFormat.colorSpace == desiredColorSpace;
		});

		if (foundFormatSpec != std::end(surfaceFormats)) {
			return *foundFormatSpec;
		}
	} else {
		assert("config.core.window.surfaceColorSpace holds unknown alternative");
	}

	const auto foundFormat = std::find_if(std::begin(surfaceFormats), std::end(surfaceFormats), [=] (const auto &surfaceFormat) {
		return surfaceFormat.format == desiredFormat;
	});
	if (foundFormat != std::end(surfaceFormats)) {
		return *foundFormat;
	} else {
		return surfaceFormats[0];
	}
}

} // rn::vki::context
