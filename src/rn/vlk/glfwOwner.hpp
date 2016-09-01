#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "glfw.hpp"

namespace rn {

namespace vlk {

class GLFWOwner {
public:
	GLFW handle{};

	GLFWOwner() = default;
	explicit GLFWOwner(GLFW &&glfw)
		: handle{std::move(glfw)} {
		glfw = GLFW{};
		if (handle) {
			std::cout << "creating GLFW" << std::endl;
		}
	}

	GLFWOwner(GLFWOwner &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = GLFW{};
		if (handle) {
			std::cout << "creating GLFW" << std::endl;
		}
	}

	GLFWOwner & operator=(GLFWOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		rhs.handle = GLFW{};

		return *this;
	}

	void destroy() {
		if (handle) {
			std::cout << "destroying GLFW" << std::endl;
			handle.terminate();
			handle = GLFW{};
		}
	}

	GLFWOwner(const GLFWOwner &rhs) = delete;
	GLFWOwner & operator=(const GLFWOwner &rhs) = delete;

	~GLFWOwner() {
		if (handle) {
			std::cout << "destroying GLFW" << std::endl;
			handle.terminate();
		}
	}
};

} // vlk

} // rn
