#pragma once

#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../../../ngn/str.hpp"

#include "../../window.hpp"

#include "../context.hpp"

#include "instanceCreator.hpp"
#include "surfaceCreator.hpp"
#include "debugCallbackCreator.hpp"
// #include "physicalDeviceCreator.hpp"
#include "deviceCreator.hpp"
// #include "queuesCreator.hpp"

#include "instancePlanner.hpp"

#include "physicalDeviceSelector.hpp"
#include "queuesSelector.hpp"

namespace rn {

namespace vlk {

class ContextCreator {
public:
	rn::Window &window;

	struct Creators {
		InstanceCreator instance{};
		DebugCallbackCreator debugCallback{};
		SurfaceCreator surface{};
		DeviceCreator device{};
	} creators{};

	struct Selectors {
		PhysicalDeviceSelector physicalDevice{};
		QueuesSelector queues{};
	} selectors;

	ContextCreator(rn::Window &window)
		: window(window) {}

	Context create() {
		Context context{};

		context.owners.instance = createInstance(context.owners, context.handles);
		context.owners.debugCallback = createDebugCallback(context.owners, context.handles);
		context.owners.surface = createSurface(context.owners, context.handles);

		context.handles.physicalDevice = selectPhysicalDevice(context.owners, context.handles);
		context.owners.device = createDevice(context.owners, context.handles);

		context.handles.queues = selectQueues(context.owners, context.handles);

		if (ngn::config::core.dirty() && ! ngn::config::core.store()) {
			ngn::log::error("Failed to store core config in \"{}\":\n{}", ngn::config::core.source(), ngn::config::core.dump());
		}

		return context;
	}

	vk::UniqueInstance createInstance(Context::Owners &/*owners*/, Context::Handles &/*handles*/) {
		// return creators.instance.create(creators.debugCallback, creators.surface);

		// surface
		creators.instance.requireExtensions(creators.surface.requiredExtensions);

		// debugCallback
		/*
		InstancePlanner instancePlanner = InstancePlanner{}
			.includeExtensions(creators.surface.requiredExtensions)
			.fallback([] (std::vector<std::string> missingExts) {
				std::string exts = ngn::str::implode(missingExts);

				ngn::log::error("Vulkan debug reporting disabled, missing {} extension(s): {}", missingExts.size(), exts);

				throw std::runtime_error{"Failed to include " + exts + " Vulkan extension(s)"};
			})
			.includeExtensions(creators.debugCallback.optionalExtensions)
			.tap([] () {
				creators.debugCallback.isAvailable = true;
			})
			.fallback([] (std::vector<std::string> missingExts) {
				creators.debugCallback.isAvailable = false;

				ngn::log::warn("Vulkan debug reporting disabled, missing {} extension(s): {}", missingExts.size(), ngn::str::implode(missingExts));
			})
			.includeLayers(creators.debugCallback.optionalLayers)
			.fallback([] ([] (std::vector<std::string> missingLayers) {
				ngn::log::warn("Vulkan debug reporting might be limited, missing {} layer(s): {}", missingLayers.size(), ngn::str::implode(missingLayers));
			});

		return creators.instance.create(instancePlanner);
		*/
		std::vector<std::string> missingExts = creators.instance.findMissingExtensions(creators.debugCallback.optionalExtensions);
		creators.debugCallback.isAvailable = missingExts.empty();

		if (creators.debugCallback.isAvailable) {
			creators.instance.requireExtensions(creators.debugCallback.optionalExtensions);

			std::vector<std::string> missingLayers = creators.instance.tryIncludeLayers(creators.debugCallback.optionalLayers);

			if ( ! missingLayers.empty()) {
				ngn::log::warn("Vulkan debug reporting might be limited, missing {} layer(s): {}", missingLayers.size(), ngn::str::implode(missingLayers));
			}
		} else {
			ngn::log::warn("Vulkan debug reporting disabled, missing {} extension(s): {}", missingExts.size(), ngn::str::implode(missingExts));
		}

		return creators.instance.create();
	}

	vk::UniqueDebugReportCallbackEXT createDebugCallback(Context::Owners &owners, Context::Handles &/*handles*/) {
		return creators.debugCallback.create(owners.instance);
	}

	vk::UniqueSurfaceKHR createSurface(Context::Owners &owners, Context::Handles &/*handles*/) {
		return creators.surface.create(window, owners.instance);
	}

	PhysicalDeviceHandle selectPhysicalDevice(Context::Owners &owners, Context::Handles &/*handles*/) {
		return selectors.physicalDevice.select(owners.surface, owners.instance);
	}

	vk::UniqueDevice createDevice(Context::Owners &owners, Context::Handles &handles) {
		return creators.device.create(owners.surface, owners.instance, handles.physicalDevice);
	}

	QueuesHandle selectQueues(Context::Owners &owners, Context::Handles &handles) {
		return selectors.queues.select(owners.surface, owners.instance, handles.physicalDevice, owners.device);
	}

	// void appendRequiredLayers(const std::vector<std::string> &layers) {
	// 	for (const std::string &layer : layers) {
	// 		if ( ! creators.instance.appendLayer(layer)) {
	// 			throw std::runtime_error{"Required layer \"" + layer + "\" is not available"};
	// 		}
	// 	}
	// }

	// void appendRequiredExtensions(const std::vector<std::string> &extensions) {
	// 	for (const std::string &extension : extensions) {
	// 		if ( ! creators.instance.appendExtension(extension)) {
	// 			throw std::runtime_error{"Required extension \"" + extension + "\" is not available"};
	// 		}
	// 	}
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

	// void appendGLFWExtensionsAndLayers(InstanceCreator &instanceCreator) {
	// 	std::vector<std::string> glfwExtensions = glfw.getRequiredInstanceExtensions();
	// 	for (const std::string &extension : glfwExtensions) {
	// 		if ( ! instanceCreator.addExtension(extension)) {
	// 			throw std::runtime_error{"Required GLFW extension \"" + extension + "\" is not available"};
	// 		}
	// 	}
	// }

	// void appendDebugExtensionsAndLayers(InstanceCreator &instanceCreator) {
	// 	creators.debugCallback.isAvailable = true;

	// 	for (const auto &extension : creators.debugCallback.optionalExtensions) {
	// 		if ( ! instanceCreator.addExtension(extension)) {
	// 			std::clog << "Warn: required debug extension \"" << extension << "\" not available, debugging disabled" << std::endl;
	// 			creators.debugCallback.isAvailable = false;
	// 			return;
	// 		}
	// 	}

	// 	for (const auto &layer : creators.debugCallback.optionalLayers) {
	// 		if ( ! instanceCreator.addLayer(layer)) {
	// 			std::clog << "Warn: optional debug layer \"" << layer << "\" not available, debugging info might be limited" << std::endl;
	// 		}
	// 	}
	// }
};

} // vlk

} // rn
