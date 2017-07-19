#pragma once

#include "../ngn/config.hpp"
#include "../ngn/log.hpp"

#include <vulkan/vulkan.hpp>
#include "glfw.hpp"

namespace rn {

class Window {
public:
	typedef ngn::config::WindowMode WindowMode;

	GLFWwindow *handle = nullptr;

	struct Properties {
		std::string title{};
		int width{};
		int height{};
		int monitor{-1};
		WindowMode mode{};
	};

	Properties currentProperties{};

	Properties nextProperties{};

	Window() {
		loadState();
	}

	~Window() {
		destroy();
	}

	void loadState() {
		nextProperties.title = ngn::config::core.application.name();
		nextProperties.width = ngn::config::core.window.width();
		nextProperties.height = ngn::config::core.window.height();
		nextProperties.monitor = ngn::config::core.window.monitor();
		nextProperties.mode = ngn::config::core.window.mode();
	}

	bool needsRefresh() {
		return (
			currentProperties.title != nextProperties.title ||
			currentProperties.width != nextProperties.width ||
			currentProperties.height != nextProperties.height ||
			currentProperties.monitor != nextProperties.monitor ||
			currentProperties.mode != nextProperties.mode
		);
	}

	void refresh() {
		currentProperties = nextProperties;
	}

	void create() {
		destroy();

		glfwDefaultWindowHints();

		glfwWindowHint(GLFW_ALPHA_BITS, 0);
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
		glfwWindowHint(GLFW_STENCIL_BITS, 0);
		glfwWindowHint(GLFW_SAMPLES, 0);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		GLFWmonitor *monitor = nullptr;

		switch (nextProperties.mode) {
		case WindowMode::Borderless:
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		break;

		case WindowMode::Windowed:
			glfwWindowHint(GLFW_DECORATED, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		break;

		case WindowMode::Fullscreen:
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			if (nextProperties.monitor < 0) {
				monitor = glfwGetPrimaryMonitor();
			} else {
				int count;
				GLFWmonitor** monitors = glfwGetMonitors(&count);

				if (count <= 0) {
					ngn::log::error("Could not fetch monitors");
				} else if (monitors == nullptr) {
					ngn::log::error("No monitors found");
				} else if (nextProperties.monitor >= count) {
					ngn::log::warn("Monitor index out of bounds: {} >= {}", nextProperties.monitor, count);

					monitor = glfwGetPrimaryMonitor();
					nextProperties.monitor = -1;
				} else {
					monitor = monitors[nextProperties.monitor];
				}
			}

			const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
		break;
		}

		ngn::log::debug("Creating window: {} x {} with title \"{}\"", nextProperties.width, nextProperties.height, nextProperties.title);

		handle = glfwCreateWindow(nextProperties.width, nextProperties.height, nextProperties.title.c_str(), monitor, nullptr);

		if (handle == nullptr) {
			throw std::runtime_error{"Failed to create the window"};
		}

		glfwSetWindowUserPointer(handle, this);
		glfwSetWindowSizeCallback(handle, Window::onResize);

		currentProperties = nextProperties;
	}

	void destroy() {
		if (handle != nullptr) {
			glfwDestroyWindow(handle);
			handle = nullptr;
		}
	}

	vk::SurfaceKHR createSurface(vk::Instance &instance) {
		if (handle == nullptr) {
			ngn::log::error("Could not create surface without a window");
			return vk::SurfaceKHR{};
		}

		VkSurfaceKHR surface = VK_NULL_HANDLE;

		vk::Result err = static_cast<vk::Result>(glfwCreateWindowSurface(instance, handle, NULL, &surface));
		if (err != vk::Result::eSuccess) {
			ngn::log::error("Failed to create window surface: {}", vk::to_string(err));
			return vk::SurfaceKHR{};
		}

		return vk::SurfaceKHR{surface};
	}

	static void onResize(GLFWwindow *handle, int width, int height) {
		if (width == 0 || height == 0) {
			return;
		}

		ngn::log::debug("Window resize: {} {}", width, height);
		Window* window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(handle));
		window->nextProperties.width = width;
		window->nextProperties.height = height;
	}
};

}
