#pragma once

#include "../dispatch.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn::vki::context {

class InstanceCreator {
public:
	rn::vki::UniqueTableInstance create(ngn::config::Config &config);
};

} // rn::vki
