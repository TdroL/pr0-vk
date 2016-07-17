#pragma once

#include <cassert>
#include <algorithm>
#include <vector>

#include <vulkan/vulkan.h>

#include "context.hpp"
#include "instanceWrap.hpp"

namespace rn {

namespace vlk {

class InstanceInit {
public:
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
		"VK_LAYER_LUNARG_standard_validation"
	};

	std::vector<std::string> extensions{
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	std::vector<std::string> availableLayers{};
	std::vector<std::string> availableExtensions{};

	std::string applicationName = "pr0-vk";
	Version applicationVersion = {0, 0, 0};
	std::string engineName = "ngn";
	Version engineVersion = {0, 0, 0};

	bool enableDebug = true;

	void init() {
		VkResult err;

		// load available instance layers
		availableLayers.empty();
		std::vector<VkLayerProperties> layerProperties{};
		uint32_t layerPropertiesCount = 0;
		err = vkEnumerateInstanceLayerProperties(&layerPropertiesCount, nullptr);
		if (err == VK_SUCCESS) {
			layerProperties.resize(layerPropertiesCount);

			err = vkEnumerateInstanceLayerProperties(&layerPropertiesCount, layerProperties.data());
			if (err == VK_SUCCESS) {
				availableLayers.resize();
				std::transform(begin(layerProperties), end(layerProperties), begin(availableLayers), [] (const VkLayerProperties &properties) {
					return std::string{properties.layerName};
				});
			}
		}

		// load available instance extensions
		availableExtensions.empty();
		std::vector<VkLayerProperties> extensionProperties{};
		uint32_t extensionPropertiesCount = 0;
		err = vkEnumerateInstanceExtensionProperties(&extensionPropertiesCount, nullptr);
		if (err == VK_SUCCESS) {
			extensionProperties.resize(extensionPropertiesCount);

			err = vkEnumerateInstanceExtensionProperties(&extensionPropertiesCount, extensionProperties.data());
			if (err == VK_SUCCESS) {
				availableExtensions.resize();
				std::transform(begin(extensionProperties), end(extensionProperties), begin(availableExtensions), [] (const VkLayerProperties &properties) {
					return std::string{properties.layerName};
				});
			}
		}
	}

	bool addOptionalLayer(const std::string &layer) {
		if ( ! std::any_of(begin(availableLayers), end(availableLayers), layer)) {
			return false;
		}

		addRequiredLayer(layers);

		return true;
	}

	bool addOptionalExtension(const std::string &Extension) {
		if ( ! std::any_of(begin(availableExtensions), end(availableExtensions), Extension)) {
			return false;
		}

		addRequiredExtension(Extensions);

		return true;
	}

	void addRequiredLayer(const std::string &layer) {
		if ( ! std::any_of(begin(layers), end(layers), layer)) {
			layers.push_back(layer);
		}
	}

	void addRequiredExtension(const std::string &extension) {
		if ( ! std::any_of(begin(extensions), end(extensions), extension)) {
			extensions.push_back(extension);
		}
	}

	VkResult run(Context &context) {
		VkApplicationInfo applicationInfo {
			/*.sType =*/              VK_STRUCTURE_TYPE_APPLICATION_INFO,
			/*.pNext =*/              nullptr,
			/*.pApplicationName =*/   applicationName.c_str(),
			/*.applicationVersion =*/ VK_MAKE_VERSION(applicationVersion.major, applicationVersion.minor, applicationVersion.patch),
			/*.pEngineName =*/        engineName.c_str(),
			/*.engineVersion =*/      VK_MAKE_VERSION(engineVersion.major, engineVersion.minor, engineVersion.patch),
			/*.apiVersion =*/         VK_API_VERSION_1_0
		};

		std::vector<const char *> requestedLayers(layers, nullptr);
		std::transform(begin(layers), end(layers), begin(requestedLayers), [] (const std::string &layer) {
			return layer.data();
		});

		std::vector<const char *> requestedExtensions(extensions, nullptr);
		std::transform(begin(extensions), end(extensions), begin(requestedExtensions), [] (const std::string &extension) {
			return extension.data();
		});

		VkInstanceCreateInfo createInfo {
			/*.sType =*/                   VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			/*.pNext =*/                   nullptr,
			/*.flags =*/                   0,
			/*.pApplicationInfo =*/        &applicationInfo,
			/*.enabledLayerCount =*/       static_cast<uint32_t>(requestedLayers.size()),
			/*.ppEnabledLayerNames =*/     requestedLayers.data(),
			/*.enabledExtensionCount =*/   static_cast<uint32_t>(requestedExtensions.size()),
			/*.ppEnabledExtensionNames =*/ requestedExtensions.data()
		};

		VkInstance instanceHandler{};

		return VK_SUCCESS;
	}
};

} // vlk

} // rn
