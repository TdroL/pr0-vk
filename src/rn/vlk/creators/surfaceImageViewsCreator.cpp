#include "surfaceImageViewsCreator.hpp"

#include <cassert>
#include <string>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"

namespace rn::vlk {

std::vector<vk::UniqueImageView> SurfaceImageViewsCreator::create(Context &context) {
	vk::Device device = context.device;
	std::vector<vk::Image> &surfaceImages = context.surface.images;

	assert(device);
	assert(surfaceImages.size() > 0);

	vk::Format imageFormat = fromConfig(ngn::config::core.window.surfaceFormat());
	std::vector<vk::UniqueImageView> surfaceImageViews{};
	surfaceImageViews.resize(surfaceImages.size());

	for (uint32_t i = 0, l = surfaceImages.size(); i < l; i++) {
		vk::ImageViewCreateInfo imageViewCreateInfo{
			/*.flags=*/ vk::ImageViewCreateFlags{},
			/*.image=*/ surfaceImages[i],
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

		surfaceImageViews[i] = RN_VLK_TRACE(device.createImageViewUnique(imageViewCreateInfo));
	}

	return surfaceImageViews;
}

} // rn::vlk
