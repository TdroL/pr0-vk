#include "handle.hpp"

#include <cstdint>
#include <limits>

#include "../mesh.hpp"

namespace rn::vlk::db::mesh {

Handle::Handle(uint32_t index, rn::vlk::db::Mesh &db) noexcept :
	index{index},
	db{&db}
{}

Handle::Handle(Handle &&other) noexcept :
	index{std::move(other.index)},
	db{std::move(other.db)}
{
	other.index = std::numeric_limits<uint32_t>::max();
}

Handle & Handle::operator=(Handle &&other) noexcept {
	reset();

	index = std::move(other.index);
	db = std::move(other.db);

	other.index = std::numeric_limits<uint32_t>::max();

	return *this;
}

Handle::Handle(const Handle &other) noexcept :
	index{other.index},
	db{other.db}
{
	if (index != std::numeric_limits<uint32_t>::max()) {
		db->acquire(index);
	}
}

Handle & Handle::operator=(const Handle &other) noexcept {
	reset();

	index = other.index;
	db = other.db;

	if (index != std::numeric_limits<uint32_t>::max()) {
		db->acquire(index);
	}

	return *this;
}

void Handle::reset() {
	if (index != std::numeric_limits<uint32_t>::max()) {
		db->release(index);
		index = std::numeric_limits<uint32_t>::max();
	}
}

} // rn::vlk::db::mesh
