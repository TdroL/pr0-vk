#pragma once

#include <vulkan/vulkan.hpp> // vulkan.h needs to be included before glfw3
#include <GLFW/glfw3.h>

namespace ware::contextGLFW {

struct State {
	~State();
};

State setup();

void refresh(State &state);

void process(State &state);

} // ware::contextGLFW
