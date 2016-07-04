#pragma once

#include "instance.hpp"
#include "surface.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "swapchain.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	Instance instance;
	Surface surface;
	PhysicalDevice physicalDevice;
	Device device;
	Swapchain swapchain;

	#include "contextDispatch.inl"

	void loadGlobalVulkanFunctions() {
		loadGlobalProc(this->vkCreateInstance, "vkCreateInstance");
		loadGlobalProc(this->vkEnumerateInstanceExtensionProperties, "vkEnumerateInstanceExtensionProperties");
		loadGlobalProc(this->vkEnumerateInstanceLayerProperties, "vkEnumerateInstanceLayerProperties");

		assert(this->vkCreateInstance);
		assert(this->vkEnumerateInstanceExtensionProperties);
		assert(this->vkEnumerateInstanceLayerProperties);
	}
};

} // vlk

} // rn