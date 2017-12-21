#pragma once

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class SurfaceExtentSelector {
public:
	vk::Extent2D select(Context &context);
};

} // rn::vlk
