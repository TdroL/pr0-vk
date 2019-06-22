#include "threading.hpp"

#include <thread>

namespace ngn::threading {

uint32_t concurrency(const ngn::config::Config &config) {
	return std::max(1u, config.core.threading.forcePoolSize > 0 ? static_cast<uint32_t>(config.core.threading.forcePoolSize) : static_cast<uint32_t>(std::thread::hardware_concurrency()));
}

} // ngn::threading