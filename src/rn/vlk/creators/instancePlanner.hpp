#pragma once

#include <map>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>

#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

struct InstancePlannerResult {
	std::vector<std::string> extensions{};
	std::vector<std::string> layers{};

	bool all = false;
	bool surface = false;
	bool debugCallback = false;
};

struct InstancePlanner {
	enum Flow {
		SKIP = 0,
		IGNORE_MISSING,
		FAIL_STAGE,
		FAIL_ALL
	};

	struct Selector;
	struct Stage {
		Selector &parent;
		bool &flag;
		bool active;

		Stage(Selector &parent, bool &flag)
			: parent(parent), flag(flag), active(active) {
				flag = true;
		}

		Stage & includeExtensions(const std::vector<std::string> &extensions, std::function<Flow (const std::vector<std::string> &)> callback) {
			if ( ! active || ! parent.active) {
				return *this;
			}

			std::vector<std::string> missing{};

			for (const std::string &extension : extensions) {
				if (std::find(std::begin(parent.availableExtensions), std::end(parent.availableExtensions), extension) == std::end(parent.availableExtensions)) {
					missing.push_back(extension);
				}
			}

			if ( ! missing.empty()) {
				Flow flow = callback(std::move(missing));

				switch (flow) {
					case FAIL_ALL: {
						parent.flag = false;
						parent.active = false;
						[[fallthrough]]
					}
					case FAIL_STAGE: {
						flag = false;
						active = false;
						break;
					}
					case SKIP: {
						active = false;
					},
					case IGNORE_MISSING: {
						break;
					}
				}
			} else {
				for (const std::string &extension : extensions) {
					if (std::find(std::begin(parent.extensions), std::end(parent.extensions), extension) == std::end(parent.extensions)) {
						parent.extensions.push_back(extension);
					}
				}
			}
			
			return *this;
		}

		Stage & includeLayers(const std::vector<std::string> &layers, std::function<Flow (const std::vector<std::string> &)> callback) {
			return *this;
		}

		// void appendRequiredLayers(const std::vector<std::string> &layers) {
		// 	for (const std::string &layer : layers) {
		// 		if ( ! creators.instance.appendLayer(layer)) {
		// 			throw std::runtime_error{"Required layer \"" + layer + "\" is not available"};
		// 		}
		// 	}
		// }

		// void appendRequiredExtensions(const std::vector<std::string> &extensions) {
		// }

		// std::vector<std::string> appendOptionalLayers(const std::vector<std::string> &layers) {
		// 	std::vector<std::string> notFound{};

		// 	for (const std::string &layer : layers) {
		// 		if ( ! creators.instance.appendLayer(layer)) {
		// 			notFound.push_back(layer);
		// 		}
		// 	}

		// 	return notFound;
		// }
	};

	struct Selector {
		std::vector<std::string> availableExtensions{};
		std::vector<std::string> availableLayers{};

		bool active = false;

		Selector() {
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

		Stage stage(bool &flag) {
			return Stage{*this, flag};
		}
	};

	InstancePlannerResult selectExtensionsAndLayers(DebugCallbackCreator &debugCallback, SurfaceCreator &surface) {
		InstancePlannerResult result{};
		Selector selector{};

		// surface
		selector.stage(result.surface)
			.includeExtensions(surface.requiredExtensions, [] (const std::vector<std::string> &missingExts) {
				size_t size = missingExts.size();
				std::string exts = ngn::str::implode(missingExts);

				ngn::log::error("Vulkan debug reporting disabled, missing {} extension(s): {}", size, exts);
				throw std::runtime_error{"Missing " + exts + " vulkan extension(s)"};

				return Flow::FAIL_ALL;
			});

		// debugCallback
		selector.stage(result.debugCallback)
			.includeExtensions(debugCallback.optionalExtensions, [] (const std::vector<std::string> &missingExts) {
				size_t size = missingExts.size();
				std::string exts = ngn::str::implode(missingExts);

				ngn::log::warn("Vulkan debug reporting disabled, missing {} extension(s): {}", size, exts);
				return Flow::SKIP;
			})
			.includeLayers(debugCallback.optionalLayers, [] (const std::vector<std::string> &missingLayers) {
				size_t size = missingExts.size();
				std::string exts = ngn::str::implode(missingExts);

				ngn::log::warn("Vulkan debug reporting might be limited, missing {} layer(s): {}", size, exts);
				return Flow::FAIL_STAGE;
			});

		selector.getResult(result.all);
	}
};

} // vlk

} // rn