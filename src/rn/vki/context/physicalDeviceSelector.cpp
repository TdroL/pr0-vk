#include "physicalDeviceSelector.hpp"

#include "../../../ngn/config.hpp"
#include "../../../util/map.hpp"
#include "../trace.hpp"
#include "queuesPlanner.hpp"

namespace rn::vki::context {

bool hasReqiredPhysicalDeviceFeatures(const vk::PhysicalDeviceFeatures2 &requiredFeatures, rn::vki::HandlePhysicalDevice &&physicalDevice) {
	const vk::PhysicalDeviceProperties2 properties = RN_VKI_TRACE(physicalDevice->getProperties2(physicalDevice.table()));

	uint32_t major = VK_VERSION_MAJOR(properties.properties.apiVersion);
	uint32_t minor = VK_VERSION_MINOR(properties.properties.apiVersion);

	if (major <= 1 && minor < 1) {
		return false;
	}

	const vk::PhysicalDeviceFeatures2 features = RN_VKI_TRACE(physicalDevice->getFeatures2(physicalDevice.table()));

	// #define X(propName) result = (result && ( ! requiredFeatures.features.propName || (requiredFeatures.features.propName && features.features.propName)))
	#define X(propName) do { if (requiredFeatures.features.propName && ! features.features.propName) { return false; } } while (false)
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

	return true;
}

bool hasPresentationSupport(size_t familyPropertiesCount, rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandlePhysicalDevice &&physicalDevice) {
	for (size_t i = 0; i < familyPropertiesCount; i++) {
		if (RN_VKI_TRACE(physicalDevice->getSurfaceSupportKHR(static_cast<uint32_t>(i), surface.get(), physicalDevice.table()))) {
			return true;
		}
	}

	return false;
}

std::vector<rn::vki::HandlePhysicalDevice> rejectUnsuitablePhysicalDevices(const vk::PhysicalDeviceFeatures2 &requiredFeatures, rn::vki::HandleSurfaceKHR &&surface, const std::vector<rn::vki::HandlePhysicalDevice> &physicalDevices) {
	std::vector<rn::vki::HandlePhysicalDevice> result{physicalDevices};

	result.erase(std::remove_if(std::begin(result), std::end(result), [&] (auto &physicalDevice) {
		bool hasFeatures = hasReqiredPhysicalDeviceFeatures(requiredFeatures, physicalDevice.handle());

		std::vector<vk::QueueFamilyProperties2> familyProperties = RN_VKI_TRACE(physicalDevice->getQueueFamilyProperties2(physicalDevice.table()));

		bool supportsPresentation = hasPresentationSupport(familyProperties.size(), surface.handle(), physicalDevice.handle());
		bool hasRequiredQueues = QueuesPlanner{surface.handle(), physicalDevice.handle(), std::move(familyProperties)}.selectQueueIndices();

		return ! ( hasFeatures && supportsPresentation && hasRequiredQueues);
	}), std::end(result));

	return result;
}

rn::vki::HandlePhysicalDevice findPhysicalDevice(uint32_t vendorID, uint32_t deviceID, const std::vector<rn::vki::HandlePhysicalDevice> &physicalDevices) {
	for (auto &physicalDevice : physicalDevices) {
		vk::PhysicalDeviceProperties2 properties = RN_VKI_TRACE(physicalDevice->getProperties2(physicalDevice.table()));

		if (vendorID == properties.properties.vendorID && deviceID == properties.properties.deviceID) {
			return physicalDevice;
		}
	}

	return rn::vki::HandlePhysicalDevice{};
}

std::tuple<rn::vki::HandlePhysicalDevice, rn::vki::context::PhysicalDeviceDescription> PhysicalDeviceSelector::select(rn::vki::HandleInstance &&instance, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config) {
	std::vector<rn::vki::HandlePhysicalDevice> physicalDevices = util::map(RN_VKI_TRACE(instance->enumeratePhysicalDevices(instance.table())), [&] (auto physicalDevice) {
		return rn::vki::HandlePhysicalDevice{ physicalDevice, instance.table() };
	});

	physicalDevices = rejectUnsuitablePhysicalDevices(requiredFeatures, surface.handle(), physicalDevices);

	if (physicalDevices.empty()) {
		throw std::runtime_error{"No valid physical devices available"};
	}

	auto vendorIdValue = config.core.physicalDevice.vendorId;
	auto deviceIdValue = config.core.physicalDevice.deviceId;

	rn::vki::HandlePhysicalDevice physicalDevice = findPhysicalDevice(vendorIdValue, deviceIdValue, physicalDevices);

	if ( ! physicalDevice) {
		physicalDevice = physicalDevices[0]; // select first-fit
		vk::PhysicalDeviceProperties2 properties = RN_VKI_TRACE(physicalDevice->getProperties2(physicalDevice.table()));

		config.core.physicalDevice.vendorId = properties.properties.vendorID;
		config.core.physicalDevice.deviceId = properties.properties.deviceID;
		config.core.dirty = true;
	}

	vk::PhysicalDeviceProperties2 properties = RN_VKI_TRACE(physicalDevice->getProperties2(physicalDevice.table()));
	vk::PhysicalDeviceMemoryProperties2 memoryProperties = RN_VKI_TRACE(physicalDevice->getMemoryProperties2(physicalDevice.table()));
	vk::PhysicalDeviceFeatures2 features = RN_VKI_TRACE(physicalDevice->getFeatures2(physicalDevice.table()));

	return std::make_tuple(std::move(physicalDevice), rn::vki::context::PhysicalDeviceDescription{
		properties,
		memoryProperties,
		features,
		requiredFeatures
	});
}

} // rn::vki::context
