#include "surfaceImageViewsCreator.hpp"

#include <cassert>
#include <cstdlib>
#include <string>
#include <variant>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"
#include "types.hpp"

namespace rn::vki::context {

std::vector<rn::vki::UniqueImageView> SurfaceImageViewsCreator::create(rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device, ngn::config::Config &config) {
	vk::Format imageFormat;
	if (auto value = std::get_if<rn::PixelFormat>(&config.core.window.surfaceFormat)) {
		imageFormat = fromCommon(*value);
	} else if (auto value = std::get_if<uint32_t>(&config.core.window.surfaceFormat)) {
		static_assert(sizeof(vk::Format) == sizeof(*value));
		imageFormat = static_cast<vk::Format>(*value);
	} else {
		ngn::log::error("rn::vki::context::SurfaceImageViewsCreator::create() => undefined surface format (variant index: {})", config.core.window.surfaceFormat.valueless_by_exception() ? std::string{"valueless"} : std::to_string(config.core.window.surfaceFormat.index()));
		std::exit(EXIT_FAILURE);
		// TODO: fallback to some default format?
	}

	std::vector<rn::vki::UniqueImageView> surfaceImageViews{};
	surfaceImageViews.resize(surfaceDescription.images.size());

	for (size_t i = 0; i < surfaceDescription.images.size(); i++) {
		vk::ImageViewCreateInfo imageViewCreateInfo{
			/*.flags=*/ vk::ImageViewCreateFlags{},
			/*.image=*/ surfaceDescription.images[i],
			/*.viewType=*/ vk::ImageViewType::e2D,
			/*.format=*/ imageFormat,
			/*.components=*/ {
				/*.r=*/ vk::ComponentSwizzle::eIdentity,
				/*.g=*/ vk::ComponentSwizzle::eIdentity,
				/*.b=*/ vk::ComponentSwizzle::eIdentity,
				/*.a=*/ vk::ComponentSwizzle::eIdentity
			},
			/*.subresourceRange=*/ {
				/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
				/*.baseMipLevel=*/ 0,
				/*.levelCount=*/ 1,
				/*.baseArrayLayer=*/ 0,
				/*.layerCount=*/ 1
			}
		};

		surfaceImageViews[i] = {
			RN_VKI_TRACE(device->createImageViewUnique(imageViewCreateInfo, nullptr, device.table())),
			device.table()
		};
	}

	return surfaceImageViews;
}

} // rn::vki::context
