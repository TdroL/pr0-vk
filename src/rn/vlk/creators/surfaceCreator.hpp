#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"
#include "../../window.hpp"

namespace rn::vlk {

class SurfaceCreator {
public:
	std::vector<std::string> requiredExtensions{};

	SurfaceCreator();

	vk::UniqueSurfaceKHR create(Context &context, rn::Window &window);
};

} // rn::vlk
