#include "surfaceImageViewsCreator.hpp"

#include <cassert>
#include <variant>

#include "../../../ngn/config.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"
#include "types.hpp"

namespace rn::vki::context {

std::vector<rn::vki::UniqueImageView> SurfaceImageViewsCreator::create(rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device, ngn::config::Config &config) {
	vk::Format imageFormat;
	if (auto ptr = std::get_if<rn::PixelFormat>(&config.core.window.surfaceFormat)) {
		imageFormat = fromCommon(*ptr);
	} else if (auto ptr = std::get_if<uint32_t>(&config.core.window.surfaceFormat)) {
		static_assert(sizeof(vk::Format) == sizeof(*ptr));
		imageFormat = static_cast<vk::Format>(*ptr);
	} else {
		assert("config.core.window.surfaceFormat holds unknown alternative");
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
