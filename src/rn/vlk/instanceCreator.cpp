#include "instanceCreator.hpp"
#include "context.hpp"

#include <iostream>

namespace rn {

namespace vlk {

InstanceCreator::InstanceCreator() {
	VkResult err;

	// load available instance layers
	availableLayers.empty();
	std::vector<VkLayerProperties> layerProperties{};
	uint32_t layerPropertiesCount = 0;
	err = vkEnumerateInstanceLayerProperties(&layerPropertiesCount, nullptr);
	if (err == VK_SUCCESS) {
		layerProperties.resize(layerPropertiesCount);

		err = vkEnumerateInstanceLayerProperties(&layerPropertiesCount, layerProperties.data());
		if (err == VK_SUCCESS) {
			availableLayers.resize(layerProperties.size());
			std::transform(std::begin(layerProperties), std::end(layerProperties), std::begin(availableLayers), [] (const VkLayerProperties &properties) {
				return std::string{properties.layerName};
			});
		}
	}

	// load available instance extensions
	availableExtensions.empty();
	std::vector<VkExtensionProperties> extensionProperties{};
	uint32_t extensionPropertiesCount = 0;
	err = vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, nullptr);
	if (err == VK_SUCCESS) {
		extensionProperties.resize(extensionPropertiesCount);

		err = vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, extensionProperties.data());
		if (err == VK_SUCCESS) {
			availableExtensions.resize(extensionProperties.size());
			std::transform(std::begin(extensionProperties), std::end(extensionProperties), std::begin(availableExtensions), [] (const VkExtensionProperties &properties) {
				return std::string{properties.extensionName};
			});
		}
	}
}

bool InstanceCreator::addLayer(const std::string &layer) {
	if (std::find(std::begin(availableLayers), std::end(availableLayers), layer) == std::end(availableLayers)) {
		return false;
	}

	if (std::find(std::begin(layers), std::end(layers), layer) == std::end(layers)) {
		layers.push_back(layer);
	}

	return true;
}

bool InstanceCreator::addExtension(const std::string &extension) {
	if (std::find(std::begin(availableExtensions), std::end(availableExtensions), extension) == std::end(availableExtensions)) {
		return false;
	}

	if (std::find(std::begin(extensions), std::end(extensions), extension) == std::end(extensions)) {
		extensions.push_back(extension);
	}

	return true;
}

VkResult InstanceCreator::run(Context &context) {
	VkApplicationInfo applicationInfo {
		/*.sType=*/              VK_STRUCTURE_TYPE_APPLICATION_INFO,
		/*.pNext=*/              nullptr,
		/*.pApplicationName=*/   applicationName.c_str(),
		/*.applicationVersion=*/ VK_MAKE_VERSION(applicationVersion.major, applicationVersion.minor, applicationVersion.patch),
		/*.pEngineName=*/        engineName.c_str(),
		/*.engineVersion=*/      VK_MAKE_VERSION(engineVersion.major, engineVersion.minor, engineVersion.patch),
		/*.apiVersion=*/         VK_API_VERSION_1_0
	};

	std::vector<const char *> requestedLayers(layers.size());
	std::transform(std::begin(layers), std::end(layers), std::begin(requestedLayers), [] (const std::string &layer) {
		return layer.data();
	});

	std::vector<const char *> requestedExtensions(extensions.size());
	std::transform(std::begin(extensions), std::end(extensions), std::begin(requestedExtensions), [] (const std::string &extension) {
		return extension.data();
	});

	VkInstanceCreateInfo instanceCreateInfo {
		/*.sType=*/                   VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		/*.pNext=*/                   nullptr,
		/*.flags=*/                   0,
		/*.pApplicationInfo=*/        &applicationInfo,
		/*.enabledLayerCount=*/       static_cast<uint32_t>(requestedLayers.size()),
		/*.ppEnabledLayerNames=*/     requestedLayers.data(),
		/*.enabledExtensionCount=*/   static_cast<uint32_t>(requestedExtensions.size()),
		/*.ppEnabledExtensionNames=*/ requestedExtensions.data()
	};

	std::cout << "requestedLayers.size()=" << requestedLayers.size() << std::endl;
	for (const auto layer : requestedLayers) {
		std::cout << "  " << layer << std::endl;
	}
	std::cout << "requestedExtensions.size()=" << requestedExtensions.size() << std::endl;
	for (const auto extension : requestedExtensions) {
		std::cout << "  " << extension << std::endl;
	}
	std::cout << "vkCreateInstance=" << reinterpret_cast<void *>(vkCreateInstance) << std::endl;

	Instance instance{};
	std::cout << "instance.handle=" << reinterpret_cast<void *>(&instance.handle) << std::endl;

	VkResult err = vkCreateInstance(&instanceCreateInfo, nullptr, &instance.handle);
	return VK_SUCCESS;

	if (err != VK_SUCCESS) {
		return err;
	}

	context.instance = std::move(instance);

	return VK_SUCCESS;
}

} // vlk

} // rn
