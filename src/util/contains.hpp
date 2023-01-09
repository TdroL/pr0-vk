#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace util {

template<typename T>
inline bool contains(const std::vector<T> &records, const T &item) {
	return std::find(std::begin(records), std::end(records), item) != std::end(records);
}

inline bool contains(const std::vector<std::string> &records, const std::string_view &item) {
	return std::find(std::begin(records), std::end(records), item) != std::end(records);
}

template<typename T, typename CB>
inline bool containsIf(const std::vector<T> &records, CB callback) {
	return std::find_if(std::begin(records), std::end(records), callback) != std::end(records);
}

} // util
