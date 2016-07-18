#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

// #include "Vulkan.hpp"

#include "rn/vlk/interface.hpp"
#include "rn/vlk/context.hpp"

int windowWidth = 1600;
int windowHeight = 900;

int vlk() {
	rn::vlk::Context context{};
	context.init();

	return EXIT_SUCCESS;
}

int main() {
	try {
		return vlk();
	} catch (std::runtime_error const &e) {
		std::cerr << "Runtime exception: " << e.what() << std::endl;
	}
	// return proto();
}