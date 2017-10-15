#pragma once

#define SYMBOL(name) (::util::symbol(#name))

#include <cstdint>
#include <cstdlib>

namespace util {

template<int N>
constexpr uint32_t symbol(const char (&name)[N]) {
	// FNV-1a
	uint32_t hash = 2166136261u;

	for (size_t i = 0; i < N; ++i) {
		hash = (hash ^ name[i]) * 16777619u;
	}

	return hash;
}

} // util
