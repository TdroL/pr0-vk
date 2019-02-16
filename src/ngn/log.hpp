#pragma once

#include <spdlog/spdlog.h>

namespace ngn::log {

spdlog::logger & logger();

void level(uint32_t newLevel);
uint32_t level();

template<typename S, typename... Args>
void trace(const S &formatString, const Args &...args) { // level = 0
	logger().trace(formatString, std::forward<const Args>(args)...);
}

template<typename S, typename... Args>
void debug(const S &formatString, const Args &...args) { // level = 1
	logger().debug(formatString, std::forward<const Args>(args)...);
}

template<typename S, typename... Args>
void info(const S &formatString, const Args &...args) { // level = 2
	logger().info(formatString, std::forward<const Args>(args)...);
}

template<typename S, typename... Args>
void warn(const S &formatString, const Args &...args) { // level = 3
	logger().warn(formatString, std::forward<const Args>(args)...);
}

template<typename S, typename... Args>
void error(const S &formatString, const Args &...args) { // level = 4
	logger().error(formatString, std::forward<const Args>(args)...);
}

template<typename S, typename... Args>
void critical(const S &formatString, const Args &...args) { // level = 5
	logger().critical(formatString, std::forward<const Args>(args)...);
}

} // ngn::log
