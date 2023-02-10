#pragma once

#include <string>
#include <tuple>

#include <vulkan/vulkan.hpp>

namespace ware::config {

enum struct WindowMode : uint32_t {
	Borderless,
	Windowed,
	Fullscreen,
};

struct State {
	struct Window {
		int32_t width;
		int32_t height;
		int32_t monitor;
		std::string title;
		WindowMode mode;
	} window;

	struct VK {
		uint32_t enableValidation;
		uint32_t enableLogging;
		int32_t logLevel;
		std::string appName;
		uint32_t appVersion;
		std::string engineName;
		uint32_t engineVersion;
		int32_t vendorId;
		int32_t deviceId;
		std::tuple<int32_t, int32_t> presentationQueue;
		std::tuple<int32_t, int32_t> graphicQueue;
		std::tuple<int32_t, int32_t> computeQueue;
		std::tuple<int32_t, int32_t> transferQueue;
		vk::Format surfaceFormat;
		vk::ColorSpaceKHR surfaceColorSpace;
		vk::PresentModeKHR swapchainPresentMode;
		int32_t swapchainImageCount;
	} vk;
};

State setup();

void refresh(State &state);

void process(State &state);

} // ware::config
