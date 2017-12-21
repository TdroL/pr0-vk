#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class SwapchainCreator {
public:
	vk::UniqueSwapchainKHR create(Context &context);

	uint32_t chooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);

	vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &presentModes);
};

} // rn::vlk
