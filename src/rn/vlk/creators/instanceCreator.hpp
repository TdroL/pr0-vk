#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "debugCallbackCreator.hpp"
#include "surfaceCreator.hpp"
#include "instancePlanner.hpp"

#include "../context.hpp"
#include "../../glfw.hpp"

#include "../../../ngn/config.hpp"

#include "../../../util/map.hpp"
#include "../../../util/implode.hpp"

namespace rn::vlk {

class InstanceCreator {
public:
	struct Version {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};

	vk::UniqueInstance create(Context &/*context*/, DebugCallbackCreator &debugCallback, const SurfaceCreator &surface) {
		if (glfwVulkanSupported() != GLFW_TRUE) {
			throw std::runtime_error{"Vulkan not supported"};
		}

		InstancePlannerResult result = InstancePlanner{}.selectExtensionsAndLayers(debugCallback, surface);
		if ( ! result.all) {
			throw std::runtime_error{"Missing required " + util::implode(result.missingList) + " vulkan " + result.missingType + "(s)"};
		}

		debugCallback.isAvailable = result.debugCallback;

		ngn::log::debug("Enabled {} vulkan extension(s): {}", result.extensions.size(), util::implode(result.extensions));
		ngn::log::debug("Enabled {} vulkan layer(s): {}", result.layers.size(), util::implode(result.layers));

		auto &appConfig = ngn::config::core.application;
		auto &engineConfig = ngn::config::core.engine;

		vk::ApplicationInfo applicationInfo{};
		applicationInfo.pApplicationName   = appConfig.name().data();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(appConfig.version.major(), appConfig.version.minor(), appConfig.version.patch());
		applicationInfo.pEngineName        = engineConfig.name().data();
		applicationInfo.engineVersion      = VK_MAKE_VERSION(engineConfig.version.major(), engineConfig.version.minor(), engineConfig.version.patch());
		applicationInfo.apiVersion         = VK_API_VERSION_1_0;

		const auto getStringData = [] (const std::string &value) -> const char * {
			return value.data();
		};

		// std::vector<const char *> requestedLayers(result.layers.size());
		// std::transform(std::begin(result.layers), std::end(result.layers), std::begin(requestedLayers), getStringData);
		std::vector<const char *> requestedLayers = util::map<const char *>(result.layers, getStringData);

		// std::vector<const char *> requestedExtensions(result.extensions.size());
		// std::transform(std::begin(result.extensions), std::end(result.extensions), std::begin(requestedExtensions), getStringData);
		std::vector<const char *> requestedExtensions = util::map<const char *>(result.extensions, getStringData);

		vk::InstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.pApplicationInfo        = &applicationInfo;
		instanceCreateInfo.enabledLayerCount       = static_cast<uint32_t>(requestedLayers.size());
		instanceCreateInfo.ppEnabledLayerNames     = requestedLayers.data();
		instanceCreateInfo.enabledExtensionCount   = static_cast<uint32_t>(requestedExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requestedExtensions.data();

		vk::UniqueInstance instanceOwner{vk::createInstanceUnique(instanceCreateInfo)};

		if ( ! instanceOwner) {
			throw std::runtime_error{"Vulkan instance could not be created"};
		}

		return instanceOwner;
	}
};

} // rn::vlk
