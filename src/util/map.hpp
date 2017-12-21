#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <deque>

namespace util {

template<typename T, typename CB>
auto map(const std::vector<T> &src, CB cb) -> std::vector<decltype(cb(src[0]))> {
	std::vector<decltype(cb(src[0]))> dst{};
	dst.resize(src.size());

	std::transform(std::begin(src), std::end(src), std::begin(dst), cb);

	return dst;
}

template<typename T, typename CB>
auto mapIndexed(const std::vector<T> &src, CB cb) -> std::vector<decltype(cb(src[0], size_t{0}))> {
	std::vector<decltype(cb(src[0], size_t{0}))> dst{};
	dst.resize(src.size());

	size_t i = size_t{0};
	std::transform(std::begin(src), std::end(src), std::begin(dst), [&cb, &i] (auto &value) {
		return cb(value, i++);
	});

	return dst;
}

} // util
