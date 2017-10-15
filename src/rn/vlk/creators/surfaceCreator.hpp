#pragma once

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"
#include "../../window.hpp"
#include "../../glfw.hpp"

namespace rn::vlk {

class SurfaceCreator {
public:
	std::vector<std::string> requiredExtensions{};

	SurfaceCreator() {
		uint32_t count;
		const char **extensions = glfwGetRequiredInstanceExtensions(&count);

		requiredExtensions = std::vector<std::string>(extensions, extensions + count);
	}

	vk::UniqueSurfaceKHR create(Context &context, rn::Window &window) {
		vk::Instance instance = context.instance;

		assert(instance);

		vk::UniqueSurfaceKHR surfaceOwner{window.createSurface(instance), {instance}};

		if ( ! surfaceOwner) {
			throw std::runtime_error{"Vulkan surface could not be created"};
		}

		return surfaceOwner;
	}
};

} // rn::vlk
