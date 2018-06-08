#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../memory/handle.hpp"
#include "payload.hpp"

namespace rn::vki {

class Context;

} // rn::vki

namespace rn::vki::db {

class Mesh;

} // rn::vki::db

namespace rn::vki::db::mesh {

class Transferrer {
public:
	vk::UniqueCommandPool commandPool{};

	rn::vki::Context &context;
	rn::vki::db::Mesh &db;

	explicit Transferrer(rn::vki::Context &context, rn::vki::db::Mesh &db);

	void run(std::vector<Payload> &&payloads);
};

} // rn::vki::db::mesh
