#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.hpp>

#include "debugCallbackWrapper.hpp"

namespace rn {

namespace vlk {

class Context;

class DebugCallbackCreator {
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

	DebugCallbackCreator(Context &context) : context(context) {}

	DebugCallbackWrapper create() {
		return DebugCallbackWrapper{};
	}
};

} // vlk

} // rn
