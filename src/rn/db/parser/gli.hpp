#pragma once

#include <cstddef>
#include <string_view>

#include "../../../util/either.hpp"
#include "types.hpp"

namespace rn::db::parser {

class GLI {
public:
	static bool isParsable(std::string_view mimeType, std::string_view fileName);

	util::EitherOption<std::string, ParsedTextureResult> parse(const std::byte *data, size_t dataLength);
};

} // rn::db::parser
