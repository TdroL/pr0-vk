#pragma once

#include <cstddef>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../memory/handle.hpp"
#include "payload.hpp"

namespace rn::vki {

class Context;

} // rn::vki

namespace rn::vki::db {

class Texture;

} // rn::vki::db

namespace rn::vki::db::texture {

class Transferrer {
public:
	enum class TransitionType {
		UNDEFINED_TO_TRANSFERABLE,
		TRANSFERABLE_TO_READABLE,
	};

	struct ImageData {
		vk::Image image{};
		uint32_t levels;
		uint32_t layers;
	};

	vk::UniqueCommandPool commandPool{};

	rn::vki::Context &context;
	rn::vki::db::Texture &db;

	explicit Transferrer(rn::vki::Context &context, rn::vki::db::Texture &db);

	void run(std::vector<Payload> &&payloads);

	void transitionImages(TransitionType type, vk::CommandBuffer commandBuffer, const std::vector<ImageData> &imageDatas);
};

} // rn::vki::db::texture
