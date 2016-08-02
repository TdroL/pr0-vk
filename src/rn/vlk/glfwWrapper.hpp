#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "glfw.hpp"

namespace rn {

namespace vlk {

class GLFWWrapper {
public:
	GLFW handle{};

	GLFWWrapper() = default;
	explicit GLFWWrapper(GLFW &&glfw)
		: handle{std::move(glfw)} {
		glfw = GLFW{};
	}

	GLFWWrapper(GLFWWrapper &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = GLFW{};
	}

	GLFWWrapper & operator=(GLFWWrapper &&rhs) {
		handle = std::move(rhs.handle);
		rhs.handle = GLFW{};

		return *this;
	}

	GLFWWrapper(const GLFWWrapper &rhs) = delete;
	GLFWWrapper & operator=(const GLFWWrapper &rhs) = delete;

	~GLFWWrapper() {
		if (handle) {
			std::cout << "destroying GLFW" << std::endl;
			handle.terminate();
		}
	}
};

} // vlk

} // rn
