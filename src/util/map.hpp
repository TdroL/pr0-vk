#pragma once

#include <algorithm>
#include <vector>

namespace util {

template<typename T, typename CB>
requires requires (CB cb, T src) {
	{ cb(*std::begin(src)) };
}
auto map(const T &src, CB cb) -> std::vector<decltype(cb(*std::begin(src)))> {
	std::vector<decltype(cb(*std::begin(src)))> dst{};
	dst.reserve(std::size(src));

	auto it = std::begin(src);
	auto end = std::end(src);
	while (it != end) {
		dst.push_back(cb(*it));
		it++;
	}

	return dst;
}

template<typename T, typename CB>
requires requires (CB cb, T src, size_t index) {
	{ cb(*std::begin(src), index) };
}
auto mapIndexed(const T &src, CB cb) -> std::vector<decltype(cb(*std::begin(src), size_t{0}))> {
	std::vector<decltype(cb(*std::begin(src), size_t{0}))> dst{};
	dst.reserve(std::size(src));

	size_t i = 0;
	auto it = std::begin(src);
	auto end = std::end(src);
	while (it != end) {
		dst.push_back(cb(*it, i));
		i++;
		it++;
	}

	return dst;
}

template<typename CB>
requires requires (CB cb, size_t index) {
	{ cb(index) };
}
auto mapRange(size_t range, CB cb) -> std::vector<decltype(cb(size_t{0}))> {
	std::vector<decltype(cb(size_t{0}))> dst{};
	dst.reserve(range);

	size_t i = 0;
	while (i < range) {
		dst.push_back(cb(i));
		i++;
	}

	return dst;
}

} // util
