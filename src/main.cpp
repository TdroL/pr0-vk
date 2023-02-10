#include <thread>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#include "ware/config/config.hpp"
#include "ware/contextGLFW/contextGLFW.hpp"
#include "ware/windowGLFW/windowGLFW.hpp"
#include "ware/contextVK/contextVK.hpp"
#include "ware/contextImgui/contextImgui.hpp"
#include "ware/swapchainVK/swapchainVK.hpp"
#include "ware/rendererVK/rendererVK.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	try {
		spdlog::info("started");

		tracy::Profiler::SendFrameMark("main");

		spdlog::set_level(spdlog::level::debug);

		auto config = ware::config::setup();
		auto glfw = ware::contextGLFW::setup();
		auto window = ware::windowGLFW::setup(config, glfw);
		auto context = ware::contextVK::setup(config, glfw, window);
		auto imgui = ware::contextImgui::setup(glfw, window);
		auto swapchain = ware::swapchainVK::setup(config, window, context);
		auto renderer = ware::rendererVK::setup(window, context, imgui, swapchain);

		for (size_t i = 0; /** / i < 1 /*/true/**/; i++) {
			ZoneScopedN("loop");

			ware::config::refresh(config);
			ware::contextGLFW::refresh(glfw);
			ware::windowGLFW::refresh(window);
			ware::contextVK::refresh(context);
			ware::contextImgui::refresh(imgui);
			ware::swapchainVK::refresh(swapchain);
			ware::rendererVK::refresh(renderer);

			ware::rendererVK::process(renderer);
			ware::swapchainVK::process(swapchain);
			ware::contextImgui::process(imgui);
			ware::contextVK::process(context);
			ware::windowGLFW::process(window);
			ware::contextGLFW::process(glfw);
			ware::config::process(config);

			if (window.shouldClose) {
				break;
			}

			FrameMark;
		}

		spdlog::info("exiting");

		return EXIT_SUCCESS;
	}
	catch (std::system_error const &e) {
		spdlog::critical("System exception: #{} {}", e.code().value(), e.what());

		return EXIT_FAILURE;
	}
	catch (std::runtime_error const &e) {
		spdlog::critical("Runtime exception: {}", e.what());

		return EXIT_FAILURE;
	}
	catch (...) {
		spdlog::critical("Unknown exception");
		return EXIT_FAILURE;
	}
}
