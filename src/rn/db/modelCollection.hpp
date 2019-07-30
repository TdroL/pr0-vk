#pragma once

#include <string>
#include <vector>

#include "../../util/flatStorage.hpp"
#include "../resources.hpp"
#include "../types.hpp"
#include "model.hpp"

namespace rn::db {

template<class T>
class ModelCollection {
public:
	rn::Resources<T> &resources;

	using Handle = util::FlatStorage<std::string, rn::db::Model<T>>::Handle;
	util::FlatStorage<std::string, rn::db::Model<T>> storage{};

	Handle create(std::string name, std::string filePath) {
		auto foundO = storage.find(name);

		if (foundO) {
			foundO->value.reset();
			foundO->value.filePath = std::move(filePath);

			return foundO->handle;
		} else {
			return storage.assign(std::move(name), { std::move(filePath) });
		}
	}

	std::optional<std::reference_wrapper<std::pair<std::string, rn::db::Model<T>>>> access(Handle handle) {
		return storage.access(handle);
	}
};

} // rn::db