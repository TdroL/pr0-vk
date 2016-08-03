#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "glfwCreator.hpp"
#include "instanceCreator.hpp"
#include "debugCallbackCreator.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	struct Owners {
		GLFWOwner glfw{};
		InstanceOwner instance{};
		DebugCallbackOwner debugCallback{};
	} owners{};

	GLFW &glfw = owners.glfw.handle;
	vk::Instance &instance = owners.instance.handle;

	struct Creators {
		GLFWCreator glfw{};
		InstanceCreator instance{};
		DebugCallbackCreator debugCallback{};
	} creators{};

	void init() {
		createGLFW();
		createInstance();
		createDebugCallback();
	}

	void appendExtensionsAndLayers(InstanceCreator &instanceCreator) {
		appendGLFWExtensionsAndLayers(instanceCreator);
		appendDebugExtensionsAndLayers(instanceCreator);
	}

	void appendGLFWExtensionsAndLayers(InstanceCreator &instanceCreator) {
		std::vector<std::string> glfwExtensions = glfw.getRequiredInstanceExtensions();
		for (const std::string &extension : glfwExtensions) {
			if ( ! instanceCreator.addExtension(extension)) {
				throw std::runtime_error{"Required GLFW extension \"" + extension + "\" is not available"};
			}
		}
	}

	void appendDebugExtensionsAndLayers(InstanceCreator &instanceCreator) {
		creators.debugCallback.isAvailable = true;

		for (const auto &extension : creators.debugCallback.requiredExtensions) {
			if ( ! instanceCreator.addExtension(extension)) {
				std::clog << "Warn: required debug extension \"" << extension << "\" not available, debugging disabled" << std::endl;
				creators.debugCallback.isAvailable = false;
				return;
			}
		}

		for (const auto &layer : creators.debugCallback.optionalLayers) {
			if ( ! instanceCreator.addLayer(layer)) {
				std::clog << "Warn: optional debug layer \"" << layer << "\" not available, debugging info might be limited" << std::endl;
			}
		}
	}

	void createGLFW() {
		owners.glfw = creators.glfw.create();
	}

	void createInstance() {
		creators.instance.applicationName = "pr0-vk";

		creators.instance.loadAvailableLayersAndExtensions();

		appendExtensionsAndLayers(creators.instance);

		owners.instance = creators.instance.create();
	}

	void createDebugCallback() {
		owners.debugCallback = creators.debugCallback.create();
	}
};

} // vlk

} // rn
