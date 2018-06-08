#pragma once

#include <tuple>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vki {

class QueuesSelector {
public:
	std::tuple<Context::Queue, Context::Family> select(Context &context);
};

} // rn::vki
