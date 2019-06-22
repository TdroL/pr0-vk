#pragma once

#include "types.hpp"
#include "../dispatch.hpp"

namespace rn::vki::context {

class SynchronizationCreator {
public:
	rn::vki::context::Synchronization create(const rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device);
};

} // rn::vki::context
