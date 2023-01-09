#pragma once

#include <optional>
#include <utility>

namespace util {

template<class T, class Deleter = void (*)(T)>
class UniqueResource {
public:
	std::optional<std::pair<T, Deleter>> resource{std::nullopt};

	UniqueResource() = default;
	UniqueResource(T &&handle, Deleter deleter)
		: resource{{handle, deleter}}
	{}

	UniqueResource(UniqueResource &&other)
		: resource{std::move(other.resource)}
	{
		other.resource = std::nullopt;
	}

	UniqueResource(const UniqueResource &other) = delete;

	UniqueResource & operator=(UniqueResource &&other) {
		release();

		resource = std::move(other.resource);

		other.resource = std::nullopt;

		return *this;
	}

	UniqueResource & operator=(const UniqueResource &other) = delete;

	T & get() {
		return resource->first;
	}

	void release() {
		if (resource) {
			resource->second(resource->first);

			resource = std::nullopt;
		}
	}

	virtual ~UniqueResource() {
		release();
	}
};

}
