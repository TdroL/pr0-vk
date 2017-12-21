#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class SurfaceImagesSelector {
public:
	std::vector<vk::Image> select(Context &context);
};

} // rn::vlk
