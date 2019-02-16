#pragma once

#include <cstddef>
#include <limits>

#include <vulkan/vulkan.hpp>

#include "../dispatch.hpp"
#include "mapping.hpp"

namespace rn::vki::memory {

struct BlockAllocationHandle {
	rn::vki::HandleDeviceMemory memory{};
	vk::DeviceSize offset{std::numeric_limits<vk::DeviceSize>::max()};
	uint32_t leafIdx = std::numeric_limits<uint32_t>::max();
	uint32_t memoryTypeIndex = std::numeric_limits<uint32_t>::max();
	vk::MemoryPropertyFlags flags{};
	void *pointer = nullptr;
};

class Block {
public:
	rn::vki::UniqueDeviceMemory deviceMemory;
	Mapping mapping;
	vk::DeviceSize blockSize;
	uint32_t levels;
	uint32_t memoryTypeIndex;
	vk::MemoryPropertyFlags flags;

	std::vector<bool> leafs;

	Block(rn::vki::UniqueDeviceMemory &&deviceMemory, Mapping &&mapping, vk::DeviceSize blockSize, uint32_t levels, uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags);

	Block(Block &&other) noexcept;
	Block & operator=(Block &&other) noexcept;

	Block(const Block &) = delete;
	Block & operator=(const Block &) = delete;

	~Block();

	BlockAllocationHandle alloc(const vk::MemoryRequirements2 &requirements);
	void free(uint32_t leafIdx);
	bool isEmpty();
	void reset();

	operator bool() {
		return static_cast<bool>(deviceMemory);
	}

	bool operator !() {
		return !static_cast<bool>(deviceMemory);
	}

	uint32_t levelFromIdx(uint32_t idx) const;
	int32_t findLevel(vk::DeviceSize requiredSize) const;
	vk::DeviceSize findOffset(uint32_t idx, const vk::MemoryRequirements2 &requirements) const;

	void markUp(uint32_t idx);
	void unmarkUp(uint32_t idx);
	void markDown(uint32_t idx);
	void unmarkDown(uint32_t idx);
};

} // rn::vki::memory
