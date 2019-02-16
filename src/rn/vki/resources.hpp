#pragma once

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <string_view>
#include <optional>

#include "../context.hpp"
#include "../types.hpp"
#include "dispatch.hpp"
#include "memory/handle.hpp"

namespace rn::vki {

class Context;

class Resources {
public:
	rn::vki::Context &context;

	struct ImageSlot {
		rn::vki::UniqueImage image;
		rn::vki::memory::Handle memory;
	};

	struct BufferSlot {
		rn::vki::UniqueBuffer buffer;
		rn::vki::memory::Handle memory;
	};

	std::vector<ImageSlot> imageSlots{};
	std::vector<BufferSlot> bufferSlots{};

	explicit Resources(rn::vki::Context &context);

	Resources(const Resources &other) = delete;
	Resources(Resources &&other);

	rn::TextureHandle createTexture(const rn::TextureDescription &description);
	bool destroyTexture(rn::TextureHandle handle);

	rn::BufferHandle createBuffer(const rn::BufferDescription &description);
	bool destroyBuffer(rn::BufferHandle handle);

	static Resources create(rn::Context<rn::vki::Context> &context);
};

} // rn::vki