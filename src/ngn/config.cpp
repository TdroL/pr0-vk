#include "config.hpp"

namespace ngn::config {

Config Config::factory() {
	return {
		/*.core=*/ Core::load("core.json")
	};
}

} // ngn::config
