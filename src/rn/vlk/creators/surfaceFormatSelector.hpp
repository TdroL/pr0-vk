#pragma once

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class SurfaceFormatSelector {
public:
	vk::SurfaceFormatKHR select(Context &context);
};

} // rn::vlk
