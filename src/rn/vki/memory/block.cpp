#include "block.hpp"

#include <stdexcept>

#include "../../../ngn/log.hpp"
#include "../../../util/leadingZeroes.hpp"
#include "../../../util/map.hpp"
#include "../../../util/join.hpp"
#include "../id.hpp"

namespace rn::vki::memory {

Block::Block(rn::vki::UniqueDeviceMemory &&deviceMemory, Mapping &&mapping, vk::DeviceSize blockSize, uint32_t levels, uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags) :
	deviceMemory{std::move(deviceMemory)},
	mapping{std::move(mapping)},
	blockSize{blockSize},
	levels{levels},
	memoryTypeIndex{memoryTypeIndex},
	flags{flags},
	leafs((1u << levels) - 1u, false)
{}

Block::Block(Block &&other) noexcept :
	deviceMemory{std::move(other.deviceMemory)},
	mapping{std::move(other.mapping)},
	blockSize{std::move(other.blockSize)},
	levels{other.levels},
	memoryTypeIndex{other.memoryTypeIndex},
	flags{std::move(other.flags)},
	leafs{std::move(other.leafs)}
{
	other.deviceMemory = rn::vki::UniqueDeviceMemory{};
}

Block & Block::operator=(Block &&other) noexcept {
	reset();

	deviceMemory = std::move(other.deviceMemory);
	mapping = std::move(other.mapping);
	blockSize = std::move(other.blockSize);
	levels = other.levels;
	memoryTypeIndex = other.memoryTypeIndex;
	flags = std::move(other.flags);
	leafs = std::move(other.leafs);

	other.deviceMemory = rn::vki::UniqueDeviceMemory{};

	return *this;
}

Block::~Block() {
	reset();
}

BlockAllocationHandle Block::alloc(const vk::MemoryRequirements2 &requirements) {
	if ( ! deviceMemory) {
		throw std::runtime_error{"Memory block not initialized"};
	}

	int32_t level = findLevel(requirements.memoryRequirements.size);

	while (level >= 0) {
		uint32_t start = (1u << level) - 1u;
		uint32_t end = (1u << (level + 1u)) - 1u;

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
				deviceMemory.handle(),
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

bool Block::isEmpty() {
	return leafs.size() == 0 || ! leafs[0];
}

void Block::reset() {
	if (deviceMemory) {
		if ( ! isEmpty()) {
			ngn::log::error("rn::vki::memory::Block::reset() <{:#x}> => some allocations were not freed before block destruction [{}]", rn::vki::id(deviceMemory.get()), util::join(util::map(leafs, [] (bool leaf) {
				return std::string_view{leaf ? "1" : "0"};
			}), ""));
		} else {
			ngn::log::error("rn::vki::memory::Block::reset() <{:#x}> => block destroyed", rn::vki::id(deviceMemory.get()));
		}

		mapping.reset();
		deviceMemory = {};
	}
}

uint32_t Block::levelFromIdx(uint32_t idx) const {
	return 31u - util::leadingZeroes(idx + 1u);
}

int32_t Block::findLevel(vk::DeviceSize requiredSize) const {
	vk::DeviceSize leafSize = blockSize / (1u << (levels - 1u));

	for (uint32_t i = 0u; i < levels; i++) {
		if (requiredSize <= (leafSize << i)) {
			return static_cast<int32_t>(levels - 1u - i);
		}
	}

	return -1;
}

vk::DeviceSize Block::findOffset(uint32_t idx, const vk::MemoryRequirements2 &requirements) const {
	uint32_t levelSize = 1u << levelFromIdx(idx);
	uint32_t idxOffset = (idx + 1u) - levelSize;

	vk::DeviceSize leafSize = blockSize / levelSize;
	vk::DeviceSize leafOffset = leafSize * idxOffset;

	vk::DeviceSize offset = ((leafOffset + requirements.memoryRequirements.alignment - 1u) / requirements.memoryRequirements.alignment) * requirements.memoryRequirements.alignment;
	vk::DeviceSize end = offset + requirements.memoryRequirements.size;

	if (end <= leafOffset + leafSize) {
		return offset;
	} else {
		return std::numeric_limits<vk::DeviceSize>::max();
	}
}

void Block::markUp(uint32_t idx) {
	if (idx >= leafs.size()) {
		return;
	}

	while (true) {
		if (leafs[idx]) {
			return;
		}

		leafs[idx] = true;

		if (idx == 0u) {
			return;
		}

		idx = (idx - (2u - (idx & 1u))) / 2u;
	}
}

void Block::unmarkUp(uint32_t idx) {
	if (idx >= leafs.size()) {
		return;
	}

	while (true) {
		leafs[idx] = false;

		if (idx == 0u) {
			return;
		}

		uint32_t neighbor = idx - 1u + 2u * (idx & 1);

		if (leafs[neighbor]) {
			return;
		}

		idx = (idx - (2u - (idx & 1u))) / 2u;
	}
}

void Block::markDown(uint32_t idx) {
	if (idx >= leafs.size()) {
		return;
	}

	uint32_t left = idx;
	uint32_t right = idx;

	uint32_t level = levelFromIdx(idx);

	for (uint32_t i = 0u; i < (levels - 1u - level); i++) {
		left = left * 2u + 1u;
		right = right * 2u + 2u;

		for (uint32_t idx = left; idx <= right; idx++) {
			leafs[idx] = true;
		}
	}
}

void Block::unmarkDown(uint32_t idx) {
	if (idx >= leafs.size()) {
		return;
	}

	uint32_t left = idx;
	uint32_t right = idx;

	uint32_t level = levelFromIdx(idx);

	for (uint32_t i = 0u; i < (levels - 1u - level); i++) {
		left = left * 2u + 1u;
		right = right * 2u + 2u;

		for (uint32_t idx = left; idx <= right; idx++) {
			leafs[idx] = false;
		}
	}
}

} // rn::vki::memory
