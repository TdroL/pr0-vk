#pragma once

#include "../config/config.hpp"
#include "../contextGLFW/contextGLFW.hpp"

#include <memory>

namespace ware::windowGLFW {

using Properties = ware::config::State::Window;

struct Description {
	Properties current;
	Properties next;
	bool changed;
};

struct State {
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;
	std::unique_ptr<Description> description;

	double refreshTimePoint;
	bool shouldClose = false;

	~State();
};

State setup(ware::config::State &config, ware::contextGLFW::State &glfw);

void refresh(State &state);

void process(State &state);

} // ware::windowGLFW
