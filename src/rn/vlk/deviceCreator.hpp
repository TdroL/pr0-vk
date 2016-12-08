#pragma once

#include <map>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

#include "deviceOwner.hpp"
#include "queuesPlanner.hpp"
#include "../../ngn/config.hpp"
#include "../../ngn/log.hpp"

namespace rn {

namespace vlk {

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

	DeviceOwner create(GLFWOwner &glfwOwner, InstanceOwner &instanceOwner, PhysicalDeviceOwner &physicalDeviceOwner) {
		GLFW &glfw = glfwOwner.handle;
		vk::Instance &instance = instanceOwner.handle;
		vk::PhysicalDevice &physicalDevice = physicalDeviceOwner.handle;

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = buildDeviceQueueCreateInfos(glfw, instance, physicalDevice).second;

		std::vector<const char *> deviceExtensions{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		vk::PhysicalDeviceFeatures physicalDeviceFeatures = buildDeviceFeatures(physicalDeviceOwner.requiredFeatures, physicalDeviceOwner.availableFeatures);

		vk::DeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

		return DeviceOwner{
			physicalDevice.createDevice(deviceCreateInfo),
			std::move(physicalDeviceFeatures)
		};
	}

	std::pair<std::vector<std::vector<float>>, std::vector<vk::DeviceQueueCreateInfo>> buildDeviceQueueCreateInfos(const GLFW &glfw, const vk::Instance &instance, const vk::PhysicalDevice &physicalDevice) {
		std::map<uint32_t, uint32_t> usageCount = QueuesPlanner{glfw, instance, physicalDevice}.countQueueFamilyUsage();

		std::vector<vk::DeviceQueueCreateInfo> createInfos{};
		createInfos.reserve(usageCount.size());

		std::vector<std::vector<float>> prioritiesList{};
		prioritiesList.reserve(usageCount.size());

		for (const auto &pair : usageCount) {
			prioritiesList.emplace_back(pair.second, 1.f);

			createInfos.emplace_back(
				vk::DeviceQueueCreateFlags{},
				pair.first,
				pair.second,
				prioritiesList.back().data()
			);
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

} // vlk

} // rn
