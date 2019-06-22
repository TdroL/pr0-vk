#pragma once

#include <vulkan/vulkan.hpp>

#include "../dispatch.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn::vki::context {

class SurfaceFormatSelector {
public:
	vk::SurfaceFormatKHR select(rn::vki::HandlePhysicalDevice &&physicalDevice, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config);
};

} // rn::vki::context
