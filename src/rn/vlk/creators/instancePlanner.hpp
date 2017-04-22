#pragma once

#include <map>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>

#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

struct InstanceExtensionsAndLayers {
	std::vector<std::string> extensions{};
	std::vector<std::string> layers{};
}

struct InstanceExtensionsAndLayersSelector {
	std::vector<std::string> availableExtensions{};
	std::vector<std::string> availableLayers{};

	std::vector<std::string> enabledExtensions{};
	std::vector<std::string> enabledLayers{};

	std::vector<std::string> missing{};

	InstanceExtensionsAndLayersSelector() {
		initAvailableLayers();
		initAvailableExtensions();
	}

	InstanceExtensionsAndLayersSelector(InstanceExtensionsAndLayersSelector &rhs) {
		//
	}

	void initAvailableLayers() {
		// load available instance layers
		std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
		availableLayers.resize(layerProperties.size());
		std::transform(std::begin(layerProperties), std::end(layerProperties), std::begin(availableLayers), [] (const vk::LayerProperties &properties) {
			return std::string{properties.layerName};
		});
	}

	void initAvailableExtensions() {
		// load available instance extensions
		std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
		availableExtensions.resize(extensionProperties.size());
		std::transform(std::begin(extensionProperties), std::end(extensionProperties), std::begin(availableExtensions), [] (const vk::ExtensionProperties &properties) {
			return std::string{properties.extensionName};
		});
	}

	// includeExtensions(const std::vector<std::string> &extensions)
	// includeLayers(const std::vector<std::string> &layers)
	// fallback(std::function<bool (const std::vector<std::string> &)> callback)
	// tap(std::function<bool ()> callback)
	// getExtensionsAndLayers()
};

struct InstancePlanner {
	struct Stages {
		bool all = false;
		bool surface = false;
		bool debugCallback = false;
	};

	enum Flow {
		SKIP = 0,
		RESET_STAGE,
		FAIL_STAGE,
		FAIL_ALL
	};

	InstanceExtensionsAndLayers selectExtensionsAndLayers(DebugCallbackCreator &debugCallback, SurfaceCreator &surface) {
		Stages stages{};

		return InstanceExtensionsAndLayersSelector{stages.all}
			// surface
			.stage(stages.surface)
				.includeExtensions(surface.requiredExtensions)
				.onMissing([] (const std::vector<std::string> &missingExts) {
					size_t size = missingExts.size();
					std::string exts = ngn::str::implode(missingExts);

					ngn::log::error("Vulkan debug reporting disabled, missing {} extension(s): {}", size, exts);
					throw std::runtime_error{"Missing " + exts + " vulkan extension(s)"};

					return Flow::FAIL_ALL;
				})
			// debugCallback
			.stage(stages.debugCallback)
				.includeExtensions(debugCallback.optionalExtensions)
				.onMissing([] (const std::vector<std::string> &missingExts) {
					size_t size = missingExts.size();
					std::string exts = ngn::str::implode(missingExts);

					ngn::log::warn("Vulkan debug reporting disabled, missing {} extension(s): {}", size, exts);
					return Flow::SKIP;
				})
				.includeLayers(debugCallback.optionalLayers)
				.onMissing([] (const std::vector<std::string> &missingLayers) {
					size_t size = missingExts.size();
					std::string exts = ngn::str::implode(missingExts);

					ngn::log::warn("Vulkan debug reporting might be limited, missing {} layer(s): {}", size, exts);
					return Flow::RESET_STAGE;
				})
			.getExtensionsAndLayers();
	}
};

} // vlk

} // rn