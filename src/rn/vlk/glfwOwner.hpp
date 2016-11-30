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
	}

	GLFWOwner(GLFWOwner &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = GLFW{};
	}

	GLFWOwner & operator=(GLFWOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		rhs.handle = GLFW{};

		return *this;
	}

	void destroy() {
		if (handle) {
			handle.terminate();
			handle = GLFW{};
		}
	}

	GLFWOwner(const GLFWOwner &rhs) = delete;
	GLFWOwner & operator=(const GLFWOwner &rhs) = delete;

	~GLFWOwner() {
		if (handle) {
			handle.terminate();
		}
	}
};

} // vlk

} // rn
