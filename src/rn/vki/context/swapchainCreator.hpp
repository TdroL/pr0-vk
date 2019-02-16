#pragma once

#include "../dispatch.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn::vki::context {

struct Family;
struct SurfaceDescription;

class SwapchainCreator {
public:
	rn::vki::UniqueSwapchainKHR create(rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::context::Family &family, rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandleDevice &&device, rn::vki::HandlePhysicalDevice &&physicalDevice, ngn::config::Config &config);
};

} // rn::vki::context
