#pragma once

#include <string>

#include <fmt/format.h>

namespace util {

template<typename S, typename... Args>
std::string format(const S &formatString, const Args &...args) {
	return fmt::format(formatString, std::forward<const Args>(args)...);
}

} // util