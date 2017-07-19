#include "log.hpp"

#include <mutex>

namespace ngn
{

namespace log
{

spdlog::level::level_enum currentLevel{spdlog::level::debug};

std::mutex consoleMutex;
std::shared_ptr<spdlog::logger> console{nullptr};

spdlog::logger & logger() {
	if ( ! console) {
		std::lock_guard<std::mutex> guard{consoleMutex};

		if ( ! console) {
			console = spdlog::stdout_logger_mt("console");
			console->set_level(spdlog::level::debug);
			console->set_pattern("[%H:%M:%S.%e T#%t] %v");
		}
	}

	return *console;
}

void level(uint32_t newLevel) {
	switch (newLevel) {
		case 0: {
			currentLevel = spdlog::level::trace;
			break;
		}
		case 1: {
			currentLevel = spdlog::level::debug;
			break;
		}
		case 2: {
			currentLevel = spdlog::level::info;
			break;
		}
		case 3: {
			currentLevel = spdlog::level::warn;
			break;
		}
		case 4: {
			currentLevel = spdlog::level::err;
			break;
		}
		case 5: {
			currentLevel = spdlog::level::critical;
			break;
		}
		case 6:
		default: {
			currentLevel = spdlog::level::off;
			break;
		}
	}

	logger().set_level(currentLevel);
}

uint32_t level() {
	switch (currentLevel) {
		case spdlog::level::trace: {
			return 0;
		}
		case spdlog::level::debug: {
			return 1;
		}
		case spdlog::level::info: {
			return 2;
		}
		case spdlog::level::warn: {
			return 3;
		}
		case spdlog::level::err: {
			return 4;
		}
		case spdlog::level::critical: {
			return 5;
		}
		default:
		case spdlog::level::off: {
			return 6;
		}
	}
}

} // log

} // ngn
