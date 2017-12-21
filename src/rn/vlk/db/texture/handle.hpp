#pragma once

namespace rn::vlk::db {

class Texture;

} // rn::vlk::db

namespace rn::vlk::db::texture {

struct Handle {
	uint32_t index;
	rn::vlk::db::Texture *db;

	Handle() = delete;

	explicit Handle(uint32_t index, rn::vlk::db::Texture &db) noexcept;

	Handle(Handle &&other) noexcept;

	Handle & operator=(Handle &&other) noexcept;

	Handle(const Handle &other) noexcept;

	Handle & operator=(const Handle &other) noexcept;

	void reset();

	~Handle() {
		reset();
	}
};

} // rn::vlk::db::texture
