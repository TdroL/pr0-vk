#include "contextGLFW.hpp"

#include <spdlog/spdlog.h>

namespace ware::contextGLFW {

void errorCallback(int code, const char *message) {
	spdlog::error("ware::contextGLFW::errorCallback => error: ({}) {}", code, message);
}

State::~State() {
	glfwPollEvents();
	glfwTerminate();
}

State setup() {
	glfwSetErrorCallback(errorCallback);

	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error{"Failed to initialize GLFW"};
	}

	return State{};
}

void refresh([[maybe_unused]] State &state) {
	glfwPollEvents();
}

void process([[maybe_unused]] State &state) {}

} // ware::contextGLFW
