#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "block.hpp"
#include "handle.hpp"

namespace rn::vlk::memory {

enum class Usage {
	GPU,
	CPU,
	CPU_TO_GPU,
	GPU_TO_CPU
};

class Pool {
public:
	vk::Device device{};
	vk::PhysicalDeviceMemoryProperties memoryProperties{};
	uint32_t baseLevels{};
	vk::DeviceSize baseMemorySize{};

	std::vector<Block> blocks{};

	Pool() = default;

	explicit Pool(vk::Device device, vk::PhysicalDeviceMemoryProperties memoryProperties, uint32_t baseLevels, vk::DeviceSize baseMemorySize) noexcept;

	Pool(Pool &&other) noexcept;
	Pool & operator=(Pool &&other) noexcept;

	Pool(const Pool &) = delete;
	Pool & operator=(const Pool &) = delete;

	Handle alloc(const Usage usage, const vk::MemoryRequirements &requirements);

	vk::MemoryPropertyFlags getPreferredPropertyFlags(const Usage usage) const;
	vk::MemoryPropertyFlags getRequiredPropertyFlags(const Usage usage) const;

	uint32_t findMemoryTypeIndex(vk::MemoryPropertyFlags flags, const vk::MemoryRequirements &requirements, uint32_t start = 0) const;
	Block & appendBlock(uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags, const vk::MemoryRequirements &requirements);
};

} // rn::vlk::memory
