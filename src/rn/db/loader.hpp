#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>

#include "../../ngn/fs.hpp"
#include "../../util/either.hpp"
// #include "../../util/format.hpp"
#include "../../util/flatStorage.hpp"
#include "../resources.hpp"
#include "loader/gltf.hpp"

namespace rn::db {

class LoadError {
public:
	std::string message;

	explicit LoadError(LoadError &&other) noexcept :
		message{std::move(other.message)}
	{}

	explicit LoadError(std::string &&message) noexcept :
		message{std::move(message)}
	{}
};

template<typename T>
class Loader {
public:
	rn::Resources<T> &resources;
	rn::db::loader::GlTF gltf{};

	Loader(rn::Resources<T> &resources) :
		resources{resources}
	{}

	util::EitherOption<LoadError, size_t> loadModel(std::string_view name) {
		std::optional<std::string> filePathOpt = ngn::fs::find(name);

		if ( ! filePathOpt) {
			return LoadError{fmt::format("Could not find file \"{}\"", name)};
		}

		auto &filePath = *filePathOpt;

		std::string_view fileExt = ngn::fs::ext(filePath);

		if (fileExt == ".gltf" || fileExt == ".glb") {
			gltf.loadModel(filePath, resources);
			return size_t{0};
		} else {
			return LoadError{fmt::format("Unsupported file format \"{}\"", fileExt)};
		}
	}

	// std::optional<Handle> find(std::string_view name) {
	// 	auto foundOpt = entries.find(name);

	// 	if (foundOpt) {
	// 		return { foundOpt->handle };
	// 	} else {
	// 		return std::nullopt;
	// 	}
	// }
};

} // rn::db