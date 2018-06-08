#pragma once

#include <cstddef>
#include <optional>
#include <string_view>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "payload.hpp"

namespace rn::vki {

class Context;

} // rn::vki

namespace rn::vki::db {

class Texture;

} // rn::vki::db

namespace rn::vki::db::texture {

class Reader {
public:
	rn::vki::Context &context;
	rn::vki::db::Texture &db;

	explicit Reader(rn::vki::Context &context, rn::vki::db::Texture &db);

	std::optional<Payload> run(Payload &&payload);

	bool hasExt(const std::string_view &source, const std::vector<std::string_view> &exts) const;

	bool isStbiReadable(const std::string_view &source) const;
	bool isGliReadable(const std::string_view &source) const;
	std::optional<Payload> readWithStbi(Payload &&payload);
	std::optional<Payload> readWithGli(Payload &&payload);
};

} // rn::vki::db::texture
