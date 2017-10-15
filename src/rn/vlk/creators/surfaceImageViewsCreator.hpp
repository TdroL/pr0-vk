#pragma once

#include <cassert>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"
#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"

namespace rn::vlk {

class SurfaceImageViewsCreator {
public:
	std::vector<vk::UniqueImageView> create(Context &context) {
		vk::Device device = context.device;
		std::vector<vk::Image> &surfaceImages = context.surface.images;

		assert(device);
		assert(surfaceImages.size() > 0);

		vk::Format imageFormat = fromConfig(ngn::config::core.window.surfaceFormat());
		std::vector<vk::UniqueImageView> surfaceImageViews{};
		surfaceImageViews.resize(surfaceImages.size());

		for (uint32_t i = 0, l = surfaceImages.size(); i < l; i++) {
			vk::ImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.image = surfaceImages[i];
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = imageFormat;
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			surfaceImageViews[i] = device.createImageViewUnique(imageViewCreateInfo);
		}

		return surfaceImageViews;
	}
};

} // rn::vlk
