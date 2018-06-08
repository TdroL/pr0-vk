#pragma once

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vki {

class SurfaceExtentSelector {
public:
	vk::Extent2D select(Context &context);
};

} // rn::vki
