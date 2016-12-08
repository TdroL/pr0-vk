#pragma once

#include <string>

namespace ngn
{

namespace str
{

std::string implode(const std::vector<std::string> &records) {
	if (records.size() == 1) {
		return records[0];
	}

	std::string delim = ", ";
	size_t reserveSize = 0;

	for (const auto &item : records) {
		reserveSize += item.size();
		reserveSize += delim.size();
	}

	std::string result{};
	result.reserve(reserveSize - delim.size());

	for (size_t i = 0, l = records.size() - 1; i < l; i++) {
		result += records[i];
		result += delim;
	}

	result += records[records.size() - 1];

	return result;
}

} // str

} // ngn