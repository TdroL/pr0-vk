#include "deviceCreator.hpp"

#include <map>
#include <numeric>
#include <stdexcept>

#include "../trace.hpp"

namespace rn::vki::context {

std::tuple<std::vector<float>, std::vector<vk::DeviceQueueCreateInfo>> buildDeviceQueueCreateInfos(const rn::vki::context::QueueFamilyIndex &queueFamilyIndex) {
	std::map<uint32_t, uint32_t> queueFamilyCount{};
	queueFamilyCount.insert_or_assign(queueFamilyIndex.presentation.family, 1);
	queueFamilyCount.insert_or_assign(queueFamilyIndex.graphic.family, 1);
	queueFamilyCount.insert_or_assign(queueFamilyIndex.compute.family, 1);
	queueFamilyCount.insert_or_assign(queueFamilyIndex.transfer.family, 1);

	queueFamilyCount[queueFamilyIndex.presentation.family] = std::max(queueFamilyCount[queueFamilyIndex.presentation.family], queueFamilyIndex.presentation.index + 1);
	queueFamilyCount[queueFamilyIndex.graphic.family] = std::max(queueFamilyCount[queueFamilyIndex.graphic.family], queueFamilyIndex.graphic.index + 1);
	queueFamilyCount[queueFamilyIndex.compute.family] = std::max(queueFamilyCount[queueFamilyIndex.compute.family], queueFamilyIndex.compute.index + 1);
	queueFamilyCount[queueFamilyIndex.transfer.family] = std::max(queueFamilyCount[queueFamilyIndex.transfer.family], queueFamilyIndex.transfer.index + 1);

	std::vector<vk::DeviceQueueCreateInfo> createInfos{};
	createInfos.reserve(queueFamilyCount.size());

	size_t prioritiesSize = std::accumulate(std::begin(queueFamilyCount), std::end(queueFamilyCount), 0, [] (size_t acc, const auto &item) {
		return acc + item.second;
	});

	std::vector<float> priorities{};
	priorities.resize(prioritiesSize, 1.f);

	ptrdiff_t cursor = 0;
	for (auto [family, indexCount] : queueFamilyCount) {
		createInfos.emplace_back(
			vk::DeviceQueueCreateFlags{},
			family,
			indexCount,
			&priorities[cursor]
		);

		cursor += indexCount;
	}

	return std::make_tuple(std::move(priorities), std::move(createInfos));
}

vk::PhysicalDeviceFeatures2 buildDeviceFeatures(const vk::PhysicalDeviceFeatures2 &requiredFeatures, const vk::PhysicalDeviceFeatures2 &optionalFeatures, const vk::PhysicalDeviceFeatures2 &availableFeatures) {
	vk::PhysicalDeviceFeatures2 features{};

	#define X(name) features.features.name = requiredFeatures.features.name || (availableFeatures.features.name && optionalFeatures.features.name)
	X(robustBufferAccess);
	X(fullDrawIndexUint32);
	X(imageCubeArray);
	X(independentBlend);
	X(geometryShader);
	X(tessellationShader);
	X(sampleRateShading);
	X(dualSrcBlend);
	X(logicOp);
	X(multiDrawIndirect);
	X(drawIndirectFirstInstance);
	X(depthClamp);
	X(depthBiasClamp);
	X(fillModeNonSolid);
	X(depthBounds);
	X(wideLines);
	X(largePoints);
	X(alphaToOne);
	X(multiViewport);
	X(samplerAnisotropy);
	X(textureCompressionETC2);
	X(textureCompressionASTC_LDR);
	X(textureCompressionBC);
	X(occlusionQueryPrecise);
	X(pipelineStatisticsQuery);
	X(vertexPipelineStoresAndAtomics);
	X(fragmentStoresAndAtomics);
	X(shaderTessellationAndGeometryPointSize);
	X(shaderImageGatherExtended);
	X(shaderStorageImageExtendedFormats);
	X(shaderStorageImageMultisample);
	X(shaderStorageImageReadWithoutFormat);
	X(shaderStorageImageWriteWithoutFormat);
	X(shaderUniformBufferArrayDynamicIndexing);
	X(shaderSampledImageArrayDynamicIndexing);
	X(shaderStorageBufferArrayDynamicIndexing);
	X(shaderStorageImageArrayDynamicIndexing);
	X(shaderClipDistance);
	X(shaderCullDistance);
	X(shaderFloat64);
	X(shaderInt64);
	X(shaderInt16);
	X(shaderResourceResidency);
	X(shaderResourceMinLod);
	X(sparseBinding);
	X(sparseResidencyBuffer);
	X(sparseResidencyImage2D);
	X(sparseResidencyImage3D);
	X(sparseResidency2Samples);
	X(sparseResidency4Samples);
	X(sparseResidency8Samples);
	X(sparseResidency16Samples);
	X(sparseResidencyAliased);
	X(variableMultisampleRate);
	X(inheritedQueries);
	#undef X

	return features;
}

rn::vki::UniqueTableDevice DeviceCreator::create(rn::vki::HandlePhysicalDevice &&physicalDevice, const rn::vki::context::PhysicalDeviceDescription &physicalDeviceDescription, const rn::vki::context::QueueFamilyIndex &queueFamilyIndex, rn::vki::HandleInstance &&instance) {
	const vk::PhysicalDeviceFeatures2 &availableFeatures = physicalDeviceDescription.availableFeatures;
	const vk::PhysicalDeviceFeatures2 &requiredFeatures = physicalDeviceDescription.requiredFeatures;

	const auto prioritiesCreateInfosTuple = buildDeviceQueueCreateInfos(queueFamilyIndex);
	const std::vector<vk::DeviceQueueCreateInfo> &deviceQueueCreateInfos = std::get<1>(prioritiesCreateInfosTuple);

	std::vector<const char *> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	vk::StructureChain<vk::DeviceCreateInfo, vk::PhysicalDeviceFeatures2> deviceCreateInfoChain{
		vk::DeviceCreateInfo{
			/*.flags=*/ vk::DeviceCreateFlags{},
			/*.queueCreateInfoCount=*/ static_cast<uint32_t>(deviceQueueCreateInfos.size()),
			/*.pQueueCreateInfos=*/ deviceQueueCreateInfos.data(),
			/*.enabledLayerCount=*/ 0,
			/*.ppEnabledLayerNames=*/ nullptr,
			/*.enabledExtensionCount=*/ static_cast<uint32_t>(deviceExtensions.size()),
			/*.ppEnabledExtensionNames=*/ deviceExtensions.data(),
			/*.pEnabledFeatures=*/ nullptr,
		},
		buildDeviceFeatures(requiredFeatures, optionalFeatures, availableFeatures)
	};

	vk::UniqueDevice deviceOwner = RN_VKI_TRACE(physicalDevice->createDeviceUnique(deviceCreateInfoChain.get<vk::DeviceCreateInfo>()));

	if ( ! deviceOwner) {
		throw std::runtime_error{"Vulkan device could not be created"};
	}

	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddrTemp = reinterpret_cast<PFN_vkGetInstanceProcAddr>(instance->getProcAddr("vkGetInstanceProcAddr"));
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddrTemp = reinterpret_cast<PFN_vkGetDeviceProcAddr>(deviceOwner->getProcAddr("vkGetDeviceProcAddr"));

	std::unique_ptr<vk::DispatchLoaderDynamic> table = std::make_unique<vk::DispatchLoaderDynamic>(*instance, vkGetInstanceProcAddrTemp, *deviceOwner, vkGetDeviceProcAddrTemp);

	return { std::move(deviceOwner), std::move(table) };
}

} // rn::vki::context
