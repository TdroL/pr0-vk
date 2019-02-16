#pragma once

#include "config/core.hpp"

namespace ngn::config {

struct Config {
	Core core{};

	static Config factory();
};


} // ngn::config
