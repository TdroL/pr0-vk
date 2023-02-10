#include "contextVK.hpp"

#include <algorithm>
#include <iterator>
#include <locale>
#include <numeric>
#include <string_view>
#include <tuple>
#include <vector>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#include <util/contains.hpp>
#include <util/map.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace ware::contextVK {

bool isSpecIdentifier(const char *text) {
	if ( ! text || *text == '\0') {
		return false;
	}

	while (*text != '\0') {
		if ( ! (std::isupper(*text) || std::isdigit(*text) || *text == '_')) {
			return false;
		}

		text++;
	}

	return true;
}

VkBool32 debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, [[maybe_unused]] void *pUserData) {
	using namespace std::literals;

	std::string_view severity = "UNKNOWN";
	if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)) {
		severity = "ERROR"sv;
	} else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)) {
		severity = "WARNING"sv;
	} else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)) {
		severity = "INFO"sv;
	} else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)) {
		severity = "LOG"sv;
	}

	std::vector<std::string_view> types{};
	if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)) {
		types.push_back("GENERAL"sv);
	}
	if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)) {
		types.push_back("VALIDATION"sv);
	}
	if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)) {
		types.push_back("PERFORMANCE"sv);
	}

	if (types.empty()) {
		types.push_back("UNKNOWN");
	}

	if (isSpecIdentifier(pCallbackData->pMessageIdName)) {
		spdlog::debug("Vulkan report: ({}: {}) [{}:{}]: {} (spec: https://www.khronos.org/registry/vulkan/specs/1.3/html/vkspec.html#{})", severity, fmt::join(types, " | "), pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage, pCallbackData->pMessageIdName);
	} else {
		spdlog::debug("Vulkan report: ({}: {}) [{}:{}]: {}", severity, fmt::join(types, " | "), pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
	}

	return VK_FALSE;
}

bool hasFeatures(const vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features> &requiredFeatures, const vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features> &availableFeatures) {
	const auto &requiredFeatures10 = requiredFeatures.get<vk::PhysicalDeviceFeatures2>().features;
	const auto &requiredFeatures12 = requiredFeatures.get<vk::PhysicalDeviceVulkan12Features>();
	const auto &requiredFeatures13 = requiredFeatures.get<vk::PhysicalDeviceVulkan13Features>();

	const auto &availableFeatures10 = availableFeatures.get<vk::PhysicalDeviceFeatures2>().features;
	const auto &availableFeatures12 = availableFeatures.get<vk::PhysicalDeviceVulkan12Features>();
	const auto &availableFeatures13 = availableFeatures.get<vk::PhysicalDeviceVulkan13Features>();

	#define X(a, b, propName) ( ! a.propName || b.propName)

	return (
		// vk::PhysicalDeviceFeatures2
		X(requiredFeatures10, availableFeatures10, robustBufferAccess) &&
		X(requiredFeatures10, availableFeatures10, fullDrawIndexUint32) &&
		X(requiredFeatures10, availableFeatures10, imageCubeArray) &&
		X(requiredFeatures10, availableFeatures10, independentBlend) &&
		X(requiredFeatures10, availableFeatures10, geometryShader) &&
		X(requiredFeatures10, availableFeatures10, tessellationShader) &&
		X(requiredFeatures10, availableFeatures10, sampleRateShading) &&
		X(requiredFeatures10, availableFeatures10, dualSrcBlend) &&
		X(requiredFeatures10, availableFeatures10, logicOp) &&
		X(requiredFeatures10, availableFeatures10, multiDrawIndirect) &&
		X(requiredFeatures10, availableFeatures10, drawIndirectFirstInstance) &&
		X(requiredFeatures10, availableFeatures10, depthClamp) &&
		X(requiredFeatures10, availableFeatures10, depthBiasClamp) &&
		X(requiredFeatures10, availableFeatures10, fillModeNonSolid) &&
		X(requiredFeatures10, availableFeatures10, depthBounds) &&
		X(requiredFeatures10, availableFeatures10, wideLines) &&
		X(requiredFeatures10, availableFeatures10, largePoints) &&
		X(requiredFeatures10, availableFeatures10, alphaToOne) &&
		X(requiredFeatures10, availableFeatures10, multiViewport) &&
		X(requiredFeatures10, availableFeatures10, samplerAnisotropy) &&
		X(requiredFeatures10, availableFeatures10, textureCompressionETC2) &&
		X(requiredFeatures10, availableFeatures10, textureCompressionASTC_LDR) &&
		X(requiredFeatures10, availableFeatures10, textureCompressionBC) &&
		X(requiredFeatures10, availableFeatures10, occlusionQueryPrecise) &&
		X(requiredFeatures10, availableFeatures10, pipelineStatisticsQuery) &&
		X(requiredFeatures10, availableFeatures10, vertexPipelineStoresAndAtomics) &&
		X(requiredFeatures10, availableFeatures10, fragmentStoresAndAtomics) &&
		X(requiredFeatures10, availableFeatures10, shaderTessellationAndGeometryPointSize) &&
		X(requiredFeatures10, availableFeatures10, shaderImageGatherExtended) &&
		X(requiredFeatures10, availableFeatures10, shaderStorageImageExtendedFormats) &&
		X(requiredFeatures10, availableFeatures10, shaderStorageImageMultisample) &&
		X(requiredFeatures10, availableFeatures10, shaderStorageImageReadWithoutFormat) &&
		X(requiredFeatures10, availableFeatures10, shaderStorageImageWriteWithoutFormat) &&
		X(requiredFeatures10, availableFeatures10, shaderUniformBufferArrayDynamicIndexing) &&
		X(requiredFeatures10, availableFeatures10, shaderSampledImageArrayDynamicIndexing) &&
		X(requiredFeatures10, availableFeatures10, shaderStorageBufferArrayDynamicIndexing) &&
		X(requiredFeatures10, availableFeatures10, shaderStorageImageArrayDynamicIndexing) &&
		X(requiredFeatures10, availableFeatures10, shaderClipDistance) &&
		X(requiredFeatures10, availableFeatures10, shaderCullDistance) &&
		X(requiredFeatures10, availableFeatures10, shaderFloat64) &&
		X(requiredFeatures10, availableFeatures10, shaderInt64) &&
		X(requiredFeatures10, availableFeatures10, shaderInt16) &&
		X(requiredFeatures10, availableFeatures10, shaderResourceResidency) &&
		X(requiredFeatures10, availableFeatures10, shaderResourceMinLod) &&
		X(requiredFeatures10, availableFeatures10, sparseBinding) &&
		X(requiredFeatures10, availableFeatures10, sparseResidencyBuffer) &&
		X(requiredFeatures10, availableFeatures10, sparseResidencyImage2D) &&
		X(requiredFeatures10, availableFeatures10, sparseResidencyImage3D) &&
		X(requiredFeatures10, availableFeatures10, sparseResidency2Samples) &&
		X(requiredFeatures10, availableFeatures10, sparseResidency4Samples) &&
		X(requiredFeatures10, availableFeatures10, sparseResidency8Samples) &&
		X(requiredFeatures10, availableFeatures10, sparseResidency16Samples) &&
		X(requiredFeatures10, availableFeatures10, sparseResidencyAliased) &&
		X(requiredFeatures10, availableFeatures10, variableMultisampleRate) &&
		X(requiredFeatures10, availableFeatures10, inheritedQueries) &&
		// vk::PhysicalDeviceVulkan12Features
		X(requiredFeatures12, availableFeatures12, samplerMirrorClampToEdge) &&
		X(requiredFeatures12, availableFeatures12, drawIndirectCount) &&
		X(requiredFeatures12, availableFeatures12, storageBuffer8BitAccess) &&
		X(requiredFeatures12, availableFeatures12, uniformAndStorageBuffer8BitAccess) &&
		X(requiredFeatures12, availableFeatures12, storagePushConstant8) &&
		X(requiredFeatures12, availableFeatures12, shaderBufferInt64Atomics) &&
		X(requiredFeatures12, availableFeatures12, shaderSharedInt64Atomics) &&
		X(requiredFeatures12, availableFeatures12, shaderFloat16) &&
		X(requiredFeatures12, availableFeatures12, shaderInt8) &&
		X(requiredFeatures12, availableFeatures12, descriptorIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderInputAttachmentArrayDynamicIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderUniformTexelBufferArrayDynamicIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderStorageTexelBufferArrayDynamicIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderUniformBufferArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderSampledImageArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderStorageBufferArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderStorageImageArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderInputAttachmentArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderUniformTexelBufferArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, shaderStorageTexelBufferArrayNonUniformIndexing) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingUniformBufferUpdateAfterBind) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingSampledImageUpdateAfterBind) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingStorageImageUpdateAfterBind) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingStorageBufferUpdateAfterBind) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingUniformTexelBufferUpdateAfterBind) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingStorageTexelBufferUpdateAfterBind) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingUpdateUnusedWhilePending) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingPartiallyBound) &&
		X(requiredFeatures12, availableFeatures12, descriptorBindingVariableDescriptorCount) &&
		X(requiredFeatures12, availableFeatures12, runtimeDescriptorArray) &&
		X(requiredFeatures12, availableFeatures12, samplerFilterMinmax) &&
		X(requiredFeatures12, availableFeatures12, scalarBlockLayout) &&
		X(requiredFeatures12, availableFeatures12, imagelessFramebuffer) &&
		X(requiredFeatures12, availableFeatures12, uniformBufferStandardLayout) &&
		X(requiredFeatures12, availableFeatures12, shaderSubgroupExtendedTypes) &&
		X(requiredFeatures12, availableFeatures12, separateDepthStencilLayouts) &&
		X(requiredFeatures12, availableFeatures12, hostQueryReset) &&
		X(requiredFeatures12, availableFeatures12, timelineSemaphore) &&
		X(requiredFeatures12, availableFeatures12, bufferDeviceAddress) &&
		X(requiredFeatures12, availableFeatures12, bufferDeviceAddressCaptureReplay) &&
		X(requiredFeatures12, availableFeatures12, bufferDeviceAddressMultiDevice) &&
		X(requiredFeatures12, availableFeatures12, vulkanMemoryModel) &&
		X(requiredFeatures12, availableFeatures12, vulkanMemoryModelDeviceScope) &&
		X(requiredFeatures12, availableFeatures12, vulkanMemoryModelAvailabilityVisibilityChains) &&
		X(requiredFeatures12, availableFeatures12, shaderOutputViewportIndex) &&
		X(requiredFeatures12, availableFeatures12, shaderOutputLayer) &&
		X(requiredFeatures12, availableFeatures12, subgroupBroadcastDynamicId) &&
		// vk::PhysicalDeviceVulkan13Features
		X(requiredFeatures13, availableFeatures13, robustImageAccess) &&
		X(requiredFeatures13, availableFeatures13, inlineUniformBlock) &&
		X(requiredFeatures13, availableFeatures13, descriptorBindingInlineUniformBlockUpdateAfterBind) &&
		X(requiredFeatures13, availableFeatures13, pipelineCreationCacheControl) &&
		X(requiredFeatures13, availableFeatures13, privateData) &&
		X(requiredFeatures13, availableFeatures13, shaderDemoteToHelperInvocation) &&
		X(requiredFeatures13, availableFeatures13, shaderTerminateInvocation) &&
		X(requiredFeatures13, availableFeatures13, subgroupSizeControl) &&
		X(requiredFeatures13, availableFeatures13, computeFullSubgroups) &&
		X(requiredFeatures13, availableFeatures13, synchronization2) &&
		X(requiredFeatures13, availableFeatures13, textureCompressionASTC_HDR) &&
		X(requiredFeatures13, availableFeatures13, shaderZeroInitializeWorkgroupMemory) &&
		X(requiredFeatures13, availableFeatures13, dynamicRendering) &&
		X(requiredFeatures13, availableFeatures13, shaderIntegerDotProduct) &&
		X(requiredFeatures13, availableFeatures13, maintenance4)
	);

	#undef X
}

[[nodiscard]] std::tuple<vk::UniqueInstance, bool> createInstance(const ware::config::State &config, [[maybe_unused]] ware::contextGLFW::State &glfw) {
	using namespace std::literals;

	// check GLFW support
	if ( ! ware::contextGLFW::isVulkanSupported(glfw)) {
		throw std::runtime_error{"Vulkan rendering requires surface"};
	}

	// check if required Vulkan version is available
	VULKAN_HPP_DEFAULT_DISPATCHER.init(ware::contextGLFW::loadVulkanGetInstanceProcAddr(glfw));
	if (VULKAN_HPP_DEFAULT_DISPATCHER.vkEnumerateInstanceVersion == nullptr || vk::enumerateInstanceVersion() < VK_API_VERSION_1_3) {
		throw std::runtime_error{"Vulkan 1.3 or later is required"};
	}

	// load available instance layers
	std::vector<std::string> availableLayers = util::map(vk::enumerateInstanceLayerProperties(), [] (auto &properties) {
		return std::string{properties.layerName};
	});

	// load available instance extensions
	std::vector<std::string> availableExtensions = util::map(vk::enumerateInstanceExtensionProperties(), [] (auto &properties) {
		return std::string{properties.extensionName};
	});

	// select layers
	std::vector<const char *> enabledLayers{};

	// enable validation
	if (config.vk.enableValidation && util::contains(availableLayers, "VK_LAYER_KHRONOS_validation"sv)) {
		enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
	}

	// select extensions
	std::vector<const char *> enabledExtensions{};

	// select surface extensions
	{
		auto extensions = ware::contextGLFW::getVulkanRequiredInstanceExtensions(glfw);
		enabledExtensions.assign(std::begin(extensions), std::end(extensions));
	}

	// enable debugging
	bool hasDebugUtilsExtension = false;
	if (config.vk.enableLogging && util::contains(availableExtensions, "VK_EXT_debug_utils"sv)) {
		enabledExtensions.push_back("VK_EXT_debug_utils");
		hasDebugUtilsExtension = true;
	}

	// enable validation layer features
	bool hasValidationFeaturesEnabled = false;
	if (config.vk.enableValidation && util::contains(availableExtensions, "VK_EXT_validation_features"sv)) {
		enabledExtensions.push_back("VK_EXT_validation_features");
		hasValidationFeaturesEnabled = true;
	}

	// set validation layer features
	std::vector<vk::ValidationFeatureEnableEXT> validationFeatureEnable{};
	if (hasValidationFeaturesEnabled) {
		validationFeatureEnable.assign({
			vk::ValidationFeatureEnableEXT::eGpuAssisted,
			vk::ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot,
			vk::ValidationFeatureEnableEXT::eBestPractices,
			vk::ValidationFeatureEnableEXT::eDebugPrintf,
			vk::ValidationFeatureEnableEXT::eSynchronizationValidation,
		});
	}
	std::vector<vk::ValidationFeatureDisableEXT> validationFeatureDisable{};

	spdlog::debug("ware::contextVK::createInstance() => enabling {} layer(s): {}", enabledLayers.size(), fmt::join(enabledLayers, ", "));
	spdlog::debug("ware::contextVK::createInstance() => enabling {} extension(s): {}", enabledExtensions.size(), fmt::join(enabledExtensions, ", "));

	// create instance
	vk::ApplicationInfo applicationInfo{
		.pApplicationName = config.vk.appName.data(),
		.applicationVersion = config.vk.appVersion,
		.pEngineName = config.vk.engineName.data(),
		.engineVersion = config.vk.engineVersion,
		.apiVersion = VK_API_VERSION_1_3
	};

	vk::StructureChain instanceCreateInfo{
		vk::InstanceCreateInfo{
			.flags = vk::InstanceCreateFlags{},
			.pApplicationInfo = &applicationInfo,
			.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size()),
			.ppEnabledLayerNames = enabledLayers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
			.ppEnabledExtensionNames = enabledExtensions.data()
		},
		vk::ValidationFeaturesEXT{
			.enabledValidationFeatureCount = static_cast<uint32_t>(validationFeatureEnable.size()),
			.pEnabledValidationFeatures = validationFeatureEnable.data(),
			.disabledValidationFeatureCount = static_cast<uint32_t>(validationFeatureDisable.size()),
			.pDisabledValidationFeatures = validationFeatureDisable.data(),
		}
	};

	vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo.get());

	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());

	return std::make_tuple(std::move(instance), hasDebugUtilsExtension);
}

[[nodiscard]] vk::UniqueDebugUtilsMessengerEXT createDebugUtilsMessanger(const ware::config::State &config, vk::Instance &instance) {
	using namespace std::literals;

	vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity{};
	switch (config.vk.logLevel) {
		case 0: {
			messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
			[[fallthrough]];
		}
		case 1: {
			messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
			[[fallthrough]];
		}
		case 2: {
			[[fallthrough]];
		}
		case 3: {
			messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
			[[fallthrough]];
		}
		case 4: {
			messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
		}
	}

	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{
		.messageSeverity = messageSeverity,
		.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		.pfnUserCallback = debugUtilsMessengerCallback,
		.pUserData = nullptr,
	};

	return instance.createDebugUtilsMessengerEXTUnique(debugUtilsMessengerCreateInfo);
}

[[nodiscard]] vk::UniqueSurfaceKHR createSurface(ware::windowGLFW::State &window, vk::Instance &instance) {
	return ware::windowGLFW::createVulkanSurface(window, instance);
}

std::string decodeVulkanDriverVersion(uint32_t vendorID, uint32_t driverVersion) {
	// from https://github.com/SaschaWillems/vulkan.gpuinfo.org/blob/1e6ca6e3c0763daabd6a101b860ab4354a07f5d3/functions.php#L294
	/*
	0x1002 - AMD
	0x1010 - ImgTec
	0x10de - NVIDIA
	0x13b5 - ARM
	0x5143 - Qualcomm
	0x8086 - INTEL
	*/

	// NVIDIA
	if (vendorID == 0x10de) {
		return fmt::format("{}.{}.{}.{}", ((driverVersion >> 22) & 0x3ff), ((driverVersion >> 14) & 0x0ff), ((driverVersion >> 6) & 0x0ff), (driverVersion & 0x003f));
	}

#if defined(_WIN32)
	// INTEL
	if (vendorID == 0x8086)
	{
		return fmt::format("{}.{}", (driverVersion >> 14), ((driverVersion) & 0x3fff));
	}
#endif

	// AMD and any other
	return fmt::format("{}.{}.{}", VK_API_VERSION_MAJOR(driverVersion), VK_API_VERSION_MINOR(driverVersion), VK_API_VERSION_PATCH(driverVersion));
}

[[nodiscard]] std::tuple<vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features>, vk::PhysicalDevice, vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>, vk::PhysicalDeviceMemoryProperties2, std::vector<vk::QueueFamilyProperties2>> selectPhysicalDevice(const ware::config::State &config, vk::Instance instance, vk::SurfaceKHR surface) {
	vk::StructureChain requiredFeatures{
		vk::PhysicalDeviceFeatures2{
			.features = {
				.independentBlend = true, // for separate color blending
				.sampleRateShading = true, // for multisampling
				.samplerAnisotropy = true, // for anisotropy
			},
		},
		vk::PhysicalDeviceVulkan12Features{
			.descriptorIndexing = true, // for bindless rendering
			.descriptorBindingUniformBufferUpdateAfterBind = true, // for bindless rendering
			.descriptorBindingSampledImageUpdateAfterBind = true, // for bindless rendering
			.descriptorBindingStorageImageUpdateAfterBind = true, // for bindless rendering
			.descriptorBindingStorageBufferUpdateAfterBind = true, // for bindless rendering
			.descriptorBindingUniformTexelBufferUpdateAfterBind = true, // for bindless rendering
			.descriptorBindingStorageTexelBufferUpdateAfterBind = true, // for bindless rendering
			.descriptorBindingUpdateUnusedWhilePending = true, // for bindless rendering
			.descriptorBindingPartiallyBound = true, // for bindless rendering
			.descriptorBindingVariableDescriptorCount = true, // for bindless rendering
			.runtimeDescriptorArray = true, // for bindless rendering
			.imagelessFramebuffer = true, // for simplified renderpass creation
			.timelineSemaphore = true, // for easier presentation sync
			.bufferDeviceAddress = true, // for VMA
		},
		vk::PhysicalDeviceVulkan13Features{
			.synchronization2 = true, // for simplified image layouts
			.dynamicRendering = true, // for rendering without renderpass/framebuffer
		},
	};

	std::vector<std::tuple<vk::PhysicalDevice, vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>, vk::PhysicalDeviceMemoryProperties2, std::vector<vk::QueueFamilyProperties2>>> physicalDeviceCandidates{};
	for (vk::PhysicalDevice physicalDevice : instance.enumeratePhysicalDevices()) {
		// reject devices that do not support Vulkan 1.3
		const vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
		if (properties.apiVersion < VK_API_VERSION_1_3) {
			continue;
		}

		// vkGetPhysicalDeviceProperties2 available on devices that support Vulkan 1.3 and above
		vk::StructureChain properties2 = physicalDevice.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>();
		if (properties2.get<vk::PhysicalDeviceProperties2>().properties.apiVersion < VK_API_VERSION_1_3) {
			continue;
		}

		// reject devices that do not support required features
		const auto availableFeatures = physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features>();
		if ( ! hasFeatures(requiredFeatures, availableFeatures)) {
			continue;
		}

		std::vector<vk::QueueFamilyProperties2> queueFamilyProperties2 = physicalDevice.getQueueFamilyProperties2();
		bool hasSurfaceSupport = false;
		for (uint32_t i = 0; i < queueFamilyProperties2.size(); i++) {
			if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface)) {
				hasSurfaceSupport = true;
				break;
			}
		}

		if ( ! hasSurfaceSupport) {
			continue;
		}

		bool hasGraphicQueue = false;
		for (const auto properties : queueFamilyProperties2) {
			if ((properties.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics) {
				hasGraphicQueue = true;
				break;
			}
		}

		if ( ! hasGraphicQueue) {
			continue;
		}

		const vk::PhysicalDeviceMemoryProperties2 memoryProperties = physicalDevice.getMemoryProperties2();

		physicalDeviceCandidates.emplace_back(physicalDevice, properties2, memoryProperties, std::move(queueFamilyProperties2));
	}

	for (const auto &candidate : physicalDeviceCandidates) {
		const auto &properties = std::get<vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>>(candidate).get<vk::PhysicalDeviceProperties2>().properties;
		const auto &driverProperties = std::get<vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>>(candidate).get<vk::PhysicalDeviceDriverProperties>();

		if ((config.vk.vendorId < 0 || static_cast<uint32_t>(config.vk.vendorId) == properties.vendorID) && (config.vk.deviceId < 0 || static_cast<uint32_t>(config.vk.deviceId) == properties.deviceID)) {
			spdlog::debug("ware::contextVK::selectPhysicalDevice() => selected device: {}, vendor ID: {}, device ID: {}, Vulkan driver: {}, device driver: {} {} ({})", properties.deviceName, properties.vendorID, properties.deviceID, decodeVulkanDriverVersion(properties.vendorID, properties.driverVersion), vk::to_string(driverProperties.driverID), driverProperties.driverInfo, driverProperties.driverName);

			return std::tuple_cat(std::make_tuple(requiredFeatures), candidate);
		}
	}

	if ( ! physicalDeviceCandidates.empty()) {
		const auto &properties = std::get<vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>>(physicalDeviceCandidates[0]).get<vk::PhysicalDeviceProperties2>().properties;
		const auto &driverProperties = std::get<vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDriverProperties>>(physicalDeviceCandidates[0]).get<vk::PhysicalDeviceDriverProperties>();

		spdlog::debug("ware::contextVK::selectPhysicalDevice() => selected device: {}, vendor ID: {}, device ID: {}, Vulkan driver: {}, device driver: {} {} ({})", properties.deviceName, properties.vendorID, properties.deviceID, decodeVulkanDriverVersion(properties.vendorID, properties.driverVersion), vk::to_string(driverProperties.driverID), driverProperties.driverInfo, driverProperties.driverName);

		return std::tuple_cat(std::make_tuple(requiredFeatures), physicalDeviceCandidates[0]);
	}

	throw std::runtime_error{"No valid physical devices available"};
}

struct QueueSource {
	int32_t family;
	int32_t index;

	friend auto operator<=>(const QueueSource &a, const QueueSource &b) = default;
};

QueueSource reserveQueueSourceFromPreferred(std::tuple<int32_t, int32_t> preferred, std::vector<std::vector<uint32_t>> &reservations) {
	QueueSource selection{
		.family = -1,
		.index = -1,
	};

	auto [preferredFamily, preferredIndex] = preferred;
	if (preferredFamily >= 0 && preferredFamily < static_cast<int32_t>(reservations.size())) {
		selection.family = preferredFamily;

		if (preferredIndex >= 0 && preferredIndex < static_cast<int32_t>(reservations[preferredFamily].size())) {
			selection.index = preferredIndex;

			reservations[preferredFamily][preferredIndex]++;
		}
	}

	return selection;
}

bool matchQueueFlags(vk::QueueFlags flags, vk::QueueFlags requiredFlags, vk::QueueFlags notAllowedFlags) {
	return (flags & requiredFlags) == requiredFlags && (flags & notAllowedFlags) == vk::QueueFlags{};
}

struct QueueRequirements {
	vk::QueueFlags requiredFlags;
	vk::QueueFlags notAllowedFlags;
	vk::Extent3D granularity = { 0, 0, 0 };
};

int32_t findMatchingPresentationQueueFamily(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, const std::vector<vk::QueueFamilyProperties2> &queueFamilyProperties2, const std::vector<QueueRequirements> &requirements) {
	for (const auto &requirement : requirements) {
		for (int32_t family = 0; family < static_cast<int32_t>(queueFamilyProperties2.size()); family++) {
			const auto &properties = queueFamilyProperties2[family].queueFamilyProperties;

			if (matchQueueFlags(properties.queueFlags, requirement.requiredFlags, requirement.notAllowedFlags) && physicalDevice.getSurfaceSupportKHR(family, surface)) {
				return family;
			}
		}
	}

	return -1;
}

int32_t findMatchingGeneralQueueFamily(const std::vector<vk::QueueFamilyProperties2> &queueFamilyProperties2, const std::vector<QueueRequirements> &requirements) {
	for (const auto &requirement : requirements) {
		for (int32_t family = 0; family < static_cast<int32_t>(queueFamilyProperties2.size()); family++) {
			const auto &properties = queueFamilyProperties2[family].queueFamilyProperties;

			if (matchQueueFlags(properties.queueFlags, requirement.requiredFlags, requirement.notAllowedFlags)) {
				return family;
			}
		}
	}

	return -1;
}

int32_t findMatchingTransferQueueFamily(const std::vector<vk::QueueFamilyProperties2> &queueFamilyProperties2, const std::vector<QueueRequirements> &requirements) {
	for (const auto &requirement : requirements) {
		for (int32_t family = 0; family < static_cast<int32_t>(queueFamilyProperties2.size()); family++) {
			const auto &properties = queueFamilyProperties2[family].queueFamilyProperties;

			if (matchQueueFlags(properties.queueFlags, requirement.requiredFlags, requirement.notAllowedFlags) && (requirement.granularity == vk::Extent3D{ 0, 0, 0 } || requirement.granularity <= properties.minImageTransferGranularity)) {
				return family;
			}
		}
	}

	return -1;
}

int32_t reserveLeastOccupiedQueueIndex(std::vector<uint32_t> &familyReservations) {
	int32_t index = static_cast<int32_t>(std::distance(std::begin(familyReservations), std::min_element(std::begin(familyReservations), std::end(familyReservations))));
	familyReservations[index]++;

	return index;
}

struct QueueSources {
	QueueSource presentation;
	QueueSource graphic;
	QueueSource compute;
	QueueSource transfer;
};

[[nodiscard]] QueueSources chooseQueueSources(const ware::config::State &config, vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, const std::vector<vk::QueueFamilyProperties2> &queueFamilyProperties2) {
	std::vector<std::vector<uint32_t>> reservations = util::map(queueFamilyProperties2, [] (const auto &entry) {
		return std::vector<uint32_t>(entry.queueFamilyProperties.queueCount, 0);
	});

	// select families and indices from configuration
	QueueSources queueSources{
		.presentation = reserveQueueSourceFromPreferred(config.vk.presentationQueue, reservations),
		.graphic = reserveQueueSourceFromPreferred(config.vk.graphicQueue, reservations),
		.compute = reserveQueueSourceFromPreferred(config.vk.computeQueue, reservations),
		.transfer = reserveQueueSourceFromPreferred(config.vk.transferQueue, reservations),
	};

	// select families
	if (queueSources.presentation.family < 0) {
		std::vector<QueueRequirements> requirements{
			{ vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer },
			{ vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer },
			{ vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer, vk::QueueFlags{} },
			{ vk::QueueFlagBits::eGraphics, vk::QueueFlags{} },
			{ vk::QueueFlagBits::eTransfer, vk::QueueFlags{} },
			{ vk::QueueFlagBits::eCompute, vk::QueueFlags{} },
			{ vk::QueueFlags{}, vk::QueueFlags{} },
		};

		if (auto family = findMatchingPresentationQueueFamily(surface, physicalDevice, queueFamilyProperties2, requirements); family >= 0) {
			queueSources.presentation.family = family;

			const auto &properties = queueFamilyProperties2[family].queueFamilyProperties;
			if ((properties.queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics && queueSources.graphic.family < 0) {
				queueSources.graphic.family = family;
			}
		} else {
			throw std::runtime_error{"No valid presentation queue family found"};
		}
	}

	if (queueSources.graphic.family < 0) {
		std::vector<QueueRequirements> requirements{
			{ vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer },
			{ vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer },
			{ vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eCompute },
			{ vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer, vk::QueueFlags{} },
			{ vk::QueueFlagBits::eGraphics, vk::QueueFlags{} },
		};

		if (auto family = findMatchingGeneralQueueFamily(queueFamilyProperties2, requirements); family >= 0) {
			queueSources.graphic.family = family;
		} else {
			throw std::runtime_error{"No valid graphic queue family found"};
		}
	}

	if (queueSources.compute.family < 0) {
		std::vector<QueueRequirements> requirements{
			{ vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer },
			{ vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics },
			{ vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eTransfer },
			{ vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eGraphics, vk::QueueFlags{} },
			{ vk::QueueFlagBits::eCompute, vk::QueueFlags{} },
			{ vk::QueueFlagBits::eGraphics, vk::QueueFlags{} }, // any queue that supports graphic, must support compute
		};

		if (auto family = findMatchingGeneralQueueFamily(queueFamilyProperties2, requirements); family >= 0) {
			queueSources.compute.family = family;
		} else {
			throw std::runtime_error{"No valid compute queue family found"};
		}
	}

	if (queueSources.transfer.family < 0) {
		std::vector<QueueRequirements> requirements{
			{ vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute, { 1, 1, 1 } },
			{ vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eGraphics, { 1, 1, 1 } },
			{ vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute, { 1, 1, 1 } },
			{ vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eGraphics, vk::QueueFlags{}, { 1, 1, 1 } },
			{ vk::QueueFlagBits::eTransfer, vk::QueueFlags{}, { 1, 1, 1 } },
			{ vk::QueueFlagBits::eTransfer, vk::QueueFlags{}, { 0, 0, 0 } },
			{ vk::QueueFlagBits::eGraphics, vk::QueueFlags{}, { 0, 0, 0 } }, // any queue that supports graphic, must support transfer
		};

		if (auto family = findMatchingTransferQueueFamily(queueFamilyProperties2, requirements); family >= 0) {
			queueSources.transfer.family = family;
		} else {
			throw std::runtime_error{"No valid transfer queue family found"};
		}
	}

	// select indices
	if (queueSources.presentation.index < 0) {
		queueSources.presentation.index = reserveLeastOccupiedQueueIndex(reservations[queueSources.presentation.family]);
	}

	if (queueSources.graphic.index < 0) {
		queueSources.graphic.index = reserveLeastOccupiedQueueIndex(reservations[queueSources.graphic.family]);
	}

	if (queueSources.compute.index < 0) {
		queueSources.compute.index = reserveLeastOccupiedQueueIndex(reservations[queueSources.compute.family]);
	}

	if (queueSources.transfer.index < 0) {
		queueSources.transfer.index = reserveLeastOccupiedQueueIndex(reservations[queueSources.transfer.family]);
	}

	return queueSources;
}

void upsertQueueCreateInfo(std::vector<vk::DeviceQueueCreateInfo> &queueCreateInfos, uint32_t family, uint32_t index) {
	for (auto &queueCreateInfo : queueCreateInfos) {
		if (queueCreateInfo.queueFamilyIndex == family) {
			queueCreateInfo.queueCount = std::max(queueCreateInfo.queueCount, index + 1);

			return;
		}
	}

	queueCreateInfos.push_back(vk::DeviceQueueCreateInfo{
		.queueFamilyIndex = family,
		.queueCount = index + 1,
	});
}

void distributeQueueCreateInfoPriorities(std::vector<vk::DeviceQueueCreateInfo> &queueCreateInfos, std::vector<float> &priorities) {
	size_t count = std::accumulate(std::begin(queueCreateInfos), std::end(queueCreateInfos), 0, [] (size_t acc, const auto &queueCreateInfo) {
		return acc + queueCreateInfo.queueCount;
	});

	priorities.resize(count, 0.0f);

	size_t offset = 0;
	for (auto &queueCreateInfo : queueCreateInfos) {
		queueCreateInfo.pQueuePriorities = std::next(priorities.data(), static_cast<ptrdiff_t>(offset));

		for (size_t i = 0; i < queueCreateInfo.queueCount; i++) {
			priorities[offset + i] = 1.0f / static_cast<float>(queueCreateInfo.queueCount);
		}

		offset += queueCreateInfo.queueCount;
	}
}

[[nodiscard]] std::tuple<std::vector<vk::DeviceQueueCreateInfo>, std::vector<float>> buildQueueCreateInfos(const QueueSources &queueSources) {
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};
	upsertQueueCreateInfo(queueCreateInfos, static_cast<uint32_t>(queueSources.presentation.family), static_cast<uint32_t>(queueSources.presentation.index));
	upsertQueueCreateInfo(queueCreateInfos, static_cast<uint32_t>(queueSources.graphic.family), static_cast<uint32_t>(queueSources.graphic.index));
	upsertQueueCreateInfo(queueCreateInfos, static_cast<uint32_t>(queueSources.compute.family), static_cast<uint32_t>(queueSources.compute.index));
	upsertQueueCreateInfo(queueCreateInfos, static_cast<uint32_t>(queueSources.transfer.family), static_cast<uint32_t>(queueSources.transfer.index));

	std::vector<float> priorities{};
	distributeQueueCreateInfoPriorities(queueCreateInfos, priorities);

	return { std::move(queueCreateInfos), std::move(priorities) };
}

[[nodiscard]] std::tuple<vk::UniqueDevice, bool, bool, bool> createDevice(const vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features> &features, vk::PhysicalDevice physicalDevice, const QueueSources &queueSources) {
	using namespace std::literals;

	const auto infoTuple = buildQueueCreateInfos(queueSources);
	auto &queueCreateInfos = std::get<std::vector<vk::DeviceQueueCreateInfo>>(infoTuple);

	// load available device extensions
	std::vector<std::string> availableExtensions = util::map(physicalDevice.enumerateDeviceExtensionProperties(), [] (auto &properties) {
		return std::string{properties.extensionName};
	});

	std::vector<const char *> enabledExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// for VMA
	bool hasMemoryBudgetExtension = false;
	if (util::contains(availableExtensions, "VK_EXT_memory_budget"sv)) {
		enabledExtensions.push_back("VK_EXT_memory_budget");
		hasMemoryBudgetExtension = true;
	}

	// for VMA
	bool hasMemoryPriorityExtension = false;
	if (util::contains(availableExtensions, "VK_EXT_memory_priority"sv)) {
		enabledExtensions.push_back("VK_EXT_memory_priority");
		hasMemoryPriorityExtension = true;
	}

	// for VMA
	bool hasAmdDeviceCoherentMemoryExtension = false;
	if (util::contains(availableExtensions, "VK_AMD_device_coherent_memory"sv)) {
		enabledExtensions.push_back("VK_AMD_device_coherent_memory");
		hasAmdDeviceCoherentMemoryExtension = true;
	}

	spdlog::debug("ware::contextVK::createDevice() => enabling {} extension(s): {}", enabledExtensions.size(), fmt::join(enabledExtensions, ", "));

	vk::StructureChain deviceCreateInfoChain{
		vk::DeviceCreateInfo{
			.flags = vk::DeviceCreateFlags{},
			.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
			.pQueueCreateInfos = queueCreateInfos.data(),
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
			.ppEnabledExtensionNames = enabledExtensions.data(),
			.pEnabledFeatures = nullptr,
		},
		features.get<vk::PhysicalDeviceFeatures2>()
	};

	auto device = physicalDevice.createDeviceUnique(deviceCreateInfoChain.get());

	VULKAN_HPP_DEFAULT_DISPATCHER.init(device.get());

	return { std::move(device), hasMemoryBudgetExtension, hasMemoryPriorityExtension, hasAmdDeviceCoherentMemoryExtension };
}

vk::Queue retrievQueue(vk::Device device, std::vector<std::tuple<QueueSource, vk::Queue>> &retrievedQueues, QueueSource queueSource) {
	for (auto [retrievedQueueSource, handle] : retrievedQueues) {
		if (retrievedQueueSource == queueSource) {
			return handle;
		}
	}

	auto handle = device.getQueue2({
		.queueFamilyIndex = static_cast<uint32_t>(queueSource.family),
		.queueIndex = static_cast<uint32_t>(queueSource.index),
	});

	retrievedQueues.push_back({ queueSource, handle });

	return handle;
}

[[nodiscard]] std::tuple<vk::Queue, vk::Queue, vk::Queue, vk::Queue> selectQueues(vk::Device device, const QueueSources &queueSources) {
	std::vector<std::tuple<QueueSource, vk::Queue>> retrievedQueues{};

	vk::Queue presentation = retrievQueue(device, retrievedQueues, queueSources.presentation);
	vk::Queue graphic = retrievQueue(device, retrievedQueues, queueSources.graphic);
	vk::Queue compute = retrievQueue(device, retrievedQueues, queueSources.compute);
	vk::Queue transfer = retrievQueue(device, retrievedQueues, queueSources.transfer);

	return { presentation, graphic, compute, transfer };
}

[[nodiscard]] util::UniqueResource<VmaAllocator> createAllocator(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device, bool hasMemoryBudgetExtension, bool hasMemoryPriorityExtension, bool hasAmdDeviceCoherentMemoryExtension) {
	VmaVulkanFunctions vulkanFunctions{
		.vkGetInstanceProcAddr = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetDeviceProcAddr,
		.vkGetPhysicalDeviceProperties = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceProperties,
		.vkGetPhysicalDeviceMemoryProperties = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties,
		.vkAllocateMemory = VULKAN_HPP_DEFAULT_DISPATCHER.vkAllocateMemory,
		.vkFreeMemory = VULKAN_HPP_DEFAULT_DISPATCHER.vkFreeMemory,
		.vkMapMemory = VULKAN_HPP_DEFAULT_DISPATCHER.vkMapMemory,
		.vkUnmapMemory = VULKAN_HPP_DEFAULT_DISPATCHER.vkUnmapMemory,
		.vkFlushMappedMemoryRanges = VULKAN_HPP_DEFAULT_DISPATCHER.vkFlushMappedMemoryRanges,
		.vkInvalidateMappedMemoryRanges = VULKAN_HPP_DEFAULT_DISPATCHER.vkInvalidateMappedMemoryRanges,
		.vkBindBufferMemory = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory,
		.vkBindImageMemory = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory,
		.vkGetBufferMemoryRequirements = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements,
		.vkGetImageMemoryRequirements = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements,
		.vkCreateBuffer = VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateBuffer,
		.vkDestroyBuffer = VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyBuffer,
		.vkCreateImage = VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateImage,
		.vkDestroyImage = VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyImage,
		.vkCmdCopyBuffer = VULKAN_HPP_DEFAULT_DISPATCHER.vkCmdCopyBuffer,
		.vkGetBufferMemoryRequirements2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements2KHR,
		.vkGetImageMemoryRequirements2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements2KHR,
		.vkBindBufferMemory2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory2KHR,
		.vkBindImageMemory2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory2KHR,
		.vkGetPhysicalDeviceMemoryProperties2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties2KHR,
		.vkGetDeviceBufferMemoryRequirements = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetDeviceBufferMemoryRequirements,
		.vkGetDeviceImageMemoryRequirements = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetDeviceImageMemoryRequirements,
	};

	VmaAllocatorCreateInfo allocatorCreateInfo{
		.flags = static_cast<VmaAllocatorCreateFlags>(
			VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT
			| (hasMemoryBudgetExtension ? VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT : 0)
			| (hasMemoryPriorityExtension ? VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT : 0)
			| (hasAmdDeviceCoherentMemoryExtension ? VMA_ALLOCATOR_CREATE_AMD_DEVICE_COHERENT_MEMORY_BIT : 0)
		),
		.physicalDevice = static_cast<VkPhysicalDevice>(physicalDevice),
		.device = static_cast<VkDevice>(device),
		.preferredLargeHeapBlockSize = 256 * 1024 * 1024,
		.pAllocationCallbacks = nullptr,
		.pDeviceMemoryCallbacks = nullptr,
		.pHeapSizeLimit = 0,
		.pVulkanFunctions = &vulkanFunctions,
		.instance = static_cast<VkInstance>(instance),
		.vulkanApiVersion = VK_API_VERSION_1_3,
		.pTypeExternalMemoryHandleTypes = nullptr,
	};

	VmaAllocator allocator;
	vk::Result result = static_cast<vk::Result>(vmaCreateAllocator(&allocatorCreateInfo, &allocator));
	if (result != vk::Result::eSuccess) {
		throw std::runtime_error{fmt::format("Vulkan allocator could not be created (result: {})", vk::to_string(result))};
	}

	return util::UniqueResource<VmaAllocator>{std::move(allocator), vmaDestroyAllocator};
}

vk::UniquePipelineCache createPipelineCache(vk::Device device) {
	return device.createPipelineCacheUnique({
		.initialDataSize = 0,
		.pInitialData = nullptr,
	});
}

void requestWaitIdle(State &context) {
	if (context.requestedWaitIdle) {
		return;
	}

	spdlog::debug("ware::contextVK::requestWaitIdle() => waiting");
	context.device->waitIdle();
	context.presentationQueue.waitIdle();
	context.requestedWaitIdle = true;
}

void destroyBuffer(BufferState &state) {
	vmaDestroyBuffer(*state.allocator, static_cast<VkBuffer>(state.buffer), state.allocation);
}

UniqueBuffer createBuffer(State &context, vk::BufferCreateInfo &bufferCreateInfo, vma::AllocationCreateInfo &allocationCreateInfo) {
	vk::Buffer buffer{};
	VmaAllocation allocation{};
	VmaAllocationInfo allocationInfo{};

	vmaCreateBuffer(context.allocator.get(), reinterpret_cast<VkBufferCreateInfo *>(&bufferCreateInfo), reinterpret_cast<VmaAllocationCreateInfo *>(&allocationCreateInfo), reinterpret_cast<VkBuffer *>(&buffer), &allocation, &allocationInfo);

	return UniqueBuffer{
		BufferState{
			.buffer = buffer,
			.memory = static_cast<vk::DeviceMemory>(allocationInfo.deviceMemory),
			.offset = allocationInfo.offset,
			.size = allocationInfo.size,
			.mappedData = allocationInfo.pMappedData,
			.allocation = allocation,
			.allocator = &context.allocator.get()
		},
		destroyBuffer
	};
}

void destroyImage(ImageState &state) {
	vmaDestroyImage(*state.allocator, static_cast<VkImage>(state.image), state.allocation);
}

UniqueImage createImage(State &context, vk::ImageCreateInfo &imageCreateInfo, vma::AllocationCreateInfo &allocationCreateInfo) {
	vk::Image image{};
	VmaAllocation allocation{};
	VmaAllocationInfo allocationInfo{};

	vmaCreateImage(context.allocator.get(), reinterpret_cast<VkImageCreateInfo *>(&imageCreateInfo), reinterpret_cast<VmaAllocationCreateInfo *>(&allocationCreateInfo), reinterpret_cast<VkImage *>(&image), &allocation, &allocationInfo);

	return UniqueImage{
		ImageState{
			.image = image,
			.format = imageCreateInfo.format,
			.extent = imageCreateInfo.extent,
			.memory = static_cast<vk::DeviceMemory>(allocationInfo.deviceMemory),
			.offset = allocationInfo.offset,
			.size = allocationInfo.size,
			.mappedData = allocationInfo.pMappedData,
			.allocation = allocation,
			.allocator = &context.allocator.get()
		},
		destroyImage
	};
}

void flushMappedData(UniqueBuffer &buffer, vk::DeviceSize offset, vk::DeviceSize size) {
	if (size == VK_WHOLE_SIZE) {
		size = buffer->size - std::min(buffer->size, offset);
	}

	vmaFlushAllocation(*buffer->allocator, buffer->allocation, offset, size);
}

void flushMappedData(UniqueImage &image, vk::DeviceSize offset, vk::DeviceSize size) {
	if (size == VK_WHOLE_SIZE) {
		size = image->size - std::min(image->size, offset);
	}

	vmaFlushAllocation(*image->allocator, image->allocation, offset, size);
}

State setup(ware::config::State &config, [[maybe_unused]] ware::contextGLFW::State &glfw, ware::windowGLFW::State &window) {
	auto [instance, hasDebugUtilsExtension] = createInstance(config, glfw);

	vk::UniqueDebugUtilsMessengerEXT debugUtilsMessanger{};
	if (hasDebugUtilsExtension) {
		debugUtilsMessanger = createDebugUtilsMessanger(config, instance.get());
	}

	auto surface = createSurface(window, instance.get());

	auto [features, physicalDevice, physicalDeviceProperties2, physicalDeviceMemoryProperties2, queueFamilyProperties2] = selectPhysicalDevice(config, instance.get(), surface.get());

	auto queueSources = chooseQueueSources(config, surface.get(), physicalDevice, queueFamilyProperties2);

	auto [device, hasMemoryBudgetExtension, hasMemoryPriorityExtension, hasAmdDeviceCoherentMemoryExtension] = createDevice(features, physicalDevice, queueSources);

	auto [presentation, graphic, compute, transfer] = selectQueues(device.get(), queueSources);

	auto allocator = createAllocator(instance.get(), physicalDevice, device.get(), hasMemoryBudgetExtension, hasMemoryPriorityExtension, hasAmdDeviceCoherentMemoryExtension);

	auto pipelineCache = createPipelineCache(device.get());

	return State{
		.instance = std::move(instance),
		.debugUtilsMessanger = std::move(debugUtilsMessanger),
		.surface = std::move(surface),
		.features = features,
		.physicalDevice = std::move(physicalDevice),
		.physicalDeviceProperties2 = physicalDeviceProperties2.get<vk::PhysicalDeviceProperties2>(),
		.physicalDeviceDriverProperties = physicalDeviceProperties2.get<vk::PhysicalDeviceDriverProperties>(),
		.physicalDeviceMemoryProperties2 = std::move(physicalDeviceMemoryProperties2),
		.queueFamilyProperties2 = std::move(queueFamilyProperties2),
		.device = std::move(device),
		.presentationQueue = presentation,
		.presentationQueueFamily = static_cast<uint32_t>(queueSources.presentation.family),
		.presentationQueueIndex = static_cast<uint32_t>(queueSources.presentation.index),
		.graphicQueue = graphic,
		.graphicQueueFamily = static_cast<uint32_t>(queueSources.graphic.family),
		.graphicQueueIndex = static_cast<uint32_t>(queueSources.graphic.index),
		.computeQueue = compute,
		.computeQueueFamily = static_cast<uint32_t>(queueSources.compute.family),
		.computeQueueIndex = static_cast<uint32_t>(queueSources.compute.index),
		.transferQueue = transfer,
		.transferQueueFamily = static_cast<uint32_t>(queueSources.transfer.family),
		.transferQueueIndex = static_cast<uint32_t>(queueSources.transfer.index),
		.allocator = std::move(allocator),
		.pipelineCache = std::move(pipelineCache),
		.requestedWaitIdle = false,
	};
}

void refresh([[maybe_unused]] State &state) {
	ZoneScopedN("ware::contextVK::refresh()");

	if (state.requestedWaitIdle) {
		state.requestedWaitIdle = false;
	}
}

void process([[maybe_unused]] State &state) {
	ZoneScopedN("ware::contextVK::process()");

	if (state.requestedWaitIdle) {
		state.requestedWaitIdle = false;
	}
}

} // ware::contextVK
