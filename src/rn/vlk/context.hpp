#pragma once

#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "glfwOwner.hpp"
#include "instanceOwner.hpp"
#include "debugCallbackOwner.hpp"
#include "deviceOwner.hpp"

#define HANDLE_ALIAS(name) decltype(owners.name.handle) &name = owners.name.handle

namespace rn {

namespace vlk {

class Context {
public:
	struct Owners {
		GLFWOwner glfw{};
		InstanceOwner instance{};
		DebugCallbackOwner debugCallback{};
		DeviceOwner device{};
	} owners{};

	HANDLE_ALIAS(glfw);
	HANDLE_ALIAS(instance);
	HANDLE_ALIAS(device);
};

} // vlk

} // rn

#undef HANDLE_ALIAS