#pragma once

#include <string>
#include <vector>

#include "../dispatch.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn {

class Window;

} // rn

namespace rn::vki::context {

class SurfaceCreator {
public:
	rn::vki::UniqueSurfaceKHR create(rn::vki::HandleInstance &&instance, rn::Window &window);

	static std::vector<std::string> selectLayers(ngn::config::Config &config, std::vector<std::string> &availableLayers, std::vector<std::string> selectedLayers);

	static std::vector<std::string> selectExtensions(ngn::config::Config &config, std::vector<std::string> &availableExtensions, std::vector<std::string> selectedExtensions);
};

} // rn::vki::context
