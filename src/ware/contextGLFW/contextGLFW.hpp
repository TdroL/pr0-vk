#pragma once

#include <span>

#include <vulkan/vulkan.hpp> // vulkan.h needs to be included before glfw3
#include <GLFW/glfw3.h>

namespace ware::contextGLFW {

struct State {
	~State();
};

bool isVulkanSupported(State &state);

PFN_vkGetInstanceProcAddr loadVulkanGetInstanceProcAddr(State &state);

std::span<const char *> getVulkanRequiredInstanceExtensions(State &state);

State setup();

void refresh(State &state);

void process(State &state);

} // ware::contextGLFW
