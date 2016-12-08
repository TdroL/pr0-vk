#pragma once

#include <map>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

#include "physicalDeviceOwner.hpp"
#include "queuesPlanner.hpp"
#include "../../ngn/config.hpp"
#include "../../ngn/log.hpp"

namespace rn {

namespace vlk {

class PhysicalDeviceCreator {
public:
	vk::PhysicalDeviceFeatures requiredFeatures{
		/*.robustBufferAccess=*/ false,
		/*.fullDrawIndexUint32=*/ false,
		/*.imageCubeArray=*/ false,
		/*.independentBlend=*/ true,
		/*.geometryShader=*/ false,
		/*.tessellationShader=*/ false,
		/*.sampleRateShading=*/ false,
		/*.dualSrcBlend=*/ false,
		/*.logicOp=*/ false,
		/*.multiDrawIndirect=*/ false,
		/*.drawIndirectFirstInstance=*/ false,
		/*.depthClamp=*/ false,
		/*.depthBiasClamp=*/ false,
		/*.fillModeNonSolid=*/ false,
		/*.depthBounds=*/ false,
		/*.wideLines=*/ false,
		/*.largePoints=*/ false,
		/*.alphaToOne=*/ false,
		/*.multiViewport=*/ false,
		/*.samplerAnisotropy=*/ true,
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

	PhysicalDeviceOwner create(GLFWOwner &glfwOwner, InstanceOwner &instanceOwner) {
		GLFW &glfw = glfwOwner.handle;
		vk::Instance &instance = instanceOwner.handle;

		std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

		physicalDevices = rejectUnsuitablePhysicalDevices(glfw, instance, physicalDevices);

		if (physicalDevices.empty()) {
			throw std::runtime_error{"No valid physical devices available"};
		}

		auto vendorIDValue = ngn::config::get<uint32_t>("/physicalDevice/vendorID");
		auto deviceIDValue = ngn::config::get<uint32_t>("/physicalDevice/deviceID");

		vk::PhysicalDevice physicalDevice = findPhysicalDevice(vendorIDValue.value(), deviceIDValue.value(), physicalDevices);

		if ( ! physicalDevice) {
			physicalDevice = physicalDevices[0]; // select first-fit
			vk::PhysicalDeviceProperties props = physicalDevice.getProperties();

			ngn::config::set<uint32_t>("/physicalDevice/vendorID", props.vendorID);
			ngn::config::set<uint32_t>("/physicalDevice/deviceID", props.deviceID);

			ngn::config::flush();
		}

		vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
		vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
		vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures();

		return PhysicalDeviceOwner{
			std::move(physicalDevice),
			std::move(properties),
			std::move(memoryProperties),
			std::move(features),
			requiredFeatures
		};
	}

	std::vector<vk::PhysicalDevice> rejectUnsuitablePhysicalDevices(const GLFW &glfw, const vk::Instance &instance, const std::vector<vk::PhysicalDevice> &physicalDevices) {
		std::vector<vk::PhysicalDevice> result{physicalDevices};

		result.erase(std::remove_if(std::begin(result), std::end(result), [&] (const vk::PhysicalDevice &physicalDevice) {
			bool hasFeatures = hasReqiredPhysicalDeviceFeatures(physicalDevice);
			bool supportsPresentation = glfw.physicalDeviceSupported(instance, physicalDevice);
			bool hasRequiredQueues = QueuesPlanner{glfw, instance, physicalDevice}.selectQueueIndices();

			return ! ( hasFeatures && supportsPresentation && hasRequiredQueues);
		}), std::end(result));

		return result;
	}

	vk::PhysicalDevice findPhysicalDevice(uint32_t vendorID, uint32_t deviceID, const std::vector<vk::PhysicalDevice> &physicalDevices) {
		for (const auto entry : physicalDevices) {
			vk::PhysicalDeviceProperties props = entry.getProperties();

			if (vendorID == props.vendorID && deviceID == props.deviceID) {
				return entry;
			}
		}

		return vk::PhysicalDevice{};
	}

	bool hasReqiredPhysicalDeviceFeatures(const vk::PhysicalDevice &physicalDevice) {
		bool result = true;

		const vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures();

		#define TEST(prop) (result && ( ! requiredFeatures.prop || (requiredFeatures.prop && features.prop)))
		result = TEST(robustBufferAccess);
		result = TEST(fullDrawIndexUint32);
		result = TEST(imageCubeArray);
		result = TEST(independentBlend);
		result = TEST(geometryShader);
		result = TEST(tessellationShader);
		result = TEST(sampleRateShading);
		result = TEST(dualSrcBlend);
		result = TEST(logicOp);
		result = TEST(multiDrawIndirect);
		result = TEST(drawIndirectFirstInstance);
		result = TEST(depthClamp);
		result = TEST(depthBiasClamp);
		result = TEST(fillModeNonSolid);
		result = TEST(depthBounds);
		result = TEST(wideLines);
		result = TEST(largePoints);
		result = TEST(alphaToOne);
		result = TEST(multiViewport);
		result = TEST(samplerAnisotropy);
		result = TEST(textureCompressionETC2);
		result = TEST(textureCompressionASTC_LDR);
		result = TEST(textureCompressionBC);
		result = TEST(occlusionQueryPrecise);
		result = TEST(pipelineStatisticsQuery);
		result = TEST(vertexPipelineStoresAndAtomics);
		result = TEST(fragmentStoresAndAtomics);
		result = TEST(shaderTessellationAndGeometryPointSize);
		result = TEST(shaderImageGatherExtended);
		result = TEST(shaderStorageImageExtendedFormats);
		result = TEST(shaderStorageImageMultisample);
		result = TEST(shaderStorageImageReadWithoutFormat);
		result = TEST(shaderStorageImageWriteWithoutFormat);
		result = TEST(shaderUniformBufferArrayDynamicIndexing);
		result = TEST(shaderSampledImageArrayDynamicIndexing);
		result = TEST(shaderStorageBufferArrayDynamicIndexing);
		result = TEST(shaderStorageImageArrayDynamicIndexing);
		result = TEST(shaderClipDistance);
		result = TEST(shaderCullDistance);
		result = TEST(shaderFloat64);
		result = TEST(shaderInt64);
		result = TEST(shaderInt16);
		result = TEST(shaderResourceResidency);
		result = TEST(shaderResourceMinLod);
		result = TEST(sparseBinding);
		result = TEST(sparseResidencyBuffer);
		result = TEST(sparseResidencyImage2D);
		result = TEST(sparseResidencyImage3D);
		result = TEST(sparseResidency2Samples);
		result = TEST(sparseResidency4Samples);
		result = TEST(sparseResidency8Samples);
		result = TEST(sparseResidency16Samples);
		result = TEST(sparseResidencyAliased);
		result = TEST(variableMultisampleRate);
		result = TEST(inheritedQueries);
		#undef TEST

		return result;
	}
};

} // vlk

} // rn
