#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "glfwOwner.hpp"
#include "instanceOwner.hpp"
#include "debugCallbackOwner.hpp"
#include "physicalDeviceOwner.hpp"
#include "deviceOwner.hpp"
#include "queuesOwner.hpp"
#include "swapchainOwner.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	struct Owners {
		GLFWOwner glfw{};
		InstanceOwner instance{};
		DebugCallbackOwner debugCallback{};
		PhysicalDeviceOwner physicalDevice{};
		DeviceOwner device{};
		QueuesOwner queues{};
	} owners{};

	#define OWNER_HANDLE_ALIAS(name) decltype(owners.name.handle) &name = owners.name.handle
	OWNER_HANDLE_ALIAS(glfw);
	OWNER_HANDLE_ALIAS(instance);
	OWNER_HANDLE_ALIAS(physicalDevice);
	OWNER_HANDLE_ALIAS(device);
	#undef OWNER_HANDLE_ALIAS

};

} // vlk

} // rn
