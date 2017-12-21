#pragma once

#include <functional>
#include <future>
#include <utility>
#include <mutex>

namespace rn::vlk::db::mesh {

template<typename... Args>
class Event {
public:
	std::function<void (Args... args)> listener{};
	std::mutex listenerMutex{};

	void listen(std::function<void (Args... args)> &&callback) {
		std::lock_guard lock{listenerMutex};

		listener = std::move(callback);
	}

	void emit(Args... args) {
		std::lock_guard lock{listenerMutex};

		if (listener) {
			listener(std::forward<Args>(args)...);
		}
	}
};

} // rn::vlk::db::mesh
