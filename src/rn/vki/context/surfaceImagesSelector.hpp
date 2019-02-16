#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../dispatch.hpp"

namespace rn::vki::context {

class SurfaceImagesSelector {
public:
	std::vector<vk::Image> select(rn::vki::HandleSwapchainKHR &&swapchain, rn::vki::HandleDevice &&device);
};

} // rn::vki::context
