#include "windowGLFW.hpp"

#include <tuple>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

namespace ware::windowGLFW {

using WindowMode = ware::config::WindowMode;

void onResize(GLFWwindow *window, int width, int height) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onResize() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onResize) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, width, height);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onResize() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onResize() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onResize() => unknown callback failure");
		}
	}
}

void onWindowFocus(GLFWwindow *window, int focused) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onWindowFocus() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onWindowFocus) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, focused);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onWindowFocus() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onWindowFocus() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onWindowFocus() => unknown callback failure");
		}
	}
}

void onCursorEnter(GLFWwindow *window, int entered) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onCursorEnter() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onCursorEnter) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, entered);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onCursorEnter() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onCursorEnter() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onCursorEnter() => unknown callback failure");
		}
	}
}

void onCursorPos(GLFWwindow *window, double xpos, double ypos) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onCursorPos() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onCursorPos) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, xpos, ypos);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onCursorPos() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onCursorPos() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onCursorPos() => unknown callback failure");
		}
	}
}

void onMouseButton(GLFWwindow *window, int button, int action, int mods) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onMouseButton() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onMouseButton) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, button, action, mods);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onMouseButton() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onMouseButton() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onMouseButton() => unknown callback failure");
		}
	}
}

void onScroll(GLFWwindow *window, double xoffset, double yoffset) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onScroll() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onScroll) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, xoffset, yoffset);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onScroll() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onScroll() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onScroll() => unknown callback failure");
		}
	}
}

void onKey(GLFWwindow *window, int key, int scanCode, int action, int mods) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onKey() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onKey) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, key, scanCode, action, mods);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onKey() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onKey() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onKey() => unknown callback failure");
		}
	}
}

void onChar(GLFWwindow *window, unsigned int codePoint) {
	auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
	if ( ! callbacks) {
		spdlog::error("ware::windowGLFW::onChar() => window callbacks missing");
		return;
	}

	for (auto &callback : callbacks->onChar) {
		if ( ! callback) {
			continue;
		}

		try {
			callback(window, codePoint);
		}
		catch (std::system_error const &e) {
			spdlog::critical("ware::windowGLFW::onChar() => callback system exception: #{} {}", e.code().value(), e.what());
		}
		catch (std::runtime_error const &e) {
			spdlog::critical("ware::windowGLFW::onChar() => callback runtime exception: {}", e.what());
		}
		catch (...) {
			spdlog::warn("ware::windowGLFW::onChar() => unknown callback failure");
		}
	}
}

void unregisterOnResize(CallbackHandle::Type handle) {
	handle.first->onResize[handle.second] = nullptr;
}

void unregisterOnWindowFocus(CallbackHandle::Type handle) {
	handle.first->onWindowFocus[handle.second] = nullptr;
}

void unregisterOnCursorEnter(CallbackHandle::Type handle) {
	handle.first->onCursorEnter[handle.second] = nullptr;
}

void unregisterOnCursorPos(CallbackHandle::Type handle) {
	handle.first->onCursorPos[handle.second] = nullptr;
}

void unregisterOnMouseButton(CallbackHandle::Type handle) {
	handle.first->onMouseButton[handle.second] = nullptr;
}

void unregisterOnScroll(CallbackHandle::Type handle) {
	handle.first->onScroll[handle.second] = nullptr;
}

void unregisterOnKey(CallbackHandle::Type handle) {
	handle.first->onKey[handle.second] = nullptr;
}

void unregisterOnChar(CallbackHandle::Type handle) {
	handle.first->onChar[handle.second] = nullptr;
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

[[nodiscard]] std::tuple<std::unique_ptr<Callbacks>, std::unique_ptr<Description>, std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>> createWindow(const ware::config::State &config) {
	std::unique_ptr<Callbacks> callbacks{new Callbacks{}};

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

	std::unique_ptr<Description> description{new Description{
		.width = config.window.width,
		.height = config.window.height,
		.monitor = index,
		.title = config.window.title,
		.mode = config.window.mode,
		.changed = false,
	}};

	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window{glfwCreateWindow(config.window.width, config.window.height, config.window.title.data(), monitor, nullptr), glfwDestroyWindow};
	if ( ! window) {
		const char *description;
		glfwGetError(&description);
		throw std::runtime_error{fmt::format("Failed to create the window (error: {})", description)};
	}

	glfwSetWindowUserPointer(window.get(), callbacks.get());
	glfwSetWindowSizeCallback(window.get(), onResize);
	glfwSetWindowFocusCallback(window.get(), onWindowFocus);
	glfwSetCursorEnterCallback(window.get(), onCursorEnter);
	glfwSetCursorPosCallback(window.get(), onCursorPos);
	glfwSetMouseButtonCallback(window.get(), onMouseButton);
	glfwSetScrollCallback(window.get(), onScroll);
	glfwSetKeyCallback(window.get(), onKey);
	glfwSetCharCallback(window.get(), onChar);

	callbacks->onResize.emplace_back([description = description.get()] ([[maybe_unused]] GLFWwindow *window, int width, int height) {
		if (width == 0 || height == 0) {
			return;
		}

		spdlog::info("ware::windowGLFW::onResize() => window resize: {} {}", width, height);

		description->width = width;
		description->height = height;
		description->changed = true;
	});

	return { std::move(callbacks), std::move(description), std::move(window) };
}

CallbackHandle registerOnResize(State &state, std::move_only_function<void (GLFWwindow *window, int width, int height)> &&callback) {
	uint64_t index = state.callbacks->onResize.size();

	state.callbacks->onResize.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnResize };
}

CallbackHandle registerOnWindowFocus(State &state, std::move_only_function<void (GLFWwindow *window, int focused)> &&callback) {
	uint64_t index = state.callbacks->onWindowFocus.size();

	state.callbacks->onWindowFocus.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnWindowFocus };
}

CallbackHandle registerOnCursorEnter(State &state, std::move_only_function<void (GLFWwindow *window, int entered)> &&callback) {
	uint64_t index = state.callbacks->onCursorEnter.size();

	state.callbacks->onCursorEnter.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnCursorEnter };
}

CallbackHandle registerOnCursorPos(State &state, std::move_only_function<void (GLFWwindow *window, double xpos, double ypos)> &&callback) {
	uint64_t index = state.callbacks->onCursorPos.size();

	state.callbacks->onCursorPos.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnCursorPos };
}

CallbackHandle registerOnMouseButton(State &state, std::move_only_function<void (GLFWwindow *window, int button, int action, int mods)> &&callback) {
	uint64_t index = state.callbacks->onMouseButton.size();

	state.callbacks->onMouseButton.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnMouseButton };
}

CallbackHandle registerOnScroll(State &state, std::move_only_function<void (GLFWwindow *window, double xoffset, double yoffset)> &&callback) {
	uint64_t index = state.callbacks->onScroll.size();

	state.callbacks->onScroll.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnScroll };
}

CallbackHandle registerOnKey(State &state, std::move_only_function<void (GLFWwindow *window, int key, int scanCode, int action, int mods)> &&callback) {
	uint64_t index = state.callbacks->onKey.size();

	state.callbacks->onKey.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnKey };
}

CallbackHandle registerOnChar(State &state, std::move_only_function<void (GLFWwindow *window, unsigned int codePoint)> &&callback) {
	uint64_t index = state.callbacks->onChar.size();

	state.callbacks->onChar.emplace_back(std::move(callback));

	return { { state.callbacks.get(), index }, unregisterOnChar };
}

vk::UniqueSurfaceKHR createVulkanSurface(State &state, vk::Instance &instance) {
	if (state.window.get() == nullptr) {
		throw std::runtime_error{"Vulkan surface could not be created"};
	}

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	const auto result = static_cast<vk::Result>(glfwCreateWindowSurface(static_cast<VkInstance>(instance), state.window.get(), nullptr, &surface));
	if (result != vk::Result::eSuccess) {
		throw std::runtime_error{fmt::format("Vulkan surface could not be created (result: {})", vk::to_string(result))};
	}

	return vk::UniqueSurfaceKHR{surface, { instance }};
}

State::~State() {
	if (window) {
		glfwSetWindowShouldClose(window.get(), true);
	}
}

State setup(ware::config::State &config, [[maybe_unused]] ware::contextGLFW::State &glfw) {
	auto [callbacks, description, window] = createWindow(config);

	auto previousDescription = *description;

	return State{
		.callbacks = std::move(callbacks),
		.description = std::move(description),
		.previousDescription = std::move(previousDescription),
		.window = std::move(window),
		.refreshTimePoint = glfwGetTime(),
	};
}

void refresh(State &state) {
	ZoneScopedN("ware::windowGLFW::refresh()");

	if ( ! state.window) {
		state.shouldClose = true;
		return;
	}

	auto *window = state.window.get();

	{
		double dt = (glfwGetTime() - state.refreshTimePoint) * 1000.0;
		state.description->title = fmt::format("{:.3f}ms {:.2f}fps", dt, dt > 0.0 ? 1000.0 / dt : 0.0);
		state.description->changed = true;
	}

	if (state.description->changed) {
		// update title
		if (state.previousDescription.title != state.description->title) {
			glfwSetWindowTitle(window, state.description->title.data());
		}

		// update size
		if (state.previousDescription.width != state.description->width || state.previousDescription.height != state.description->height) {
			glfwSetWindowSize(window, state.description->width, state.description->height);
		}

		// update mode
		if (state.previousDescription.mode != state.description->mode) {
			switch (state.description->mode) {
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
		if (state.description->mode == WindowMode::Fullscreen && (state.previousDescription.mode != state.description->mode || state.previousDescription.monitor != state.description->monitor)) {
			auto [monitor, index] = selectMonitor(state.description->monitor);
			state.description->monitor = index;

			const GLFWvidmode *mode = glfwGetVideoMode(monitor);

			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		} else if (state.previousDescription.mode == WindowMode::Fullscreen && state.previousDescription.mode != state.description->mode) {
			glfwSetWindowMonitor(window, nullptr, 0, 0, state.description->width, state.description->height, GLFW_DONT_CARE);
		}
	}

	state.refreshTimePoint = glfwGetTime(),
	state.shouldClose = glfwWindowShouldClose(window);
}

void process(State &state) {
	ZoneScopedN("ware::windowGLFW::process()");

	// commit changes
	if (state.description->changed) {
		state.previousDescription = *state.description;
		state.description->changed = false;
	}
}

} // ware::windowGLFW
