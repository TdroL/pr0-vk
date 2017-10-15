#include "pool.hpp"

#include <stdexcept>

#include "../../ngn/log.hpp"
#include "mapping.hpp"

namespace rn::vlk::memory {

Pool::Pool(vk::Device device, vk::PhysicalDeviceMemoryProperties memoryProperties, uint32_t baseLevels, vk::DeviceSize baseMemorySize) noexcept :
	device{device},
	memoryProperties{memoryProperties},
	baseLevels{baseLevels},
	baseMemorySize{baseMemorySize}
{}

Pool::Pool(Pool &&other) noexcept :
	device{std::move(other.device)},
	memoryProperties{std::move(other.memoryProperties)},
	baseLevels{std::move(other.baseLevels)},
	baseMemorySize{std::move(other.baseMemorySize)},
	blocks{std::move(other.blocks)}
{
	other.device = vk::Device{};
	other.blocks.clear();
}

Pool & Pool::operator=(Pool &&other) noexcept {
	blocks.clear();

	device = std::move(other.device);
	memoryProperties = std::move(other.memoryProperties);
	baseLevels = std::move(other.baseLevels);
	baseMemorySize = std::move(other.baseMemorySize);
	blocks = std::move(other.blocks);

	other.device = vk::Device{};
	other.blocks.clear();

	return *this;
}

Handle Pool::alloc(const Usage usage, const vk::MemoryRequirements &requirements) {
	if (device == vk::Device{}) {
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
			if (allocation.deviceMemory) {
				return Handle{
					allocation.deviceMemory,
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
			auto &block = appendBlock(memoryTypeIndex, memoryProperties.memoryTypes[memoryTypeIndex].propertyFlags, requirements);

			BlockAllocationHandle allocation = block.alloc(requirements);
			if (allocation.deviceMemory) {
				return Handle{
					allocation.deviceMemory,
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
				ngn::log::warn("Out of host memory for memory type {}", memoryTypeIndex);
				start = memoryTypeIndex + 1;
			} else {
				// re-throw
				throw e;
			}
		} catch (const vk::OutOfDeviceMemoryError &e) {
			if (memoryTypeIndex + 1 < memoryProperties.memoryTypeCount) {
				//  try another memory type
				ngn::log::warn("Out of device memory for memory type {}", memoryTypeIndex);
				start = memoryTypeIndex + 1;
			} else {
				// re-throw
				throw e;
			}
		}
	}

	throw std::runtime_error{"Could not allocate memory"};
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
		ngn::log::warn("Created bigger block than expected: {} > {}", size, baseMemorySize);
	}

	vk::UniqueDeviceMemory deviceMemory = device.allocateMemoryUnique({ size, memoryTypeIndex });

	Mapping mapping{};
	if ((flags & vk::MemoryPropertyFlagBits::eHostVisible)) {
		// if memory is host visible then automatically map whole block
		mapping = Mapping{device, deviceMemory.get(), device.mapMemory(deviceMemory.get(), 0, VK_WHOLE_SIZE)};
	}

	Block block{std::move(deviceMemory), std::move(mapping), size, baseLevels, memoryTypeIndex, flags};
	blocks.push_back(std::move(block));

	return blocks.back();
}

} // rn::vlk::memory
