#pragma once

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vki {

class MuticesSelector {
public:
	Context::Mutex select(Context &context);
};

} // rn::vki
