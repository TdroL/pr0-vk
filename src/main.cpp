#include "rn/window.hpp"
#include "rn/vlk/creators/contextCreator.hpp"
#include "rn/vlk/context.hpp"

#include "ngn/log.hpp"
#include "ngn/config.hpp"

#include "app/main/state.hpp"

int main() {
	try {
		if (ngn::config::core.dirty()) {
			if ( ! ngn::config::core.store()) {
				ngn::log::error("Failed to store core config");
			}
		}

		// rn::GLFW glfw{};
		rn::Window window{};
		window.create();

		auto contextStart = glfwGetTime();
		rn::vlk::Context context = rn::vlk::ContextCreator{window}.create();
		auto contextEnd = glfwGetTime();
		ngn::log::debug("context#create: {}ms", 1000.0 * (contextEnd - contextStart));

		// ngn::Inputs inputs{context};

		if (ngn::config::core.dirty()) {
			ngn::log::warn("Config is dirty after context creation");
		}

		app::main::State mainState{window, context};

		auto mainStateStart = glfwGetTime();
		mainState.init();
		auto mainStateEnd = glfwGetTime();
		ngn::log::debug("mainState#init: {}ms", 1000.0 * (mainStateEnd - mainStateStart));

		auto last = glfwGetTime();
		for (size_t i = 0; /*i < 6 &&*/ ! glfwWindowShouldClose(window.handle); i++) {
			auto loopStart = glfwGetTime();
			glfwPollEvents();

			if ( ! window.needsRefresh()) {
				window.refresh();
				mainState.refresh();
			}

			mainState.render();

			auto loopEnd = glfwGetTime();
			auto now = glfwGetTime();

			std::string title =
				std::to_string((loopEnd - loopStart) * 1000.0) +
				"ms / " +
				std::to_string((now - last) * 1000.0) +
				"ms | " +
				std::to_string(1.0 / (loopEnd - loopStart)) +
				"fps / " +
				std::to_string(1.0 / (now - last)) +
				"fps";

			glfwSetWindowTitle(window.handle, title.c_str());
			last = now;
		}

		mainState.deinit();

		context.owners.device.get().waitIdle();

		return EXIT_SUCCESS;
	} catch (std::runtime_error const &e) {
		ngn::log::critical("Runtime exception: {}", e.what());
	} catch (...) {
		ngn::log::critical("Unknown exception");
	}
}