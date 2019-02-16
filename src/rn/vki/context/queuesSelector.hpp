#pragma once

#include <tuple>

#include "../dispatch.hpp"
#include "types.hpp"

namespace rn::vki::context {

class QueuesSelector {
public:
	std::tuple<rn::vki::context::Queue, rn::vki::context::Family> select(rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandleDevice &&device, rn::vki::HandlePhysicalDevice &&physicalDevice);
};

} // rn::vki::context
