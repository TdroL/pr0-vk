#include "threading.hpp"

#include <thread>

namespace ngn::threading {

uint32_t concurrency(const ngn::config::Config &config) {
	if (config.core.threading.forcePoolSize > 0) {
		return static_cast<uint32_t>(config.core.threading.forcePoolSize);
	} else {
		return std::max(1u, static_cast<uint32_t>(std::thread::hardware_concurrency()));
	}
}

} // ngn::threading