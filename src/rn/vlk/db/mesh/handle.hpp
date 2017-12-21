#pragma once

namespace rn::vlk::db {

class Mesh;

} // rn::vlk::db

namespace rn::vlk::db::mesh {

struct Handle {
	uint32_t index;
	rn::vlk::db::Mesh *db;

	Handle() = delete;

	explicit Handle(uint32_t index, rn::vlk::db::Mesh &db) noexcept;

	Handle(Handle &&other) noexcept;

	Handle & operator=(Handle &&other) noexcept;

	Handle(const Handle &other) noexcept;

	Handle & operator=(const Handle &other) noexcept;

	void reset();

	~Handle() {
		reset();
	}
};

} // rn::vlk::db::mesh
