#pragma once

#include <map>
#include <string_view>

namespace util {

class Opts {
public:
	std::map<std::string_view, std::string_view> values;

	explicit Opts(int argc, char *argv[]) {
		for (int i = 1; i < argc; i++) {
			std::string_view key = argv[i];
			std::string_view value = argv[i];

			auto it = key.find('=');

			if (it != std::string::npos) {
				key = key.substr(0, it);
				value = value.substr(it + 1);
			} else if (i + 1 < argc && argv[i + 1][0] != '-') {
				value = argv[i + 1];
				i++;
			} else {
				value = "";
			}

			values.insert_or_assign(key, value);
		}
	}

	bool has(const std::string_view &key) const {
		return values.find(key) != std::end(values);
	}

	std::string_view get(const std::string_view &key) const {
		static std::string_view empty = "";

		const auto it = values.find(key);

		if (it == std::end(values)) {
			return empty;
		} else {
			return it->second;
		}
	}
};

} // util
