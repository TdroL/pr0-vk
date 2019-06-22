#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "../../util/either.hpp"
#include "../../util/flatStorage.hpp"
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

	// CompileError & operator=(CompileError &&other) {
	// 	message = std::move(other.message);

	// 	return *this;
	// }
};

using SetupResult = std::variant<rn::graph::GraphicSetupResult, rn::graph::ComputeSetupResult, rn::graph::TransferSetupResult>;

struct CompileData {
	std::vector<std::string_view> resolvedPassNames{};
	util::FlatStorage<std::string, rn::graph::SetupResult> setupResults{};
};

using CompileResult = util::EitherOption<rn::graph::CompileError, rn::graph::CompileData>;

rn::graph::CompileResult compile(rn::graph::Passes &&passes, std::string_view rootPass);

} // rn::graph
