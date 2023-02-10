#include "mapping.hpp"

#include <array>

#include <GLFW/glfw3.h>

std::array<ImGuiKey, GLFW_KEY_LAST> createMap() {
	std::array<ImGuiKey, GLFW_KEY_LAST> map{ImGuiKey_None};

	map[GLFW_KEY_SPACE] = ImGuiKey_Space;
	map[GLFW_KEY_APOSTROPHE] = ImGuiKey_Apostrophe;
	map[GLFW_KEY_COMMA] = ImGuiKey_Comma;
	map[GLFW_KEY_MINUS] = ImGuiKey_Minus;
	map[GLFW_KEY_PERIOD] = ImGuiKey_Period;
	map[GLFW_KEY_SLASH] = ImGuiKey_Slash;
	map[GLFW_KEY_0] = ImGuiKey_0;
	map[GLFW_KEY_1] = ImGuiKey_1;
	map[GLFW_KEY_2] = ImGuiKey_2;
	map[GLFW_KEY_3] = ImGuiKey_3;
	map[GLFW_KEY_4] = ImGuiKey_4;
	map[GLFW_KEY_5] = ImGuiKey_5;
	map[GLFW_KEY_6] = ImGuiKey_6;
	map[GLFW_KEY_7] = ImGuiKey_7;
	map[GLFW_KEY_8] = ImGuiKey_8;
	map[GLFW_KEY_9] = ImGuiKey_9;
	map[GLFW_KEY_SEMICOLON] = ImGuiKey_Semicolon;
	map[GLFW_KEY_EQUAL] = ImGuiKey_Equal;
	map[GLFW_KEY_A] = ImGuiKey_A;
	map[GLFW_KEY_B] = ImGuiKey_B;
	map[GLFW_KEY_C] = ImGuiKey_C;
	map[GLFW_KEY_D] = ImGuiKey_D;
	map[GLFW_KEY_E] = ImGuiKey_E;
	map[GLFW_KEY_F] = ImGuiKey_F;
	map[GLFW_KEY_G] = ImGuiKey_G;
	map[GLFW_KEY_H] = ImGuiKey_H;
	map[GLFW_KEY_I] = ImGuiKey_I;
	map[GLFW_KEY_J] = ImGuiKey_J;
	map[GLFW_KEY_K] = ImGuiKey_K;
	map[GLFW_KEY_L] = ImGuiKey_L;
	map[GLFW_KEY_M] = ImGuiKey_M;
	map[GLFW_KEY_N] = ImGuiKey_N;
	map[GLFW_KEY_O] = ImGuiKey_O;
	map[GLFW_KEY_P] = ImGuiKey_P;
	map[GLFW_KEY_Q] = ImGuiKey_Q;
	map[GLFW_KEY_R] = ImGuiKey_R;
	map[GLFW_KEY_S] = ImGuiKey_S;
	map[GLFW_KEY_T] = ImGuiKey_T;
	map[GLFW_KEY_U] = ImGuiKey_U;
	map[GLFW_KEY_V] = ImGuiKey_V;
	map[GLFW_KEY_W] = ImGuiKey_W;
	map[GLFW_KEY_X] = ImGuiKey_X;
	map[GLFW_KEY_Y] = ImGuiKey_Y;
	map[GLFW_KEY_Z] = ImGuiKey_Z;
	map[GLFW_KEY_LEFT_BRACKET] = ImGuiKey_LeftBracket;
	map[GLFW_KEY_BACKSLASH] = ImGuiKey_Backslash;
	map[GLFW_KEY_RIGHT_BRACKET] = ImGuiKey_RightBracket;
	map[GLFW_KEY_GRAVE_ACCENT] = ImGuiKey_GraveAccent;
	map[GLFW_KEY_ESCAPE] = ImGuiKey_Escape;
	map[GLFW_KEY_ENTER] = ImGuiKey_Enter;
	map[GLFW_KEY_TAB] = ImGuiKey_Tab;
	map[GLFW_KEY_BACKSPACE] = ImGuiKey_Backspace;
	map[GLFW_KEY_INSERT] = ImGuiKey_Insert;
	map[GLFW_KEY_DELETE] = ImGuiKey_Delete;
	map[GLFW_KEY_RIGHT] = ImGuiKey_RightArrow;
	map[GLFW_KEY_LEFT] = ImGuiKey_LeftArrow;
	map[GLFW_KEY_DOWN] = ImGuiKey_DownArrow;
	map[GLFW_KEY_UP] = ImGuiKey_UpArrow;
	map[GLFW_KEY_PAGE_UP] = ImGuiKey_PageUp;
	map[GLFW_KEY_PAGE_DOWN] = ImGuiKey_PageDown;
	map[GLFW_KEY_HOME] = ImGuiKey_Home;
	map[GLFW_KEY_END] = ImGuiKey_End;
	map[GLFW_KEY_CAPS_LOCK] = ImGuiKey_CapsLock;
	map[GLFW_KEY_SCROLL_LOCK] = ImGuiKey_ScrollLock;
	map[GLFW_KEY_NUM_LOCK] = ImGuiKey_NumLock;
	map[GLFW_KEY_PRINT_SCREEN] = ImGuiKey_PrintScreen;
	map[GLFW_KEY_PAUSE] = ImGuiKey_Pause;
	map[GLFW_KEY_F1] = ImGuiKey_F1;
	map[GLFW_KEY_F2] = ImGuiKey_F2;
	map[GLFW_KEY_F3] = ImGuiKey_F3;
	map[GLFW_KEY_F4] = ImGuiKey_F4;
	map[GLFW_KEY_F5] = ImGuiKey_F5;
	map[GLFW_KEY_F6] = ImGuiKey_F6;
	map[GLFW_KEY_F7] = ImGuiKey_F7;
	map[GLFW_KEY_F8] = ImGuiKey_F8;
	map[GLFW_KEY_F9] = ImGuiKey_F9;
	map[GLFW_KEY_F10] = ImGuiKey_F10;
	map[GLFW_KEY_F11] = ImGuiKey_F11;
	map[GLFW_KEY_F12] = ImGuiKey_F12;
	map[GLFW_KEY_KP_0] = ImGuiKey_Keypad0;
	map[GLFW_KEY_KP_1] = ImGuiKey_Keypad1;
	map[GLFW_KEY_KP_2] = ImGuiKey_Keypad2;
	map[GLFW_KEY_KP_3] = ImGuiKey_Keypad3;
	map[GLFW_KEY_KP_4] = ImGuiKey_Keypad4;
	map[GLFW_KEY_KP_5] = ImGuiKey_Keypad5;
	map[GLFW_KEY_KP_6] = ImGuiKey_Keypad6;
	map[GLFW_KEY_KP_7] = ImGuiKey_Keypad7;
	map[GLFW_KEY_KP_8] = ImGuiKey_Keypad8;
	map[GLFW_KEY_KP_9] = ImGuiKey_Keypad9;
	map[GLFW_KEY_KP_DECIMAL] = ImGuiKey_KeypadDecimal;
	map[GLFW_KEY_KP_DIVIDE] = ImGuiKey_KeypadDivide;
	map[GLFW_KEY_KP_MULTIPLY] = ImGuiKey_KeypadMultiply;
	map[GLFW_KEY_KP_SUBTRACT] = ImGuiKey_KeypadSubtract;
	map[GLFW_KEY_KP_ADD] = ImGuiKey_KeypadAdd;
	map[GLFW_KEY_KP_ENTER] = ImGuiKey_KeypadEnter;
	map[GLFW_KEY_KP_EQUAL] = ImGuiKey_KeypadEqual;
	map[GLFW_KEY_LEFT_SHIFT] = ImGuiKey_LeftShift;
	map[GLFW_KEY_LEFT_CONTROL] = ImGuiKey_LeftCtrl;
	map[GLFW_KEY_LEFT_ALT] = ImGuiKey_LeftAlt;
	map[GLFW_KEY_LEFT_SUPER] = ImGuiKey_LeftSuper;
	map[GLFW_KEY_RIGHT_SHIFT] = ImGuiKey_RightShift;
	map[GLFW_KEY_RIGHT_CONTROL] = ImGuiKey_RightCtrl;
	map[GLFW_KEY_RIGHT_ALT] = ImGuiKey_RightAlt;
	map[GLFW_KEY_RIGHT_SUPER] = ImGuiKey_RightSuper;
	map[GLFW_KEY_MENU] = ImGuiKey_Menu;

	return map;
}

ImGuiKey mapGLFWKeyToImguiKey(int key) {
	static auto map = createMap();

	if (key < 0 || key >= static_cast<int>(map.size())) {
		return ImGuiKey_None;
	}

	return map[key];
}
