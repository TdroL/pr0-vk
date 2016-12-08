#pragma once

#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class GLFW {
public:
	bool initialized = false;

	GLFW() = default;
	explicit GLFW(bool initialized)
		: initialized{initialized} {}

	GLFW(GLFW &&rhs)
		: initialized{rhs.initialized} {
		rhs.initialized = false;
	}

	GLFW & operator=(GLFW &&rhs) {
		initialized = rhs.initialized;
		rhs.initialized = false;

		return *this;
	}

	GLFW(const GLFW &rhs) = delete;
	GLFW & operator=(const GLFW &rhs) = delete;

	operator bool() const {
		return initialized;
	}

	bool operator !() const {
		return !initialized;
	}

	void terminate() {
		if (initialized) {
			glfwTerminate();
			initialized = false;
		}
	}

	std::vector<std::string> requiredInstanceExtensions() const {
		if ( ! initialized) {
			throw std::runtime_error{"GLFW not initialized"};
		}

		uint32_t count;
		const char **extensions = glfwGetRequiredInstanceExtensions(&count);

		return std::vector<std::string>(extensions, extensions + count);
	}

	bool physicalDeviceSupported(const vk::Instance &instance, const vk::PhysicalDevice &physicalDevice) const {
		if ( ! initialized) {
			throw std::runtime_error{"GLFW not initialized"};
		}

		uint32_t queueFamilyPropertyCount;
		physicalDevice.getQueueFamilyProperties(&queueFamilyPropertyCount, nullptr);

		for (size_t i = 0; i < queueFamilyPropertyCount; i++) {
			if (getFamilyQueuePresentationSupport(instance, physicalDevice, i)) {
				return true;
			}
		}

		return false;
	}

	bool getFamilyQueuePresentationSupport(const vk::Instance &instance, const vk::PhysicalDevice &physicalDevice, uint32_t familyIndex) const {
		return glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, familyIndex);
	}
};

} // vlk

} // rn
