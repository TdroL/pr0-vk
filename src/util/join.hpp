#pragma once

#include <string>
#include <vector>
#include <numeric>

namespace util {

inline std::string join(const std::vector<std::string> &records, std::string_view separator = ", ") {
	if (records.size() == 0) {
		return "";
	}

	if (records.size() == 1) {
		return records[0];
	}

	size_t reserveSize = std::accumulate(std::begin(records), std::end(records), separator.size() * (records.size() - 1), [] (size_t acc, const auto &item) {
		return acc + item.size();
	});

	std::string result{};
	result.reserve(reserveSize);

	for (size_t i = 0, l = records.size() - 1; i < l; i++) {
		result += records[i];
		result += separator;
	}

	result += records[records.size() - 1];

	return result;
}

inline std::string join(const std::vector<std::string_view> &records, std::string_view separator = ", ") {
	if (records.size() == 0) {
		return "";
	}

	if (records.size() == 1) {
		return std::string{records[0]};
	}

	size_t reserveSize = std::accumulate(std::begin(records), std::end(records), separator.size() * (records.size() - 1), [] (size_t acc, const auto &item) {
		return acc + item.size();
	});

	std::string result{};
	result.reserve(reserveSize);

	for (size_t i = 0, l = records.size() - 1; i < l; i++) {
		result += records[i];
		result += separator;
	}

	result += records[records.size() - 1];

	return result;
}

} // util
