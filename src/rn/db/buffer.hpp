#pragma once

#include <cstdint>

#include "../resources.hpp"
#include "../types.hpp"

namespace rn::db {

template<class T>
class Buffer {
public:
	rn::Resources<T> *resources = nullptr;
	rn::BufferHandle handle{rn::end<rn::BufferHandle>()};
	rn::FenceStamp fenceStamp{rn::end<rn::FenceStamp>()};

	Buffer() = default;

	Buffer(rn::Resources<T> &resources, rn::BufferHandle handle, rn::FenceStamp fenceStamp = rn::end<rn::FenceStamp>()) :
		resources{&resources},
		handle{handle},
		fenceStamp{fenceStamp}
	{}

	Buffer(Buffer &&other) :
		resources{other.resources},
		handle{other.handle},
		fenceStamp{other.fenceStamp}
	{
		other.reset();
	}

	Buffer & operator=(Buffer &&other) {
		reset();

		resources = other.resources;
		handle = other.handle;
		fenceStamp = other.fenceStamp;

		other.handle = rn::end<rn::BufferHandle>();
		other.fenceStamp = rn::end<rn::FenceStamp>();

		return *this;
	}

	Buffer(const Buffer &other) = delete;
	Buffer & operator=(const Buffer &other) = delete;

	void reset() {
		if (resources != nullptr && handle != rn::end<rn::BufferHandle>()) {
			resources->buffer.retire(handle);
		}

		handle = rn::end<rn::BufferHandle>();
		fenceStamp = rn::end<rn::FenceStamp>();
	}

	~Buffer() {
		reset();
	}
};

} // rn:db
