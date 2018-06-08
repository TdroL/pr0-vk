#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vki {

class SurfaceImagesSelector {
public:
	std::vector<vk::Image> select(Context &context);
};

} // rn::vki
