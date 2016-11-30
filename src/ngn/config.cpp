#include "config.hpp"
#include "fs.hpp"
#include "log.hpp"

#include <iostream>
#include <cstring>
#include <algorithm>

#include <json.hpp>

using json = nlohmann::json;

namespace ngn {

namespace config {

std::string configPath = "config/core.json";
std::string configBakPath = "config/core.json.bak";

json originalRoot{};
json root{};
std::string body{};

void load() {
	if (body.empty()) {
		body = ngn::fs::contents(configPath);

		if (body.empty()) {
			body = "{}";
		}

		root = json::parse(body);
		originalRoot = root;
	}
}

template<typename T>
bool getterT(const char *path, T &out) {
	load();

	try {
		out = root.at(json::json_pointer{path});
		return true;
	} catch (const std::out_of_range &e) {
		return false;
	} catch (const std::domain_error &e) {
		return false;
	} catch (const std::invalid_argument &e) {
		return false;
	}
}

template<typename T>
bool setterT(const char *path, T &in) {
	load();

	try {
		root = root.patch(json::array({json{
			{ "op", "replace" },
			{ "path", path },
			{ "value", in }
		}}));
		return true;
	} catch (const std::out_of_range &e) {
		return false;
	} catch (const std::domain_error &e) {
		return false;
	} catch (const std::invalid_argument &e) {
		return false;
	}
}

bool getter(const char *path, std::string &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, bool &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, int32_t &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, uint32_t &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, int64_t &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, uint64_t &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, float &out) { return getterT<decltype(out)>(path, out); }
bool getter(const char *path, double &out) { return getterT<decltype(out)>(path, out); }

bool setter(const char *path, const std::string &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const bool &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const int32_t &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const uint32_t &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const int64_t &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const uint64_t &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const float &in) { return setterT<decltype(in)>(path, in); }
bool setter(const char *path, const double &in) { return setterT<decltype(in)>(path, in); }

bool flush() {
	std::string newBody = root.dump(4);

	if ( ! ngn::fs::contents(configBakPath, body, false)) {
		ngn::log::warn("Failed to save \"{}\"", configBakPath);
		return false;
	}

	if ( ! ngn::fs::contents(configPath, newBody, false)) {
		ngn::log::warn("Failed to save \"{}\"", configPath);
		ngn::log::info("Original config:\n{}", originalRoot.dump(4));
		ngn::log::info("Current config:\n{}", root.dump(4));
		return false;
	}

	originalRoot = root;
	body = std::move(newBody);

	return true;
}

} // config

} // ngn