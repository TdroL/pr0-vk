#pragma once

#include <algorithm>
#include <cstddef>

namespace conv {

template<class T>
std::byte * copy(std::byte *target, T *source, size_t n = 1) {
	return std::copy(reinterpret_cast<std::byte *>(source), reinterpret_cast<std::byte *>(source) + sizeof(T) * n, target);
}

} // conv
