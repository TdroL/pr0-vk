#pragma once

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#include "context.hpp"

namespace rn {

namespace vlk {

class GLFWCreator {
public:
	void init(Context &context) {
		if (glfwInit() != GLFW_TRUE) {
			throw std::runtime_error{"Failed to initialize GLFW"};
		}

		if (glfwVulkanSupported() != GLFW_TRUE) {
			throw std::runtime_error{"Vulkan not supported"};
		}
	}

	std::vector<std::string> getRequiredInstanceExtensions() {
		uint32_t count;
		const char **extensions = glfwGetRequiredInstanceExtensions(&count);

		return std::vector<std::string>(extensions, extensions + count);
		// std::vector<std::string> extensionVector{};
		// extensionVector.reserve(count);
		// for (uint32_t i = 0; i < count; ++i) {
		// 	extensionVector.emplace_back(extensions[i]);
		// }
		// return extensionVector;
	}

	void deinit(Context &context) {
		glfwTerminate();
	}
};

} // vlk

} // rn
