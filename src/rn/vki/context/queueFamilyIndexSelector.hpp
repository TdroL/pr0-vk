#pragma once

#include <tuple>

#include "../../../ngn/config.hpp"
#include "types.hpp"

namespace rn::vki::context {

class QueueFamilyIndexSelector {
public:
	rn::vki::context::QueueFamilyIndex select(rn::vki::HandlePhysicalDevice &&physicalDevice, const rn::vki::context::PhysicalDeviceDescription &physicalDeviceDescription, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config);
};

} // rn::vki::context
