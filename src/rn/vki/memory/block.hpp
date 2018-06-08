#pragma once

#include <cstddef>
#include <limits>

#include <vulkan/vulkan.hpp>

#include "mapping.hpp"

namespace rn::vki::memory {

struct BlockAllocationHandle {
	vk::DeviceMemory memory{};
	vk::DeviceSize offset{std::numeric_limits<vk::DeviceSize>::max()};
	uint32_t leafIdx = std::numeric_limits<uint32_t>::max();
	uint32_t memoryTypeIndex = std::numeric_limits<uint32_t>::max();
	vk::MemoryPropertyFlags flags{};
	void *pointer = nullptr;
};

class Block {
public:
	vk::UniqueDeviceMemory deviceMemory;
	Mapping mapping;
	vk::DeviceSize blockSize;
	uint32_t levels;
	uint32_t memoryTypeIndex;
	vk::MemoryPropertyFlags flags;

	std::vector<bool> leafs;

	explicit Block(vk::UniqueDeviceMemory &&deviceMemory, Mapping &&mapping, vk::DeviceSize blockSize, uint32_t levels, uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags);

	Block(Block &&other) noexcept;
	Block & operator=(Block &&other) noexcept;

	Block(const Block &) = delete;
	Block & operator=(const Block &) = delete;

	~Block();

	BlockAllocationHandle alloc(const vk::MemoryRequirements &requirements);
	void free(uint32_t leafIdx);

	uint32_t levelFromIdx(uint32_t idx) const;
	int32_t findLevel(vk::DeviceSize requiredSize) const;
	vk::DeviceSize findOffset(uint32_t idx, const vk::MemoryRequirements &requirements) const;

	void markUp(uint32_t idx);
	void unmarkUp(uint32_t idx);
	void markDown(uint32_t idx);
	void unmarkDown(uint32_t idx);
};

} // rn::vki::memory
