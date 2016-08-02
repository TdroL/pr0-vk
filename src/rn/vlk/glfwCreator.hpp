#pragma once

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#include "glfwWrapper.hpp"

namespace rn {

namespace vlk {

class Context;

class GLFWCreator {
public:
	Context &context;
	GLFWCreator(Context &context) : context(context) {}

	GLFWWrapper create() {
		if (glfwInit() != GLFW_TRUE) {
			throw std::runtime_error{"Failed to initialize GLFW"};
		}

		GLFWWrapper glfw{GLFW{true}};

		if (glfwVulkanSupported() != GLFW_TRUE) {
			throw std::runtime_error{"Vulkan not supported"};
		}

		return glfw;
	}
};

} // vlk

} // rn
