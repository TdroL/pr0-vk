#pragma once

#include <cstdint>

#include "../resources.hpp"
#include "../types.hpp"

namespace rn::db {

template<class T>
class Texture {
public:
	rn::Resources<T> *resources = nullptr;
	rn::TextureHandle handle{rn::end<rn::TextureHandle>()};
	rn::FenceStamp fenceStamp{rn::end<rn::FenceStamp>()};

	Texture() = default;

	Texture(rn::Resources<T> &resources, rn::TextureHandle handle, rn::FenceStamp fenceStamp = rn::end<rn::FenceStamp>()) :
		resources{&resources},
		handle{handle},
		fenceStamp{fenceStamp}
	{}

	Texture(Texture &&other) :
		resources{other.resources},
		handle{other.handle},
		fenceStamp{other.fenceStamp}
	{
		other.reset();
	}

	Texture & operator=(Texture &&other) {
		reset();

		resources = other.resources;
		handle = other.handle;
		fenceStamp = other.fenceStamp;

		other.handle = rn::end<rn::TextureHandle>();
		other.fenceStamp = rn::end<rn::FenceStamp>();

		return *this;
	}

	Texture(const Texture &other) = delete;
	Texture & operator=(const Texture &other) = delete;

	void reset() {
		if (resources != nullptr && handle != rn::end<rn::TextureHandle>()) {
			resources->texture.retire(handle);
		}

		handle = rn::end<rn::TextureHandle>();
		fenceStamp = rn::end<rn::FenceStamp>();
	}

	~Texture() {
		reset();
	}
};

} // rn:db
