#pragma once

#include "../context.hpp"

namespace rn::vki {

class AllocatorCreator {
public:
	Context::Allocator create(Context &context);
};

} // rn::vki
