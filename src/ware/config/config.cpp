#include "config.hpp"

#include <tracy/Tracy.hpp>

namespace ware::config {

State setup() {
	return State{
		.window = {
			.width = 1600,
			.height = 900,
			.monitor = -1,
			.title = "pr0-vk",
			.mode = WindowMode::Windowed,
		},
		.vk = {
			.enableValidation = true,
			.enableLogging = true,
			.logLevel = 1,
			.appName = "pr0-vk",
			.appVersion = 1,
			.engineName = "pr0-vk",
			.engineVersion = 1,
			.vendorId = -1,
			.deviceId = -1,
			.presentationQueue = { -1, -1 },
			.graphicQueue = { -1, -1 },
			.computeQueue = { -1, -1 },
			.transferQueue = { -1, -1 },
			.surfaceFormat = vk::Format::eUndefined,
			.surfaceColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear,
			.swapchainPresentMode = vk::PresentModeKHR::eImmediate,
			.swapchainImageCount = -1,
		},
	};
}

void refresh([[maybe_unused]] State &state) {
	ZoneScopedN("ware::config::refresh()");
}

void process([[maybe_unused]] State &state) {
	ZoneScopedN("ware::config::process()");
}

} // ware::config
