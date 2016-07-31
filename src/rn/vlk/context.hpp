#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "glfwCreator.hpp"
#include "instanceCreator.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	vk::Instance instance{};
	bool debugAvailable = false;

	struct Creators {
		Context &context;
		Creators(Context &context) : context(context) {}

		GLFWCreator glfw{context};
		InstanceCreator instance{context};
	} creators{*this};

	void init() {
		initGLFW();

		initInstance();
		initDebug();
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
		std::vector<std::string> glfwExtensions = creators.glfw.getRequiredInstanceExtensions();
		for (const std::string &extension : glfwExtensions) {
			if ( ! creators.instance.addExtension(extension)) {
				throw std::runtime_error{"Required GLFW extension \"" + extension + "\" is not available"};
			}
		}

		debugAvailable = true;
		if ( ! creators.instance.addExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
			std::clog << "Warn: \"" VK_EXT_DEBUG_REPORT_EXTENSION_NAME "\" not available" << std::endl;
			debugAvailable = false;
		}

		if (debugAvailable && ! creators.instance.addLayer("VK_LAYER_LUNARG_standard_validation")) {
			std::clog << "Warn: \"VK_LAYER_LUNARG_standard_validation\" not available" << std::endl;
		}

		creators.instance.init();
	}

	void deinitInstance() {
		// creators.instance.deinit();
	}

	void initDebug() {}

	void deinitDebug() {}

	~Context() {
		deinitDebug();
		deinitInstance();
		deinitGLFW();
	}

	// VkInstance instance = VK_NULL_HANDLE;

	// VkDevice device = VK_NULL_HANDLE;

	// VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	// VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};

	// VkDebugReportCallbackEXT debugReportCallback = VK_NULL_HANDLE;
};

} // vlk

} // rn
