#include "surfaceCreator.hpp"

#include <cassert>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/external/glfw.hpp"
#include "../../../ngn/external.hpp"
#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../../../util/contains.hpp"
#include "../../../util/join.hpp"
#include "../../window.hpp"
#include "../dispatch.hpp"
#include "../trace.hpp"

namespace rn::vki::context {

const auto initialize = ngn::external::initialize<ngn::external::GLFW>();

rn::vki::UniqueSurfaceKHR SurfaceCreator::create(rn::vki::HandleInstance &&instance, rn::Window &window) {
	if (window.handle == nullptr) {
		ngn::log::error("rn::vki::context::SurfaceCreator::create() => could not create surface without a window");

		throw std::runtime_error{"Vulkan surface could not be created"};
	}

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	vk::Result err = static_cast<vk::Result>(RN_VKI_TRACE(glfwCreateWindowSurface(instance.get(), reinterpret_cast<GLFWwindow *>(window.handle.get()), nullptr, &surface)));
	if (err != vk::Result::eSuccess) {
		ngn::log::error("rn::vki::context::SurfaceCreator::create() => failed to create window surface: {}", vk::to_string(err));

		throw std::runtime_error{"Vulkan surface could not be created"};
	}

	return {
		vk::UniqueHandle<vk::SurfaceKHR, vk::DispatchLoaderDynamic>{surface, { instance.get(), nullptr, instance.table() }},
		instance.table()
	};
}

std::vector<std::string> SurfaceCreator::selectLayers([[maybe_unused]] ngn::config::Config &config, [[maybe_unused]] std::vector<std::string> &availableLayers, std::vector<std::string> selectedLayers) {
	return selectedLayers;
}

std::vector<std::string> SurfaceCreator::selectExtensions([[maybe_unused]] ngn::config::Config &config, std::vector<std::string> &availableExtensions, std::vector<std::string> selectedExtensions) {
	uint32_t count;
	const char **extensions = glfwGetRequiredInstanceExtensions(&count);
	std::vector<std::string> requiredExtensions = std::vector<std::string>(extensions, extensions + count);

	for (auto &&extension : requiredExtensions) {
		if (util::contains(availableExtensions, extension)) {
			selectedExtensions.push_back(extension);
		} else {
			throw std::runtime_error{"Missing required extension \"" + extension + "\""};
		}
	}

	return selectedExtensions;
}

} // rn::vki::context
