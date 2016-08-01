#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "glfwCreator.hpp"
#include "instanceCreator.hpp"
#include "debugCreator.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	vk::Instance instance{};

	struct Creators {
		Context &context;
		Creators(Context &context) : context(context) {}

		GLFWCreator glfw{context};
		InstanceCreator instance{context};
		DebugCreator debug{context};
	} creators{*this};

	void init() {
		initGLFW();
		initInstance();
		initDebug();
	}

	void deinit() {
		deinitDebug();
		deinitInstance();
		deinitGLFW();
	}

	void appendGLFWExtensionsAndLayers() {
		std::vector<std::string> glfwExtensions = creators.glfw.getRequiredInstanceExtensions();
		for (const std::string &extension : glfwExtensions) {
			if ( ! creators.instance.addExtension(extension)) {
				throw std::runtime_error{"Required GLFW extension \"" + extension + "\" is not available"};
			}
		}
	}

	void appendDebugExtensionsAndLayers() {
		creators.debug.isAvailable = true;

		for (const auto &extension : creators.debug.requiredExtensions) {
			if ( ! creators.instance.addExtension(extension)) {
				std::clog << "Warn: required debug extension \"" << extension << "\" not available, debugging disabled" << std::endl;
				creators.debug.isAvailable = false;
				return;
			}
		}

		for (const auto &layer : creators.debug.optionalLayers) {
			if ( ! creators.instance.addLayer(layer)) {
				std::clog << "Warn: optional debug layer \"" << layer << "\" not available, debugging info might be limited" << std::endl;
			}
		}
	}

	void initGLFW() {
		creators.glfw.init();
	}

	void deinitGLFW() {
		creators.glfw.deinit();
	}

	void initInstance() {
		creators.instance.applicationName = "pr0-vk";

		creators.instance.loadAvailableLayersAndExtensions();

		appendGLFWExtensionsAndLayers();
		appendDebugExtensionsAndLayers();

		creators.instance.init();
	}

	void deinitInstance() {
		creators.instance.deinit();
	}

	void initDebug() {
		creators.debug.init();
	}

	void deinitDebug() {
		creators.debug.deinit();
	}

	~Context() {
		deinit();
	}
};

} // vlk

} // rn
