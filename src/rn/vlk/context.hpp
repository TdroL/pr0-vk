#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "interface.hpp"
#include "instanceCreator.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	VkInstance instance{};
	bool debugAvailable = false;

	void init() {
		initGLFW();

		initInstance();
		initDebug();
	}

	void initGLFW() {
		if (glfwInit() != GLFW_TRUE) {
			throw std::runtime_error{"Failed to initialize GLFW"};
		}

		if (glfwVulkanSupported() != GLFW_TRUE) {
			throw std::runtime_error{"Vulkan not supported"};
		}

		auto getInstanceProcAddress = glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr");
		if ( ! vk.loadGlobalProcs(getInstanceProcAddress)) {
			throw std::runtime_error{"Failed to load global Vulkan functions"};
		}
	}

	void deinitGLFW() {
		vk.unloadGlobalProcs();
		glfwTerminate();
	}

	void initInstance() {
		InstanceCreator instanceCreator{};
		instanceCreator.applicationName = "pr0-vk";

		uint32_t glfwExtensionCount;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		for (uint32_t i = 0; i < glfwExtensionCount; i++) {
			if ( ! instanceCreator.addExtension(glfwExtensions[i])) {
				throw std::runtime_error{"Required GLFW extension \"" + std::string{glfwExtensions[i]} + "\" is not available"};
			}
		}

		if ( ! instanceCreator.addExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
			std::clog << "Warn: \"" VK_EXT_DEBUG_REPORT_EXTENSION_NAME "\" not available" << std::endl;
			debugAvailable = false;
		} else {
			debugAvailable = true;
		}

		if ( ! instanceCreator.addLayer("VK_LAYER_LUNARG_standard_validation")) {
			std::clog << "Warn: \"VK_LAYER_LUNARG_standard_validation\" not available" << std::endl;
		}

		VkResult result = instanceCreator.run(*this);
		if (result != VK_SUCCESS) {
			throw std::runtime_error{"Failed to initialize Vulkan instance (" + resultToString(result) + ")"};
		}

		if ( ! vk.loadInstanceProcs(instance)) {
			throw std::runtime_error{"Failed to load instance Vulkan functions"};
		}
	}

	void deinitInstance() {
		vk.destoyHandle(instance);
		vk.unloadInstanceProcs();
	}

	void initDebug() {}

	void deinitDebug() {}

	std::string resultToString(VkResult result) {
		switch (result) {
			case VK_SUCCESS: return "VK_SUCCESS";
			case VK_NOT_READY: return "VK_NOT_READY";
			case VK_TIMEOUT: return "VK_TIMEOUT";
			case VK_EVENT_SET: return "VK_EVENT_SET";
			case VK_EVENT_RESET: return "VK_EVENT_RESET";
			case VK_INCOMPLETE: return "VK_INCOMPLETE";
			case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
			case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
			case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
			case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
			case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
			case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
			case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
			case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
			case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
			case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
			case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
			case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
			case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
			case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
			default: return std::to_string(result);
		}
	}

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
