#include "block.hpp"

#include <stdexcept>

#include "../../ngn/log.hpp"
#include "../../util/leadingZeroes.hpp"
#include "../id.hpp"

namespace rn::vlk::memory {

constexpr uint32_t levelFromIdx(uint32_t idx) {
	return 31 - util::leadingZeroes(idx + 1);
}

Block::Block(vk::UniqueDeviceMemory &&deviceMemory, Mapping &&mapping, vk::DeviceSize blockSize, uint32_t levels, uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags) :
	deviceMemory{std::move(deviceMemory)},
	mapping{std::move(mapping)},
	blockSize{blockSize},
	levels{levels},
	memoryTypeIndex{memoryTypeIndex},
	flags{flags}
{
	if ( ! deviceMemory.get()) {
		throw std::runtime_error{"Memory block requires device memory"};
	}

	leafs.resize((1u << levels) - 1, false);
}

Block::Block(Block &&other) noexcept :
	deviceMemory{std::move(other.deviceMemory)},
	mapping{std::move(other.mapping)},
	blockSize{std::move(other.blockSize)},
	levels{std::move(other.levels)},
	memoryTypeIndex{std::move(other.memoryTypeIndex)},
	flags{std::move(other.flags)},
	leafs{std::move(other.leafs)}
{
	other.deviceMemory = vk::UniqueDeviceMemory{};
}

Block & Block::operator=(Block &&other) noexcept {
	deviceMemory = std::move(other.deviceMemory);
	mapping = std::move(other.mapping);
	blockSize = std::move(other.blockSize);
	levels = std::move(other.levels);
	memoryTypeIndex = std::move(other.memoryTypeIndex);
	flags = std::move(other.flags);
	leafs = std::move(other.leafs);

	other.deviceMemory = vk::UniqueDeviceMemory{};

	return *this;
}

Block::~Block() {
	if (deviceMemory) {
		if (leafs.size() && leafs[0]) {
			ngn::log::error("Some allocations were not freed before Block was destroyed: {:x}", rn::vlk::id(deviceMemory));
		}
	}
}

BlockAllocationHandle Block::alloc(const vk::MemoryRequirements &requirements) {
	int32_t level = findLevel(requirements.size);

	while (level >= 0) {
		uint32_t start = (1u << level) - 1;
		uint32_t end = (1u << (level + 1)) - 1;

		bool hasFreeLeafs = false;

		for (uint32_t leafIdx = start; leafIdx < end; leafIdx++) {
			if (leafs[leafIdx]) {
				// leaf already in use
				continue;
			}

			hasFreeLeafs = true;

			vk::DeviceSize offset = findOffset(leafIdx, requirements);

			if (offset == std::numeric_limits<vk::DeviceSize>::max()) {
				// offset not within leaf address range
				continue;
			}

			markUp(leafIdx);
			markDown(leafIdx);

			return BlockAllocationHandle{
				deviceMemory.get(),
				offset,
				leafIdx,
				memoryTypeIndex,
				flags,
				mapping.get(offset)
			};
		}

		if ( ! hasFreeLeafs) {
			break;
		} else {
			// try at parent level -- probably due to alignment the subblock could not fit in any of the leafs
			level--;
		}
	}

	return BlockAllocationHandle{};
}

void Block::free(uint32_t leafIdx) {
	unmarkUp(leafIdx);
	unmarkDown(leafIdx);
}

int32_t Block::findLevel(vk::DeviceSize requiredSize) const {
	vk::DeviceSize leafSize = blockSize / (1u << (levels - 1));

	for (uint32_t i = 0; i < levels; i++) {
		if (requiredSize <= (leafSize << i)) {
			return static_cast<int32_t>(levels - 1 - i);
		}
	}

	return -1;
}

vk::DeviceSize Block::findOffset(uint32_t idx, const vk::MemoryRequirements &requirements) const {
	uint32_t level = levelFromIdx(idx);
	uint32_t idxOffset = (idx + 1u) - (1u << level);

	vk::DeviceSize leafSize = blockSize / (1u << level);
	vk::DeviceSize leafOffset = leafSize * idxOffset;

	vk::DeviceSize offset = ((leafOffset + requirements.alignment - 1u) / requirements.alignment) * requirements.alignment;
	vk::DeviceSize end = offset + requirements.size;

	if (end <= leafOffset + leafSize) {
		return offset;
	} else {
		return std::numeric_limits<vk::DeviceSize>::max();
	}
}

void Block::markUp(uint32_t idx) {
	while (true) {
		if (leafs[idx]) {
			return;
		}

		leafs[idx] = true;

		if (idx == 0) {
			return;
		}

		idx = (idx - (idx % 2u == 0 ? 2u : 1u)) / 2u;
	}
}

void Block::unmarkUp(uint32_t idx) {
	while (true) {
		leafs[idx] = false;

		if (idx == 0) {
			return;
		}

		uint32_t neighbor = idx % 2 == 0 ? idx - 1 : idx + 1;

		if (leafs[neighbor]) {
			return;
		}

		idx = (idx - (idx % 2u == 0 ? 2u : 1u)) / 2u;
	}
}

void Block::markDown(uint32_t idx) {
	uint32_t left = idx;
	uint32_t right = idx;

	uint32_t level = levelFromIdx(idx);

	for (uint32_t i = 0; i < (levels - 1 - level); i++) {
		left = left * 2 + 1;
		right = right * 2 + 2;

		for (uint32_t idx = left; idx <= right; idx++) {
			leafs[idx] = true;
		}
	}
}

void Block::unmarkDown(uint32_t idx) {
	uint32_t left = idx;
	uint32_t right = idx;

	uint32_t level = levelFromIdx(idx);

	for (uint32_t i = 0; i < (levels - 1 - level); i++) {
		left = left * 2 + 1;
		right = right * 2 + 2;

		for (uint32_t idx = left; idx <= right; idx++) {
			leafs[idx] = false;
		}
	}
}

} // rn::vlk::memory
