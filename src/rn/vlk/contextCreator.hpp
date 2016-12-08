#pragma once

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <vulkan/vulkan.hpp>

#include "../../ngn/log.hpp"
#include "../../ngn/str.hpp"

#include "context.hpp"
#include "glfwCreator.hpp"
#include "instanceCreator.hpp"
#include "debugCallbackCreator.hpp"
#include "physicalDeviceCreator.hpp"
#include "deviceCreator.hpp"
#include "queuesCreator.hpp"

namespace rn {

namespace vlk {

class ContextCreator {
public:
	struct Creators {
		GLFWCreator glfw{};
		InstanceCreator instance{};
		DebugCallbackCreator debugCallback{};
		PhysicalDeviceCreator physicalDevice{};
		DeviceCreator device{};
		QueuesCreator queues{};
	} creators{};

	Context create() {
		Context context{};

		context.owners.glfw = createGLFW(context.owners);
		context.owners.instance = createInstance(context.owners);
		context.owners.debugCallback = createDebugCallback(context.owners);
		context.owners.physicalDevice = createPhysicalDevice(context.owners);
		context.owners.device = createDevice(context.owners);
		context.owners.queues = createQueues(context.owners);

		return context;
	}

	GLFWOwner createGLFW(Context::Owners &/*context*/) {
		return creators.glfw.create();
	}

	InstanceOwner createInstance(Context::Owners &owners) {
		creators.instance.applicationName = "pr0-vk";

		// glfw
		appendRequiredExtensions(owners.glfw.handle.requiredInstanceExtensions());

		// debugCallback
		std::vector<std::string> notFoundResults;
		notFoundResults = appendOptionalExtensions(creators.debugCallback.optionalExtensions);
		if ( ! notFoundResults.empty()) {
			ngn::log::warn("Debug callback disabled, missing {} extension(s): {}", notFoundResults.size(), ngn::str::implode(notFoundResults));
			creators.debugCallback.isAvailable = false;
		} else {
			creators.debugCallback.isAvailable = true;

			notFoundResults = appendOptionalLayers(creators.debugCallback.optionalLayers);

			if ( ! notFoundResults.empty()) {
				ngn::log::warn("Debugging info might be limited, missing {} layer(s): {}", notFoundResults.size(), ngn::str::implode(notFoundResults));
			}
		}

		return creators.instance.create();
	}

	DebugCallbackOwner createDebugCallback(Context::Owners &owners) {
		return creators.debugCallback.create(owners.instance);
	}

	PhysicalDeviceOwner createPhysicalDevice(Context::Owners &owners) {
		return creators.physicalDevice.create(owners.glfw, owners.instance);
	}

	DeviceOwner createDevice(Context::Owners &owners) {
		return creators.device.create(owners.glfw, owners.instance, owners.physicalDevice);
	}

	QueuesOwner createQueues(Context::Owners &owners) {
		return creators.queues.create(owners.glfw, owners.instance, owners.physicalDevice, owners.device);
	}

	void appendRequiredLayers(const std::vector<std::string> &layers) {
		for (const std::string &layer : layers) {
			if ( ! creators.instance.appendLayer(layer)) {
				throw std::runtime_error{"Required layer \"" + layer + "\" is not available"};
			}
		}
	}

	void appendRequiredExtensions(const std::vector<std::string> &extensions) {
		for (const std::string &extension : extensions) {
			if ( ! creators.instance.appendExtension(extension)) {
				throw std::runtime_error{"Required extension \"" + extension + "\" is not available"};
			}
		}
	}

	std::vector<std::string> appendOptionalLayers(const std::vector<std::string> &layers) {
		std::vector<std::string> notFound{};

		for (const std::string &layer : layers) {
			if ( ! creators.instance.appendLayer(layer)) {
				notFound.push_back(layer);
			}
		}

		return notFound;
	}

	std::vector<std::string> appendOptionalExtensions(const std::vector<std::string> &extensions) {
		std::vector<std::string> notFound{};

		for (const std::string &extension : extensions) {
			if ( ! creators.instance.appendExtension(extension)) {
				notFound.push_back(extension);
			}
		}

		return notFound;
	}


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
