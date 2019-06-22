#pragma once

#include "../dispatch.hpp"
#include "types.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn::vki::context {

struct Family;
struct SurfaceDescription;

class SwapchainCreator {
public:
	rn::vki::UniqueSwapchainKHR create(rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device, rn::vki::HandlePhysicalDevice &&physicalDevice, rn::vki::context::QueueFamilyIndex &queueFamilyIndex, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config);
};

} // rn::vki::context
