#pragma once

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <vulkan/vulkan.hpp>

#include "context.hpp"
#include "glfwCreator.hpp"
#include "instanceCreator.hpp"
#include "debugCallbackCreator.hpp"
#include "deviceCreator.hpp"

namespace rn {

namespace vlk {

class ContextCreator {
public:
	struct Creators {
		GLFWCreator glfw{};
		InstanceCreator instance{};
		DebugCallbackCreator debugCallback{};
		DeviceCreator device{};
	} creators{};

	Context create() {
		Context context{};

		context.owners.glfw = createGLFW(context);
		context.owners.instance = createInstance(context);
		context.owners.debugCallback = createDebugCallback(context);
		context.owners.device = createDevice(context);

		return context;
	}

	GLFWOwner createGLFW(Context &/*context*/) {
		return creators.glfw.create();
	}

	InstanceOwner createInstance(Context &context) {
		creators.instance.applicationName = "pr0-vk";

		// glfw
		appendRequiredExtensions(context.glfw.requiredInstanceExtensions());

		// debugCallback
		std::vector<std::string> notFoundResults;
		notFoundResults = appendOptionalExtensions(creators.debugCallback.optionalExtensions);
		if ( ! notFoundResults.empty()) {
			std::clog << "Warn: debug callback disabled, missing " << notFoundResults.size() << " extension(s): " << implode(notFoundResults) << std::endl;
			creators.debugCallback.isAvailable = false;
		} else {
			creators.debugCallback.isAvailable = true;

			notFoundResults = appendOptionalLayers(creators.debugCallback.optionalLayers);

			if ( ! notFoundResults.empty()) {
				std::clog << "Warn: debugging info might be limited, missing " << notFoundResults.size() << " layer(s): " << implode(notFoundResults) << std::endl;
			}
		}

		return creators.instance.create();
	}

	DebugCallbackOwner createDebugCallback(Context &context) {
		return creators.debugCallback.create(context.instance);
	}

	DeviceOwner createDevice(Context &context) {
		return creators.device.create(context.glfw, context.instance);
	}

	std::string implode(const std::vector<std::string> &records) {
		if (records.size() == 1) {
			return records[0];
		}

		std::string delim = ", ";
		size_t reserveSize = 0;

		for (const auto &item : records) {
			reserveSize += item.size();
			reserveSize += delim.size();
		}

		std::string result{};
		result.reserve(reserveSize - delim.size());

		for (size_t i = 0, l = records.size() - 1; i < l; i++) {
			result += records[i];
			result += delim;
		}

		result += records[records.size() - 1];

		return result;
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
