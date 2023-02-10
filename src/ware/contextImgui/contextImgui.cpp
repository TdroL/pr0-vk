#include "contextImgui.hpp"

#include <algorithm>
#include <limits>
// #include <tuple>

// #include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#if defined(_WIN32)
	#undef APIENTRY
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h> // for glfwGetWin32Window
#endif

#include "mapping.hpp"

namespace ware::contextImgui {

void setClipboardText([[maybe_unused]] void *userData, [[maybe_unused]] const char *text) {
	//
}

const char * getClipboardText([[maybe_unused]] void *userData) {
	return nullptr;
}

[[nodiscard]] std::unique_ptr<ImGuiContext, decltype(&ImGui::DestroyContext)> createContext(ware::windowGLFW::State &window) {
	IMGUI_CHECKVERSION();

	std::unique_ptr<ImGuiContext, decltype(&ImGui::DestroyContext)> context{ ImGui::CreateContext(), ImGui::DestroyContext };

	ImGui::StyleColorsDark();

	auto &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
	io.DisplayFramebufferScale = ImVec2{1.0f, 1.0f};

	io.BackendPlatformName = "ware::contextImgui";
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;

	io.SetClipboardTextFn = setClipboardText;
	io.GetClipboardTextFn = getClipboardText;
	io.ClipboardUserData = nullptr;

#if defined(_WIN32)
	auto *viewport = ImGui::GetMainViewport();
	viewport->PlatformHandleRaw = reinterpret_cast<void *>(glfwGetWin32Window(window.window.get()));
#endif

	return context;
}

std::vector<std::unique_ptr<GLFWcursor, decltype(&glfwDestroyCursor)>> createCursors([[maybe_unused]] ware::windowGLFW::State &window) {
	GLFWerrorfun prevErrorCallback = glfwSetErrorCallback(nullptr);

	std::vector<std::unique_ptr<GLFWcursor, decltype(&glfwDestroyCursor)>> cursors{};
	cursors.reserve(ImGuiMouseCursor_COUNT);
	std::generate_n(std::back_inserter(cursors), ImGuiMouseCursor_COUNT, [] () -> decltype(cursors)::value_type { return { nullptr, glfwDestroyCursor }; });

	cursors[ImGuiMouseCursor_Arrow].reset(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
	cursors[ImGuiMouseCursor_TextInput].reset(glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
	cursors[ImGuiMouseCursor_ResizeAll].reset(glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR));
	cursors[ImGuiMouseCursor_ResizeNS].reset(glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
	cursors[ImGuiMouseCursor_ResizeEW].reset(glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
	cursors[ImGuiMouseCursor_ResizeNESW].reset(glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR));
	cursors[ImGuiMouseCursor_ResizeNWSE].reset(glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR));
	cursors[ImGuiMouseCursor_Hand].reset(glfwCreateStandardCursor(GLFW_HAND_CURSOR));
	cursors[ImGuiMouseCursor_NotAllowed].reset(glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR));

	glfwSetErrorCallback(prevErrorCallback);

	{
		int error = GLFW_NO_ERROR;
		const char *description = nullptr;
		while ((error = glfwGetError(&description)) != GLFW_NO_ERROR) {
			spdlog::warn("ware::contextImgui::createContext() => unexpected GLFW error: {} ({:#010x})", description ? description : "<unknown error>", error);
		}
	}

	return cursors;
}

Handles registerCallbacks(ware::windowGLFW::State &window) {
	auto onWindowFocusHandle = ware::windowGLFW::registerOnWindowFocus(window, [] ([[maybe_unused]] GLFWwindow *window, int focused) {
		auto &io = ImGui::GetIO();
		io.AddFocusEvent(focused != 0);
	});

	auto onCursorEnterHandle = ware::windowGLFW::registerOnCursorEnter(window, [] ([[maybe_unused]] GLFWwindow *window, [[maybe_unused]] int entered) {
		// ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();

		// if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		// 	return;
		// }

		// auto &io = ImGui::GetIO();

		// if (entered) {
		// 	// bd->MouseWindow = window;
		// 	io.AddMousePosEvent(bd->LastValidMousePos.x, bd->LastValidMousePos.y);
		// } else if (!entered) {
		// 	bd->LastValidMousePos = io.MousePos;
		// 	bd->MouseWindow = nullptr;
		// 	io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
		// }
	});

	auto onCursorPosHandle = ware::windowGLFW::registerOnCursorPos(window, [] ([[maybe_unused]] GLFWwindow *window, double xpos, double ypos) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			return;
		}

		auto &io = ImGui::GetIO();
		io.AddMousePosEvent(static_cast<float>(xpos), static_cast<float>(ypos));
		// bd->LastValidMousePos = ImVec2((float)x, (float)y);
	});

	auto onMouseButtonHandle = ware::windowGLFW::registerOnMouseButton(window, [] (GLFWwindow *window, int button, int action, [[maybe_unused]] int mods) {
		auto &io = ImGui::GetIO();
		std::array modifierKeys{
			std::tuple{ImGuiMod_Ctrl, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL},
			std::tuple{ImGuiMod_Shift, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT},
			std::tuple{ImGuiMod_Alt, GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT},
			std::tuple{ImGuiMod_Super, GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SUPER},
		};
		for (auto [imguiKey, glfwLeftKey, glfwRightKey] : modifierKeys) {
			io.AddKeyEvent(imguiKey, (glfwGetKey(window, glfwLeftKey) == GLFW_PRESS) || (glfwGetKey(window, glfwRightKey) == GLFW_PRESS));
		}

		if (button >= 0 && button < ImGuiMouseButton_COUNT) {
			io.AddMouseButtonEvent(button, action == GLFW_PRESS);
		}
	});

	auto onScrollHandle = ware::windowGLFW::registerOnScroll(window, [] ([[maybe_unused]] GLFWwindow *window, double xoffset, double yoffset) {
		auto &io = ImGui::GetIO();
		io.AddMouseWheelEvent(static_cast<float>(xoffset), static_cast<float>(yoffset));
	});

	auto onKeyHandle = ware::windowGLFW::registerOnKey(window, [] ([[maybe_unused]] GLFWwindow *window, int key, [[maybe_unused]] int scanCode, int action, [[maybe_unused]] int mods) {
		if (action != GLFW_PRESS && action != GLFW_RELEASE) {
			return;
		}

		auto &io = ImGui::GetIO();
		std::array modifierKeys{
			std::tuple{ImGuiMod_Ctrl, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL},
			std::tuple{ImGuiMod_Shift, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT},
			std::tuple{ImGuiMod_Alt, GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT},
			std::tuple{ImGuiMod_Super, GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SUPER},
		};
		for (auto [imguiKey, glfwLeftKey, glfwRightKey] : modifierKeys) {
			io.AddKeyEvent(imguiKey, (glfwGetKey(window, glfwLeftKey) == GLFW_PRESS) || (glfwGetKey(window, glfwRightKey) == GLFW_PRESS));
		}

		auto imguiKey = mapGLFWKeyToImguiKey(key);
		io.AddKeyEvent(imguiKey, action == GLFW_PRESS);
	});

	auto onCharHandle = ware::windowGLFW::registerOnChar(window, [] ([[maybe_unused]] GLFWwindow *window, unsigned int codePoint) {
		auto &io = ImGui::GetIO();
    io.AddInputCharacter(codePoint);
	});

	return {
		.onWindowFocusHandle = std::move(onWindowFocusHandle),
		.onCursorEnterHandle = std::move(onCursorEnterHandle),
		.onCursorPosHandle = std::move(onCursorPosHandle),
		.onMouseButtonHandle = std::move(onMouseButtonHandle),
		.onScrollHandle = std::move(onScrollHandle),
		.onKeyHandle = std::move(onKeyHandle),
		.onCharHandle = std::move(onCharHandle),
	};
}

State::~State() {
	// assumes only one instance of ware::contextImgui::State exists at the same time
	glfwSetCursor(window.window.get(), nullptr);
	glfwSetInputMode(window.window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

State setup([[maybe_unused]] ware::contextGLFW::State &glfw, ware::windowGLFW::State &window) {
	auto context = createContext(window);

	auto cursors = createCursors(window);

	auto handles = registerCallbacks(window);

	return State{
		.window = window,
		.context = std::move(context),
		.cursors = std::move(cursors),
		.handles = std::move(handles),
		.time = glfwGetTime(),
	};
}

void refresh(State &state) {
	ZoneScopedN("ware::contextImgui::refresh()");

	const double time = glfwGetTime();

	auto &io = ImGui::GetIO();
	io.DeltaTime = static_cast<float>(time - state.time);
	io.DisplaySize = ImVec2{static_cast<float>(state.window.description->width), static_cast<float>(state.window.description->height)};

	// update mouse position
	if (glfwGetInputMode(state.window.window.get(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		io.AddMousePosEvent(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	} else if (glfwGetWindowAttrib(state.window.window.get(), GLFW_FOCUSED) != 0) {
		if (io.WantSetMousePos) {
			glfwSetCursorPos(state.window.window.get(), static_cast<double>(io.MousePos.x), static_cast<double>(io.MousePos.y));
		} else {
			double xpos, ypos;
			glfwGetCursorPos(state.window.window.get(), &xpos, &ypos);

			io.AddMousePosEvent(static_cast<float>(xpos), static_cast<float>(ypos));
		}
	}

	// update mouse cursor
	auto cursor = ImGui::GetMouseCursor();
	if (cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
		glfwSetInputMode(state.window.window.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	} else {
		glfwSetCursor(state.window.window.get(), state.cursors[cursor] ? state.cursors[cursor].get() : state.cursors[ImGuiMouseCursor_Arrow].get());
		glfwSetInputMode(state.window.window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	state.time = time;
}

void process([[maybe_unused]] State &state) {
	ZoneScopedN("ware::contextImgui::process()");
}

} // ware::contextImgui
