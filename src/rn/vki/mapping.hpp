#pragma once

#include <vulkan/vulkan.hpp>

#include "../../ngn/config/core.hpp"
#include "../types.hpp"

namespace rn::vki {

vk::Format fromCommon(rn::PixelFormat pixelFormat);

vk::Format fromCommon(rn::DataFormat dataFormat);

vk::ImageType fromCommon(rn::ImageType imageType);

vk::ImageViewType fromCommon(rn::ImageViewType imageViewType);

vk::PresentModeKHR fromCommon(rn::VSync vsync);

vk::ColorSpaceKHR fromCommon(rn::ColorSpace colorSpace);

template<typename T, typename U>
T toCommon(U value);

template<>
rn::PixelFormat toCommon<rn::PixelFormat>(vk::Format pixelFormat);

template<>
rn::DataFormat toCommon<rn::DataFormat>(vk::Format dataFormat);

template<>
rn::ImageType toCommon<rn::ImageType>(vk::ImageType imageType);

template<>
rn::ImageViewType toCommon<rn::ImageViewType>(vk::ImageViewType imageViewType);

template<>
rn::VSync toCommon<rn::VSync>(vk::PresentModeKHR vsync);

template<>
rn::ColorSpace toCommon<rn::ColorSpace>(vk::ColorSpaceKHR colorSpace);

} // rn::vki
