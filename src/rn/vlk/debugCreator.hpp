#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class Context;

class DebugCreator {
public:
	Context &context;

	bool enabled = true;
	bool isAvailable = false;
	std::vector<std::string> requiredExtensions {
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	std::vector<std::string> optionalLayers {
		"VK_LAYER_LUNARG_standard_validation"
	};

	DebugCreator(Context &context) : context(context) {}

	void init() {
		//
	}

	void deinit() {
		//
	}
};

} // vlk

} // rn
