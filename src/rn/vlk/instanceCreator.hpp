#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "instanceOwner.hpp"

namespace rn {

namespace vlk {

class Context;

class InstanceCreator {
public:
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

	void loadAvailableLayersAndExtensions() {
		// load available instance layers
		std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
		availableLayers.resize(layerProperties.size());
		std::transform(std::begin(layerProperties), std::end(layerProperties), std::begin(availableLayers), [] (const vk::LayerProperties &properties) {
			return std::string{properties.layerName};
		});

		// load available instance extensions
		std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
		availableExtensions.resize(extensionProperties.size());
		std::transform(std::begin(extensionProperties), std::end(extensionProperties), std::begin(availableExtensions), [] (const vk::ExtensionProperties &properties) {
			return std::string{properties.extensionName};
		});
	}

	bool addLayer(const std::string &layer) {
		if (std::find(std::begin(availableLayers), std::end(availableLayers), layer) == std::end(availableLayers)) {
			return false;
		}

		if (std::find(std::begin(layers), std::end(layers), layer) == std::end(layers)) {
			layers.push_back(layer);
		}

		return true;
	}

	bool addExtension(const std::string &extension) {
		if (std::find(std::begin(availableExtensions), std::end(availableExtensions), extension) == std::end(availableExtensions)) {
			return false;
		}

		if (std::find(std::begin(extensions), std::end(extensions), extension) == std::end(extensions)) {
			extensions.push_back(extension);
		}

		return true;
	}

	InstanceOwner create() {
		vk::ApplicationInfo applicationInfo{};
		applicationInfo.pApplicationName   = applicationName.data();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(applicationVersion.major, applicationVersion.minor, applicationVersion.patch);
		applicationInfo.pEngineName        = engineName.data();
		applicationInfo.engineVersion      = VK_MAKE_VERSION(engineVersion.major, engineVersion.minor, engineVersion.patch);
		applicationInfo.apiVersion         = VK_API_VERSION_1_0;

		auto getRawData = [] (const std::string &value) {
			return value.data();
		};

		std::vector<const char *> requestedLayers(layers.size());
		std::transform(std::begin(layers), std::end(layers), std::begin(requestedLayers), getRawData);

		std::vector<const char *> requestedExtensions(extensions.size());
		std::transform(std::begin(extensions), std::end(extensions), std::begin(requestedExtensions), getRawData);

		vk::InstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.pApplicationInfo        = &applicationInfo;
		instanceCreateInfo.enabledLayerCount       = static_cast<uint32_t>(requestedLayers.size());
		instanceCreateInfo.ppEnabledLayerNames     = requestedLayers.data();
		instanceCreateInfo.enabledExtensionCount   = static_cast<uint32_t>(requestedExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requestedExtensions.data();

		return InstanceOwner{vk::createInstance(instanceCreateInfo)};
	}
};

} // vlk

} // rn
