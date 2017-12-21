#pragma once

#include <vulkan/vulkan.hpp>

#include "../../ngn/config/core.hpp"
#include "../imageFormat.hpp"
#include "../imageType.hpp"
#include "../imageViewType.hpp"

namespace rn::vlk {

vk::Format fromCore(rn::ImageFormat imageFormat);
vk::ImageType fromCore(rn::ImageType imageType);
vk::ImageViewType fromCore(rn::ImageViewType imageViewType);

rn::ImageFormat toCore(vk::Format imageFormat);
rn::ImageType toCore(vk::ImageType imageType);
rn::ImageViewType toCore(vk::ImageViewType imageViewType);

vk::Format fromConfig(ngn::config::ImageFormat imageFormat);
vk::ColorSpaceKHR fromConfig(ngn::config::ColorSpace colorSpace);
vk::PresentModeKHR fromConfig(ngn::config::VSync vsync);

ngn::config::ImageFormat toConfig(vk::Format imageformat);
ngn::config::ColorSpace toConfig(vk::ColorSpaceKHR colorSpace);
ngn::config::VSync toConfig(vk::PresentModeKHR vsync);

} // rn::vlk
