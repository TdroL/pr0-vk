#include "glfw.hpp"

#include "../ngn/log.hpp"

#include <string>
#include <stdexcept>

namespace rn {

void errorCallback(int code, const char *message) {
	ngn::log::error("GLFW error: ({}) {}", code, message);
}

uint32_t GLFW::initialized = 0;

GLFW::GLFW() {
	if (initialized > 0) {
		// TODO: support multiple GLFW objects?
		throw std::runtime_error{"GLFW already initialized"};
	}

	glfwSetErrorCallback(errorCallback);

	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error{"Failed to initialize GLFW"};
	}

	initialized++;
}

GLFW::~GLFW() {
	initialized--;
	glfwTerminate();
}

GLFW glfw{};

} // rn
