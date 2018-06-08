#pragma once

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vki {

class SurfaceFormatSelector {
public:
	vk::SurfaceFormatKHR select(Context &context);
};

} // rn::vki
