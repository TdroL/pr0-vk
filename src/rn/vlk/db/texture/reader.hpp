#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <future>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "event.hpp"
#include "payload.hpp"

namespace rn::vlk {

class Context;

} // rn::vlk

namespace rn::vlk::db {

class Texture;

} // rn::vlk::db

namespace rn::vlk::db::texture {

class Reader {
public:
	std::thread workerThread{};
	std::mutex tasksMutex{};
	std::condition_variable tasksCond{};
	std::atomic<bool> shouldStop{false};
	std::deque<Payload> tasks{};
	Event<Payload> event{};

	bool forceSynchronusLoading = ngn::config::core.db.texture.forceSynchronusLoading();

	rn::vlk::Context &context;
	rn::vlk::db::Texture &db;

	explicit Reader(rn::vlk::Context &context, rn::vlk::db::Texture &db);

	~Reader();

	void run(Payload &&payload);

	void read(Payload &&payload);

	bool hasExt(const std::string_view &source, const std::vector<std::string_view> &exts) const;

	bool isStbiReadable(const std::string_view &source) const;
	bool isGliReadable(const std::string_view &source) const;
	void readWithStbi(Payload &&payload);
	void readWithGli(Payload &&payload);
};

} // rn::vlk::db::texture
