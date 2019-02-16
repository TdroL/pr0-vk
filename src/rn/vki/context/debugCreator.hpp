#pragma once

#include <string>
#include <vector>

// #include <vulkan/vulkan.hpp>

#include "types.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn::vki {

// class UniqueTableInstance;

} // rn::vki

namespace rn::vki::context {

class DebugCreator {
public:
	bool enabled = true;
	bool isAvailable = false;

	int initialLogLevel = 0;

	rn::vki::context::Debug create(rn::vki::HandleInstance &&instance, ngn::config::Config &config);

	static std::vector<std::string> selectLayers(ngn::config::Config &config, std::vector<std::string> &availableLayers, std::vector<std::string> selectedLayers);

	static std::vector<std::string> selectExtensions(ngn::config::Config &config, std::vector<std::string> &availableExtensions, std::vector<std::string> selectedExtensions);
};

} // rn::vki::context
