#pragma once

#include "../contextGLFW/contextGLFW.hpp"
#include "../windowGLFW/windowGLFW.hpp"

#include <memory>
#include <vector>

#include <imgui.h>

namespace ware::contextImgui {

struct Handles {
	ware::windowGLFW::CallbackHandle onWindowFocusHandle;
	ware::windowGLFW::CallbackHandle onCursorEnterHandle;
	ware::windowGLFW::CallbackHandle onCursorPosHandle;
	ware::windowGLFW::CallbackHandle onMouseButtonHandle;
	ware::windowGLFW::CallbackHandle onScrollHandle;
	ware::windowGLFW::CallbackHandle onKeyHandle;
	ware::windowGLFW::CallbackHandle onCharHandle;
};

struct State {
	ware::windowGLFW::State &window;
	std::unique_ptr<ImGuiContext, decltype(&ImGui::DestroyContext)> context;
	std::vector<std::unique_ptr<GLFWcursor, decltype(&glfwDestroyCursor)>> cursors;
	Handles handles;
	double time;

	~State();
};

State setup(ware::contextGLFW::State &glfw, ware::windowGLFW::State &window);

void refresh(State &state);

void process(State &state);

} // ware::contextImgui
