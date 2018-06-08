#pragma once

namespace rn::vki::db {

class Mesh;

} // rn::vki::db

namespace rn::vki::db::mesh {

struct Handle {
	uint32_t index;
	rn::vki::db::Mesh *db;

	Handle() noexcept;
	explicit Handle(uint32_t index, rn::vki::db::Mesh &db) noexcept;

	Handle(Handle &&other) noexcept;
	Handle & operator=(Handle &&other) noexcept;

	Handle(const Handle &other) noexcept;
	Handle & operator=(const Handle &other) noexcept;

	void reset();

	~Handle() {
		reset();
	}
};

} // rn::vki::db::mesh
