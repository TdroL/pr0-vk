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

#include <chrono>
#include <thread>

int main() {
	try {
		// rn::GLFW glfw{};
		rn::Window window{};
		window.create();

		rn::vlk::Context context = rn::vlk::ContextCreator{window}.create();
		// ngn::Inputs inputs{context};

		for (int i = 0; i < 120; i++) {
			glfwPollEvents();

			// if ( ! window.isValid()) {
			// 	window.refresh();
			// }

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		return EXIT_SUCCESS;
	} catch (std::runtime_error const &e) {
		std::cerr << "Runtime exception: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
}