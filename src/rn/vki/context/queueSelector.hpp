#pragma once

#include <tuple>

#include "../dispatch.hpp"
#include "types.hpp"

namespace rn::vki::context {

class QueueSelector {
public:
	rn::vki::context::Queue select(rn::vki::HandleDevice &&device, const rn::vki::context::QueueFamilyIndex &queueFamilyIndex);
};

} // rn::vki::context
