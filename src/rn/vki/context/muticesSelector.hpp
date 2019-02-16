#pragma once

#include "types.hpp"

namespace rn::vki::context {

class MuticesSelector {
public:
	rn::vki::context::Mutex select(rn::vki::context::Queue &queue);
};

} // rn::vki::context
