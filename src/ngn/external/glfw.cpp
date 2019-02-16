#include "glfw.hpp"

#include "../../util/scope.hpp"
#include "../log.hpp"

// #include <mutex>

namespace ngn::external {

// class GLFWInstance {
// public:
// 	bool owner;

// 	explicit GLFWInstance(bool owner = false) noexcept :
// 		owner{std::move(owner)}
// 	{}

// 	GLFWInstance(const GLFWInstance &other) = delete;
// 	GLFWInstance(GLFWInstance &&other) noexcept :
// 		owner{std::move(other.owner)}
// 	{
// 		other.owner = false;
// 	}

// 	GLFWInstance & operator=(const GLFWInstance &other) = delete;
// 	GLFWInstance & operator=(GLFWInstance &&other) {
// 		owner = std::move(other.owner);
// 		other.owner = false;

// 		return *this;
// 	}

// 	~GLFWInstance() {
// 		if (owner) {
// 			glfwTerminate();
// 		}
// 	}
// };

void errorCallback(int code, const char *message) {
	ngn::log::error("ngn::external::GLFWInstance[errorCallback] => error: ({}) {}", code, message);
}

// GLFWInstance create() {
// 	glfwSetErrorCallback(errorCallback);

// 	if (glfwInit() != GLFW_TRUE) {
// 		throw std::runtime_error{"Failed to initialize GLFW"};
// 	}

// 	return GLFWInstance{true};
// }

void GLFW::initialize() {
	// static GLFWInstance glfwInstance{create()};
	static auto instance = util::createScopeExit(
		[] () {
			ngn::log::debug("ngn::external::GLFW::initialize() => initialize GLFW");

			glfwSetErrorCallback(errorCallback);

			if (glfwInit() != GLFW_TRUE) {
				throw std::runtime_error{"Failed to initialize GLFW"};
			}
		},
		[] () {
			ngn::log::debug("ngn::external::GLFW::initialize() => destroy GLFW");

			glfwTerminate();
		}
	);

	// static std::once_flag createGLFWFlag;

	// std::call_once(createGLFWFlag, [&] () {
	// 	glfwSetErrorCallback(errorCallback);

	// 	if (glfwInit() != GLFW_TRUE) {
	// 		throw std::runtime_error{"Failed to initialize GLFW"};
	// 	}

	// 	glfwInstance = GLFWInstance{true};
	// });
}

} // ngn::external
