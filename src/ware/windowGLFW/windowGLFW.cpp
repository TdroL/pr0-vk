#include "windowGLFW.hpp"

#include <tuple>

#include <spdlog/spdlog.h>

namespace ware::windowGLFW {

using WindowMode = ware::config::WindowMode;

void onResize(GLFWwindow *window, int width, int height) {
	if (width == 0 || height == 0) {
		return;
	}

	Description *description = reinterpret_cast<Description *>(glfwGetWindowUserPointer(window));
	if ( ! description) {
		spdlog::error("ware::windowGLFW::onResize() => window description missing");
		return;
	}

	spdlog::info("ware::windowGLFW::onResize() => window resize: {} {}", width, height);
	description->next.width = width;
	description->next.height = height;
	description->changed = true;
}

[[nodiscard]] std::tuple<GLFWmonitor *, int> selectMonitor(int index) {
	if (index < 0) {
		return { glfwGetPrimaryMonitor(), index };
	}

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	if (count <= 0 || monitors == nullptr) {
		spdlog::warn("ware::windowGLFW::setup({}) => could not find monitors, default to primary monitor", index);
		return { glfwGetPrimaryMonitor(), -1 };
	} else if (count > 0 && index >= count) {
		spdlog::warn("ware::windowGLFW::setup({}) => monitor index out of bounds (found {} entries), default to primary monitor", index, count);

		return { glfwGetPrimaryMonitor(), -1 };
	} else {
		return { monitors[index], index };
	}
}

[[nodiscard]] std::tuple<std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>, std::unique_ptr<Description>> createWindow(const ware::config::State &config) {
	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_SAMPLES, 0);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWmonitor *monitor = nullptr;
	int index = config.window.monitor;

	switch (config.window.mode) {
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

			std::tie(monitor, index) = selectMonitor(config.window.monitor);

			const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

			break;
		}
	}

	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window{glfwCreateWindow(config.window.width, config.window.height, config.window.title.data(), monitor, nullptr), glfwDestroyWindow};
	if ( ! window) {
		const char *description;
		glfwGetError(&description);
		throw std::runtime_error{fmt::format("Failed to create the window (error: {})", description)};
	}

	auto current = config.window;
	current.monitor = index;
	std::unique_ptr<Description> description{new Description{current, current, false}};

	glfwSetWindowUserPointer(window.get(), description.get());
	glfwSetWindowSizeCallback(window.get(), onResize);

	return { std::move(window), std::move(description) };
}

State::~State() {
	if (window) {
		glfwSetWindowShouldClose(window.get(), true);
	}
}

State setup(ware::config::State &config, [[maybe_unused]] ware::contextGLFW::State &glfw) {
	auto [window, description] = createWindow(config);

	return State{
		.window = std::move(window),
		.description = std::move(description),
		.refreshTimePoint = glfwGetTime(),
	};
}

void refresh(State &state) {
	if ( ! state.window) {
		state.shouldClose = true;
		return;
	}

	auto *window = state.window.get();

	{
		double dt = (glfwGetTime() - state.refreshTimePoint) * 1000.0;
		state.description->next.title = fmt::format("{:.3f}ms {:.2f}fps", dt, dt > 0.0 ? 1000.0 / dt : 0.0);
		state.description->changed = true;
	}

	if (state.description->changed) {
		// update title
		if (state.description->current.title != state.description->next.title) {
			glfwSetWindowTitle(window, state.description->next.title.data());
		}

		// update size
		if (state.description->current.width != state.description->next.width || state.description->current.height != state.description->next.height) {
			glfwSetWindowSize(window, state.description->next.width, state.description->next.height);
		}

		// update mode
		if (state.description->current.mode != state.description->next.mode) {
			switch (state.description->next.mode) {
				case WindowMode::Borderless: {
					glfwSetWindowAttrib(window, GLFW_DECORATED, GL_FALSE);
					glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_FALSE);

					break;
				}
				case WindowMode::Windowed: {
					glfwSetWindowAttrib(window, GLFW_DECORATED, GL_TRUE);
					glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_TRUE);

					break;
				}
				case WindowMode::Fullscreen: {
					glfwSetWindowAttrib(window, GLFW_DECORATED, GL_FALSE);
					glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_FALSE);

					break;
				}
			}
		}

		// update monitor
		if (state.description->next.mode == WindowMode::Fullscreen && (state.description->current.mode != state.description->next.mode || state.description->current.monitor != state.description->next.monitor)) {
			auto [monitor, index] = selectMonitor(state.description->next.monitor);
			state.description->next.monitor = index;

			const GLFWvidmode *mode = glfwGetVideoMode(monitor);

			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		} else if (state.description->current.mode == WindowMode::Fullscreen && state.description->current.mode != state.description->next.mode) {
			glfwSetWindowMonitor(window, nullptr, 0, 0, state.description->next.width, state.description->next.height, GLFW_DONT_CARE);
		}
	}

	state.refreshTimePoint = glfwGetTime(),
	state.shouldClose = glfwWindowShouldClose(window);
}

void process(State &state) {
	// commit changes
	if (state.description->changed) {
		state.description->current = state.description->next;
		state.description->changed = false;
	}
}

} // ware::windowGLFW
