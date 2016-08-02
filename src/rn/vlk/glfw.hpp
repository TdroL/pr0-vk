#pragma once

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

namespace rn {

namespace vlk {

class GLFW {
public:
	bool initialized = false;

	GLFW() = default;
	explicit GLFW(bool initialized)
		: initialized{initialized} {}

	GLFW(GLFW &&rhs)
		: initialized{rhs.initialized} {
		rhs.initialized = false;
	}

	GLFW & operator=(GLFW &&rhs) {
		initialized = rhs.initialized;
		rhs.initialized = false;

		return *this;
	}

	GLFW(const GLFW &rhs) = delete;
	GLFW & operator=(const GLFW &rhs) = delete;

	operator bool() const {
		return initialized;
	}

	operator !() const {
		return !initialized;
	}

	void terminate() {
		if (initialized) {
			glfwTerminate();
			initialized = false;
		}
	}

	std::vector<std::string> getRequiredInstanceExtensions() {
		if ( ! initialized) {
			throw std::runtime_error{"GLFW not initialized"};
		}

		uint32_t count;
		const char **extensions = glfwGetRequiredInstanceExtensions(&count);

		return std::vector<std::string>(extensions, extensions + count);
	}
};

} // vlk

} // rn
