#pragma once

#include "../../../ngn/config.hpp"
#include "../dispatch.hpp"
#include "types.hpp"

namespace rn::vki::context {

class CommandPoolGroupsCreator {
public:
	rn::vki::context::CommandPoolGroups create(const rn::vki::context::SurfaceDescription &surfaceDescription, const ngn::config::Config &config);
};

} // rn::vki::context