#pragma once

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <string_view>
#include <optional>

#include "../../util/nonNull.hpp"
#include "../context.hpp"
#include "../types.hpp"
#include "dispatch.hpp"
#include "memory/handle.hpp"

namespace rn::vki {

class Context;

class Resources {
public:
	struct ImageSlot {
		rn::vki::UniqueImage image{};
		rn::vki::memory::Handle memory{};
	};

	struct BufferSlot {
		rn::vki::UniqueBuffer buffer{};
		rn::vki::memory::Handle memory{};
	};

	util::NonNull<rn::vki::Context> context;
	std::vector<ImageSlot> imageSlots{};
	std::vector<BufferSlot> bufferSlots{};

	explicit Resources(rn::vki::Context &implementation);

	Resources(const Resources &other) = delete;
	Resources(Resources &&other);

	Resources & operator=(const Resources &other) = delete;
	Resources & operator=(Resources &&other);

	rn::TextureHandle createTexture(const rn::TextureDescription &description);
	bool destroyTexture(rn::TextureHandle handle);

	rn::BufferHandle createBuffer(const rn::BufferDescription &description);
	bool destroyBuffer(rn::BufferHandle handle);

	static Resources create(rn::Context<rn::vki::Context> &context);
};

} // rn::vki
