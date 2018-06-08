#pragma once

#include <future>

#include "handle.hpp"
#include "reader.hpp"
#include "transferrer.hpp"

namespace rn::vki {

class Context;

} // rn::vki

namespace rn::vki::db {

class Texture;

} // rn::vki::db

namespace rn::vki::db::texture {

class Loader {
public:
	rn::vki::db::Texture &db;

	Reader reader;
	Transferrer transferrer;

	std::atomic<bool> shouldStop{false};
	bool forceSynchronusLoading = ngn::config::core.db.texture.forceSynchronusLoading();

	std::thread readerThread{};
	std::mutex readerMutex{};
	std::condition_variable readerCond{};
	std::deque<Payload> readerPayloads{};

	std::thread transferrerThread{};
	std::mutex transferrerMutex{};
	std::condition_variable transferrerCond{};
	std::deque<Payload> transferrerPayloads{};

	explicit Loader(rn::vki::Context &context, rn::vki::db::Texture &db);
	~Loader();

	Loader(const Loader &other) = delete;
	Loader & operator=(const Loader &other) = delete;

	std::future<bool> load(Handle &handle);
};

} // rn::vki::db::texture
