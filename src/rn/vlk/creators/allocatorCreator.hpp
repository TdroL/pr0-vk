#pragma once

#include "../context.hpp"

namespace rn::vlk {

class AllocatorCreator {
public:
	Context::Allocator create(Context &context);
};

} // rn::vlk
