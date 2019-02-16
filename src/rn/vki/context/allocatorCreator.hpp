#pragma once

#include "../dispatch.hpp"
#include "types.hpp"

namespace rn::vki::context {

class AllocatorCreator {
public:
	rn::vki::context::Allocator create(rn::vki::HandleDevice &&device, rn::vki::context::PhysicalDeviceDescription &physicalDeviceDescription);
};

} // rn::vki::context
