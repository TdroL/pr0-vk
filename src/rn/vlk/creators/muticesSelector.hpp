#pragma once

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class MuticesSelector {
public:
	Context::Mutex select(Context &context);
};

} // rn::vlk
