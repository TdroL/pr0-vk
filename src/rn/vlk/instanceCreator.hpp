#pragma once

#include <cassert>
#include <algorithm>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

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

	std::vector<std::string> layers{
		// "VK_LAYER_GOOGLE_threading"
		// "VK_LAYER_LUNARG_parameter_validation"
		// "VK_LAYER_LUNARG_device_limits"
		// "VK_LAYER_LUNARG_object_tracker"
		// "VK_LAYER_LUNARG_image"
		// "VK_LAYER_LUNARG_core_validation"
		// "VK_LAYER_LUNARG_swapchain"
		// "VK_LAYER_GOOGLE_unique_objects"
		// "VK_LAYER_LUNARG_standard_validation"
	};

	std::vector<std::string> extensions{
		// VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	std::vector<std::string> availableLayers{};
	std::vector<std::string> availableExtensions{};

	std::string applicationName = "Unnamed application";
	Version applicationVersion = {0, 0, 0};
	std::string engineName = "ngn";
	Version engineVersion = {0, 0, 0};

	InstanceCreator(Context &context)
		: context(context) {}

	void preloadData();

	bool addLayer(const std::string &layer);

	bool addExtension(const std::string &extension);

	VkResult init();
};

} // vlk

} // rn
