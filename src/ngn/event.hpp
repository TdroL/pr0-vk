#pragma once

#include <functional>
#include <vector>
#include <algorithm>

#define NGN_EVENT(EventName) struct EventName : public ngn::Event<EventName>

namespace ngn {

template<class T>
class Event
{
public:
	static size_t & listenerCount() {
		static size_t count = 0;

		return count;
	}

	static std::vector<std::function<void(const T &)>> & listeners() {
		static std::vector<std::function<void(const T &)>> container{};

		return container;
	}

	static size_t subscribe(std::function<void(const T &)> &&callback) {
		auto &container = Event<T>::listeners();
		auto &count = Event<T>::listenerCount();

		if (container.size() == count) {
			container.push_back(std::move(callback));
			count++;

			return container.size();
		}
		else {
			for (size_t i = 0; i < container.size(); i++) {
				if ( ! container[i]) {
					container[i] = std::move(callback);
					count++;

					return i + 1;
				}
			}

			// this should not happen...
			container.push_back(std::move(callback));
			count++;

			return container.size();
		}
	}

	static void unsubscribe(size_t callbackId) {
		if ( ! callbackId) {
			return;
		}

		auto &container = Event<T>::listeners();
		auto &count = Event<T>::listenerCount();

		if (container.size() < callbackId) {
			return;
		}

		container[callbackId - 1] = nullptr;
		count--;
	}

	virtual ~Event() {}
};

template<class T>
class Listener
{
public:
	std::vector<size_t> indices{};

	Listener() = default;
	explicit Listener(std::function<void(const T &)> &&callback) {
		attach(std::move(callback));
	}

	size_t attach(std::function<void(const T &)> &&callback) {
		indices.push_back(Event<T>::subscribe(std::move(callback)));
		return indices.back();
	}

	void detach(size_t callbackId) {
		if (std::find(indices.begin(), indices.end(), callbackId) == indices.end()) {
			return;
		}

		Event<T>::unsubscribe(callbackId);

		indices.erase(std::remove(indices.begin(), indices.end(), callbackId), indices.end());
	}

	void reset() {
		for (const auto idx : indices) {
			Event<T>::unsubscribe(idx);
		}

		indices.clear();
	}

	~Listener() {
		reset();
	}
};

namespace event {

template<typename T>
static std::vector<std::function<void(const T &)>> & listeners()
{
	return ngn::Event<T>::listeners();
}

template<typename T>
static size_t & listenerCount()
{
	return ngn::Event<T>::listenerCount();
}

template<typename T>
static void emit(const T &event)
{
	for (const auto &listener : ngn::Event<T>::listeners()) {
		if (listener) {
			listener(event);
		}
	}
}

} // event

} // ngn