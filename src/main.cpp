// #include <algorithm>
// #include <chrono>
// #include <cassert>
// #include <cstdlib>
// #include <cstring>
#include <iostream>
// #include <memory>
// #include <set>
// #include <string>
// #include <vector>
// #include <functional>
// #include <stdexcept>

// #include "rn/glfw.hpp"
#include "rn/window.hpp"
#include "rn/vlk/creators/contextCreator.hpp"
#include "rn/vlk/context.hpp"

#include "ngn/log.hpp"
#include "ngn/config.hpp"

#include "app/mainState.hpp"

#include <chrono>
#include <thread>

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

		rn::vlk::Context context = rn::vlk::ContextCreator{window}.create();
		// ngn::Inputs inputs{context};

		if (ngn::config::core.dirty()) {
			ngn::log::warn("Config is dirty after context creation");
		}

		app::MainState mainState{window, context};

		mainState.init();

		for (int i = 0; i < 4 && ! glfwWindowShouldClose(window.handle); i++) {
			glfwPollEvents();

			if ( ! window.needsRefresh()) {
				window.refresh();
				mainState.refresh();
			}

			mainState.render();

			// std::this_thread::sleep_for(std::chrono::milliseconds(16));
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}

		context.device.waitIdle();

		return EXIT_SUCCESS;
	} catch (std::runtime_error const &e) {
		std::cerr << "Runtime exception: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
}