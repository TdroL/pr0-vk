#pragma once

#include <optional>
#include <utility>

namespace util {

template<class T, class D = void (*)(T)>
class UniqueResource {
public:
	using Type = T;
	using Deleter = D;

	std::optional<std::pair<Type, Deleter>> resource{std::nullopt};

	UniqueResource() = default;
	UniqueResource(Type &&handle, Deleter deleter)
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

	operator bool() const {
		return resource.has_value();
	}

	Type * operator->() {
		return &resource->first;
	}

	Type & get() {
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
