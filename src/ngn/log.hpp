#pragma once

#include <spdlog/spdlog.h>

namespace ngn::log
{

spdlog::logger & logger();

void level(uint32_t newLevel);
uint32_t level();

template<typename... Args>
void trace(const char *format, const Args &...args) { // level = 0
	logger().trace(format, std::forward<const Args>(args)...);
}

template<typename... Args>
void debug(const char *format, const Args &...args) { // level = 1
	logger().debug(format, std::forward<const Args>(args)...);
}

template<typename... Args>
void info(const char *format, const Args &...args) { // level = 2
	logger().info(format, std::forward<const Args>(args)...);
}

template<typename... Args>
void warn(const char *format, const Args &...args) { // level = 3
	logger().warn(format, std::forward<const Args>(args)...);
}

template<typename... Args>
void error(const char *format, const Args &...args) { // level = 4
	logger().error(format, std::forward<const Args>(args)...);
}

template<typename... Args>
void critical(const char *format, const Args &...args) { // level = 5
	logger().critical(format, std::forward<const Args>(args)...);
}

} // ngn::log
