#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

#include "../../ngn/log.hpp"

namespace rn {

namespace vlk {

class DeviceOwner {
public:
	vk::Device handle{};
	vk::PhysicalDeviceFeatures features{};

	DeviceOwner() = default;
	DeviceOwner(
		vk::Device &&device,
		vk::PhysicalDeviceFeatures &&features
	) :
		handle{std::move(device)},
		features{std::move(features)} {

		device = vk::Device{};
		features = vk::PhysicalDeviceFeatures{};
	}

	DeviceOwner(DeviceOwner &&rhs)
		: handle{std::move(rhs.handle)},
		features{std::move(rhs.features)} {

		rhs.handle = vk::Device{};
		rhs.features = vk::PhysicalDeviceFeatures{};
	}

	DeviceOwner & operator=(DeviceOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		features = std::move(rhs.features);

		rhs.handle = vk::Device{};
		rhs.features = vk::PhysicalDeviceFeatures{};

		return *this;
	}

	void destroy() {
		if (handle) {
			handle.waitIdle();
			handle.destroy();

			handle = vk::Device{};
			features = vk::PhysicalDeviceFeatures{};
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
