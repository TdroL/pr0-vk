#pragma once

#include <algorithm>
#include <deque>
#include <functional>
#include <vector>

namespace util {

template<typename T, typename CB>
auto map(const T &src, CB cb) -> std::vector<decltype(cb(*std::begin(src)))> {
	std::vector<decltype(cb(*std::begin(src)))> dst{};
	dst.resize(std::size(src));

	std::transform(std::begin(src), std::end(src), std::begin(dst), cb);

	return dst;
}

template<typename T, typename CB>
auto mapIndexed(const T &src, CB cb) -> std::vector<decltype(cb(*std::begin(src), size_t{0}))> {
	std::vector<decltype(cb(*std::begin(src), size_t{0}))> dst{};
	dst.resize(std::size(src));

	size_t i = 0;
	std::transform(std::begin(src), std::end(src), std::begin(dst), [&cb, &i] (auto &value) {
		return cb(value, i++);
	});

	return dst;
}

} // util
