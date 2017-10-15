#pragma once

#include <cassert>
#include <map>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"
#include "queuesPlanner.hpp"

namespace rn::vlk {

class DeviceCreator {
public:
	vk::PhysicalDeviceFeatures optionalFeatures{
		/*.robustBufferAccess=*/ false,
		/*.fullDrawIndexUint32=*/ false,
		/*.imageCubeArray=*/ false,
		/*.independentBlend=*/ false,
		/*.geometryShader=*/ false,
		/*.tessellationShader=*/ false,
		/*.sampleRateShading=*/ false,
		/*.dualSrcBlend=*/ false,
		/*.logicOp=*/ false,
		/*.multiDrawIndirect=*/ false,
		/*.drawIndirectFirstInstance=*/ false,
		/*.depthClamp=*/ false,
		/*.depthBiasClamp=*/ false,
		/*.fillModeNonSolid=*/ true,
		/*.depthBounds=*/ false,
		/*.wideLines=*/ false,
		/*.largePoints=*/ false,
		/*.alphaToOne=*/ false,
		/*.multiViewport=*/ false,
		/*.samplerAnisotropy=*/ false,
		/*.textureCompressionETC2=*/ false,
		/*.textureCompressionASTC_LDR=*/ false,
		/*.textureCompressionBC=*/ false,
		/*.occlusionQueryPrecise=*/ false,
		/*.pipelineStatisticsQuery=*/ false,
		/*.vertexPipelineStoresAndAtomics=*/ false,
		/*.fragmentStoresAndAtomics=*/ false,
		/*.shaderTessellationAndGeometryPointSize=*/ false,
		/*.shaderImageGatherExtended=*/ false,
		/*.shaderStorageImageExtendedFormats=*/ false,
		/*.shaderStorageImageMultisample=*/ false,
		/*.shaderStorageImageReadWithoutFormat=*/ false,
		/*.shaderStorageImageWriteWithoutFormat=*/ false,
		/*.shaderUniformBufferArrayDynamicIndexing=*/ false,
		/*.shaderSampledImageArrayDynamicIndexing=*/ false,
		/*.shaderStorageBufferArrayDynamicIndexing=*/ false,
		/*.shaderStorageImageArrayDynamicIndexing=*/ false,
		/*.shaderClipDistance=*/ false,
		/*.shaderCullDistance=*/ false,
		/*.shaderFloat64=*/ false,
		/*.shaderInt64=*/ false,
		/*.shaderInt16=*/ false,
		/*.shaderResourceResidency=*/ false,
		/*.shaderResourceMinLod=*/ false,
		/*.sparseBinding=*/ false,
		/*.sparseResidencyBuffer=*/ false,
		/*.sparseResidencyImage2D=*/ false,
		/*.sparseResidencyImage3D=*/ false,
		/*.sparseResidency2Samples=*/ false,
		/*.sparseResidency4Samples=*/ false,
		/*.sparseResidency8Samples=*/ false,
		/*.sparseResidency16Samples=*/ false,
		/*.sparseResidencyAliased=*/ false,
		/*.variableMultisampleRate=*/ false,
		/*.inheritedQueries=*/ false,
	};

	vk::UniqueDevice create(Context &context) {
		vk::SurfaceKHR surface = context.owners.surface.get();
		vk::Instance instance = context.owners.instance.get();
		vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;
		vk::PhysicalDeviceFeatures &availableFeatures = context.physicalDevice.availableFeatures;
		vk::PhysicalDeviceFeatures &requiredFeatures = context.physicalDevice.requiredFeatures;

		assert(surface);
		assert(instance);
		assert(physicalDevice);

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = buildDeviceQueueCreateInfos(surface, instance, physicalDevice).second;

		std::vector<const char *> deviceExtensions{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		vk::PhysicalDeviceFeatures physicalDeviceFeatures = buildDeviceFeatures(requiredFeatures, availableFeatures);

		vk::DeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

		vk::UniqueDevice deviceOwner{physicalDevice.createDeviceUnique(deviceCreateInfo)};

		if ( ! deviceOwner) {
			throw std::runtime_error{"Vulkan device could not be created"};
		}

		return deviceOwner;
	}

	std::pair<std::vector<std::vector<float>>, std::vector<vk::DeviceQueueCreateInfo>> buildDeviceQueueCreateInfos(const vk::SurfaceKHR &surface, const vk::Instance &instance, const vk::PhysicalDevice &physicalDevice) {
		std::map<uint32_t, uint32_t> usageCount = QueuesPlanner{surface, instance, physicalDevice}.countQueueFamilyUsage();

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

	vk::PhysicalDeviceFeatures buildDeviceFeatures(const vk::PhysicalDeviceFeatures &requiredFeatures, const vk::PhysicalDeviceFeatures &availableFeatures) {
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
};

} // rn::vlk
