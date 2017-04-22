#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

// #include "../surfaceOwner.hpp"
// #include "../instanceOwner.hpp"

#include "../../window.hpp"
#include "../../glfw.hpp"

namespace rn {

namespace vlk {

class SurfaceCreator {
public:
	std::vector<std::string> requiredExtensions{};

	SurfaceCreator() {
		uint32_t count;
		const char **extensions = glfwGetRequiredInstanceExtensions(&count);

		requiredExtensions = std::vector<std::string>(extensions, extensions + count);
	}

	vk::UniqueSurfaceKHR create(rn::Window &window, vk::UniqueInstance &instanceOwner) {
		vk::Instance instance = instanceOwner.get();
		vk::UniqueSurfaceKHR surfaceOwner{window.createSurface(instance), {instance}};

		if ( ! surfaceOwner) {
			throw std::runtime_error{"Vulkan surface could not be created"};
		}

		return surfaceOwner;
	}
};

} // vlk

} // rn
