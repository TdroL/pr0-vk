#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

// #include "instanceOwner.hpp"
// #include "surfaceOwner.hpp"
// #include "debugCallbackOwner.hpp"
// #include "physicalDeviceOwner.hpp"
// #include "deviceOwner.hpp"
// #include "queuesOwner.hpp"
// #include "swapchainOwner.hpp"

#include "physicalDeviceHandle.hpp"
#include "queuesHandle.hpp"

namespace rn {

namespace vlk {

class Context {
public:
	// struct Owners {
	// 	InstanceOwner instance{};
	// 	DebugCallbackOwner debugCallback{};
	// 	SurfaceOwner surface{};
	// 	PhysicalDeviceOwner physicalDevice{};
	// 	DeviceOwner device{};
	// 	QueuesOwner queues{};
	// } owners{};

	struct Owners {
		vk::UniqueInstance instance{};
		vk::UniqueDebugReportCallbackEXT debugCallback{};
		vk::UniqueSurfaceKHR surface{};
		vk::UniqueDevice device{};
		vk::UniqueSwapchainKHR swapchain{}
	} owners{};

	struct Handles {
		PhysicalDeviceHandle physicalDevice{};
		QueuesHandle queues{};
	} handles{};

	// #define OWNER_HANDLE_ALIAS(name) decltype(owners.name.handle) &name = owners.name.handle
	// OWNER_HANDLE_ALIAS(instance);
	// OWNER_HANDLE_ALIAS(surface);
	// OWNER_HANDLE_ALIAS(physicalDevice);
	// OWNER_HANDLE_ALIAS(device);
	// #undef OWNER_HANDLE_ALIAS
};

} // vlk

} // rn
