#include "surfaceCreator.hpp"

#include <cassert>
#include <stdexcept>

namespace rn::vlk {

SurfaceCreator::SurfaceCreator() {
	uint32_t count;
	const char **extensions = glfwGetRequiredInstanceExtensions(&count);

	requiredExtensions = std::vector<std::string>(extensions, extensions + count);
}

vk::UniqueSurfaceKHR SurfaceCreator::create(Context &context, rn::Window &window) {
	vk::Instance instance = context.instance;

	assert(instance);

	vk::UniqueSurfaceKHR surfaceOwner{window.createSurface(instance), { instance }};

	if ( ! surfaceOwner) {
		throw std::runtime_error{"Vulkan surface could not be created"};
	}

	return surfaceOwner;
}

} // rn::vlk
