#pragma once

namespace rn::vki::db {

class Texture;

} // rn::vki::db

namespace rn::vki::db::texture {

struct Handle {
	uint32_t index;
	rn::vki::db::Texture *db;

	Handle() noexcept;
	explicit Handle(uint32_t index, rn::vki::db::Texture &db) noexcept;

	Handle(Handle &&other) noexcept;
	Handle & operator=(Handle &&other) noexcept;

	Handle(const Handle &other) noexcept;
	Handle & operator=(const Handle &other) noexcept;

	void reset();

	~Handle() {
		reset();
	}
};

} // rn::vki::db::texture
