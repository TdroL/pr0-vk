#pragma once

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

struct ReaderDetails;

class Reader {
public:
	rn::vki::Context &context;
	rn::vki::db::Mesh &db;

	std::unique_ptr<ReaderDetails> readerDetails;

	explicit Reader(rn::vki::Context &context, rn::vki::db::Mesh &db);
	~Reader();

	std::optional<Payload> run(Payload &&payload);

	bool hasExt(const std::string_view &source, const std::vector<std::string_view> &exts) const;

	bool isMsbReadable(const std::string_view &source) const;
	std::optional<Payload> readWithMsb(Payload &&payload);
};

} // rn::vki::db::mesh
