#pragma once

// #include <string>
// #include <string_view>
#include <vector>

// #include "../../util/either.hpp"
#include "../context.hpp"
#include "compile.hpp"

namespace rn::graph {

template<class T>
bool resolve([[maybe_unused]] rn::Context<T> &context, [[maybe_unused]] std::vector<rn::graph::ResourcesUsage> &resourcesUsageList) {
	return false;
}

} // rn::graph
