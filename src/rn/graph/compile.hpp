#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "../../util/either.hpp"
#include "passes.hpp"

namespace rn::graph {

class CompileError {
public:
	std::string message;

	explicit CompileError(CompileError &&other) noexcept :
		message{std::move(other.message)}
	{}

	explicit CompileError(std::string &&message) noexcept :
		message{std::move(message)}
	{}

	CompileError & operator=(CompileError &&other) {
		message = std::move(other.message);

		return *this;
	}
};

struct ResourcesUsage {
	std::string passName;
	rn::graph::Resources resources;
};

struct CompileData {
	std::vector<std::string_view> resolvedPassNames{};
	std::vector<rn::graph::ResourcesUsage> resourcesUsageList{};
};

using CompileResult = util::EitherOption<rn::graph::CompileError, rn::graph::CompileData>;

rn::graph::CompileResult compile(rn::graph::Passes &&passes, std::string_view rootPass);

} // rn::graph
