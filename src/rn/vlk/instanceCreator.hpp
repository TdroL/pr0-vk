#pragma once

#include <vector>
#include <string>

#include "instanceWrapper.hpp"

namespace rn {

namespace vlk {

class Context;

class InstanceCreator {
public:
	Context &context;

	struct Version {
		uint32_t major = 0;
		uint32_t minor = 0;
		uint32_t patch = 0;
	};

	std::vector<std::string> layers{};
	std::vector<std::string> extensions{};

	std::vector<std::string> availableLayers{};
	std::vector<std::string> availableExtensions{};

	std::string applicationName = "Unnamed application";
	Version applicationVersion = {0, 0, 0};
	std::string engineName = "ngn";
	Version engineVersion = {0, 0, 0};

	InstanceCreator(Context &context) : context(context) {}

	void loadAvailableLayersAndExtensions();

	bool addLayer(const std::string &layer);

	bool addExtension(const std::string &extension);

	InstanceWrapper create();
};

} // vlk

} // rn
