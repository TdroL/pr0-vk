#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace util {

template<typename U, typename T, typename CB>
std::vector<U> map(const std::vector<T> &in, CB cb) {
	std::vector<U> out{};
	out.resize(in.size());

	std::transform(std::begin(in), std::end(in), std::begin(out), cb);

	return out;
}

template<typename U, typename T, typename CB>
std::vector<U> mapIndexed(const std::vector<T> &in, CB cb) {
	std::vector<U> out{};
	out.resize(in.size());

	size_t i = 0;
	std::transform(std::begin(in), std::end(in), std::begin(out), [&cb, &i] (auto &value) {
		return cb(value, i++);
	});

	return out;
}

} // util
