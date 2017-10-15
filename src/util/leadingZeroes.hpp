#pragma once

#include <cstdint>

#if defined(_MSC_VER)
	#include <intrin.h>
#endif

namespace util
{

#if defined(__GNUC__) || defined(__clang__)

constexpr uint32_t leadingZeroes(uint32_t x) {
	return __builtin_clz(x);
}

#elif defined(_MSC_VER)

const uint32_t leadingZeroes(uint32_t x) {
	unsigned long zeroes = 0;

	if (_BitScanReverse(&zeroes, x)) {
		return 31 - zeroes;
	} else {
		return 32;
	}
}

#else

namespace details {
	constexpr uint32_t popcnt(uint32_t x) {
		x -= ((x >> 1) & 0x55555555);
		x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
		x = (((x >> 4) + x) & 0x0f0f0f0f);
		x += (x >> 8);
		x += (x >> 16);

		return x & 0x0000003f;
	}
} // details

constexpr uint32_t leadingZeroes(uint32_t x) {
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);

	return 32 - details::popcnt(x);
}

#endif

} // util
