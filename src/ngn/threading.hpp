#pragma once

#include <cstddef>

#include "config.hpp"

namespace ngn::threading {

uint32_t concurrency(const ngn::config::Config &config);

} // ngn::threading