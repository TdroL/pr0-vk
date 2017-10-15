#pragma once

#include <GLFW/glfw3.h>

namespace rn {

class GLFW {
public:
	static uint32_t initialized;

	GLFW();
	~GLFW();

	/*
	bool initialized = false;

	GLFW() = default;
	explicit GLFW(bool initialized)
		: initialized{initialized} {
		if (initialized) {
			glfwSetErrorCallback(glfwErrorCallback);
		}
	}

	GLFW(GLFW &&other)
		: initialized{other.initialized} {
		other.initialized = false;
	}

	GLFW & operator=(GLFW &&other) {
		initialized = other.initialized;
		other.initialized = false;

		return *this;
	}

	GLFW(const GLFW &other) = delete;
	GLFW & operator=(const GLFW &other) = delete;

	operator bool() const {
		return initialized;
	}

	bool operator !() const {
		return ! initialized;
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

		for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
			if (getFamilyQueuePresentationSupport(instance, physicalDevice, i)) {
				return true;
			}
		}

		return false;
	}

	bool getFamilyQueuePresentationSupport(const vk::Instance &instance, const vk::PhysicalDevice &physicalDevice, uint32_t familyIndex) const {
		if ( ! initialized) {
			throw std::runtime_error{"GLFW not initialized"};
		}

		return glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, familyIndex);
	}

	void pollEvents() {
		if ( ! initialized) {
			throw std::runtime_error{"GLFW not initialized"};
		}

		glfwPollEvents();
	}
	*/
};

extern GLFW glfw;

} // rn
