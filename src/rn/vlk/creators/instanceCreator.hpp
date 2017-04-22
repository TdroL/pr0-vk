#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

// #include "../instanceOwner.hpp"

#include "../../glfw.hpp"
#include "../../../ngn/config/core.hpp"

namespace rn {

namespace vlk {

class Context;

class InstanceCreator {
public:
	struct Version {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};

	std::vector<std::string> layers{};
	std::vector<std::string> extensions{};

	std::vector<std::string> availableLayers{};
	bool availableLayersLoaded = false;
	std::vector<std::string> availableExtensions{};
	bool availableExtensionsLoaded = false;

	void initAvailableLayers() {
		if (availableLayersLoaded) {
			return;
		}

		// load available instance layers
		std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
		availableLayers.resize(layerProperties.size());
		std::transform(std::begin(layerProperties), std::end(layerProperties), std::begin(availableLayers), [] (const vk::LayerProperties &properties) {
			return std::string{properties.layerName};
		});

		availableLayersLoaded = true;
	}

	void initAvailableExtensions() {
		if (availableExtensionsLoaded) {
			return;
		}

		// load available instance extensions
		std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
		availableExtensions.resize(extensionProperties.size());
		std::transform(std::begin(extensionProperties), std::end(extensionProperties), std::begin(availableExtensions), [] (const vk::ExtensionProperties &properties) {
			return std::string{properties.extensionName};
		});

		availableExtensionsLoaded = true;
	}

	bool validateLayer(const std::string &layer) {
		initAvailableLayers();

		return std::find(std::begin(availableLayers), std::end(availableLayers), layer) != std::end(availableLayers);
	}

	bool validateExtension(const std::string &extension) {
		initAvailableExtensions();

		return std::find(std::begin(availableExtensions), std::end(availableExtensions), extension) != std::end(availableExtensions);
	}

	bool appendLayer(const std::string &layer) {
		if ( ! validateLayer(layer)) {
			return false;
		}

		if (std::find(std::begin(layers), std::end(layers), layer) == std::end(layers)) {
			layers.push_back(layer);
		}

		return true;
	}

	bool appendExtension(const std::string &extension) {
		if ( ! validateExtension(extension)) {
			return false;
		}

		if (std::find(std::begin(extensions), std::end(extensions), extension) == std::end(extensions)) {
			extensions.push_back(extension);
		}

		return true;
	}

	void requireExtensions(const std::vector<std::string> &extensions) {
		for (const std::string &extension : extensions) {
			if ( ! appendExtension(extension)) {
				throw std::runtime_error{"Required extension \"" + extension + "\" is not available"};
			}
		}
	}

	std::vector<std::string> findMissingExtensions(const std::vector<std::string> &extensions) {
		std::vector<std::string> notFound{};

		for (const std::string &extension : extensions) {
			if ( ! validateExtension(extension)) {
				notFound.push_back(extension);
			}
		}

		return notFound;
	}

	std::vector<std::string> tryIncludeLayers(const std::vector<std::string> &layers) {
		std::vector<std::string> notFound{};

		for (const std::string &layer : layers) {
			if ( ! creators.instance.appendLayer(layer)) {
				notFound.push_back(layer);
			}
		}

		return notFound;
	}

	vk::UniqueInstance create() {
		if (glfwVulkanSupported() != GLFW_TRUE) {
			throw std::runtime_error{"Vulkan not supported"};
		}

		auto &appConfig = ngn::config::core.application;
		auto &engineConfig = ngn::config::core.engine;

		vk::ApplicationInfo applicationInfo{};
		applicationInfo.pApplicationName   = appConfig.name().data();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(appConfig.version.major(), appConfig.version.minor(), appConfig.version.patch());
		applicationInfo.pEngineName        = engineConfig.name().data();
		applicationInfo.engineVersion      = VK_MAKE_VERSION(engineConfig.version.major(), engineConfig.version.minor(), engineConfig.version.patch());
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

		vk::UniqueInstance instanceOwner{vk::createInstanceUnique(instanceCreateInfo)};

		if ( ! instanceOwner) {
			throw std::runtime_error{"Vulkan instance could not be created"};
		}

		return instanceOwner;
	}
};

} // vlk

} // rn
