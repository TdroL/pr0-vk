#pragma once

// #include <string>
// #include <map>

#include "config/core.hpp"

namespace ngn {

namespace config {

extern Core core;

// bool getter(const char *path, std::string &out);
// bool getter(const char *path, bool &out);
// bool getter(const char *path, int32_t &out);
// bool getter(const char *path, uint32_t &out);
// bool getter(const char *path, int64_t &out);
// bool getter(const char *path, uint64_t &out);
// bool getter(const char *path, float &out);
// bool getter(const char *path, double &out);

// bool setter(const char *path, const std::string &in);
// bool setter(const char *path, const bool &in);
// bool setter(const char *path, const int32_t &in);
// bool setter(const char *path, const uint32_t &in);
// bool setter(const char *path, const int64_t &in);
// bool setter(const char *path, const uint64_t &in);
// bool setter(const char *path, const float &in);
// bool setter(const char *path, const double &in);

// template<typename T>
// struct Value {
// 	T data{};
// 	bool valid = false;

// 	Value() = default;
// 	explicit Value(T &&data) : data{std::move(data)}, valid{true} {}

// 	Value(Value &&rhs) : data{std::move(rhs.data)}, valid{rhs.valid} {
// 		rhs.data = T{};
// 		rhs.valid = false;
// 	}

// 	T valueOr(const T &defaultValue) {
// 		return valid ? data : defaultValue;
// 	}

// 	T value() {
// 		return valueOr(T{});
// 	}
// };

// template<typename T>
// Value<T> get(const char *path) {
// 	T value;
// 	if (getter(path, value)) {
// 		return Value<T>{std::move(value)};
// 	} else {
// 		return Value<T>{};
// 	}
// }

// template<typename T>
// bool set(const char *path, const T &value) {
// 	return setter(path, value);
// }

// bool flush();

} // config

} // ngn