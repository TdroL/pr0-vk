#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../context.hpp"

namespace rn::vlk {

class PhysicalDeviceSelector {
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

	Context::PhysicalDevice select(Context &context);

	std::vector<vk::PhysicalDevice> rejectUnsuitablePhysicalDevices(const vk::SurfaceKHR &surface, const std::vector<vk::PhysicalDevice> &physicalDevices);

	vk::PhysicalDevice findPhysicalDevice(uint32_t vendorID, uint32_t deviceID, const std::vector<vk::PhysicalDevice> &physicalDevices);

	bool hasReqiredPhysicalDeviceFeatures(const vk::PhysicalDevice &physicalDevice);

	bool hasPresentationSupport(const vk::SurfaceKHR &surface, const vk::PhysicalDevice &physicalDevice);
};

} // rn::vlk
