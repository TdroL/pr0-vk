#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class SurfaceImageViewsCreator {
public:
	std::vector<vk::UniqueImageView> create(Context &context);
};

} // rn::vlk
