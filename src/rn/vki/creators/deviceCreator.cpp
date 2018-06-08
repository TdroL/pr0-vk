#include "deviceCreator.hpp"

#include <cassert>
#include <map>
#include <stdexcept>

#include "../trace.hpp"
#include "queuesPlanner.hpp"

namespace rn::vki {

vk::UniqueDevice DeviceCreator::create(Context &context) {
	vk::SurfaceKHR surface = context.owners.surface.get();
	vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;
	vk::PhysicalDeviceFeatures &availableFeatures = context.physicalDevice.availableFeatures;
	vk::PhysicalDeviceFeatures &requiredFeatures = context.physicalDevice.requiredFeatures;

	assert(surface);
	assert(physicalDevice);

	std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = buildDeviceQueueCreateInfos(surface, physicalDevice).second;

	std::vector<const char *> deviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	vk::PhysicalDeviceFeatures physicalDeviceFeatures = buildDeviceFeatures(requiredFeatures, availableFeatures);

	vk::DeviceCreateInfo deviceCreateInfo{
		/*.flags=*/ vk::DeviceCreateFlags{},
		/*.queueCreateInfoCount=*/ static_cast<uint32_t>(deviceQueueCreateInfos.size()),
		/*.pQueueCreateInfos=*/ deviceQueueCreateInfos.data(),
		/*.enabledLayerCount=*/ 0,
		/*.ppEnabledLayerNames=*/ nullptr,
		/*.enabledExtensionCount=*/ static_cast<uint32_t>(deviceExtensions.size()),
		/*.ppEnabledExtensionNames=*/ deviceExtensions.data(),
		/*.pEnabledFeatures=*/ &physicalDeviceFeatures
	};

	vk::UniqueDevice deviceOwner{RN_VLK_TRACE(physicalDevice.createDeviceUnique(deviceCreateInfo))};

	if ( ! deviceOwner) {
		throw std::runtime_error{"Vulkan device could not be created"};
	}

	return deviceOwner;
}

std::pair<std::vector<std::vector<float>>, std::vector<vk::DeviceQueueCreateInfo>> DeviceCreator::buildDeviceQueueCreateInfos(const vk::SurfaceKHR &surface, const vk::PhysicalDevice &physicalDevice) {
	std::map<uint32_t, uint32_t> usageCount = QueuesPlanner{surface, physicalDevice}.countQueueFamilyUsage();

	std::vector<vk::DeviceQueueCreateInfo> createInfos{};
	createInfos.reserve(usageCount.size());

	std::vector<std::vector<float>> prioritiesList{};
	prioritiesList.resize(usageCount.size());

	uint32_t i = 0;
	for (const auto &pair : usageCount) {
		// for family [pair.first] request [pair.second] queues, all with equal priority
		prioritiesList[i].resize(pair.second, 1.f);

		createInfos.emplace_back(
			vk::DeviceQueueCreateFlags{},
			pair.first,
			prioritiesList[i].size(),
			prioritiesList[i].data()
		);

		i++;
	}

	return {
		prioritiesList,
		createInfos
	};
}

vk::PhysicalDeviceFeatures DeviceCreator::buildDeviceFeatures(const vk::PhysicalDeviceFeatures &requiredFeatures, const vk::PhysicalDeviceFeatures &availableFeatures) {
	vk::PhysicalDeviceFeatures features{};

	#define APPLY(name) features.name = requiredFeatures.name || (availableFeatures.name && optionalFeatures.name);
	APPLY(robustBufferAccess);
	APPLY(fullDrawIndexUint32);
	APPLY(imageCubeArray);
	APPLY(independentBlend);
	APPLY(geometryShader);
	APPLY(tessellationShader);
	APPLY(sampleRateShading);
	APPLY(dualSrcBlend);
	APPLY(logicOp);
	APPLY(multiDrawIndirect);
	APPLY(drawIndirectFirstInstance);
	APPLY(depthClamp);
	APPLY(depthBiasClamp);
	APPLY(fillModeNonSolid);
	APPLY(depthBounds);
	APPLY(wideLines);
	APPLY(largePoints);
	APPLY(alphaToOne);
	APPLY(multiViewport);
	APPLY(samplerAnisotropy);
	APPLY(textureCompressionETC2);
	APPLY(textureCompressionASTC_LDR);
	APPLY(textureCompressionBC);
	APPLY(occlusionQueryPrecise);
	APPLY(pipelineStatisticsQuery);
	APPLY(vertexPipelineStoresAndAtomics);
	APPLY(fragmentStoresAndAtomics);
	APPLY(shaderTessellationAndGeometryPointSize);
	APPLY(shaderImageGatherExtended);
	APPLY(shaderStorageImageExtendedFormats);
	APPLY(shaderStorageImageMultisample);
	APPLY(shaderStorageImageReadWithoutFormat);
	APPLY(shaderStorageImageWriteWithoutFormat);
	APPLY(shaderUniformBufferArrayDynamicIndexing);
	APPLY(shaderSampledImageArrayDynamicIndexing);
	APPLY(shaderStorageBufferArrayDynamicIndexing);
	APPLY(shaderStorageImageArrayDynamicIndexing);
	APPLY(shaderClipDistance);
	APPLY(shaderCullDistance);
	APPLY(shaderFloat64);
	APPLY(shaderInt64);
	APPLY(shaderInt16);
	APPLY(shaderResourceResidency);
	APPLY(shaderResourceMinLod);
	APPLY(sparseBinding);
	APPLY(sparseResidencyBuffer);
	APPLY(sparseResidencyImage2D);
	APPLY(sparseResidencyImage3D);
	APPLY(sparseResidency2Samples);
	APPLY(sparseResidency4Samples);
	APPLY(sparseResidency8Samples);
	APPLY(sparseResidency16Samples);
	APPLY(sparseResidencyAliased);
	APPLY(variableMultisampleRate);
	APPLY(inheritedQueries);
	#undef APPLY

	return features;
}

} // rn::vki
