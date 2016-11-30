#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class DeviceOwner {
public:
	vk::Device handle{};
	vk::PhysicalDevice physicalDevice{};
	vk::PhysicalDeviceProperties physicalDeviceProperties{};
	// vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};
	// vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	// vk::PhysicalDeviceLimits physicalDeviceLimits{};
	// vk::PhysicalDeviceSparseProperties physicalDeviceSparseProperties{};
	// std::vector<vk::QueueFamilyProperties> queueFamilyProperties{};

	DeviceOwner() = default;
	DeviceOwner(
		vk::Device &&device,
		vk::PhysicalDevice &&physicalDevice
	) :
		handle{std::move(device)},
		physicalDevice{std::move(physicalDevice)} {

		device = vk::Device{};
		physicalDevice = vk::PhysicalDevice{};

		if (handle) {
			std::cout << "creating vk::Device" << std::endl;
		}
	}

	DeviceOwner(DeviceOwner &&rhs)
		: handle{std::move(rhs.handle)}, physicalDevice{std::move(rhs.physicalDevice)} {
		rhs.handle = vk::Device{};
		rhs.physicalDevice = vk::PhysicalDevice{};

		if (handle) {
			std::cout << "creating vk::Device" << std::endl;
		}
	}

	DeviceOwner & operator=(DeviceOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		physicalDevice = std::move(rhs.physicalDevice);

		rhs.handle = vk::Device{};
		rhs.physicalDevice = vk::PhysicalDevice{};

		return *this;
	}

	void destroy() {
		if (handle) {
			std::cout << "destroying vk::Device" << std::endl;
			handle.destroy();

			handle = vk::Device{};
			physicalDevice = vk::PhysicalDevice{};
		}
	}

	DeviceOwner(const DeviceOwner &rhs) = delete;
	DeviceOwner & operator=(const DeviceOwner &rhs) = delete;

	~DeviceOwner() {
		destroy();
	}
};

} // vlk

} // rn
