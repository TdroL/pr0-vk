#pragma once

#include <string>
#include <vector>

namespace util {

inline std::string implode(const std::vector<std::string> &records) {
	if (records.size() == 0) {
		return "";
	}

	if (records.size() == 1) {
		return records[0];
	}

	std::string delim = ", ";
	size_t reserveSize = delim.size() * (records.size() - 1);

	for (const auto &item : records) {
		reserveSize += item.size();
	}

	std::string result{};
	result.reserve(reserveSize);

	for (size_t i = 0, l = records.size() - 1; i < l; i++) {
		result += records[i];
		result += delim;
	}

	result += records[records.size() - 1];

	return result;
}

inline std::string implode(const std::vector<std::string_view> &records) {
	if (records.size() == 0) {
		return "";
	}

	if (records.size() == 1) {
		return std::string{records[0]};
	}

	std::string delim = ", ";
	size_t reserveSize = delim.size() * (records.size() - 1);

	for (const auto &item : records) {
		reserveSize += item.size();
	}

	std::string result{};
	result.reserve(reserveSize);

	for (size_t i = 0, l = records.size() - 1; i < l; i++) {
		result += records[i];
		result += delim;
	}

	result += records[records.size() - 1];

	return result;
}

} // util
