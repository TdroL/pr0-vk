#include "pool.hpp"

#include <stdexcept>

#include "../../ngn/log.hpp"
#include "../id.hpp"
#include "../trace.hpp"
#include "mapping.hpp"

namespace rn::vki::memory {

namespace {

Device dummyDevice{};

}

Pool::Pool() noexcept :
	device(dummyDevice)
{}

Pool::Pool(Device &device, vk::PhysicalDeviceMemoryProperties memoryProperties, uint32_t baseLevels, vk::DeviceSize baseMemorySize) noexcept :
	device(device),
	memoryProperties{memoryProperties},
	baseLevels{baseLevels},
	baseMemorySize{baseMemorySize}
{}

Pool::Pool(Pool &&other) noexcept :
	device(other.device),
	memoryProperties{std::move(other.memoryProperties)},
	baseLevels{std::move(other.baseLevels)},
	baseMemorySize{std::move(other.baseMemorySize)},
	blocks{std::move(other.blocks)}
{
	other.blocks.clear();
}

Pool & Pool::operator=(Pool &&other) noexcept {
	blocks.clear();

	device = other.device;
	memoryProperties = std::move(other.memoryProperties);
	baseLevels = std::move(other.baseLevels);
	baseMemorySize = std::move(other.baseMemorySize);
	blocks = std::move(other.blocks);

	other.blocks.clear();

	return *this;
}

Handle Pool::alloc(const Usage usage, const vk::MemoryRequirements &requirements) {
	if ( ! device) {
		throw std::runtime_error{"Pool not initialized"};
	}

	uint32_t start = 0;
	while (start < memoryProperties.memoryTypeCount) {
		vk::MemoryPropertyFlags preferred = getPreferredPropertyFlags(usage);
		vk::MemoryPropertyFlags required = getRequiredPropertyFlags(usage);

		uint32_t memoryTypeIndex = findMemoryTypeIndex(preferred, requirements, start);
		if (memoryTypeIndex == std::numeric_limits<uint32_t>::max()) {
			memoryTypeIndex = findMemoryTypeIndex(required, requirements, start);
		}

		if (memoryTypeIndex == std::numeric_limits<uint32_t>::max()) {
			throw std::runtime_error{"Could not find a valid memory type with " + std::to_string(static_cast<uint32_t>(preferred)) + "/" + std::to_string(static_cast<uint32_t>(required)) + " flag for " + std::to_string(requirements.memoryTypeBits) + " types"};
		}

		for (uint32_t blockIdx = 0; blockIdx < blocks.size(); blockIdx++) {
			auto &block = blocks[blockIdx];

			if (block.memoryTypeIndex != memoryTypeIndex) {
				continue;
			}

			BlockAllocationHandle allocation = block.alloc(requirements);
			if (allocation.memory) {
				ngn::log::debug("rn::vki::memory::Pool::alloc({} bytes) => allocated memory: {} bytes offset, {} leaf(s)", requirements.size, allocation.offset, 1u << (block.levels - block.levelFromIdx(allocation.leafIdx)));
				return Handle{
					allocation.memory,
					allocation.offset,
					allocation.flags,
					allocation.pointer,
					this,
					blockIdx,
					allocation.leafIdx
				};
			}
		}

		try {
			Block &block = appendBlock(memoryTypeIndex, memoryProperties.memoryTypes[memoryTypeIndex].propertyFlags, requirements);

			BlockAllocationHandle allocation = block.alloc(requirements);
			if (allocation.memory) {
				ngn::log::debug("rn::vki::memory::Pool::alloc({} bytes) => allocated memory: {} bytes offset, {} leaf(s)", requirements.size, allocation.offset, 1u << (block.levels - block.levelFromIdx(allocation.leafIdx)));
				return Handle{
					allocation.memory,
					allocation.offset,
					allocation.flags,
					allocation.pointer,
					this,
					static_cast<uint32_t>(blocks.size() - 1),
					allocation.leafIdx
				};
			}
		} catch (const vk::OutOfHostMemoryError &e) {
			if (memoryTypeIndex + 1 < memoryProperties.memoryTypeCount) {
				//  try another memory type
				ngn::log::warn("rn::vki::memory::Pool::alloc({} bytes) => out of host memory for memory type {:x}", requirements.size, memoryTypeIndex);

				start = memoryTypeIndex + 1;
			} else {
				// re-throw
				throw e;
			}
		} catch (const vk::OutOfDeviceMemoryError &e) {
			if (memoryTypeIndex + 1 < memoryProperties.memoryTypeCount) {
				//  try another memory type
				ngn::log::warn("rn::vki::memory::Pool::alloc({} bytes) => out of device memory for memory type {:x}", requirements.size, memoryTypeIndex);

				start = memoryTypeIndex + 1;
			} else {
				// re-throw
				throw e;
			}
		}
	}

	throw std::runtime_error{"Could not allocate memory"};
}

void Pool::free(const Handle &handle) {
	if (handle.memory == vk::DeviceMemory{} || handle.blockIdx == std::numeric_limits<uint32_t>::max() || handle.leafIdx == std::numeric_limits<uint32_t>::max()) {
		return;
	}

	if (handle.blockIdx >= blocks.size()) {
		ngn::log::error("rn::vki::memory::Pool::free({}, {}, {}) => unable to free memory handle, block index out-of-range: {} => {}", rn::vki::id(handle.memory), handle.blockIdx, handle.leafIdx, handle.blockIdx, blocks.size());
	} else {
		blocks[handle.blockIdx].free(handle.leafIdx);

		ngn::log::debug("rn::vki::memory::Pool::free({}, {}, {}) => memory block freed", rn::vki::id(handle.memory), handle.blockIdx, handle.leafIdx);
	}
}

vk::MemoryPropertyFlags Pool::getPreferredPropertyFlags(const Usage usage) const {
	switch (usage) {
		case Usage::GPU:
			return vk::MemoryPropertyFlagBits::eDeviceLocal;
		case Usage::CPU:
			return vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
		case Usage::CPU_TO_GPU:
			return vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eDeviceLocal;
		case Usage::GPU_TO_CPU:
			[[fallthrough]];
		default:
			return vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostCached;
	}
}

vk::MemoryPropertyFlags Pool::getRequiredPropertyFlags(const Usage usage) const {
	switch (usage) {
		case Usage::GPU:
			return vk::MemoryPropertyFlagBits::eDeviceLocal;
		case Usage::CPU:
			return vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
		case Usage::CPU_TO_GPU:
			[[fallthrough]];
		case Usage::GPU_TO_CPU:
			[[fallthrough]];
		default:
			return vk::MemoryPropertyFlagBits::eHostVisible;
	}
}

uint32_t Pool::findMemoryTypeIndex(vk::MemoryPropertyFlags flags, const vk::MemoryRequirements &requirements, uint32_t start) const {
	for (uint32_t i = start; i < memoryProperties.memoryTypeCount; i++) {
		if (((1u << i) & requirements.memoryTypeBits) == 0) {
			continue;
		}

		if ((memoryProperties.memoryTypes[i].propertyFlags & flags) != flags) {
			continue;
		}

		return i;
	}

	return std::numeric_limits<uint32_t>::max();
}

Block & Pool::appendBlock(uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags, const vk::MemoryRequirements &requirements) {
	vk::DeviceSize size = std::max(requirements.size, baseMemorySize);

	vk::DeviceSize leafSize = 1u << (baseLevels - 1);

	// round up to be divisible by (1 << (levels - 1))
	size = ((size + leafSize - 1u) / leafSize) * leafSize;

	if (size > baseMemorySize) {
		ngn::log::warn("rn::vki::memory::Pool::appendBlock({:x}, {}, {} bytes) => created bigger block than expected: {} bytes > {} bytes, leaf {} bytes [{:x}: {}]", memoryTypeIndex, vk::to_string(flags), requirements.size, size, baseMemorySize, leafSize);
	} else {
		ngn::log::debug("rn::vki::memory::Pool::appendBlock({:x}, {}, {} bytes) => created new block: {} bytes, leaf {} bytes", memoryTypeIndex, vk::to_string(flags), requirements.size, size, leafSize);
	}

	vk::UniqueDeviceMemory deviceMemory = RN_VLK_TRACE(device.allocateMemoryUnique({ size, memoryTypeIndex }));

	Mapping mapping{};
	if ((flags & vk::MemoryPropertyFlagBits::eHostVisible)) {
		// if memory is host visible then automatically map whole block
		mapping = Mapping{device, deviceMemory.get(), RN_VLK_TRACE(device.mapMemory(deviceMemory.get(), 0, VK_WHOLE_SIZE))};
	}

	return blocks.emplace_back(std::move(deviceMemory), std::move(mapping), std::move(size), std::move(baseLevels), std::move(memoryTypeIndex), std::move(flags));
}

} // rn::vki::memory
