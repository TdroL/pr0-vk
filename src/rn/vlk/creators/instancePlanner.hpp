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

	struct Missing {
		std::vector<std::string> list{};
		std::string type{};
	};

	Missing missing{};

	std::vector<std::string> missingList{};
	std::string missingType{};

	bool all = false;
	bool surface = false;
	bool debugCallback = false;
};

struct InstancePlanner {
	enum Flow {
		OK = 0,
		SKIP,
		IGNORE_MISSING,
		FAIL_STAGE,
		FAIL_ALL
	};

	struct Stage {
		std::vector<std::string> requestedExtensions{};
		std::vector<std::string> requestedLayers{};
		Flow flow;

		InstancePlannerResult::Missing missing{};

		std::vector<std::string> &availableExtensions;
		std::vector<std::string> &availableLayers;
		bool &flag;

		Stage(Flow flow, std::vector<std::string> &availableExtensions, std::vector<std::string> &availableLayers, bool &flag)
			: flow{flow}, availableExtensions(availableExtensions), availableLayers(availableLayers), flag(flag) {
				flag = true;
		}

		Stage & includeExtensions(const std::vector<std::string> &extensions, std::function<Flow (const std::vector<std::string> &)> callback) {
			if (flow != Flow::OK) {
				return *this;
			}

			std::vector<std::string> missingList{};
			std::vector<std::string> found{};

			for (const std::string &extension : extensions) {
				if (std::find(std::begin(availableExtensions), std::end(availableExtensions), extension) == std::end(availableExtensions)) {
					missingList.push_back(extension);
				} else {
					found.push_back(extension);
				}
			}

			if ( ! missingList.empty()) {
				flow = callback(missingList);

				switch (flow) {
					case Flow::FAIL_ALL:
					case Flow::FAIL_STAGE: {
						missing.list = missingList;
						missing.type = "extension";
					}
					[[fallthrough]]
					case Flow::SKIP: {
						flag = false;
						found = {};
						break;
					}
					case Flow::IGNORE_MISSING: {
						flow = Flow::OK;
						break;
					}
					default: {
						/* do nothing */
					}
				}
			}

			for (const std::string &extension : found) {
				if (std::find(std::begin(requestedExtensions), std::end(requestedExtensions), extension) == std::end(requestedExtensions)) {
					requestedExtensions.push_back(extension);
				}
			}

			return *this;
		}

		Stage & includeLayers(const std::vector<std::string> &layers, std::function<Flow (const std::vector<std::string> &)> callback) {
			if (flow != Flow::OK) {
				return *this;
			}

			std::vector<std::string> missingList{};
			std::vector<std::string> found{};

			for (const std::string &layer : layers) {
				if (std::find(std::begin(availableLayers), std::end(availableLayers), layer) == std::end(availableLayers)) {
					missingList.push_back(layer);
				} else {
					found.push_back(layer);
				}
			}

			if ( ! missingList.empty()) {
				flow = callback(missingList);

				switch (flow) {
					case Flow::FAIL_ALL:
					case Flow::FAIL_STAGE: {
						missing.list = missingList;
						missing.type = "layer";
					}
					[[fallthrough]]
					case Flow::SKIP: {
						flag = false;
						found = {};
						break;
					}
					case Flow::IGNORE_MISSING: {
						flow = Flow::OK;
						break;
					}
					default: {
						/* do nothing */
					}
				}
			}

			for (const std::string &layer : found) {
				if (std::find(std::begin(requestedLayers), std::end(requestedLayers), layer) == std::end(requestedLayers)) {
					requestedLayers.push_back(layer);
				}
			}

			return *this;
		}
	};

	struct Selector {
		std::vector<std::string> availableExtensions{};
		std::vector<std::string> availableLayers{};

		struct Result {
			std::vector<std::string> extensions{};
			std::vector<std::string> layers{};
		};

		Result result{};

		Flow flow{Flow::OK};
		bool all = true;

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
			return Stage{flow, availableExtensions, availableLayers, flag};
		}

		void apply(const Stage &stage) {
			switch (stage.flow) {
				[[fallthrough]]
				case Flow::FAIL_ALL: {
					flow = Flow::FAIL_ALL;
				}
				case Flow::FAIL_STAGE: {
					all = false;
					break;
				}
				case Flow::OK: {
					for (const std::string &extension : stage.requestedExtensions) {
						if (std::find(std::begin(result.extensions), std::end(result.extensions), extension) == std::end(result.extensions)) {
							result.extensions.push_back(extension);
						}
					}

					for (const std::string &layer : stage.requestedLayers) {
						if (std::find(std::begin(result.layers), std::end(result.layers), layer) == std::end(result.layers)) {
							result.layers.push_back(layer);
						}
					}

					break;
				}
				default: {
					// IGNORE_MISSING, SKIP
				}
			}
		}

		Result getResult(bool &flag) {
			flag = all;
			return result;
		}
	};

	InstancePlannerResult selectExtensionsAndLayers(const DebugCallbackCreator &debugCallback, const SurfaceCreator &surface) {
		InstancePlannerResult instancePlannerResult{};
		Selector selector{};

		// surface
		selector.apply(selector.stage(instancePlannerResult.surface)
			.includeExtensions(surface.requiredExtensions, [] (const std::vector<std::string> &missingExts) {
				size_t size = missingExts.size();

				ngn::log::error("Vulkan surface not available, missing {} extension(s): {}", size, ngn::str::implode(missingExts));

				return Flow::FAIL_ALL;
			})
		);

		// debugCallback
		selector.apply(selector.stage(instancePlannerResult.debugCallback)
			.includeExtensions(debugCallback.optionalExtensions, [] (const std::vector<std::string> &missingExts) {
				size_t size = missingExts.size();

				ngn::log::warn("Vulkan debug reporting disabled, missing {} extension(s): {}", size, ngn::str::implode(missingExts));
				return Flow::SKIP;
			})
			.includeLayers(debugCallback.optionalLayers, [] (const std::vector<std::string> &missingLayers) {
				size_t size = missingLayers.size();

				ngn::log::warn("Vulkan debug reporting might be limited, missing {} layer(s): {}", size, ngn::str::implode(missingLayers));
				return Flow::IGNORE_MISSING;
			})
		);

		const auto selected = selector.getResult(instancePlannerResult.all);
		instancePlannerResult.extensions = selected.extensions;
		instancePlannerResult.layers = selected.layers;

		return instancePlannerResult;
	}
};

} // vlk

} // rn