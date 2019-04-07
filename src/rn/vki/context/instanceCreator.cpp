#include "instanceCreator.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../../../ngn/external/glfw.hpp"
#include "../../../ngn/external.hpp"
#include "../../../util/join.hpp"
#include "../../../util/map.hpp"
#include "../trace.hpp"
#include "debugCreator.hpp"
#include "surfaceCreator.hpp"

namespace rn::vki::context {

const auto initialize = ngn::external::initialize<ngn::external::GLFW>();

rn::vki::UniqueTableInstance InstanceCreator::create(ngn::config::Config &config) {
	if (glfwVulkanSupported() != GLFW_TRUE) {
		throw std::runtime_error{"Vulkan without surface rendering is not supported"};
	}

	// check if vkEnumerateInstanceVersion is available
	if (vk::DispatchLoaderStatic{}.vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion") == nullptr) {
		uint32_t version = RN_VKI_TRACE(vk::enumerateInstanceVersion());

		uint32_t major = VK_VERSION_MAJOR(version);
		uint32_t minor = VK_VERSION_MINOR(version);

		if (major <= 1 && minor < 1) {
			throw std::runtime_error{"Vulkan 1.0 is not supported"};
		}
	}

	// load available instance layers
	std::vector<std::string> availableLayers = util::map(RN_VKI_TRACE(vk::enumerateInstanceLayerProperties()), [] (auto &properties) {
		return std::string{properties.layerName};
	});

	std::vector<std::string> selectedLayers{};
	// select Surface layers
	selectedLayers = SurfaceCreator::selectLayers(config, availableLayers, selectedLayers);

	// select Debug layers
	selectedLayers = DebugCreator::selectLayers(config, availableLayers, selectedLayers);

	// load available instance extensions
	std::vector<std::string> availableExtensions = util::map(RN_VKI_TRACE(vk::enumerateInstanceExtensionProperties()), [] (auto &properties) {
		return std::string{properties.extensionName};
	});

	std::vector<std::string> selectedExtensions{};
	// select Surface layers
	selectedExtensions = SurfaceCreator::selectExtensions(config, availableExtensions, selectedExtensions);

	// select Debug layers
	selectedExtensions = DebugCreator::selectExtensions(config, availableExtensions, selectedExtensions);

	ngn::log::debug("rn::vki::context::InstanceCreator::create() => enabled {} layer(s): {}", selectedLayers.size(), util::join(selectedLayers));
	ngn::log::debug("rn::vki::context::InstanceCreator::create() => enabled {} extension(s): {}", selectedExtensions.size(), util::join(selectedExtensions));

	auto &appConfig = config.core.application;
	auto &engineConfig = config.core.engine;

	vk::ApplicationInfo applicationInfo{
		/*.pApplicationName=*/ appConfig.name.data(),
		/*.applicationVersion=*/ VK_MAKE_VERSION(appConfig.version.major, appConfig.version.minor, appConfig.version.patch),
		/*.pEngineName=*/ engineConfig.name.data(),
		/*.engineVersion=*/ VK_MAKE_VERSION(engineConfig.version.major, engineConfig.version.minor, engineConfig.version.patch),
		/*.apiVersion=*/ VK_API_VERSION_1_1
	};

	std::vector<const char *> requestedLayers = util::map(selectedLayers, [] (auto &value) { return value.data(); });
	std::vector<const char *> requestedExtensions = util::map(selectedExtensions, [] (auto &value) { return value.data(); });

	vk::InstanceCreateInfo instanceCreateInfo{
		/*.flags=*/ vk::InstanceCreateFlags{},
		/*.pApplicationInfo=*/ &applicationInfo,
		/*.enabledLayerCount=*/ static_cast<uint32_t>(requestedLayers.size()),
		/*.ppEnabledLayerNames=*/ requestedLayers.data(),
		/*.enabledExtensionCount=*/ static_cast<uint32_t>(requestedExtensions.size()),
		/*.ppEnabledExtensionNames=*/ requestedExtensions.data()
	};

	vk::UniqueInstance instanceOwner = RN_VKI_TRACE(vk::createInstanceUnique(instanceCreateInfo));

	if ( ! instanceOwner) {
		throw std::runtime_error{"Vulkan instance could not be created"};
	}

	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddrTemp = reinterpret_cast<PFN_vkGetInstanceProcAddr>(instanceOwner->getProcAddr("vkGetInstanceProcAddr"));

	std::unique_ptr<vk::DispatchLoaderDynamic> table = std::make_unique<vk::DispatchLoaderDynamic>(*instanceOwner, vkGetInstanceProcAddrTemp);

	return { std::move(instanceOwner), std::move(table) };
}

} // rn::vki::context
