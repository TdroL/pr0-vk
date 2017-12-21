#pragma once

#include <future>

#include "handle.hpp"
#include "reader.hpp"
#include "transferrer.hpp"

namespace rn::vlk {

class Context;

} // rn::vlk

namespace rn::vlk::db {

class Texture;

} // rn::vlk::db

namespace rn::vlk::db::texture {

class Loader {
public:
	rn::vlk::db::Texture &db;

	Reader reader;
	Transferrer transferrer;

	explicit Loader(rn::vlk::Context &context, rn::vlk::db::Texture &db);

	Loader(const Loader &other) = delete;
	Loader & operator=(const Loader &other) = delete;

	std::future<bool> load(Handle &handle);
};

} // rn::vlk::db::texture
