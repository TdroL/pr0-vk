#include "log.hpp"

#include <ctime>
#include <iomanip>
#include <memory>
#include <mutex>

#include <fmt/format.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace ngn::log {

spdlog::level::level_enum currentLevel{spdlog::level::debug};

spdlog::logger & logger() {
	static std::once_flag createConsoleFlag;
	static std::shared_ptr<spdlog::logger> console{nullptr};

	std::call_once(createConsoleFlag, [&] () {
		console = spdlog::stdout_logger_mt("console");
		console->set_level(spdlog::level::debug);
		console->set_pattern("[%H:%M:%S.%e T#%t] <%l> %v");

		spdlog::set_error_handler([&] (const std::string& message) {
			std::time_t now = std::time(nullptr);
			char dataBuffer[100] = { 0 };
			std::strftime(dataBuffer, sizeof(dataBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

			fmt::print(stderr, "[*** LOG ERROR ***] [{}] [{}] {}\n", dataBuffer, console->name(), message);
		});
	});

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
			[[fallthrough]];
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
		case spdlog::level::off:
			[[fallthrough]];
		default: {
			return 6;
		}
	}
}

} // ngn::log
