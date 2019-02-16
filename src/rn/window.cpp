#include "window.hpp"

#include <tuple>
#include "../ngn/log.hpp"
#include "../ngn/external/glfw.hpp"
#include "../ngn/external.hpp"

namespace rn {

const auto initialize = ngn::external::initialize<ngn::external::GLFW>();

void Window::destroy(Window::Handle *handle) {
	if (handle != nullptr) {
		glfwDestroyWindow(reinterpret_cast<GLFWwindow *>(handle));
	}
}

void Window::onResize(Window::Handle *handle, int width, int height) {
	if (width == 0 || height == 0) {
		return;
	}

	ngn::log::debug("rn::Window::onResize() => window resize: {} {}", width, height);
	Window* window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(reinterpret_cast<GLFWwindow *>(handle)));
	window->nextProperties.width = width;
	window->nextProperties.height = height;
}

Window::Window(Properties &&initialProperties) noexcept :
	handle{nullptr, &Window::destroy},
	nextProperties{std::move(initialProperties)}
{}

bool Window::needsRefresh() {
	return std::tie(currentProperties.title, currentProperties.width, currentProperties.height, currentProperties.monitor, currentProperties.mode) != std::tie(nextProperties.title, nextProperties.width, nextProperties.height, nextProperties.monitor, nextProperties.mode);
}

void Window::refresh() {
	currentProperties = nextProperties;
}

void Window::requestClose() {
	if ( ! handle) {
		return;
	}

	glfwSetWindowShouldClose(reinterpret_cast<GLFWwindow *>(handle.get()), GLFW_TRUE);
}

void Window::forceClose() {
	handle.reset();
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(reinterpret_cast<GLFWwindow *>(handle.get()));
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::create() {
	forceClose();

	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_SAMPLES, 0);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWmonitor *monitor = nullptr;

	switch (nextProperties.mode) {
		case WindowMode::Borderless: {
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			break;
		}
		case WindowMode::Windowed: {
			glfwWindowHint(GLFW_DECORATED, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
			break;
		}
		case WindowMode::Fullscreen: {
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			if ( ! nextProperties.monitor) {
				monitor = glfwGetPrimaryMonitor();
			} else {
				int count;
				GLFWmonitor** monitors = glfwGetMonitors(&count);

				if (count <= 0 || monitors == nullptr) {
					ngn::log::warn("rn::Window::create() => could not find monitors, default to primary");
					monitor = glfwGetPrimaryMonitor();
				} else if (count > 0 && *nextProperties.monitor >= static_cast<uint32_t>(count)) {
					ngn::log::warn("rn::Window::create() => monitor index out of bounds: {} >= {}, using primary monitor", *nextProperties.monitor, count);

					monitor = glfwGetPrimaryMonitor();
					nextProperties.monitor = std::nullopt;
				} else {
					monitor = monitors[*nextProperties.monitor];
				}
			}

			const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

			break;
		}
	}

	ngn::log::debug("rn::Window::create() => creating window: {} x {} with title \"{}\"", nextProperties.width, nextProperties.height, nextProperties.title);

	handle.reset(reinterpret_cast<Window::Handle *>(glfwCreateWindow(nextProperties.width, nextProperties.height, nextProperties.title.c_str(), monitor, nullptr)));

	if ( ! handle) {
		throw std::runtime_error{"Failed to create the window"};
	}

	glfwSetWindowUserPointer(reinterpret_cast<GLFWwindow *>(handle.get()), this);
	glfwSetWindowSizeCallback(reinterpret_cast<GLFWwindow *>(handle.get()), reinterpret_cast<GLFWwindowsizefun>(Window::onResize));

	currentProperties = nextProperties;
}

} // rn
