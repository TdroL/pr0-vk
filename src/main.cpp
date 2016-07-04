#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <functional>

#include "Vulkan.hpp"

using std::cout;
using std::cerr;
using std::endl;

int windowWidth = 1600;
int windowHeight = 900;

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

int main() {
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
		return 1;
	}

	result = vulkan.attachDebugCallback(2, &vulkanDebugCallback);
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
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
		return 1;
	}

	VkPhysicalDevice physicalDevice = vulkan.selectPhysicalDevice("GTX 770");
	if (physicalDevice == VK_NULL_HANDLE) {
		cout << "Warning: requested physical device \"GTX 770\" not found" << endl;
		physicalDevice = vulkan.selectAnyPhysicalDevice();

		if (physicalDevice == VK_NULL_HANDLE) {
			cout << "Could not find any Vulkan-compatible device" << endl;
			return 1;
		}
	}

	result = vulkan.createDevice(physicalDevice);
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
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
		return 1;
	}

	result = vulkan.createCommonCommandBuffers();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
	}

	result = vulkan.createRenderPass();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
	}

	result = vulkan.createFrameBuffers();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
	}

	result = vulkan.createGraphicPipeline();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
	}

	double start = glfwGetTime();
	result = vulkan.createVertexBuffer();
	double stop = glfwGetTime();
	if ( ! result) {
		cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
		return 1;
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
			return 1;
		}

		uint32_t availableId = vulkan.findAvailablePresentCommandBufferIndex();

		result = vulkan.recordCommands(targetIdx, availableId);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return 1;
		}

		result = vulkan.submitGraphicQueue(availableId);
		// result = vulkan.submitPresentQueue(availableId);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return 1;
		}

		result = vulkan.queuePresent(targetIdx);
		if ( ! result) {
			cout << "Message: " << result.message << ", error: " << getVkResultString(result.err) << endl;
			return 1;
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
		return 1;
	}

	vulkan.destroySurface();

	result = vulkan.destroyDevice();
	if ( ! result) {
		cout << "Message: " << (result.message ? result.message : "[none]") << ", error: " << getVkResultString(result.err) << endl;
		return 1;
	}

	glfwDestroyWindow(window);

	vulkan.destroyInstance();

	return 0;
}