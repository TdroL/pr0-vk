#pragma once

#include <vector>
#include <set>
#include <string>
#include <cassert>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>

#include <vulkan/vulkan.h>

#include "VulkanDispatch.hpp"
#include "util/countOf.hpp"
#include "ngn/fs.hpp"

#define CHECK_RESULT(err, message) do { if (err != VK_SUCCESS) { return Vulkan::Result{ false, err, #message }; } } while(0)

class Vulkan {
public:
	VkInstance instance = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkDebugReportCallbackEXT debugReportCallback = VK_NULL_HANDLE;

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> swapchainImages{};
	std::vector<VkImageView> swapchainImageViews{};
	std::vector<VkFramebuffer> swapchainFrameBuffers{};
	std::vector<bool> swapchainImagesStatus{};

	VkSemaphore surfaceImageAvailableSemaphore = VK_NULL_HANDLE;
	VkSemaphore submitFinishedSemaphore = VK_NULL_HANDLE;

	VkCommandPool graphicCommandPool = VK_NULL_HANDLE;
	VkCommandPool presentCommandPool = VK_NULL_HANDLE;

	std::vector<VkFence> presentFences{};
	std::vector<VkFence> graphicFences{};

	std::vector<VkCommandBuffer> presentCommandBuffers{};
	std::vector<VkCommandBuffer> graphicCommandBuffers{};

	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;

	VkExtent2D surfaceExtent{};
	VkSurfaceFormatKHR surfaceFormat{};
	VkPhysicalDeviceMemoryProperties memoryProperties{};

	std::vector<const char *> desiredEnabledLayers{
		// "VK_LAYER_GOOGLE_threading"
		// "VK_LAYER_LUNARG_parameter_validation"
		// "VK_LAYER_LUNARG_device_limits"
		// "VK_LAYER_LUNARG_object_tracker"
		// "VK_LAYER_LUNARG_image"
		// "VK_LAYER_LUNARG_core_validation"
		// "VK_LAYER_LUNARG_swapchain"
		// "VK_LAYER_GOOGLE_unique_objects"
		"VK_LAYER_LUNARG_standard_validation"
	};

	std::vector<const char *> desiredInstanceExtensions{
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	std::vector<const char *> desiredDeviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	std::vector<const char *> enabledLayers{};
	std::vector<const char *> instanceExtensions{};
	std::vector<const char *> deviceExtensions{};

	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos{};

	struct QueueFamilyIndices {
		uint32_t present = std::numeric_limits<uint32_t>::max();
		uint32_t graphic = std::numeric_limits<uint32_t>::max();
		uint32_t compute = std::numeric_limits<uint32_t>::max();
	};
	QueueFamilyIndices queueFamilyIndices{};

	struct Queues {
		std::vector<std::vector<VkQueue>> list{};
		size_t presentIndex = std::numeric_limits<size_t>::max();
		size_t graphicIndex = std::numeric_limits<size_t>::max();
		size_t computeIndex = std::numeric_limits<size_t>::max();

		std::vector<VkQueue> & present() {
			assert(presentIndex < list.size());
			return list[presentIndex];
		}
		std::vector<VkQueue> & graphic() {
			assert(graphicIndex < list.size());
			return list[graphicIndex];
		}
		std::vector<VkQueue> & compute() {
			assert(computeIndex < list.size());
			return list[computeIndex];
		}
	};
	Queues queues;

	enum class PresentMode {
		NoVSync,
		TripleBuffer,
		VSync,
		VSyncAdap
	};

	struct Vertex {
		float position[3];
		float color[3];
	};

	struct StagingBuffer {
		VkDeviceMemory memory = VK_NULL_HANDLE;
		VkBuffer buffer = VK_NULL_HANDLE;
	};

	struct StagingBuffers {
		StagingBuffer vertices{};
		StagingBuffer indices{};
	} stagingBuffers;

	struct VertexBuffer {
		VkDeviceMemory memory = VK_NULL_HANDLE;
		VkBuffer buffer = VK_NULL_HANDLE;
	};

	struct VertexBuffers {
		VertexBuffer vertices{};
		VertexBuffer indices{};
	} vertexBuffers;

	struct Result {
		bool status = true;
		VkResult err = VK_SUCCESS;
		const char *message = nullptr;

		operator bool() const {
			return status;
		}
	};

	VulkanDispatch vd;

	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;

	explicit Vulkan(std::function<void *()> vkGetInstanceProcAddrLoader)
		: vd{vkGetInstanceProcAddrLoader}, gen(rd()), dis(0.f, 1.f) {
		vd.loadGlobalVulkanFunctions();
	}

	Result createInstance(const char **externalExtensions, uint32_t externalExtensionCount) {
		VkResult err;
		Result result;

		std::vector<VkExtensionProperties> extensionProperties = fetchInstanceExtensionProperties(result);
		if ( ! result) {
			return result;
		}

		std::vector<VkLayerProperties> layers = fetchInstanceLayerProperties(result);
		if ( ! result) {
			return result;
		}

		std::set<std::string> layerSet{};
		for (size_t i = 0; i < layers.size(); i++) {
			layerSet.emplace(layers[i].layerName);
		}

		enabledLayers.clear();
		for (const char *layer : desiredEnabledLayers) {
			if (layerSet.find(std::string{layer}) != end(layerSet)) {
				enabledLayers.push_back(layer);
			} else {
				std::cout << "Warning: layer " << layer << " not enabled" << std::endl;
			}
		}

		instanceExtensions.clear();
		instanceExtensions.insert(end(instanceExtensions), externalExtensions, externalExtensions + externalExtensionCount);
		instanceExtensions.insert(end(instanceExtensions), begin(desiredInstanceExtensions), end(desiredInstanceExtensions));

		for (auto &layer : enabledLayers) {
			std::cout << "Enabled layer " << layer << std::endl;
		}

		VkApplicationInfo applicationInfo {
			/*.sType =*/              VK_STRUCTURE_TYPE_APPLICATION_INFO,
			/*.pNext =*/              nullptr,
			/*.pApplicationName =*/   "pr0-vk",
			/*.applicationVersion =*/ VK_MAKE_VERSION(0, 0, 0),
			/*.pEngineName =*/        "ngn",
			/*.engineVersion =*/      VK_MAKE_VERSION(0, 0, 0),
			/*.apiVersion =*/         VK_API_VERSION_1_0
		};

		VkInstanceCreateInfo createInfo {
			/*.sType =*/                   VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			/*.pNext =*/                   nullptr,
			/*.flags =*/                   0,
			/*.pApplicationInfo =*/        &applicationInfo,
			/*.enabledLayerCount =*/       static_cast<uint32_t>(enabledLayers.size()),
			/*.ppEnabledLayerNames =*/     enabledLayers.data(),
			/*.enabledExtensionCount =*/   static_cast<uint32_t>(instanceExtensions.size()),
			/*.ppEnabledExtensionNames =*/ instanceExtensions.data()
		};

		VkInstance instance;
		err = vd.vkCreateInstance(&createInfo, nullptr, &instance);
		CHECK_RESULT(err, "vkCreateInstance");

		this->instance = instance;

		vd.loadInstanceVulkanFunctions(instance);

		return { true };
	}

	void destroyInstance() {
		if (instance != VK_NULL_HANDLE) {
			vd.vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
			debugReportCallback = VK_NULL_HANDLE;

			vd.vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;

			vd.unloadInstanceVulkanFunctions();
		}
	}

	Result createSurface(std::function<Result (VkInstance, VkSurfaceKHR &)> createWindowSurface) {
		Result result;
		VkSurfaceKHR surface;

		assert(instance != VK_NULL_HANDLE);

		result = createWindowSurface(instance, surface);
		if ( ! result) {
			return result;
		}

		this->surface = surface;

		return { true };
	}

	void destroySurface() {
		assert(instance != VK_NULL_HANDLE);

		if (surface != VK_NULL_HANDLE) {
			vd.vkDestroySurfaceKHR(instance, surface, nullptr);
			surface = VK_NULL_HANDLE;
		}
	}

	VkPhysicalDevice selectPhysicalDevice(const char *deviceName = nullptr) {
		assert(surface != VK_NULL_HANDLE);

		std::vector<VkPhysicalDevice> physicalDevices = fetchPhysicalDevices();

		for (const VkPhysicalDevice &gpu : physicalDevices) {
			VkPhysicalDeviceProperties gpuProperties;
			vd.vkGetPhysicalDeviceProperties(gpu, &gpuProperties);

			if (deviceName == nullptr || std::strncmp(deviceName, gpuProperties.deviceName, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE) == 0) {
				uint32_t queueFamilyPropertyCount;
				vd.vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyPropertyCount, nullptr);

				VkBool32 supportsSurface = false;
				for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
					VkResult err = vd.vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &supportsSurface);
					if (err != VK_SUCCESS) {
						continue;
					}

					if (supportsSurface) {
						return gpu;
					}
				}
			}
		}

		return VK_NULL_HANDLE;
	}

	VkPhysicalDevice selectAnyPhysicalDevice() {
		return selectPhysicalDevice(nullptr);
	}

	Result attachDebugCallback(uint32_t logLevel, PFN_vkDebugReportCallbackEXT debugCallback) {
		VkResult err;

		assert(instance != VK_NULL_HANDLE);

		int32_t flags = 0;
		switch (logLevel) {
			case 0: flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;
			case 1: flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
			case 2: flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT;
			case 3: flags |= VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			case 4: flags |= VK_DEBUG_REPORT_ERROR_BIT_EXT;
		}

		VkDebugReportCallbackCreateInfoEXT callbackCreateInfo {
			/*.sType =*/       VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
			/*.pNext =*/       nullptr,
			/*.flags =*/       static_cast<VkDebugReportFlagBitsEXT>(flags),
			/*.pfnCallback =*/ debugCallback,
			/*.pUserData =*/   nullptr
		};

		err = vd.vkCreateDebugReportCallbackEXT(instance, &callbackCreateInfo, nullptr, &debugReportCallback);
		CHECK_RESULT(err, "vkCreateDebugReportCallbackEXT");

		return { true };
	}

	QueueFamilyIndices findQueueFamilyIndices(std::vector<VkQueueFamilyProperties> &queueFamilyProperties) {
		QueueFamilyIndices indices;

		assert(instance != VK_NULL_HANDLE);
		assert(physicalDevice != VK_NULL_HANDLE);
		assert(surface != VK_NULL_HANDLE);

		assert(queueFamilyProperties.size() <= std::numeric_limits<uint32_t>::max());

		for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
			VkQueueFamilyProperties &family = queueFamilyProperties[i];

			if (family.queueCount == 0) {
				continue;
			}

			// select first graphic queue family
			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT && indices.graphic == std::numeric_limits<uint32_t>::max()) {
				indices.graphic = i;
			}

			// select first compute queue family
			if (family.queueFlags & VK_QUEUE_COMPUTE_BIT && indices.compute == std::numeric_limits<uint32_t>::max()) {
				indices.compute = i;
			}

			VkBool32 supportsSurface;
			VkResult err = vd.vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &supportsSurface);
			if (err != VK_SUCCESS) {
				supportsSurface = false;
			}

			std::cout << "queue family#" << i << std::endl;
			std::cout << "  VK_QUEUE_GRAPHICS_BIT=" << ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) << std::endl;
			std::cout << "  VK_QUEUE_COMPUTE_BIT=" << ((family.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT) << std::endl;
			std::cout << "  VK_QUEUE_TRANSFER_BIT=" << ((family.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT) << std::endl;
			std::cout << "  VK_QUEUE_SPARSE_BINDING_BIT=" << ((family.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) == VK_QUEUE_SPARSE_BINDING_BIT) << std::endl;
			std::cout << "  supportsSurface=" << supportsSurface << std::endl;

			// prefer queue family which supports presentation and graphics
			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT && supportsSurface) {
				indices.graphic = i;
				indices.present = i;
			} else {
				// otherwise, select first queue family which supports presentation
				if (supportsSurface && indices.present == std::numeric_limits<uint32_t>::max()) {
					indices.present = i;
				}
			}
		}

		return indices;
	}

	std::vector<std::vector<VkQueue>> findQueues(std::vector<VkDeviceQueueCreateInfo> &deviceQueueCreateInfos) {
		assert(device != VK_NULL_HANDLE);

		std::vector<std::vector<VkQueue>> list(deviceQueueCreateInfos.size());

		for (size_t i = 0; i < deviceQueueCreateInfos.size(); i++) {
			uint32_t queueCount = deviceQueueCreateInfos[i].queueCount;
			uint32_t queueFamilyIndex = deviceQueueCreateInfos[i].queueFamilyIndex;

			list[i].resize(queueCount);

			for (size_t j = 0; j < queueCount; j++) {
				vd.vkGetDeviceQueue(device, queueFamilyIndex, j, &list[i][j]);
			}
		}

		return list;
	}

	VkPhysicalDeviceMemoryProperties findPhysicalDeviceMemoryProperties() {
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vd.vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

		return physicalDeviceMemoryProperties;
	}

	Result createDevice(VkPhysicalDevice physicalDevice) {
		if (physicalDevice == VK_NULL_HANDLE) {
			return {
				false,
				VK_RESULT_MAX_ENUM,
				"VkPhysicalDevice == VK_NULL_HANDLE"
			};
		}

		this->physicalDevice = physicalDevice;

		std::vector<VkQueueFamilyProperties> queueFamilyProperties = fetchQueueFamilyProperties();

		QueueFamilyIndices queueFamilyIndices = findQueueFamilyIndices(queueFamilyProperties);

		if (queueFamilyIndices.present == std::numeric_limits<uint32_t>::max() || queueFamilyIndices.graphic == std::numeric_limits<uint32_t>::max() || queueFamilyIndices.compute == std::numeric_limits<uint32_t>::max()) {
			return {
				false,
				VK_RESULT_MAX_ENUM,
				"Invalid queueFamilyIndices.present or queueFamilyIndices.graphic or queueFamilyIndices.compute"
			};
		}

		std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos{};
		deviceQueueCreateInfos.reserve(3);

		std::vector<std::vector<float>> deviceQueueCreateInfoPriorities{};
		deviceQueueCreateInfoPriorities.reserve(3);

		{
			size_t idx = queueFamilyIndices.present;
			deviceQueueCreateInfos.emplace_back();
			deviceQueueCreateInfoPriorities.emplace_back(queueFamilyProperties[idx].queueCount, 0.0f);

			VkDeviceQueueCreateInfo &deviceQueueCreateInfo = deviceQueueCreateInfos.back();
			deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			deviceQueueCreateInfo.pNext = nullptr;
			deviceQueueCreateInfo.flags = 0;
			deviceQueueCreateInfo.queueFamilyIndex = idx;
			deviceQueueCreateInfo.queueCount = queueFamilyProperties[idx].queueCount;
			deviceQueueCreateInfo.pQueuePriorities = deviceQueueCreateInfoPriorities.back().data();

			queues.presentIndex = deviceQueueCreateInfos.size() - 1;
		}

		if (queueFamilyIndices.present == queueFamilyIndices.graphic) {
			queues.graphicIndex = queues.presentIndex;
		} else {
			size_t idx = queueFamilyIndices.graphic;
			deviceQueueCreateInfos.emplace_back();
			deviceQueueCreateInfoPriorities.emplace_back(queueFamilyProperties[idx].queueCount, 0.0f);

			VkDeviceQueueCreateInfo &deviceQueueCreateInfo = deviceQueueCreateInfos.back();
			deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			deviceQueueCreateInfo.pNext = nullptr;
			deviceQueueCreateInfo.flags = 0;
			deviceQueueCreateInfo.queueFamilyIndex = idx;
			deviceQueueCreateInfo.queueCount = queueFamilyProperties[idx].queueCount;
			deviceQueueCreateInfo.pQueuePriorities = deviceQueueCreateInfoPriorities.back().data();

			queues.graphicIndex = deviceQueueCreateInfos.size() - 1;
		}

		if (queueFamilyIndices.present == queueFamilyIndices.compute) {
			queues.computeIndex = queues.presentIndex;
		} else if (queueFamilyIndices.graphic == queueFamilyIndices.compute) {
			queues.computeIndex = queues.graphicIndex;
		} else {
			size_t idx = queueFamilyIndices.compute;
			deviceQueueCreateInfos.emplace_back();
			deviceQueueCreateInfoPriorities.emplace_back(queueFamilyProperties[idx].queueCount, 0.0f);

			VkDeviceQueueCreateInfo &deviceQueueCreateInfo = deviceQueueCreateInfos.back();
			deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			deviceQueueCreateInfo.pNext = nullptr;
			deviceQueueCreateInfo.flags = 0;
			deviceQueueCreateInfo.queueFamilyIndex = idx;
			deviceQueueCreateInfo.queueCount = queueFamilyProperties[idx].queueCount;
			deviceQueueCreateInfo.pQueuePriorities = deviceQueueCreateInfoPriorities.back().data();

			queues.computeIndex = deviceQueueCreateInfos.size() - 1;
		}

		deviceExtensions.clear();
		deviceExtensions.insert(end(deviceExtensions), begin(desiredDeviceExtensions), end(desiredDeviceExtensions));

		VkPhysicalDeviceFeatures requiredPhysicalDeviceFeatures = {
			/*.robustBufferAccess=*/                      VK_FALSE,
			/*.fullDrawIndexUint32=*/                     VK_FALSE,
			/*.imageCubeArray=*/                          VK_FALSE,
			/*.independentBlend=*/                        VK_FALSE,
			/*.geometryShader=*/                          VK_FALSE,
			/*.tessellationShader=*/                      VK_FALSE,
			/*.sampleRateShading=*/                       VK_FALSE,
			/*.dualSrcBlend=*/                            VK_FALSE,
			/*.logicOp=*/                                 VK_FALSE,
			/*.multiDrawIndirect=*/                       VK_FALSE,
			/*.drawIndirectFirstInstance=*/               VK_FALSE,
			/*.depthClamp=*/                              VK_FALSE,
			/*.depthBiasClamp=*/                          VK_FALSE,
			/*.fillModeNonSolid=*/                        VK_FALSE,
			/*.depthBounds=*/                             VK_FALSE,
			/*.wideLines=*/                               VK_FALSE,
			/*.largePoints=*/                             VK_FALSE,
			/*.alphaToOne=*/                              VK_FALSE,
			/*.multiViewport=*/                           VK_FALSE,
			/*.samplerAnisotropy=*/                       VK_FALSE,
			/*.textureCompressionETC2=*/                  VK_FALSE,
			/*.textureCompressionASTC_LDR=*/              VK_FALSE,
			/*.textureCompressionBC=*/                    VK_FALSE,
			/*.occlusionQueryPrecise=*/                   VK_FALSE,
			/*.pipelineStatisticsQuery=*/                 VK_FALSE,
			/*.vertexPipelineStoresAndAtomics=*/          VK_FALSE,
			/*.fragmentStoresAndAtomics=*/                VK_FALSE,
			/*.shaderTessellationAndGeometryPointSize=*/  VK_FALSE,
			/*.shaderImageGatherExtended=*/               VK_FALSE,
			/*.shaderStorageImageExtendedFormats=*/       VK_FALSE,
			/*.shaderStorageImageMultisample=*/           VK_FALSE,
			/*.shaderStorageImageReadWithoutFormat=*/     VK_FALSE,
			/*.shaderStorageImageWriteWithoutFormat=*/    VK_FALSE,
			/*.shaderUniformBufferArrayDynamicIndexing=*/ VK_FALSE,
			/*.shaderSampledImageArrayDynamicIndexing=*/  VK_FALSE,
			/*.shaderStorageBufferArrayDynamicIndexing=*/ VK_FALSE,
			/*.shaderStorageImageArrayDynamicIndexing=*/  VK_FALSE,
			/*.shaderClipDistance=*/                      VK_TRUE,
			/*.shaderCullDistance=*/                      VK_FALSE,
			/*.shaderFloat64=*/                           VK_FALSE,
			/*.shaderInt64=*/                             VK_FALSE,
			/*.shaderInt16=*/                             VK_FALSE,
			/*.shaderResourceResidency=*/                 VK_FALSE,
			/*.shaderResourceMinLod=*/                    VK_FALSE,
			/*.sparseBinding=*/                           VK_FALSE,
			/*.sparseResidencyBuffer=*/                   VK_FALSE,
			/*.sparseResidencyImage2D=*/                  VK_FALSE,
			/*.sparseResidencyImage3D=*/                  VK_FALSE,
			/*.sparseResidency2Samples=*/                 VK_FALSE,
			/*.sparseResidency4Samples=*/                 VK_FALSE,
			/*.sparseResidency8Samples=*/                 VK_FALSE,
			/*.sparseResidency16Samples=*/                VK_FALSE,
			/*.sparseResidencyAliased=*/                  VK_FALSE,
			/*.variableMultisampleRate=*/                 VK_FALSE,
			/*.inheritedQueries=*/                        VK_FALSE
		};
		VkPhysicalDeviceFeatures availablePhysicalDeviceFeatures;
		vd.vkGetPhysicalDeviceFeatures(physicalDevice, &availablePhysicalDeviceFeatures);

		Result result = validatePhysicalDeviceFeatures(requiredPhysicalDeviceFeatures, availablePhysicalDeviceFeatures);
		if ( ! result) {
			return result;
		}

		VkDeviceCreateInfo deviceCreateInfo {
			/*.sType =*/                   VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			/*.pNext =*/                   nullptr,
			/*.flags =*/                   0,
			/*.queueCreateInfoCount =*/    static_cast<uint32_t>(deviceQueueCreateInfos.size()),
			/*.pQueueCreateInfos =*/       deviceQueueCreateInfos.data(),
			/*.enabledLayerCount =*/       0,
			/*.ppEnabledLayerNames =*/     nullptr,
			/*.enabledExtensionCount =*/   static_cast<uint32_t>(deviceExtensions.size()),
			/*.ppEnabledExtensionNames =*/ deviceExtensions.data(),
			/*.pEnabledFeatures =*/        &requiredPhysicalDeviceFeatures
		};

		VkDevice device;
		VkResult err = vd.vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
		CHECK_RESULT(err, "vkCreateDevice");

		this->queueFamilyIndices = queueFamilyIndices;
		this->deviceQueueCreateInfos = deviceQueueCreateInfos;
		this->device = device;

		vd.loadDeviceVulkanFunctions(device);

		memoryProperties = findPhysicalDeviceMemoryProperties();
		queues.list = findQueues(deviceQueueCreateInfos);

		if (queueFamilyIndices.present != queueFamilyIndices.graphic) {
			presentCommandPool = createCommandPool(queueFamilyIndices.present, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
			graphicCommandPool = createCommandPool(queueFamilyIndices.graphic, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		} else {
			presentCommandPool = createCommandPool(queueFamilyIndices.present, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
			graphicCommandPool = presentCommandPool;
		}

		return { true };
	}

	Result validatePhysicalDeviceFeatures(VkPhysicalDeviceFeatures &requiredPhysicalDeviceFeatures, VkPhysicalDeviceFeatures &availablePhysicalDeviceFeatures) {
		if (requiredPhysicalDeviceFeatures.robustBufferAccess && ! availablePhysicalDeviceFeatures.robustBufferAccess) { return { false, VK_RESULT_MAX_ENUM, "Required feature robustBufferAccess not available" }; }
		if (requiredPhysicalDeviceFeatures.fullDrawIndexUint32 && ! availablePhysicalDeviceFeatures.fullDrawIndexUint32) { return { false, VK_RESULT_MAX_ENUM, "Required feature fullDrawIndexUint32 not available" }; }
		if (requiredPhysicalDeviceFeatures.imageCubeArray && ! availablePhysicalDeviceFeatures.imageCubeArray) { return { false, VK_RESULT_MAX_ENUM, "Required feature imageCubeArray not available" }; }
		if (requiredPhysicalDeviceFeatures.independentBlend && ! availablePhysicalDeviceFeatures.independentBlend) { return { false, VK_RESULT_MAX_ENUM, "Required feature independentBlend not available" }; }
		if (requiredPhysicalDeviceFeatures.geometryShader && ! availablePhysicalDeviceFeatures.geometryShader) { return { false, VK_RESULT_MAX_ENUM, "Required feature geometryShader not available" }; }
		if (requiredPhysicalDeviceFeatures.tessellationShader && ! availablePhysicalDeviceFeatures.tessellationShader) { return { false, VK_RESULT_MAX_ENUM, "Required feature tessellationShader not available" }; }
		if (requiredPhysicalDeviceFeatures.sampleRateShading && ! availablePhysicalDeviceFeatures.sampleRateShading) { return { false, VK_RESULT_MAX_ENUM, "Required feature sampleRateShading not available" }; }
		if (requiredPhysicalDeviceFeatures.dualSrcBlend && ! availablePhysicalDeviceFeatures.dualSrcBlend) { return { false, VK_RESULT_MAX_ENUM, "Required feature dualSrcBlend not available" }; }
		if (requiredPhysicalDeviceFeatures.logicOp && ! availablePhysicalDeviceFeatures.logicOp) { return { false, VK_RESULT_MAX_ENUM, "Required feature logicOp not available" }; }
		if (requiredPhysicalDeviceFeatures.multiDrawIndirect && ! availablePhysicalDeviceFeatures.multiDrawIndirect) { return { false, VK_RESULT_MAX_ENUM, "Required feature multiDrawIndirect not available" }; }
		if (requiredPhysicalDeviceFeatures.drawIndirectFirstInstance && ! availablePhysicalDeviceFeatures.drawIndirectFirstInstance) { return { false, VK_RESULT_MAX_ENUM, "Required feature drawIndirectFirstInstance not available" }; }
		if (requiredPhysicalDeviceFeatures.depthClamp && ! availablePhysicalDeviceFeatures.depthClamp) { return { false, VK_RESULT_MAX_ENUM, "Required feature depthClamp not available" }; }
		if (requiredPhysicalDeviceFeatures.depthBiasClamp && ! availablePhysicalDeviceFeatures.depthBiasClamp) { return { false, VK_RESULT_MAX_ENUM, "Required feature depthBiasClamp not available" }; }
		if (requiredPhysicalDeviceFeatures.fillModeNonSolid && ! availablePhysicalDeviceFeatures.fillModeNonSolid) { return { false, VK_RESULT_MAX_ENUM, "Required feature fillModeNonSolid not available" }; }
		if (requiredPhysicalDeviceFeatures.depthBounds && ! availablePhysicalDeviceFeatures.depthBounds) { return { false, VK_RESULT_MAX_ENUM, "Required feature depthBounds not available" }; }
		if (requiredPhysicalDeviceFeatures.wideLines && ! availablePhysicalDeviceFeatures.wideLines) { return { false, VK_RESULT_MAX_ENUM, "Required feature wideLines not available" }; }
		if (requiredPhysicalDeviceFeatures.largePoints && ! availablePhysicalDeviceFeatures.largePoints) { return { false, VK_RESULT_MAX_ENUM, "Required feature largePoints not available" }; }
		if (requiredPhysicalDeviceFeatures.alphaToOne && ! availablePhysicalDeviceFeatures.alphaToOne) { return { false, VK_RESULT_MAX_ENUM, "Required feature alphaToOne not available" }; }
		if (requiredPhysicalDeviceFeatures.multiViewport && ! availablePhysicalDeviceFeatures.multiViewport) { return { false, VK_RESULT_MAX_ENUM, "Required feature multiViewport not available" }; }
		if (requiredPhysicalDeviceFeatures.samplerAnisotropy && ! availablePhysicalDeviceFeatures.samplerAnisotropy) { return { false, VK_RESULT_MAX_ENUM, "Required feature samplerAnisotropy not available" }; }
		if (requiredPhysicalDeviceFeatures.textureCompressionETC2 && ! availablePhysicalDeviceFeatures.textureCompressionETC2) { return { false, VK_RESULT_MAX_ENUM, "Required feature textureCompressionETC2 not available" }; }
		if (requiredPhysicalDeviceFeatures.textureCompressionASTC_LDR && ! availablePhysicalDeviceFeatures.textureCompressionASTC_LDR) { return { false, VK_RESULT_MAX_ENUM, "Required feature textureCompressionASTC_LDR not available" }; }
		if (requiredPhysicalDeviceFeatures.textureCompressionBC && ! availablePhysicalDeviceFeatures.textureCompressionBC) { return { false, VK_RESULT_MAX_ENUM, "Required feature textureCompressionBC not available" }; }
		if (requiredPhysicalDeviceFeatures.occlusionQueryPrecise && ! availablePhysicalDeviceFeatures.occlusionQueryPrecise) { return { false, VK_RESULT_MAX_ENUM, "Required feature occlusionQueryPrecise not available" }; }
		if (requiredPhysicalDeviceFeatures.pipelineStatisticsQuery && ! availablePhysicalDeviceFeatures.pipelineStatisticsQuery) { return { false, VK_RESULT_MAX_ENUM, "Required feature pipelineStatisticsQuery not available" }; }
		if (requiredPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics && ! availablePhysicalDeviceFeatures.vertexPipelineStoresAndAtomics) { return { false, VK_RESULT_MAX_ENUM, "Required feature vertexPipelineStoresAndAtomics not available" }; }
		if (requiredPhysicalDeviceFeatures.fragmentStoresAndAtomics && ! availablePhysicalDeviceFeatures.fragmentStoresAndAtomics) { return { false, VK_RESULT_MAX_ENUM, "Required feature fragmentStoresAndAtomics not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize && ! availablePhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderTessellationAndGeometryPointSize not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderImageGatherExtended && ! availablePhysicalDeviceFeatures.shaderImageGatherExtended) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderImageGatherExtended not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderStorageImageExtendedFormats && ! availablePhysicalDeviceFeatures.shaderStorageImageExtendedFormats) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderStorageImageExtendedFormats not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderStorageImageMultisample && ! availablePhysicalDeviceFeatures.shaderStorageImageMultisample) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderStorageImageMultisample not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat && ! availablePhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderStorageImageReadWithoutFormat not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat && ! availablePhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderStorageImageWriteWithoutFormat not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing && ! availablePhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderUniformBufferArrayDynamicIndexing not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing && ! availablePhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderSampledImageArrayDynamicIndexing not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing && ! availablePhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderStorageBufferArrayDynamicIndexing not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing && ! availablePhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderStorageImageArrayDynamicIndexing not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderClipDistance && ! availablePhysicalDeviceFeatures.shaderClipDistance) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderClipDistance not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderCullDistance && ! availablePhysicalDeviceFeatures.shaderCullDistance) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderCullDistance not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderFloat64 && ! availablePhysicalDeviceFeatures.shaderFloat64) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderFloat64 not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderInt64 && ! availablePhysicalDeviceFeatures.shaderInt64) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderInt64 not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderInt16 && ! availablePhysicalDeviceFeatures.shaderInt16) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderInt16 not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderResourceResidency && ! availablePhysicalDeviceFeatures.shaderResourceResidency) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderResourceResidency not available" }; }
		if (requiredPhysicalDeviceFeatures.shaderResourceMinLod && ! availablePhysicalDeviceFeatures.shaderResourceMinLod) { return { false, VK_RESULT_MAX_ENUM, "Required feature shaderResourceMinLod not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseBinding && ! availablePhysicalDeviceFeatures.sparseBinding) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseBinding not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidencyBuffer && ! availablePhysicalDeviceFeatures.sparseResidencyBuffer) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidencyBuffer not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidencyImage2D && ! availablePhysicalDeviceFeatures.sparseResidencyImage2D) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidencyImage2D not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidencyImage3D && ! availablePhysicalDeviceFeatures.sparseResidencyImage3D) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidencyImage3D not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidency2Samples && ! availablePhysicalDeviceFeatures.sparseResidency2Samples) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidency2Samples not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidency4Samples && ! availablePhysicalDeviceFeatures.sparseResidency4Samples) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidency4Samples not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidency8Samples && ! availablePhysicalDeviceFeatures.sparseResidency8Samples) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidency8Samples not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidency16Samples && ! availablePhysicalDeviceFeatures.sparseResidency16Samples) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidency16Samples not available" }; }
		if (requiredPhysicalDeviceFeatures.sparseResidencyAliased && ! availablePhysicalDeviceFeatures.sparseResidencyAliased) { return { false, VK_RESULT_MAX_ENUM, "Required feature sparseResidencyAliased not available" }; }
		if (requiredPhysicalDeviceFeatures.variableMultisampleRate && ! availablePhysicalDeviceFeatures.variableMultisampleRate) { return { false, VK_RESULT_MAX_ENUM, "Required feature variableMultisampleRate not available" }; }
		if (requiredPhysicalDeviceFeatures.inheritedQueries && ! availablePhysicalDeviceFeatures.inheritedQueries) { return { false, VK_RESULT_MAX_ENUM, "Required feature inheritedQueries not available" }; }

		return { true };
	}

	Result destroyDevice() {
		VkResult err;

		err = vd.vkWaitForFences(device, presentFences.size(), presentFences.data(), VK_TRUE, std::numeric_limits<uint64_t>::max());
		CHECK_RESULT(err, "vkWaitForFences#graphicFences");

		err = vd.vkWaitForFences(device, graphicFences.size(), graphicFences.data(), VK_TRUE, std::numeric_limits<uint64_t>::max());
		CHECK_RESULT(err, "vkWaitForFences#graphicFences");

		err = vd.vkDeviceWaitIdle(device);
		CHECK_RESULT(err, "vkDeviceWaitIdle");

		for (VkFence &fence : presentFences) {
			destroyFence(fence);
		}

		for (VkFence &fence : graphicFences) {
			destroyFence(fence);
		}

		if (queueFamilyIndices.present != queueFamilyIndices.graphic) {
			destroyCommandPool(presentCommandPool);
			destroyCommandPool(graphicCommandPool);
		} else {
			destroyCommandPool(presentCommandPool);
			graphicCommandPool = VK_NULL_HANDLE;
		}

		vd.vkDestroyDevice(device, nullptr);
		device = VK_NULL_HANDLE;
		physicalDevice = VK_NULL_HANDLE;

		queues.list.clear();
		queues.presentIndex = std::numeric_limits<size_t>::max();
		queues.graphicIndex = std::numeric_limits<size_t>::max();
		queues.computeIndex = std::numeric_limits<size_t>::max();

		vd.unloadDeviceVulkanFunctions();

		return { true };
	}

	VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlagBits flags = static_cast<VkCommandPoolCreateFlagBits>(0)) {
		assert(device != VK_NULL_HANDLE);

		VkCommandPoolCreateInfo commandPoolCreateInfo = {
			/*.sType=*/            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*.pNext=*/            nullptr,
			/*.flags=*/            flags,
			/*.queueFamilyIndex=*/ queueFamilyIndex
		};

		VkCommandPool commadPool = VK_NULL_HANDLE;
		VkResult err = vd.vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commadPool);
		if (err != VK_SUCCESS) {
			return VK_NULL_HANDLE;
		}

		return commadPool;
	}

	void destroyCommandPool(VkCommandPool &commandPool) {
		assert(device != VK_NULL_HANDLE);

		if (commandPool != VK_NULL_HANDLE) {
			vd.vkDestroyCommandPool(device, commandPool, nullptr);
			commandPool = VK_NULL_HANDLE;
		}
	}

	std::vector<VkCommandBuffer> createCommandBuffers(VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t bufferCount) {
		VkResult err;

		assert(device != VK_NULL_HANDLE);

		std::vector<VkCommandBuffer> buffers(bufferCount);

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			/*.sType=*/              VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*.pNext=*/              nullptr,
			/*.commandPool=*/        commandPool,
			/*.level=*/              level,
			/*.commandBufferCount=*/ bufferCount
		};
		err = vd.vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, buffers.data());
		if (err != VK_SUCCESS) {
			buffers.clear();
		}

		return buffers;
	}

	uint32_t findAvailablePresentCommandBufferIndex() {
		for (uint32_t i = 0; i < presentFences.size(); i++) {
			if (presentFences[i] != VK_NULL_HANDLE) {
				VkResult status = vd.vkGetFenceStatus(device, presentFences[i]);

				if (status == VK_SUCCESS) {
					return i;
				}
			}
		}

		std::cout << "findAvailablePresentCommandBufferIndex() - creating new command buffer and fence" << std::endl;

		VkFence fence = createFence(true);
		std::vector<VkCommandBuffer> buffers = createCommandBuffers(presentCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
		assert(buffers.size() > 0);

		presentFences.push_back(fence);
		presentCommandBuffers.push_back(buffers[0]);

		return presentFences.size() - 1;
	}

	Result createRenderPass() {
		assert(device != VK_NULL_HANDLE);

		VkResult err;

		VkAttachmentReference colorAttachmentReferences[] = {
			{
				/*.attachment=*/ 0,
				/*.layout=*/     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			}
		};

		VkAttachmentDescription attachmentDescriptions[] = {
			{
				/*.flags=*/          0,
				/*.format=*/         surfaceFormat.format,
				/*.samples=*/        VK_SAMPLE_COUNT_1_BIT,
				/*.loadOp=*/         VK_ATTACHMENT_LOAD_OP_CLEAR,
				/*.storeOp=*/        VK_ATTACHMENT_STORE_OP_STORE,
				/*.stencilLoadOp=*/  VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				/*.stencilStoreOp=*/ VK_ATTACHMENT_STORE_OP_DONT_CARE,
				/*.initialLayout=*/  VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				/*.finalLayout=*/    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			}
		};

		VkSubpassDescription subpassDescriptions[] = {
			{
				/*.flags=*/                    0,
				/*.pipelineBindPoint=*/        VK_PIPELINE_BIND_POINT_GRAPHICS,
				/*.inputAttachmentCount=*/     0,
				/*.pInputAttachments=*/        nullptr,
				/*.colorAttachmentCount=*/     1,
				/*.pColorAttachments=*/        colorAttachmentReferences,
				/*.pResolveAttachments=*/      nullptr,
				/*.pDepthStencilAttachment=*/  nullptr,
				/*.preserveAttachmentCount=*/  0,
				/*.pPreserveAttachments=*/     nullptr
			}
		};

		VkRenderPassCreateInfo renderPassCreateInfo = {
			/*.sType=*/           VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*.pNext=*/           nullptr,
			/*.flags=*/           0,
			/*.attachmentCount=*/ 1,
			/*.pAttachments=*/    attachmentDescriptions,
			/*.subpassCount=*/    1,
			/*.pSubpasses=*/      subpassDescriptions,
			/*.dependencyCount=*/ 0,
			/*.pDependencies=*/   nullptr
		};

		err = vd.vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
		CHECK_RESULT(err, "vkCreateRenderPass");

		return { true };
	}

	void destroyRenderPass() {
		assert(device != VK_NULL_HANDLE);

		if (renderPass != VK_NULL_HANDLE) {
			vd.vkDestroyRenderPass(device, renderPass, nullptr);
			renderPass = VK_NULL_HANDLE;
		}
	}

	Result createFrameBuffers() {
		VkResult err;

		assert(device != VK_NULL_HANDLE);
		assert(renderPass != VK_NULL_HANDLE);
		assert(swapchainImageViews.size() > 0);

		swapchainFrameBuffers.resize(swapchainImageViews.size(), VK_NULL_HANDLE);

		for (size_t i = 0; i < swapchainFrameBuffers.size(); i++) {
			VkFramebufferCreateInfo framebufferCreateInfo = {
				/*.sType=*/           VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				/*.pNext=*/           nullptr,
				/*.flags=*/           0,
				/*.renderPass=*/      renderPass,
				/*.attachmentCount=*/ 1,
				/*.pAttachments=*/    &swapchainImageViews[i],
				/*.width=*/           surfaceExtent.width,
				/*.height=*/          surfaceExtent.height,
				/*.layers=*/          1
			};

			err = vd.vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &swapchainFrameBuffers[i]);
			CHECK_RESULT(err, "vkCreateFramebuffer#swapchain");
		}

		return { true };
	}

	void destroyFrameBuffers() {
		assert(device != VK_NULL_HANDLE);

		for (auto &frameBuffer : swapchainFrameBuffers) {
			if (frameBuffer != VK_NULL_HANDLE) {
				vd.vkDestroyFramebuffer(device, frameBuffer, nullptr);
				frameBuffer = VK_NULL_HANDLE;
			}
		}

		swapchainFrameBuffers.clear();
	}

	Result createShaderModule(VkShaderModule &shaderModule, std::string fileName) {
		VkResult err;

		assert(device != VK_NULL_HANDLE);

		std::vector<char> code = ngn::fs::contents<std::vector<char>>(fileName);

		VkShaderModuleCreateInfo shaderModuleCreateInfo = {
			/*.sType=*/    VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			/*.pNext=*/    nullptr,
			/*.flags=*/    0,
			/*.codeSize=*/ code.size(),
			/*.pCode=*/    reinterpret_cast<const uint32_t*>(code.data())
		};

		err = vd.vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);
		CHECK_RESULT(err, "vkCreateShaderModule");

		return { true };
	}

	void destroyShaderModule(VkShaderModule &shaderModule) {
		assert(device != VK_NULL_HANDLE);

		if (shaderModule != VK_NULL_HANDLE) {
			vd.vkDestroyShaderModule(device, shaderModule, nullptr);
			shaderModule = VK_NULL_HANDLE;
		}
	}

	Result createGraphicPipeline() {
		VkResult err;
		Result result;

		assert(device != VK_NULL_HANDLE);
		assert(renderPass != VK_NULL_HANDLE);

		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;

		result = createShaderModule(vertexShaderModule, "shaders/main.vert.spv");
		if ( ! result) {
			return result;
		}

		result = createShaderModule(fragmentShaderModule, "shaders/main.frag.spv");
		if ( ! result) {
			return result;
		}

		std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos = {
			{
				/*.sType=*/               VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				/*.pNext=*/               nullptr,
				/*.flags=*/               0,
				/*.stage=*/               VK_SHADER_STAGE_VERTEX_BIT,
				/*.module=*/              vertexShaderModule,
				/*.pName=*/               "main",
				/*.pSpecializationInfo=*/ nullptr
			},
			{
				/*.sType=*/               VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				/*.pNext=*/               nullptr,
				/*.flags=*/               0,
				/*.stage=*/               VK_SHADER_STAGE_FRAGMENT_BIT,
				/*.module=*/              fragmentShaderModule,
				/*.pName=*/               "main",
				/*.pSpecializationInfo=*/ nullptr
			}
		};

		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
			/*.sType=*/                           VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			/*.pNext=*/                           nullptr,
			/*.flags=*/                           0,
			/*.vertexBindingDescriptionCount=*/   0,
			/*.pVertexBindingDescriptions=*/      nullptr,
			/*.vertexAttributeDescriptionCount=*/ 0,
			/*.pVertexAttributeDescriptions=*/    nullptr
		};

		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
			/*.sType=*/                  VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			/*.pNext=*/                  nullptr,
			/*.flags=*/                  0,
			/*.topology=*/               VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			/*.primitiveRestartEnable=*/ VK_FALSE
		};

		VkViewport viewport = {
			/*.x=*/        0.0f,
			/*.y=*/        0.0f,
			/*.width=*/    static_cast<float>(surfaceExtent.width),
			/*.height=*/   static_cast<float>(surfaceExtent.height),
			/*.minDepth=*/ 0.0f,
			/*.maxDepth=*/ 1.0f
		};

		VkRect2D scissor = {
			/*.offset= */ {
				/*.x=*/ 0,
				/*.y=*/ 0
			},
			/*.extent= */ {
				/*.width=*/  surfaceExtent.width,
				/*.height=*/ surfaceExtent.height
			}
		};

		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
			/*.sType=*/         VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			/*.pNext=*/         nullptr,
			/*.flags=*/         0,
			/*.viewportCount=*/ 1,
			/*.pViewports=*/    &viewport,
			/*.scissorCount=*/  1,
			/*.pScissors=*/     &scissor
		};

		VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {
			/*.sType=*/                   VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			/*.pNext=*/                   nullptr,
			/*.flags=*/                   0,
			/*.depthClampEnable=*/        VK_FALSE,
			/*.rasterizerDiscardEnable=*/ VK_FALSE,
			/*.polygonMode=*/             VK_POLYGON_MODE_FILL,
			/*.cullMode=*/                VK_CULL_MODE_BACK_BIT,
			/*.frontFace=*/               VK_FRONT_FACE_COUNTER_CLOCKWISE,
			/*.depthBiasEnable=*/         VK_FALSE,
			/*.depthBiasConstantFactor=*/ 0.0f,
			/*.depthBiasClamp=*/          0.0f,
			/*.depthBiasSlopeFactor=*/    0.0f,
			/*.lineWidth=*/               1.0f
		};

		VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {
			/*.sType=*/                 VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			/*.pNext=*/                 nullptr,
			/*.flags=*/                 0,
			/*.rasterizationSamples=*/  VK_SAMPLE_COUNT_1_BIT,
			/*.sampleShadingEnable=*/   VK_FALSE,
			/*.minSampleShading=*/      1.0f,
			/*.pSampleMask=*/           nullptr,
			/*.alphaToCoverageEnable=*/ VK_FALSE,
			/*.alphaToOneEnable=*/      VK_FALSE
		};

		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {
			/*.blendEnable=*/         VK_FALSE,
			/*.srcColorBlendFactor=*/ VK_BLEND_FACTOR_ONE,
			/*.dstColorBlendFactor=*/ VK_BLEND_FACTOR_ZERO,
			/*.colorBlendOp=*/        VK_BLEND_OP_ADD,
			/*.srcAlphaBlendFactor=*/ VK_BLEND_FACTOR_ONE,
			/*.dstAlphaBlendFactor=*/ VK_BLEND_FACTOR_ZERO,
			/*.alphaBlendOp=*/        VK_BLEND_OP_ADD,
			/*.colorWriteMask=*/      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {
			/*.sType=*/           VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			/*.pNext=*/           nullptr,
			/*.flags=*/           0,
			/*.logicOpEnable=*/   VK_FALSE,
			/*.logicOp=*/         VK_LOGIC_OP_COPY,
			/*.attachmentCount=*/ 1,
			/*.pAttachments=*/    &pipelineColorBlendAttachmentState,
			/*.blendConstants=*/  { 0.0f, 0.0f, 0.0f, 0.0f }
		};

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			/*.sType=*/                  VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			/*.pNext=*/                  nullptr,
			/*.flags=*/                  0,
			/*.setLayoutCount=*/         0,
			/*.pSetLayouts=*/            nullptr,
			/*.pushConstantRangeCount=*/ 0,
			/*.pPushConstantRanges=*/    nullptr
		};

		err = vd.vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
		if (err != VK_SUCCESS) {
			destroyShaderModule(vertexShaderModule);
			destroyShaderModule(fragmentShaderModule);

			destroyGraphicPipeline();
			CHECK_RESULT(err, "vkCreatePipelineLayout");
		}

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
			/*.sType=*/               VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			/*.pNext=*/               nullptr,
			/*.flags=*/               0,
			/*.stageCount=*/          static_cast<uint32_t>(pipelineShaderStageCreateInfos.size()),
			/*.pStages=*/             pipelineShaderStageCreateInfos.data(),
			/*.pVertexInputState=*/   &pipelineVertexInputStateCreateInfo,
			/*.pInputAssemblyState=*/ &pipelineInputAssemblyStateCreateInfo,
			/*.pTessellationState=*/  nullptr,
			/*.pViewportState=*/      &pipelineViewportStateCreateInfo,
			/*.pRasterizationState=*/ &pipelineRasterizationStateCreateInfo,
			/*.pMultisampleState=*/   &pipelineMultisampleStateCreateInfo,
			/*.pDepthStencilState=*/  nullptr,
			/*.pColorBlendState=*/    &pipelineColorBlendStateCreateInfo,
			/*.pDynamicState=*/       nullptr,
			/*.layout=*/              pipelineLayout,
			/*.renderPass=*/          renderPass,
			/*.subpass=*/             0,
			/*.basePipelineHandle=*/  VK_NULL_HANDLE,
			/*.basePipelineIndex=*/   -1
		};

		err = vd.vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &graphicsPipeline);
		if (err != VK_SUCCESS) {
			destroyShaderModule(vertexShaderModule);
			destroyShaderModule(fragmentShaderModule);

			destroyGraphicPipeline();
			CHECK_RESULT(err, "vkCreateGraphicsPipelines");
		}

		destroyShaderModule(vertexShaderModule);
		destroyShaderModule(fragmentShaderModule);

		return { true };
	}

	void destroyGraphicPipeline() {
		if (pipelineLayout != VK_NULL_HANDLE) {
			vd.vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
			pipelineLayout = VK_NULL_HANDLE;
		}

		if (graphicsPipeline != VK_NULL_HANDLE) {
			vd.vkDestroyPipeline(device, graphicsPipeline, nullptr);
			graphicsPipeline = VK_NULL_HANDLE;
		}
	}

	Result createVertexBuffer() {
		VkResult err;

		assert(device != VK_NULL_HANDLE);

		std::vector<Vertex> vertexData = {
			{
				/*.position=*/ { 1.0f, 1.0f, 0.0f },
				/*.color=*/    { 1.0f, 0.0f, 0.0f }
			},
			{
				/*.position=*/ {-1.0f, 1.0f, 0.0f },
				/*.color=*/    { 0.0f, 1.0f, 0.0f }
			},
			{
				/*.position=*/ { 0.0f,-1.0f, 0.0f },
				/*.color=*/    { 0.0f, 0.0f, 1.0f }
			}
		};

		std::vector<uint32_t> indexData = { 0, 1, 2 };

		// create commad buffer
		std::vector<VkCommandBuffer> copyCommandBuffers = createCommandBuffers(graphicCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
		VkCommandBuffer copyCommandBuffer = copyCommandBuffers[0];

		VkBufferCreateInfo stagingVertexBufferCreateInfo = {
			/*.sType=*/                 VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			/*.pNext=*/                 nullptr,
			/*.flags=*/                 0,
			/*.size=*/                  static_cast<VkDeviceSize>(vertexData.size() * sizeof(vertexData[0])),
			/*.usage=*/                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			/*.sharingMode=*/           VK_SHARING_MODE_EXCLUSIVE,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/   nullptr,
		};

		VkBufferCreateInfo stagingIndexBufferCreateInfo = {
			/*.sType=*/                 VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			/*.pNext=*/                 nullptr,
			/*.flags=*/                 0,
			/*.size=*/                  static_cast<VkDeviceSize>(indexData.size() * sizeof(indexData[0])),
			/*.usage=*/                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			/*.sharingMode=*/           VK_SHARING_MODE_EXCLUSIVE,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/   nullptr,
		};

		err = vd.vkCreateBuffer(device, &stagingVertexBufferCreateInfo, nullptr, &stagingBuffers.vertices.buffer);
		CHECK_RESULT(err, "vkCreateBuffer");

		err = vd.vkCreateBuffer(device, &stagingIndexBufferCreateInfo, nullptr, &stagingBuffers.indices.buffer);
		CHECK_RESULT(err, "vkCreateBuffer");

		VkMemoryRequirements stagingVertexMemoryRequirements;
		vd.vkGetBufferMemoryRequirements(device, stagingBuffers.vertices.buffer, &stagingVertexMemoryRequirements);

		{
			uint32_t stagingMemoryTypeBits = stagingVertexMemoryRequirements.memoryTypeBits;
			uint32_t stagingMemoryTypeIndex = VK_MAX_MEMORY_TYPES;

			VkFlags requiredStaginMemoryProperty = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			static_assert(VK_MAX_MEMORY_TYPES <= sizeof(uint32_t) * 8, "VK_MAX_MEMORY_TYPES must be less than 32");
			for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
				uint32_t shift = 1 << i;

				if ((stagingMemoryTypeBits & shift) && (memoryProperties.memoryTypes[i].propertyFlags & requiredStaginMemoryProperty)) {
					stagingMemoryTypeIndex = i;
					break;
				}
			}

			if (stagingMemoryTypeIndex == VK_MAX_MEMORY_TYPES) {
				return {
					false,
					VK_RESULT_MAX_ENUM,
					"Could not find suitable memory type"
				};
			}

			VkMemoryAllocateInfo stagingVertexMemoryAllocateInfo = {
				/*.sType=*/           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				/*.pNext=*/           nullptr,
				/*.allocationSize=*/  stagingVertexMemoryRequirements.size,
				/*.memoryTypeIndex=*/ stagingMemoryTypeIndex,
			};
			err = vd.vkAllocateMemory(device, &stagingVertexMemoryAllocateInfo, nullptr, &stagingBuffers.vertices.memory);
			CHECK_RESULT(err, "vkAllocateMemory");

			Vertex *data = nullptr;
			vd.vkMapMemory(device, stagingBuffers.vertices.memory, 0, static_cast<VkDeviceSize>(vertexData.size() * sizeof(vertexData[0])), 0, reinterpret_cast<void **>(&data));
			assert(data != nullptr);
			std::copy(begin(vertexData), end(vertexData), data);
			vd.vkUnmapMemory(device, stagingBuffers.vertices.memory);

			err = vd.vkBindBufferMemory(device, stagingBuffers.vertices.buffer, stagingBuffers.vertices.memory, 0);
			CHECK_RESULT(err, "vkBindBufferMemory");
		}

		VkMemoryRequirements stagingIndexMemoryRequirements;
		vd.vkGetBufferMemoryRequirements(device, stagingBuffers.indices.buffer, &stagingIndexMemoryRequirements);

		{
			uint32_t stagingMemoryTypeBits = stagingIndexMemoryRequirements.memoryTypeBits;
			uint32_t stagingMemoryTypeIndex = VK_MAX_MEMORY_TYPES;

			VkFlags requiredStaginMemoryProperty = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			static_assert(VK_MAX_MEMORY_TYPES <= sizeof(uint32_t) * 8, "VK_MAX_MEMORY_TYPES must be less than 32");
			for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
				uint32_t shift = 1 << i;

				if ((stagingMemoryTypeBits & shift) && (memoryProperties.memoryTypes[i].propertyFlags & requiredStaginMemoryProperty)) {
					stagingMemoryTypeIndex = i;
					break;
				}
			}

			if (stagingMemoryTypeIndex == VK_MAX_MEMORY_TYPES) {
				return {
					false,
					VK_RESULT_MAX_ENUM,
					"Could not find suitable memory type"
				};
			}

			VkMemoryAllocateInfo stagingIndexMemoryAllocateInfo = {
				/*.sType=*/           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				/*.pNext=*/           nullptr,
				/*.allocationSize=*/  stagingIndexMemoryRequirements.size,
				/*.memoryTypeIndex=*/ stagingMemoryTypeIndex,
			};
			err = vd.vkAllocateMemory(device, &stagingIndexMemoryAllocateInfo, nullptr, &stagingBuffers.indices.memory);
			CHECK_RESULT(err, "vkAllocateMemory");

			uint32_t *data = nullptr;
			vd.vkMapMemory(device, stagingBuffers.indices.memory, 0, static_cast<VkDeviceSize>(indexData.size() * sizeof(indexData[0])), 0, reinterpret_cast<void **>(&data));
			assert(data != nullptr);
			std::copy(begin(indexData), end(indexData), data);
			vd.vkUnmapMemory(device, stagingBuffers.indices.memory);

			err = vd.vkBindBufferMemory(device, stagingBuffers.indices.buffer, stagingBuffers.indices.memory, 0);
			CHECK_RESULT(err, "vkBindBufferMemory");
		}

		// gpu buffer
		VkBufferCreateInfo vertexBufferCreateInfo = stagingVertexBufferCreateInfo;
		VkBufferCreateInfo indexBufferCreateInfo = stagingIndexBufferCreateInfo;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		err = vd.vkCreateBuffer(device, &vertexBufferCreateInfo, nullptr, &vertexBuffers.vertices.buffer);
		CHECK_RESULT(err, "vkCreateBuffer");

		err = vd.vkCreateBuffer(device, &indexBufferCreateInfo, nullptr, &vertexBuffers.indices.buffer);
		CHECK_RESULT(err, "vkCreateBuffer");

		VkMemoryRequirements vertexMemoryRequirements;
		vd.vkGetBufferMemoryRequirements(device, vertexBuffers.vertices.buffer, &vertexMemoryRequirements);
		{
			uint32_t vertexMemoryTypeBits = vertexMemoryRequirements.memoryTypeBits;
			uint32_t vertexMemoryTypeIndex = VK_MAX_MEMORY_TYPES;

			VkFlags requiredVertexMemoryProperty = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
				uint32_t shift = 1 << i;

				if ((vertexMemoryTypeBits & shift) && (memoryProperties.memoryTypes[i].propertyFlags & requiredVertexMemoryProperty)) {
					vertexMemoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateInfo vertexMemoryAllocateInfo = {
				/*.sType=*/           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				/*.pNext=*/           nullptr,
				/*.allocationSize=*/  vertexMemoryRequirements.size,
				/*.memoryTypeIndex=*/ vertexMemoryTypeIndex,
			};
			err = vd.vkAllocateMemory(device, &vertexMemoryAllocateInfo, nullptr, &vertexBuffers.vertices.memory);
			CHECK_RESULT(err, "vkAllocateMemory");

			err = vd.vkBindBufferMemory(device, vertexBuffers.vertices.buffer, vertexBuffers.vertices.memory, 0);
			CHECK_RESULT(err, "vkBindBufferMemory");
		}

		VkMemoryRequirements indexMemoryRequirements;
		vd.vkGetBufferMemoryRequirements(device, vertexBuffers.indices.buffer, &indexMemoryRequirements);
		{
			uint32_t indexMemoryTypeBits = indexMemoryRequirements.memoryTypeBits;
			uint32_t indexMemoryTypeIndex = VK_MAX_MEMORY_TYPES;

			VkFlags requiredIndexMemoryProperty = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
				uint32_t shift = 1 << i;

				if ((indexMemoryTypeBits & shift) && (memoryProperties.memoryTypes[i].propertyFlags & requiredIndexMemoryProperty)) {
					indexMemoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateInfo indexMemoryAllocateInfo = {
				/*.sType=*/           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				/*.pNext=*/           nullptr,
				/*.allocationSize=*/  indexMemoryRequirements.size,
				/*.memoryTypeIndex=*/ indexMemoryTypeIndex,
			};
			err = vd.vkAllocateMemory(device, &indexMemoryAllocateInfo, nullptr, &vertexBuffers.indices.memory);
			CHECK_RESULT(err, "vkAllocateMemory");

			err = vd.vkBindBufferMemory(device, vertexBuffers.indices.buffer, vertexBuffers.indices.memory, 0);
			CHECK_RESULT(err, "vkBindBufferMemory");
		}

		// copy data
		VkCommandBufferBeginInfo copyBufferBeginInfo = {
			/*.sType=*/            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*.pNext=*/            nullptr,
			/*.flags=*/            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			/*.pInheritanceInfo=*/ nullptr,
		};

		err = vd.vkBeginCommandBuffer(copyCommandBuffer, &copyBufferBeginInfo);
		CHECK_RESULT(err, "vkBeginCommandBuffer");

		VkBufferCopy copyRegion = {
			/*.srcOffset=*/ 0,
			/*.dstOffset=*/ 0,
			/*.size=*/      0,
		};

		copyRegion.size = vertexBufferCreateInfo.size;
		vd.vkCmdCopyBuffer(copyCommandBuffer, stagingBuffers.vertices.buffer, vertexBuffers.vertices.buffer, 1, &copyRegion);

		copyRegion.size = indexBufferCreateInfo.size;
		vd.vkCmdCopyBuffer(copyCommandBuffer, stagingBuffers.indices.buffer, vertexBuffers.indices.buffer, 1, &copyRegion);

		err = vd.vkEndCommandBuffer(copyCommandBuffer);
		CHECK_RESULT(err, "vkEndCommandBuffer");

		// submit
		VkSubmitInfo submitInfo = {
			/*.sType=*/                VK_STRUCTURE_TYPE_SUBMIT_INFO,
			/*.pNext=*/                nullptr,
			/*.waitSemaphoreCount=*/   0,
			/*.pWaitSemaphores=*/      nullptr,
			/*.pWaitDstStageMask=*/    nullptr,
			/*.commandBufferCount=*/   1,
			/*.pCommandBuffers=*/      &copyCommandBuffer,
			/*.signalSemaphoreCount=*/ 0,
			/*.pSignalSemaphores=*/    nullptr,
		};
		err = vd.vkQueueSubmit(queues.graphic()[0], 1, &submitInfo, VK_NULL_HANDLE);
		CHECK_RESULT(err, "vkQueueSubmit");

		err = vd.vkQueueWaitIdle(queues.graphic()[0]);
		CHECK_RESULT(err, "vkQueueWaitIdle");

		// cleanup
		if (stagingBuffers.vertices.memory != VK_NULL_HANDLE) {
			vd.vkFreeMemory(device, stagingBuffers.vertices.memory, nullptr);
			stagingBuffers.vertices.memory = VK_NULL_HANDLE;
		}

		if (stagingBuffers.vertices.buffer != VK_NULL_HANDLE) {
			vd.vkDestroyBuffer(device, stagingBuffers.vertices.buffer, nullptr);
			stagingBuffers.vertices.buffer = VK_NULL_HANDLE;
		}

		if (stagingBuffers.indices.memory != VK_NULL_HANDLE) {
			vd.vkFreeMemory(device, stagingBuffers.indices.memory, nullptr);
			stagingBuffers.indices.memory = VK_NULL_HANDLE;
		}

		if (stagingBuffers.indices.buffer != VK_NULL_HANDLE) {
			vd.vkDestroyBuffer(device, stagingBuffers.indices.buffer, nullptr);
			stagingBuffers.indices.buffer = VK_NULL_HANDLE;
		}

		if (copyCommandBuffer != VK_NULL_HANDLE) {
			vd.vkFreeCommandBuffers(device, graphicCommandPool, 1, &copyCommandBuffer);
		}

		return { true };
	}

	void destroyVertexBuffer() {
		if (vertexBuffers.vertices.memory != VK_NULL_HANDLE) {
			vd.vkFreeMemory(device, vertexBuffers.vertices.memory, nullptr);
			vertexBuffers.vertices.memory = VK_NULL_HANDLE;
		}

		if (vertexBuffers.vertices.buffer != VK_NULL_HANDLE) {
			vd.vkDestroyBuffer(device, vertexBuffers.vertices.buffer, nullptr);
			vertexBuffers.vertices.buffer = VK_NULL_HANDLE;
		}

		if (vertexBuffers.indices.memory != VK_NULL_HANDLE) {
			vd.vkFreeMemory(device, vertexBuffers.indices.memory, nullptr);
			vertexBuffers.indices.memory = VK_NULL_HANDLE;
		}

		if (vertexBuffers.indices.buffer != VK_NULL_HANDLE) {
			vd.vkDestroyBuffer(device, vertexBuffers.indices.buffer, nullptr);
			vertexBuffers.indices.buffer = VK_NULL_HANDLE;
		}
	}

	Result acquireNextImage(uint32_t &targetIdx) {
		VkResult err;

		assert(device != VK_NULL_HANDLE);
		assert(swapchain != VK_NULL_HANDLE);

		err = vd.vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<uint64_t>::max(), surfaceImageAvailableSemaphore, VK_NULL_HANDLE, &targetIdx);
		CHECK_RESULT(err, "vkAcquireNextImageKHR");

		return { true };
	}

	Result recordCommands(uint32_t targetIdx, uint32_t availableId) {
		VkResult err;

		assert(swapchainImages.size() > 0);
		assert(presentCommandBuffers.size() > 0);

		VkImage targetImage = swapchainImages[targetIdx];
		VkFramebuffer targetFrameBuffer = swapchainFrameBuffers[targetIdx];

		// VkCommandBuffer presentCommandBuffer = presentCommandBuffers[availableId];
		VkCommandBuffer graphicCommandBuffer = graphicCommandBuffers[availableId];

		VkImageSubresourceRange imageSubresourceRange = {
			/*.aspectMask=*/     VK_IMAGE_ASPECT_COLOR_BIT,
			/*.baseMipLevel=*/   0,
			/*.levelCount=*/     1,
			/*.baseArrayLayer=*/ 0,
			/*.layerCount=*/     1
		};

		VkClearColorValue clearColorValue = {
			/*.float32[4]=*/ { 1.0f, 0.8f, 0.4f, 0.0f }
		};

		VkClearValue clearValue = {
			/*.color=*/        clearColorValue
		};

		VkCommandBufferBeginInfo commandBufferBeginInfo = {
			/*.sType=*/            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*.pNext=*/            nullptr,
			/*.flags=*/            VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			/*.pInheritanceInfo=*/ nullptr
		};
		err = vd.vkBeginCommandBuffer(graphicCommandBuffer, &commandBufferBeginInfo);
		CHECK_RESULT(err, "vkBeginCommandBuffer");

		if (queueFamilyIndices.present != queueFamilyIndices.graphic || ! swapchainImagesStatus[targetIdx]) {
			VkImageMemoryBarrier beginImageMemoryBarrier = {
				/*.sType=*/               VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				/*.pNext=*/               nullptr,
				/*.srcAccessMask=*/       0,
				/*.dstAccessMask=*/       VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				/*.oldLayout=*/           swapchainImagesStatus[targetIdx] ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_UNDEFINED,
				/*.newLayout=*/           VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				/*.srcQueueFamilyIndex=*/ queueFamilyIndices.present,
				/*.dstQueueFamilyIndex=*/ queueFamilyIndices.graphic,
				/*.image=*/               targetImage,
				/*.subresourceRange=*/    imageSubresourceRange
			};

			vd.vkCmdPipelineBarrier(graphicCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &beginImageMemoryBarrier );
		}

		VkRenderPassBeginInfo renderPassBeginInfo = {
			/*.sType=*/           VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			/*.pNext=*/           nullptr,
			/*.renderPass=*/      renderPass,
			/*.framebuffer=*/     targetFrameBuffer,
			/*.renderArea=*/      {
				/*.offset=*/        {
					/*.x=*/           0,
					/*.y=*/           0
				},
				/*.extent=*/ surfaceExtent
			},
			/*.clearValueCount=*/ 1,
			/*.pClearValues=*/    &clearValue
		};

		vd.vkCmdBeginRenderPass(graphicCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vd.vkCmdBindPipeline(graphicCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		vd.vkCmdDraw(graphicCommandBuffer, 3, 1, 0, 0);
		vd.vkCmdEndRenderPass(graphicCommandBuffer);

		if (queueFamilyIndices.present != queueFamilyIndices.graphic || ! swapchainImagesStatus[targetIdx]) {
			VkImageMemoryBarrier beginImageMemoryBarrier = {
				/*.sType=*/               VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				/*.pNext=*/               nullptr,
				/*.srcAccessMask=*/       VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				/*.dstAccessMask=*/       VK_ACCESS_MEMORY_READ_BIT,
				/*.oldLayout=*/           VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				/*.newLayout=*/           VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				/*.srcQueueFamilyIndex=*/ queueFamilyIndices.graphic,
				/*.dstQueueFamilyIndex=*/ queueFamilyIndices.present,
				/*.image=*/               targetImage,
				/*.subresourceRange=*/    imageSubresourceRange
			};

			vd.vkCmdPipelineBarrier(graphicCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &beginImageMemoryBarrier );

			swapchainImagesStatus[targetIdx] = true;
		}

		err = vd.vkEndCommandBuffer(graphicCommandBuffer);
		CHECK_RESULT(err, "vkEndCommandBuffer");

		// VkImageMemoryBarrier beginImageMemoryBarrier = {
		// 	/*.sType=*/               VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		// 	/*.pNext=*/               nullptr,
		// 	/*.srcAccessMask=*/       0,
		// 	/*.dstAccessMask=*/       VK_ACCESS_TRANSFER_WRITE_BIT,
		// 	/*.oldLayout=*/           VK_IMAGE_LAYOUT_UNDEFINED,
		// 	/*.newLayout=*/           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		// 	/*.srcQueueFamilyIndex=*/ queueFamilyIndices.present,
		// 	/*.dstQueueFamilyIndex=*/ queueFamilyIndices.graphic,
		// 	/*.image=*/               targetImage,
		// 	/*.subresourceRange=*/    imageSubresourceRange
		// };

		// VkImageMemoryBarrier endImageMemoryBarrier = {
		// 	/*.sType=*/               VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		// 	/*.pNext=*/               nullptr,
		// 	/*.srcAccessMask=*/       VK_ACCESS_TRANSFER_WRITE_BIT,
		// 	/*.dstAccessMask=*/       VK_ACCESS_MEMORY_READ_BIT,
		// 	/*.oldLayout=*/           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		// 	/*.newLayout=*/           VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		// 	/*.srcQueueFamilyIndex=*/ queueFamilyIndices.graphic,
		// 	/*.dstQueueFamilyIndex=*/ queueFamilyIndices.present,
		// 	/*.image=*/               targetImage,
		// 	/*.subresourceRange=*/    imageSubresourceRange
		// };

		// VkCommandBufferBeginInfo commandBufferBeginInfo = {
		// 	/*.sType=*/            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		// 	/*.pNext=*/            nullptr,
		// 	/*.flags=*/            VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		// 	/*.pInheritanceInfo=*/ nullptr
		// };
		// err = vd.vkBeginCommandBuffer(presentCommandBuffer, &commandBufferBeginInfo);
		// CHECK_RESULT(err, "vkBeginCommandBuffer");

		// vd.vkCmdPipelineBarrier(presentCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &beginImageMemoryBarrier);

		// VkClearColorValue clearColor = {
		// 	/*.float32[4]=*/ { 1.0f, 0.8f, 0.4f, 0.0f }
		// };
		// vd.vkCmdClearColorImage(presentCommandBuffer, targetImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &imageSubresourceRange);

		// vd.vkCmdPipelineBarrier(presentCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &endImageMemoryBarrier);

		// err = vd.vkEndCommandBuffer(presentCommandBuffer);
		// CHECK_RESULT(err, "vkEndCommandBuffer");

		return { true };
	}

	Result submitGraphicQueue(uint32_t availableId) {
		VkResult err;

		assert(device != VK_NULL_HANDLE);

		VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		VkSubmitInfo submitInfo = {
			/*.sType=*/                VK_STRUCTURE_TYPE_SUBMIT_INFO,
			/*.pNext=*/                nullptr,
			/*.waitSemaphoreCount=*/   1,
			/*.pWaitSemaphores=*/      &surfaceImageAvailableSemaphore,
			/*.pWaitDstStageMask=*/    &waitDstStageMask,
			/*.commandBufferCount=*/   1,
			/*.pCommandBuffers=*/      &graphicCommandBuffers[availableId],
			/*.signalSemaphoreCount=*/ 1,
			/*.pSignalSemaphores=*/    &submitFinishedSemaphore
		};

		VkFence fence = presentFences[availableId];
		err = vd.vkResetFences(device, 1, &fence);
		CHECK_RESULT(err, "vkResetFences#graphic");

		err = vd.vkQueueSubmit(queues.graphic()[0], 1, &submitInfo, fence);
		CHECK_RESULT(err, "vkQueueSubmit#graphic");

		return { true };
	}

	Result submitPresentQueue(uint32_t availableId) {
		VkResult err;

		assert(device != VK_NULL_HANDLE);

		VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		VkSubmitInfo submitInfo = {
			/*.sType=*/                VK_STRUCTURE_TYPE_SUBMIT_INFO,
			/*.pNext=*/                nullptr,
			/*.waitSemaphoreCount=*/   1,
			/*.pWaitSemaphores=*/      &surfaceImageAvailableSemaphore,
			/*.pWaitDstStageMask=*/    &waitDstStageMask,
			/*.commandBufferCount=*/   1,
			/*.pCommandBuffers=*/      &presentCommandBuffers[availableId],
			/*.signalSemaphoreCount=*/ 1,
			/*.pSignalSemaphores=*/    &submitFinishedSemaphore
		};

		VkFence fence = presentFences[availableId];
		err = vd.vkResetFences(device, 1, &fence);
		CHECK_RESULT(err, "vkResetFences#present");

		err = vd.vkQueueSubmit(queues.present()[0], 1, &submitInfo, fence);
		CHECK_RESULT(err, "vkQueueSubmit#present");

		return { true };
	}

	Result queuePresent(uint32_t targetIdx) {
		VkResult err;
		VkResult presentErr;

		VkPresentInfoKHR presentInfo = {
			/*.sType=*/              VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			/*.pNext=*/              nullptr,
			/*.waitSemaphoreCount=*/ 1,
			/*.pWaitSemaphores=*/    &submitFinishedSemaphore,
			/*.swapchainCount=*/     1,
			/*.pSwapchains=*/        &swapchain,
			/*.pImageIndices=*/      &targetIdx,
			/*.pResults=*/           &presentErr
		};

		err = vd.vkQueuePresentKHR(queues.present()[0], &presentInfo);
		CHECK_RESULT(err, "vkQueuePresentKHR#call");

		return { true };
	}

	VkPresentModeKHR selectPresentMode(PresentMode presentMode) {
		std::vector<VkPresentModeKHR> presentModes = fetchPresentModes();

		for (const auto &entry : presentModes) {
			switch (presentMode) {
				case PresentMode::NoVSync: {
					if (entry == VK_PRESENT_MODE_IMMEDIATE_KHR) {
						return entry;
					}
					break;
				}
				case PresentMode::TripleBuffer: {
					if (entry == VK_PRESENT_MODE_MAILBOX_KHR) {
						return entry;
					}
					break;
				}
				case PresentMode::VSync: {
					if (entry == VK_PRESENT_MODE_FIFO_KHR) {
						return entry;
					}
					break;
				}
				case PresentMode::VSyncAdap: {
					if (entry == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
						return entry;
					}
					break;
				}
			}
		}

		return VK_PRESENT_MODE_MAX_ENUM_KHR;
	}

	VkPresentModeKHR selectAnyPresentMode() {
		return VK_PRESENT_MODE_FIFO_KHR; // all vulkan implementations must support VK_PRESENT_MODE_FIFO_KHR
	}

	Result createSwapchain(uint32_t fallbackWidth, uint32_t fallbackHeight, VkPresentModeKHR presentMode, VkSwapchainKHR oldSwapchain) {
		VkResult err;
		Result result;

		assert(physicalDevice != VK_NULL_HANDLE);
		assert(device != VK_NULL_HANDLE);
		assert(surface != VK_NULL_HANDLE);

		std::vector<VkSurfaceFormatKHR> surfaceFormats = fetchSurfaceFormats(result);
		if ( ! result) {
			return result;
		}

		VkSurfaceFormatKHR surfaceFormat = selectSurfaceFormat(surfaceFormats);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		err = vd.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
		CHECK_RESULT(err, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");

		uint32_t surfaceImageCount = selectSurfaceImageCount(surfaceCapabilities, presentMode);
		VkExtent2D surfaceExtent = selectSurfaceExtent(surfaceCapabilities, {fallbackWidth, fallbackHeight});
		VkSurfaceTransformFlagBitsKHR surfaceTransform = selectSurfaceTransform(surfaceCapabilities);

		VkSwapchainCreateInfoKHR swapchainCreateInfo{
			/*.sType=*/                 VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			/*.pNext=*/                 nullptr,
			/*.flags=*/                 0,
			/*.surface=*/               surface,
			/*.minImageCount=*/         surfaceImageCount,
			/*.imageFormat=*/           surfaceFormat.format,
			/*.imageColorSpace=*/       surfaceFormat.colorSpace,
			/*.imageExtent=*/           surfaceExtent,
			/*.imageArrayLayers=*/      1,
			/*.imageUsage=*/            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			/*.imageSharingMode=*/      VK_SHARING_MODE_EXCLUSIVE,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/   nullptr,
			/*.preTransform=*/          surfaceTransform,
			/*.compositeAlpha=*/        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			/*.presentMode=*/           presentMode,
			/*.clipped=*/               VK_TRUE,
			/*.oldSwapchain=*/          oldSwapchain
		};

		VkSwapchainKHR swapchain;
		err = vd.vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);
		CHECK_RESULT(err, "vkCreateSwapchainKHR");

		this->surfaceExtent = surfaceExtent;
		this->surfaceFormat = surfaceFormat;
		this->swapchain = swapchain;

		if (oldSwapchain != VK_NULL_HANDLE) {
			vd.vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
		}

		swapchainImages = fetchSwapchainImages(result);
		if ( ! result) {
			return result;
		}

		swapchainImageViews.resize(swapchainImages.size(), VK_NULL_HANDLE);

		for (size_t i = 0; i < swapchainImageViews.size(); i++) {
			VkImageViewCreateInfo imageViewCreateInfo = {
				/*.sType=*/              VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				/*.pNext=*/              nullptr,
				/*.flags=*/              0,
				/*.image=*/              swapchainImages[i],
				/*.viewType=*/           VK_IMAGE_VIEW_TYPE_2D,
				/*.format=*/             surfaceFormat.format,
				/*.components=*/         {
					/*.r=*/              VK_COMPONENT_SWIZZLE_IDENTITY,
					/*.g=*/              VK_COMPONENT_SWIZZLE_IDENTITY,
					/*.b=*/              VK_COMPONENT_SWIZZLE_IDENTITY,
					/*.a=*/              VK_COMPONENT_SWIZZLE_IDENTITY
				},
				/*.subresourceRange=*/   {
					/*.aspectMask=*/     VK_IMAGE_ASPECT_COLOR_BIT,
					/*.baseMipLevel=*/   0,
					/*.levelCount=*/     1,
					/*.baseArrayLayer=*/ 0,
					/*.layerCount=*/     1
				}
			};

			err = vd.vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]);
			CHECK_RESULT(err, "vkCreateImageView#swapchain");
		}

		swapchainImagesStatus.resize(swapchainImages.size(), false);

		surfaceImageAvailableSemaphore = createSemaphore();
		submitFinishedSemaphore = createSemaphore();

		return { true };
	}

	Result destroySwapchain() {
		VkResult err;

		assert(device != VK_NULL_HANDLE);

		err = vd.vkDeviceWaitIdle(device);
		CHECK_RESULT(err, "vkDeviceWaitIdle");

		for (auto &imageView : swapchainImageViews) {
			if (imageView != VK_NULL_HANDLE) {
				vd.vkDestroyImageView(device, imageView, nullptr);
				imageView = VK_NULL_HANDLE;
			}
		}
		swapchainImageViews.clear();

		// destroySemaphore(presentQueueFinishedSemaphore);
		// destroySemaphore(graphicQueueFinishedSemaphore);
		destroySemaphore(surfaceImageAvailableSemaphore);
		destroySemaphore(submitFinishedSemaphore);

		if (swapchain != VK_NULL_HANDLE) {
			vd.vkDestroySwapchainKHR(device, swapchain, nullptr);
			swapchain = VK_NULL_HANDLE;
		}

		return { true };
	}

	Result createCommonCommandBuffers() {
		assert(swapchainImages.size() > 0);
		assert(presentCommandPool != VK_NULL_HANDLE);
		assert(graphicCommandPool != VK_NULL_HANDLE);

		presentFences.resize(swapchainImages.size(), VK_NULL_HANDLE);
		graphicFences.resize(swapchainImages.size(), VK_NULL_HANDLE);

		for (VkFence &fence : presentFences) {
			fence = createFence(true);
		}

		for (VkFence &fence : graphicFences) {
			fence = createFence(true);
		}

		presentCommandBuffers = createCommandBuffers(presentCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, swapchainImages.size());
		if (presentCommandBuffers.size() != swapchainImages.size()) {
			return {
				false,
				VK_RESULT_MAX_ENUM,
				"Failed to create present command buffers"
			};
		}

		graphicCommandBuffers = createCommandBuffers(graphicCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, swapchainImages.size());
		if (graphicCommandBuffers.size() != swapchainImages.size()) {
			return {
				false,
				VK_RESULT_MAX_ENUM,
				"Failed to create graphic command buffers"
			};
		}
		return { true };
	}

	VkSemaphore createSemaphore() {
		VkResult err;
		VkSemaphore semaphore;
		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0
		};
		err = vd.vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore);
		assert(err == VK_SUCCESS);

		return semaphore;
	}

	void destroySemaphore(VkSemaphore &semaphore) {
		assert(device != VK_NULL_HANDLE);

		if (semaphore != VK_NULL_HANDLE) {
			vd.vkDestroySemaphore(device, semaphore, nullptr);
			semaphore = VK_NULL_HANDLE;
		}
	}

	VkFence createFence(bool createSignaled) {
		VkResult err;
		VkFence fence;

		VkFenceCreateFlags flags = 0;
		if (createSignaled) {
			flags = VK_FENCE_CREATE_SIGNALED_BIT;
		}

		VkFenceCreateInfo fenceCreateInfo = {
			/*.sType=*/ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*.pNext=*/ nullptr,
			/*.flags=*/ flags
		};

		err = vd.vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
		assert(err == VK_SUCCESS);

		return fence;
	}

	void destroyFence(VkFence &fence) {
		assert(device != VK_NULL_HANDLE);

		if (fence != VK_NULL_HANDLE) {
			vd.vkDestroyFence(device, fence, nullptr);
			fence = VK_NULL_HANDLE;
		}
	}

	VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) {
		VkFormat preferredFormat = VK_FORMAT_B8G8R8A8_UNORM;
		VkColorSpaceKHR preferredColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

		if (surfaceFormats.size() == 0) {
			return {
				/*.format =*/     VK_FORMAT_MAX_ENUM,
				/*.colorSpace =*/ VK_COLOR_SPACE_MAX_ENUM_KHR
			};
		}

		if (surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
			return {
				/*.format =*/     preferredFormat,
				/*.colorSpace =*/ preferredColorSpace
			};
		}

		for (const auto &surfaceFormat : surfaceFormats) {
			if (surfaceFormat.format == preferredFormat) {
				return surfaceFormat;
			}
		}

		return surfaceFormats[0];
	}

	uint32_t selectSurfaceImageCount(VkSurfaceCapabilitiesKHR surfaceCapabilities, VkPresentModeKHR presentMode) {
		uint32_t recommendedImageCounts[] = {
			2 /* VK_PRESENT_MODE_IMMEDIATE_KHR {0} */,
			3 /* VK_PRESENT_MODE_MAILBOX_KHR {1} */,
			2 /* VK_PRESENT_MODE_FIFO_KHR {2} */,
			2 /* VK_PRESENT_MODE_FIFO_RELAXED_KHR {3} */,
		};

		uint32_t imageCount = surfaceCapabilities.minImageCount + 1;

		static_assert(sizeof(uint32_t) >= sizeof(presentMode), "Casting VkPresentModeKHR to uint32_t may be hazardous");
		uint32_t idx = static_cast<uint32_t>(presentMode);

		if (idx < (sizeof(recommendedImageCounts) / sizeof(recommendedImageCounts[0]))) {
			imageCount = recommendedImageCounts[idx];
		}

		if (imageCount < surfaceCapabilities.minImageCount) {
			imageCount = surfaceCapabilities.minImageCount;
		}

		if (imageCount > surfaceCapabilities.maxImageCount) {
			imageCount = surfaceCapabilities.maxImageCount;
		}

		return imageCount;
	}

	VkExtent2D selectSurfaceExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, VkExtent2D fallbackExtent) {
		VkExtent2D extent = fallbackExtent;

		if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() || surfaceCapabilities.currentExtent.height != std::numeric_limits<uint32_t>::max()) {
			extent = surfaceCapabilities.currentExtent;
		}

		if (extent.width < surfaceCapabilities.minImageExtent.width) {
			extent.width = surfaceCapabilities.minImageExtent.width;
		}

		if (extent.width > surfaceCapabilities.maxImageExtent.width) {
			extent.width = surfaceCapabilities.maxImageExtent.width;
		}

		if (extent.height < surfaceCapabilities.minImageExtent.height) {
			extent.height = surfaceCapabilities.minImageExtent.height;
		}

		if (extent.height > surfaceCapabilities.maxImageExtent.height) {
			extent.height = surfaceCapabilities.maxImageExtent.height;
		}

		return extent;
	}

	VkSurfaceTransformFlagBitsKHR selectSurfaceTransform(VkSurfaceCapabilitiesKHR surfaceCapabilities) {
		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}

		return surfaceCapabilities.currentTransform;
	}

	std::vector<VkExtensionProperties> fetchInstanceExtensionProperties() {
		Result result;
		return fetchInstanceExtensionProperties(result);
	}

	std::vector<VkExtensionProperties> fetchInstanceExtensionProperties(Result &result) {
		std::vector<VkExtensionProperties> extensionProperties{};

		uint32_t extensionPropertyCount = 0;
		result.err = vd.vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, nullptr);
		if (result.err != VK_SUCCESS || extensionPropertyCount == 0) {
			result.status = false;
			result.message = "vkEnumerateInstanceExtensionProperties#count";
			return extensionProperties;
		}

		extensionProperties.resize(extensionPropertyCount);
		result.err = vd.vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, extensionProperties.data());
		if (result.err != VK_SUCCESS) {
			result.status = false;
			result.message = "vkEnumerateInstanceExtensionProperties#fetch";
			extensionProperties.clear();
		}

		return extensionProperties;
	}

	std::vector<VkLayerProperties> fetchInstanceLayerProperties() {
		Result result;
		return fetchInstanceLayerProperties(result);
	}

	std::vector<VkLayerProperties> fetchInstanceLayerProperties(Result &result) {
		std::vector<VkLayerProperties> layers{};

		uint32_t layerCount = 0;
		result.err = vd.vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		if (result.err != VK_SUCCESS || layerCount == 0) {
			result.status = false;
			result.message = "vkEnumerateInstanceLayerProperties#count";
			return layers;
		}

		layers.resize(layerCount);
		result.err = vd.vkEnumerateInstanceLayerProperties(&layerCount, layers.data());
		if (result.err != VK_SUCCESS) {
			result.status = false;
			result.message = "vkEnumerateInstanceLayerProperties#fetch";
			layers.clear();
		}

		return layers;
	}

	std::vector<VkQueueFamilyProperties> fetchQueueFamilyProperties() {
		std::vector<VkQueueFamilyProperties> queueFamilyProperties{};

		assert(physicalDevice != VK_NULL_HANDLE);

		uint32_t queueFamilyPropertyCount = 0;
		vd.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);
		if (queueFamilyPropertyCount == 0) {
			return queueFamilyProperties;
		}

		queueFamilyProperties.resize(queueFamilyPropertyCount);
		vd.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

		return queueFamilyProperties;
	}

	std::vector<VkPhysicalDevice> fetchPhysicalDevices() {
		Result result;
		return fetchPhysicalDevices(result);
	}

	std::vector<VkPhysicalDevice> fetchPhysicalDevices(Result &result) {
		std::vector<VkPhysicalDevice> physicalDevices{};

		assert(instance != VK_NULL_HANDLE);

		uint32_t physicalDeviceCount = 0;
		result.err = vd.vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		if (result.err != VK_SUCCESS || physicalDeviceCount == 0) {
			result.status = false;
			result.message = "vkEnumeratePhysicalDevices#count";
			return physicalDevices;
		}

		physicalDevices.resize(physicalDeviceCount);
		result.err = vd.vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
		if (result.err != VK_SUCCESS) {
			result.status = false;
			result.message = "vkEnumeratePhysicalDevices#fetch";
			physicalDevices.clear();
		}

		return physicalDevices;
	}

	std::vector<VkPresentModeKHR> fetchPresentModes() {
		Result result;
		return fetchPresentModes(result);
	}

	std::vector<VkPresentModeKHR> fetchPresentModes(Result &result) {
		std::vector<VkPresentModeKHR> presentModes{};

		assert(physicalDevice != VK_NULL_HANDLE);
		assert(surface != VK_NULL_HANDLE);

		uint32_t presentModeCount = 0;
		result.err = vd.vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		if (result.err != VK_SUCCESS || presentModeCount == 0) {
			result.status = false;
			result.message = "vkGetPhysicalDeviceSurfacePresentModesKHR#count";
			return presentModes;
		}

		presentModes.resize(presentModeCount);
		result.err = vd.vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
		if (result.err != VK_SUCCESS) {
			result.status = false;
			result.message = "vkGetPhysicalDeviceSurfacePresentModesKHR#fetch";
			presentModes.clear();
		}

		return presentModes;
	}

	std::vector<VkSurfaceFormatKHR> fetchSurfaceFormats() {
		Result result;
		return fetchSurfaceFormats(result);
	}

	std::vector<VkSurfaceFormatKHR> fetchSurfaceFormats(Result &result) {
		std::vector<VkSurfaceFormatKHR> surfaceFormats{};

		assert(physicalDevice != VK_NULL_HANDLE);

		uint32_t surfaceFormatCount = 0;
		result.err = vd.vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);
		if (result.err != VK_SUCCESS || surfaceFormatCount == 0) {
			result.status = false;
			result.message = "vkGetPhysicalDeviceSurfaceFormatsKHR#count";
			return surfaceFormats;
		}

		surfaceFormats.resize(surfaceFormatCount);
		result.err = vd.vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());
		if (result.err != VK_SUCCESS) {
			result.status = false;
			result.message = "vkGetPhysicalDeviceSurfaceFormatsKHR#fetch";
			surfaceFormats.clear();
		}

		return surfaceFormats;
	}

	std::vector<VkImage> fetchSwapchainImages() {
		Result result;
		return fetchSwapchainImages(result);
	}

	std::vector<VkImage> fetchSwapchainImages(Result &result) {
		std::vector<VkImage> images{};

		assert(device != VK_NULL_HANDLE);
		assert(swapchain != VK_NULL_HANDLE);

		uint32_t imageCount = 0;
		result.err = vd.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
		if (result.err != VK_SUCCESS || imageCount == 0) {
			result.status = false;
			result.message = "vkGetSwapchainImagesKHR#count";
			return images;
		}

		images.resize(imageCount);
		result.err = vd.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());
		if (result.err != VK_SUCCESS) {
			result.status = false;
			result.message = "vkGetSwapchainImagesKHR#fetch";
			images.clear();
		}

		return images;
	}
};



#define getVkResultString(result) getVkResultStringImpl(result, __FILE__, __LINE__)

std::string getVkResultStringImpl(VkResult result, std::string file, unsigned int line) {
	switch (result) {
		case VK_SUCCESS:
			return file + ":" + std::to_string(line) + ": Success";
		case VK_NOT_READY:
			return file + ":" + std::to_string(line) + ": A fence or query has not yet completed";
		case VK_TIMEOUT:
			return file + ":" + std::to_string(line) + ": A wait operation has not completed in the specified time";
		case VK_EVENT_SET:
			return file + ":" + std::to_string(line) + ": An event is signaled";
		case VK_EVENT_RESET:
			return file + ":" + std::to_string(line) + ": An event is unsignaled";
		case VK_INCOMPLETE:
			return file + ":" + std::to_string(line) + ": A return array was too small for the result";
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			return file + ":" + std::to_string(line) + ": A host memory allocation has failed";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return file + ":" + std::to_string(line) + ": A device memory allocation has failed";
		case VK_ERROR_INITIALIZATION_FAILED:
			return file + ":" + std::to_string(line) + ": Initialization of an object could not be completed for implementation-specific reasons";
		case VK_ERROR_DEVICE_LOST:
			return file + ":" + std::to_string(line) + ": The logical or physical device has been lost";
		case VK_ERROR_MEMORY_MAP_FAILED:
			return file + ":" + std::to_string(line) + ": Mapping of a memory object has failed";
		case VK_ERROR_LAYER_NOT_PRESENT:
			return file + ":" + std::to_string(line) + ": A requested layer is not present or could not be loaded";
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			return file + ":" + std::to_string(line) + ": A requested extension is not supported";
		case VK_ERROR_FEATURE_NOT_PRESENT:
			return file + ":" + std::to_string(line) + ": A requested feature is not supported";
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			return file + ":" + std::to_string(line) + ": The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
		case VK_ERROR_TOO_MANY_OBJECTS:
			return file + ":" + std::to_string(line) + ": Too many objects of the type have already been created";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			return file + ":" + std::to_string(line) + ": A requested format is not supported on this device";
		case VK_ERROR_SURFACE_LOST_KHR:
			return file + ":" + std::to_string(line) + ": A surface is no longer available";
		case VK_SUBOPTIMAL_KHR:
			return file + ":" + std::to_string(line) + ": A swapchain no longer matches the surface properties exactly, but can still be used";
		case VK_ERROR_OUT_OF_DATE_KHR:
			return file + ":" + std::to_string(line) + ": A surface has changed in such a way that it is no longer compatible with the swapchain";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			return file + ":" + std::to_string(line) + ": The display used by a swapchain does not use the same presentable image layout";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			return file + ":" + std::to_string(line) + ": The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
		case VK_ERROR_VALIDATION_FAILED_EXT:
			return file + ":" + std::to_string(line) + ": A validation layer found an error";
		default:
			return file + ":" + std::to_string(line) + ": Unknown error";
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void* /*pUserData*/) {
	cerr << "[VK] ";

	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		cerr << "(INFORMATION) ";
	}

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		cerr << "(WARNING) ";
	}

	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		cerr << "(PERFORMANCE WARNING) ";
	}

	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		cerr << "(ERROR) ";
	}

	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		cerr << "(DEBUG) ";
	}

	cerr << "{" << pLayerPrefix << ":" << messageCode << "}: " << pMessage << endl;

	return VK_FALSE;
}

int proto() {
	std::function<void *()> instanceProcLoader = [&] () -> void * {
		if (glfwInit() != GLFW_TRUE) {
			return nullptr;
		}

		if (glfwVulkanSupported() != GLFW_TRUE) {
			return nullptr;
		}

		return reinterpret_cast<void *>(glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr"));
	};

	Vulkan::Result result;
	Vulkan vulkan(instanceProcLoader);

	uint32_t glfwExtensionCount;
	const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	result = vulkan.createInstance(glfwExtensions, glfwExtensionCount);
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	result = vulkan.attachDebugCallback(2, &vulkanDebugCallback);
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	// create window and surface
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	int windowWidth = 1600;
	int windowHeight = 900;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "pr0-vk", nullptr, nullptr);

	result = vulkan.createSurface([&] (VkInstance instance, VkSurfaceKHR &surface) -> Vulkan::Result {
		VkResult err;

		err = glfwCreateWindowSurface(instance, window, nullptr, &surface);
		CHECK_RESULT(err, "glfwCreateWindowSurface");

		if (surface == VK_NULL_HANDLE) {
			cout << "Could not create surface" << endl;
			return {
				false,
				VK_RESULT_MAX_ENUM,
				"glfwCreateWindowSurface"
			};
		}

		return { true };
	});
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	VkPhysicalDevice physicalDevice = vulkan.selectPhysicalDevice("GTX 770");
	if (physicalDevice == VK_NULL_HANDLE) {
		cout << "Warning: requested physical device \"GTX 770\" not found" << endl;
		physicalDevice = vulkan.selectAnyPhysicalDevice();

		if (physicalDevice == VK_NULL_HANDLE) {
			cout << "Could not find any Vulkan-compatible device" << endl;
			return EXIT_FAILURE;
		}
	}

	result = vulkan.createDevice(physicalDevice);
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	VkPresentModeKHR presentMode;
	presentMode = vulkan.selectPresentMode(Vulkan::PresentMode::NoVSync);
	if (presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
		cout << "Warning: Present mode \"NoVSync\" not supported, trying \"TripleBuffer\"" << endl;

		presentMode = vulkan.selectPresentMode(Vulkan::PresentMode::TripleBuffer);
		if (presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
			cout << "Warning: Present mode \"TripleBuffer\" not supported, trying \"VSyncAdap\"" << endl;

			presentMode = vulkan.selectPresentMode(Vulkan::PresentMode::VSyncAdap);
			if (presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
				cout << "Warning: Present mode \"VSyncAdap\" not supported, trying \"VSync\"" << endl;

				presentMode = vulkan.selectPresentMode(Vulkan::PresentMode::VSync);
				if (presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
					cout << "Warning: Present mode \"VSync\" not supported" << endl;
					presentMode = vulkan.selectAnyPresentMode();
					cout << "I'm giving up, selected any available present mode" << endl;
				} else {
					cout << "Selected \"VSync\" present mode" << endl;
				}
			} else {
				cout << "Selected \"VSyncAdap\" present mode" << endl;
			}
		} else {
			cout << "Selected \"TripleBuffer\" present mode" << endl;
		}
	} else {
		cout << "Selected \"NoVSync\" present mode" << endl;
	}

	result = vulkan.createSwapchain(windowWidth, windowHeight, presentMode, VK_NULL_HANDLE);
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	result = vulkan.createCommonCommandBuffers();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	result = vulkan.createRenderPass();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	result = vulkan.createFrameBuffers();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	result = vulkan.createGraphicPipeline();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	double start = glfwGetTime();
	result = vulkan.createVertexBuffer();
	double stop = glfwGetTime();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	} else {
		cout << "Created vertex buffers in " << ((stop - start) * 1000.0) << " ms" << endl;
	}

	for (uint32_t i = 0; /*i < 10 &&*/ ! glfwWindowShouldClose(window); i++) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		auto start = glfwGetTime();

		uint32_t targetIdx;
		result = vulkan.acquireNextImage(targetIdx);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return EXIT_FAILURE;
		}

		uint32_t availableId = vulkan.findAvailablePresentCommandBufferIndex();

		result = vulkan.recordCommands(targetIdx, availableId);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return EXIT_FAILURE;
		}

		result = vulkan.submitGraphicQueue(availableId);
		// result = vulkan.submitPresentQueue(availableId);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return EXIT_FAILURE;
		}

		result = vulkan.queuePresent(targetIdx);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return EXIT_FAILURE;
		}

		auto stop = glfwGetTime();

		glfwSetWindowTitle(window, (std::to_string((stop - start) * 1000.0) + "ms").c_str());

		glfwPollEvents();
	}

	vulkan.destroyVertexBuffer();

	vulkan.destroyGraphicPipeline();
	vulkan.destroyFrameBuffers();
	vulkan.destroyRenderPass();

	result = vulkan.destroySwapchain();
	if ( ! result) {
		cout << "Message: " << (result.message ? result.message : "[none]") << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	vulkan.destroySurface();

	result = vulkan.destroyDevice();
	if ( ! result) {
		cout << "Message: " << (result.message ? result.message : "[none]") << ", error: " << getVkResultString(result.err) << endl;
		return EXIT_FAILURE;
	}

	glfwDestroyWindow(window);

	vulkan.destroyInstance();

	return EXIT_SUCCESS;
}