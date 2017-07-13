#pragma once

#include <stdexcept>
#include <vulkan/vulkan.hpp>

#include "../../ngn/log.hpp"

namespace rn {

namespace vlk {

class PhysicalDeviceHandle {
public:
	vk::PhysicalDevice handle{};
	vk::PhysicalDeviceProperties properties{};
	vk::PhysicalDeviceMemoryProperties memoryProperties{};
	vk::PhysicalDeviceFeatures availableFeatures{};
	vk::PhysicalDeviceFeatures requiredFeatures{};

	PhysicalDeviceHandle() = default;
	PhysicalDeviceHandle(
		vk::PhysicalDevice &&physicalDevice,
		vk::PhysicalDeviceProperties &&properties,
		vk::PhysicalDeviceMemoryProperties &&memoryProperties,
		vk::PhysicalDeviceFeatures &&availableFeatures,
		const vk::PhysicalDeviceFeatures &requiredFeatures
	) :
		handle{std::move(physicalDevice)},
		properties{std::move(properties)},
		memoryProperties{std::move(memoryProperties)},
		availableFeatures{std::move(availableFeatures)},
		requiredFeatures{requiredFeatures}
	{

		physicalDevice = vk::PhysicalDevice{};
		properties = vk::PhysicalDeviceProperties{};
		memoryProperties = vk::PhysicalDeviceMemoryProperties{};
		availableFeatures = vk::PhysicalDeviceFeatures{};
	}

	PhysicalDeviceHandle(PhysicalDeviceHandle &&rhs) :
		handle{std::move(rhs.handle)},
		properties{std::move(rhs.properties)},
		memoryProperties{std::move(rhs.memoryProperties)},
		availableFeatures{std::move(rhs.availableFeatures)},
		requiredFeatures{std::move(rhs.requiredFeatures)}
	{

		rhs.handle = vk::PhysicalDevice{};
		rhs.properties = vk::PhysicalDeviceProperties{};
		rhs.memoryProperties = vk::PhysicalDeviceMemoryProperties{};
		rhs.availableFeatures = vk::PhysicalDeviceFeatures{};
		rhs.requiredFeatures = vk::PhysicalDeviceFeatures{};
	}

	PhysicalDeviceHandle & operator=(PhysicalDeviceHandle &&rhs) {
		handle = std::move(rhs.handle);
		properties = std::move(rhs.properties);
		memoryProperties = std::move(rhs.memoryProperties);
		availableFeatures = std::move(rhs.availableFeatures);
		requiredFeatures = std::move(rhs.requiredFeatures);

		rhs.handle = vk::PhysicalDevice{};
		rhs.properties = vk::PhysicalDeviceProperties{};
		rhs.memoryProperties = vk::PhysicalDeviceMemoryProperties{};
		rhs.availableFeatures = vk::PhysicalDeviceFeatures{};
		rhs.requiredFeatures = vk::PhysicalDeviceFeatures{};

		return *this;
	}

	PhysicalDeviceHandle(const PhysicalDeviceHandle &rhs) = default;
	PhysicalDeviceHandle & operator=(const PhysicalDeviceHandle &rhs) = default;
};

} // vlk

} // rn
