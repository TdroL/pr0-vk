#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <util/uniqueResource.hpp>

#include "../config/config.hpp"
#include "../contextGLFW/contextGLFW.hpp"

namespace ware::windowGLFW {

struct Callbacks {
	std::vector<std::move_only_function<void (GLFWwindow *window, int width, int height)>> onResize{};
	std::vector<std::move_only_function<void (GLFWwindow *window, int focused)>> onWindowFocus{};
	std::vector<std::move_only_function<void (GLFWwindow *window, int entered)>> onCursorEnter{};
	std::vector<std::move_only_function<void (GLFWwindow *window, double xpos, double ypos)>> onCursorPos{};
	std::vector<std::move_only_function<void (GLFWwindow *window, int button, int action, int mods)>> onMouseButton{};
	std::vector<std::move_only_function<void (GLFWwindow *window, double xoffset, double yoffset)>> onScroll{};
	std::vector<std::move_only_function<void (GLFWwindow *window, int key, int scanCode, int action, int mods)>> onKey{};
	std::vector<std::move_only_function<void (GLFWwindow *window, unsigned int codePoint)>> onChar{};
};

struct Description {
	int32_t width;
	int32_t height;
	int32_t monitor;
	std::string title;
	ware::config::WindowMode mode;
	bool changed;
};

struct State {
	std::unique_ptr<Callbacks> callbacks;
	std::unique_ptr<Description> description;
	Description previousDescription;
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;

	double refreshTimePoint;
	bool shouldClose = false;

	~State();
};

using CallbackHandle = util::UniqueResource<std::pair<Callbacks *, uint64_t>>;

CallbackHandle registerOnResize(State &state, std::move_only_function<void (GLFWwindow *window, int width, int height)> &&callback);
CallbackHandle registerOnWindowFocus(State &state, std::move_only_function<void (GLFWwindow *window, int focused)> &&callback);
CallbackHandle registerOnCursorEnter(State &state, std::move_only_function<void (GLFWwindow *window, int entered)> &&callback);
CallbackHandle registerOnCursorPos(State &state, std::move_only_function<void (GLFWwindow *window, double xpos, double ypos)> &&callback);
CallbackHandle registerOnMouseButton(State &state, std::move_only_function<void (GLFWwindow *window, int button, int action, int mods)> &&callback);
CallbackHandle registerOnScroll(State &state, std::move_only_function<void (GLFWwindow *window, double xoffset, double yoffset)> &&callback);
CallbackHandle registerOnKey(State &state, std::move_only_function<void (GLFWwindow *window, int key, int scanCode, int action, int mods)> &&callback);
CallbackHandle registerOnChar(State &state, std::move_only_function<void (GLFWwindow *window, unsigned int codePoint)> &&callback);

vk::UniqueSurfaceKHR createVulkanSurface(State &state, vk::Instance &instance);

State setup(ware::config::State &config, ware::contextGLFW::State &glfw);

void refresh(State &state);

void process(State &state);

} // ware::windowGLFW
