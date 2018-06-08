#pragma once

#include <future>

#include "handle.hpp"
#include "reader.hpp"
#include "transferrer.hpp"

namespace rn::vki {

class Context;

} // rn::vki

namespace rn::vki::db {

class Mesh;

} // rn::vki::db

namespace rn::vki::db::mesh {

class Loader {
public:
	rn::vki::db::Mesh &db;

	Reader reader;
	Transferrer transferrer;

	std::atomic<bool> shouldStop{false};
	bool forceSynchronusLoading = ngn::config::core.db.mesh.forceSynchronusLoading();

	std::thread readerThread{};
	std::mutex readerMutex{};
	std::condition_variable readerCond{};
	std::deque<Payload> readerPayloads{};

	std::thread transferrerThread{};
	std::mutex transferrerMutex{};
	std::condition_variable transferrerCond{};
	std::deque<Payload> transferrerPayloads{};

	explicit Loader(rn::vki::Context &context, rn::vki::db::Mesh &db);
	~Loader();

	Loader(const Loader &other) = delete;
	Loader & operator=(const Loader &other) = delete;

	std::future<bool> load(Handle &handle);
};

} // rn::vki::db::mesh
