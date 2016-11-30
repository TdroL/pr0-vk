#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <vulkan/vulkan.h>

#include "deviceOwner.hpp"
#include "../../ngn/config.hpp"
#include "../../ngn/log.hpp"

namespace rn {

namespace vlk {

vk::PhysicalDeviceFeatures requiredFeatures{
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
	/*.fillModeNonSolid=*/ false,
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
	/*.fillModeNonSolid=*/ false,
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

class DeviceCreator {
public:
	std::vector<std::string> requiredDeviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	DeviceOwner create(const GLFW &glfw, const vk::Instance &instance) {
		std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

		physicalDevices = rejectInvalidPhysicalDevices(glfw, instance, physicalDevices);

		if (physicalDevices.empty()) {
			throw std::runtime_error{"No valid physical devices available"};
		}

		auto vendorIDValue = ngn::config::get<uint32_t>("/physicalDevice/vendorID");
		auto deviceIDValue = ngn::config::get<uint32_t>("/physicalDevice/deviceID");

		auto uuidValue = ngn::config::get<std::string>("/device/pipelineCacheUUID");
		std::string uuid = uuidValue.value();

		vk::PhysicalDevice physicalDevice = findPhysicalDevice(physicalDevices, vendorIDValue.value(), deviceIDValue.value());

		if ( ! physicalDevice) {
			physicalDevice = physicalDevices[0]; // select first-fit
			vk::PhysicalDeviceProperties props = physicalDevice.getProperties();

			ngn::config::set<uint32_t>("/physicalDevice/vendorID", props.vendorID);
			ngn::config::set<uint32_t>("/physicalDevice/deviceID", props.deviceID);

			if ( ! ngn::config::flush()) {
				ngn::log::warn("Failed to update core config");
			}
		}

		ngn::log::debug("PhysicalDevice found: {}", static_cast<bool>(physicalDevice));

		return DeviceOwner{};
	}

	std::vector<vk::PhysicalDevice> rejectInvalidPhysicalDevices(const GLFW &glfw, const vk::Instance &instance, const std::vector<vk::PhysicalDevice> &physicalDevices) {
		std::vector<vk::PhysicalDevice> result{physicalDevices};

		result.erase(std::remove_if(std::begin(result), std::end(result), [&] (const vk::PhysicalDevice &physicalDevice) {
			return ! hasReqiredPhysicalDeviceFeatures(physicalDevice) || ! glfw.physicalDeviceSupported(instance, physicalDevice);
		}), std::end(result));

		return result;
	}

	bool hasReqiredPhysicalDeviceFeatures(const vk::PhysicalDevice &physicalDevice) {
		bool result = true;

		const vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures();

		/*
			r t v   v'
			1 1 1 = 1
			1 1 0 = 0
			1 0 0 = 0
			1 0 1 = 0
			0 1 1 = 1
			0 1 0 = 0
			0 0 0 = 0
			0 0 1 = 1

			v' = v && (!r || (r && t))
		*/

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

	vk::PhysicalDevice findPhysicalDevice(const std::vector<vk::PhysicalDevice> &physicalDevices, uint32_t vendorID, uint32_t deviceID) {
		for (const auto entry : physicalDevices) {
			vk::PhysicalDeviceProperties props = entry.getProperties();

			if (vendorID == props.vendorID && deviceID == props.deviceID) {
				return entry;
			}
		}

		return vk::PhysicalDevice{};
	}
};

} // vlk

} // rn
