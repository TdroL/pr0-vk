#pragma once

#include <cstddef>
#include <memory>

#include "../../types.hpp"

namespace rn::db::parser {

class TextureDataContainer {
public:
	virtual std::byte * data(uint32_t layer, uint32_t level) = 0;
	virtual size_t size(uint32_t layer, uint32_t level) = 0;
	virtual rn::DataFormat format() = 0;

	virtual ~TextureDataContainer() {}
};

struct ParsedTextureResult {
	std::unique_ptr<TextureDataContainer> data;
	rn::TextureDescription description;
};

} // rn::db::parser