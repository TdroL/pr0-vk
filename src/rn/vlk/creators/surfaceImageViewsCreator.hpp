#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"

namespace rn {

namespace vlk {

class SurfaceImageViewsCreator {
public:
	std::vector<vk::UniqueImageView> create(vk::UniqueDevice &deviceOwner, std::vector<vk::Image> &surfaceImages) {
		vk::Device device = deviceOwner.get();

		assert(device);

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

} // vlk

} // rn
