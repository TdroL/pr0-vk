#include "handle.hpp"

#include <cstdint>
#include <limits>

#include "../mesh.hpp"

namespace rn::vki::db::mesh {

Handle::Handle() noexcept :
	index{std::numeric_limits<uint32_t>::max()},
	db{nullptr}
{}

Handle::Handle(uint32_t index, rn::vki::db::Mesh &db) noexcept :
	index{index},
	db{&db}
{}

Handle::Handle(Handle &&other) noexcept :
	index{std::move(other.index)},
	db{std::move(other.db)}
{
	other.index = std::numeric_limits<uint32_t>::max();
	other.db = nullptr;
}

Handle & Handle::operator=(Handle &&other) noexcept {
	reset();

	index = std::move(other.index);
	db = std::move(other.db);

	other.index = std::numeric_limits<uint32_t>::max();
	other.db = nullptr;

	return *this;
}

Handle::Handle(const Handle &other) noexcept :
	index{other.index},
	db{other.db}
{
	if (index != std::numeric_limits<uint32_t>::max() && db != nullptr) {
		db->acquire(index);
	}
}

Handle & Handle::operator=(const Handle &other) noexcept {
	reset();

	index = other.index;
	db = other.db;

	if (index != std::numeric_limits<uint32_t>::max() && db != nullptr) {
		db->acquire(index);
	}

	return *this;
}

void Handle::reset() {
	if (index != std::numeric_limits<uint32_t>::max() && db != nullptr) {
		db->release(index);
		index = std::numeric_limits<uint32_t>::max();
	}
}

} // rn::vki::db::mesh
