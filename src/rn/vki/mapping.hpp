#pragma once

#include <vulkan/vulkan.hpp>

#include "../../ngn/config/core.hpp"
#include "../types.hpp"

namespace rn::vki {

template<typename T, typename U>
T toCore(U value);

vk::Format fromCore(rn::PixelFormat pixelFormat);

vk::Format fromCore(rn::VertexFormat vertexFormat);

vk::ImageType fromCore(rn::ImageType imageType);

vk::ImageViewType fromCore(rn::ImageViewType imageViewType);

vk::PresentModeKHR fromCore(rn::VSync vsync);

vk::ColorSpaceKHR fromCore(rn::ColorSpace colorSpace);

template<>
rn::PixelFormat toCore<rn::PixelFormat>(vk::Format pixelFormat);

template<>
rn::VertexFormat toCore<rn::VertexFormat>(vk::Format vertexFormat);

template<>
rn::ImageType toCore<rn::ImageType>(vk::ImageType imageType);

template<>
rn::ImageViewType toCore<rn::ImageViewType>(vk::ImageViewType imageViewType);

template<>
rn::ColorSpace toCore<rn::ColorSpace>(vk::ColorSpaceKHR colorSpace);

} // rn::vki
