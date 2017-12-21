#include "instanceCreator.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "instancePlanner.hpp"

#include "../../../ngn/config.hpp"
#include "../../../util/implode.hpp"
#include "../../../util/map.hpp"
#include "../../glfw.hpp"
#include "../trace.hpp"

namespace rn::vlk {

vk::UniqueInstance InstanceCreator::create([[maybe_unused]] Context &context, DebugCallbackCreator &debugCallback, const SurfaceCreator &surface) {
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

	vk::ApplicationInfo applicationInfo{
		/*.pApplicationName=*/ appConfig.name().data(),
		/*.applicationVersion=*/ VK_MAKE_VERSION(appConfig.version.major(), appConfig.version.minor(), appConfig.version.patch()),
		/*.pEngineName=*/ engineConfig.name().data(),
		/*.engineVersion=*/ VK_MAKE_VERSION(engineConfig.version.major(), engineConfig.version.minor(), engineConfig.version.patch()),
		/*.apiVersion=*/ VK_API_VERSION_1_0
	};

	std::vector<const char *> requestedLayers = util::map(result.layers, [] (auto &value) { return value.data(); });
	std::vector<const char *> requestedExtensions = util::map(result.extensions, [] (auto &value) { return value.data(); });

	vk::InstanceCreateInfo instanceCreateInfo{
		/*.flags=*/ vk::InstanceCreateFlags{},
		/*.pApplicationInfo=*/ &applicationInfo,
		/*.enabledLayerCount=*/ static_cast<uint32_t>(requestedLayers.size()),
		/*.ppEnabledLayerNames=*/ requestedLayers.data(),
		/*.enabledExtensionCount=*/ static_cast<uint32_t>(requestedExtensions.size()),
		/*.ppEnabledExtensionNames=*/ requestedExtensions.data()
	};

	vk::UniqueInstance instanceOwner{RN_VLK_TRACE(vk::createInstanceUnique(instanceCreateInfo))};

	if ( ! instanceOwner) {
		throw std::runtime_error{"Vulkan instance could not be created"};
	}

	return instanceOwner;
}

} // rn::vlk
