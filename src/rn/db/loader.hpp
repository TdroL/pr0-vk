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

	util::EitherOption<LoadError, rn::db::Model<T>> loadModel(std::string_view name) {
		std::optional<std::string> filePathOpt = ngn::fs::find(name);

		if ( ! filePathOpt) {
			return LoadError{fmt::format("Could not find file \"{}\"", name)};
		}

		auto &filePath = *filePathOpt;

		std::string fileExt = ngn::fs::ext(filePath);

		if (fileExt == ".gltf" || fileExt == ".glb") {
			auto results = gltf.loadModel(filePath, resources);
			if (results) {
				return std::move(*results);
			} else {
				return LoadError{fmt::format("Failed to parse file \"{}\"", name)};
			}
		} else {
			return LoadError{fmt::format("Unsupported file format \"{}\" (\"{}\")", name, fileExt)};
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