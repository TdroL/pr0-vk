#include "contextGLFW.hpp"

#include <spdlog/spdlog.h>

namespace ware::contextGLFW {

void errorCallback(int code, const char *message) {
	spdlog::error("ware::contextGLFW::errorCallback => error: ({}) {}", code, message);
}

bool isVulkanSupported([[maybe_unused]] State &state) {
	return glfwVulkanSupported() == GLFW_TRUE;
}

PFN_vkGetInstanceProcAddr loadVulkanGetInstanceProcAddr([[maybe_unused]] State &state) {
	return reinterpret_cast<PFN_vkGetInstanceProcAddr>(glfwGetInstanceProcAddress(VK_NULL_HANDLE, "vkGetInstanceProcAddr"));
}

std::span<const char *> getVulkanRequiredInstanceExtensions([[maybe_unused]] State &state) {
	uint32_t count;
	const char **extensions = glfwGetRequiredInstanceExtensions(&count);

	return { extensions, count };
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
