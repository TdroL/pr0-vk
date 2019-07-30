#include "pool.hpp"

#include <stdexcept>
#include <fmt/format.h>

#include "../../../ngn/log.hpp"
#include "../id.hpp"
#include "../trace.hpp"
#include "mapping.hpp"

namespace rn::vki::memory {

Pool::Pool(rn::vki::HandleDevice &&device, vk::PhysicalDeviceMemoryProperties2 memoryProperties, uint32_t baseLevels, vk::DeviceSize baseMemorySize, std::string &&name) noexcept :
	device{std::move(device)},
	memoryProperties{memoryProperties},
	baseLevels{baseLevels},
	baseMemorySize{baseMemorySize},
	name{std::move(name)}
{}

Pool::Pool(Pool &&other) noexcept :
	device{std::move(other.device)},
	memoryProperties{std::move(other.memoryProperties)},
	baseLevels{other.baseLevels},
	baseMemorySize{std::move(other.baseMemorySize)},
	name{std::move(other.name)},
	blocks{std::move(other.blocks)}
{
	other.reset();
}

Pool & Pool::operator=(Pool &&other) noexcept {
	reset();

	device = std::move(other.device);
	memoryProperties = std::move(other.memoryProperties);
	baseLevels = other.baseLevels;
	baseMemorySize = std::move(other.baseMemorySize);
	name = std::move(other.name);
	blocks = std::move(other.blocks);

	other.reset();

	return *this;
}

Pool::~Pool() {
	reset();
}

Handle Pool::alloc(const Usage usage, const vk::Image &image) {
	if ( ! device) {
		throw std::runtime_error{"Pool not initialized"};
	}

	vk::ImageMemoryRequirementsInfo2 memoryRequirementsInfo{
		/*.image=*/ image,
	};

	auto structureChain = RN_VKI_TRACE((device->getImageMemoryRequirements2<vk::MemoryRequirements2, vk::MemoryDedicatedRequirements>(memoryRequirementsInfo, device.table())));

	auto memoryRequirements = structureChain.get<vk::MemoryRequirements2>();
	auto memoryDedicatedRequirements = structureChain.get<vk::MemoryDedicatedRequirements>();

	bool dedicated = memoryDedicatedRequirements.requiresDedicatedAllocation || memoryDedicatedRequirements.prefersDedicatedAllocation;

	if (dedicated) {
		return alloc(usage, memoryRequirements, image);
	} else {
		return alloc(usage, memoryRequirements);
	}
}

Handle Pool::alloc(const Usage usage, const vk::Buffer &buffer) {
	if ( ! device) {
		throw std::runtime_error{"Pool not initialized"};
	}

	vk::BufferMemoryRequirementsInfo2 memoryRequirementsInfo{
		/*.buffer=*/ buffer,
	};

	auto structureChain = RN_VKI_TRACE((device->getBufferMemoryRequirements2<vk::MemoryRequirements2, vk::MemoryDedicatedRequirements>(memoryRequirementsInfo, device.table())));

	auto memoryRequirements = structureChain.get<vk::MemoryRequirements2>();
	auto memoryDedicatedRequirements = structureChain.get<vk::MemoryDedicatedRequirements>();

	bool dedicated = memoryDedicatedRequirements.requiresDedicatedAllocation || memoryDedicatedRequirements.prefersDedicatedAllocation;

	if (dedicated) {
		return alloc(usage, memoryRequirements, buffer);
	} else {
		return alloc(usage, memoryRequirements);
	}
}

Handle Pool::alloc(const Usage usage, const vk::MemoryRequirements2 &requirements, const std::variant<std::monostate, vk::Image, vk::Buffer> &dedicatedFor) {
	if ( ! device) {
		throw std::runtime_error{"Pool not initialized"};
	}

	uint32_t start = 0;
	while (start < memoryProperties.memoryProperties.memoryTypeCount) {
		vk::MemoryPropertyFlags preferred = getPreferredPropertyFlags(usage);
		vk::MemoryPropertyFlags required = getRequiredPropertyFlags(usage);

		uint32_t memoryTypeIndex = findMemoryTypeIndex(preferred, requirements, start);
		if (memoryTypeIndex == std::numeric_limits<uint32_t>::max()) {
			memoryTypeIndex = findMemoryTypeIndex(required, requirements, start);
		}

		if (memoryTypeIndex == std::numeric_limits<uint32_t>::max()) {
			static_assert(sizeof(preferred) <= sizeof(uint32_t));
			static_assert(sizeof(required) <= sizeof(uint32_t));
			throw std::runtime_error{fmt::format("Could not find a valid memory type with {}/{} ({:#b}/{:#b}) flags for 0b{:b} types", vk::to_string(preferred), static_cast<uint32_t>(preferred), vk::to_string(required), static_cast<uint32_t>(required), requirements.memoryRequirements.memoryTypeBits)};
		}

		if (std::holds_alternative<std::monostate>(dedicatedFor)) {
			for (uint32_t blockIdx = 0; blockIdx < blocks.size(); blockIdx++) {
				auto &block = blocks[blockIdx];

				if ( ! block) {
					continue;
				}

				if (block.memoryTypeIndex != memoryTypeIndex) {
					continue;
				}

				BlockAllocationHandle allocation = block.alloc(requirements);
				if (allocation.memory) {
					// ngn::log::debug("rn::vki::memory::Pool::alloc({} bytes) => allocated memory: {} bytes offset, {} leaf(s), block {}", requirements.memoryRequirements.size, allocation.offset, 1u << (block.levels - block.levelFromIdx(allocation.leafIdx)), blockIdx);
					return Handle{
						std::move(allocation.memory),
						allocation.offset,
						allocation.flags,
						allocation.pointer,
						*this,
						blockIdx,
						allocation.leafIdx
					};
				}
			}
		}

		try {
			Block &block = createBlock(memoryTypeIndex, memoryProperties.memoryProperties.memoryTypes[memoryTypeIndex].propertyFlags, requirements, dedicatedFor);

			BlockAllocationHandle allocation = block.alloc(requirements);
			if (allocation.memory) {
				// ngn::log::debug("rn::vki::memory::Pool::alloc({} bytes) => allocated memory: {} bytes offset, {} leaf(s)", requirements.memoryRequirements.size, allocation.offset, 1u << (block.levels - block.levelFromIdx(allocation.leafIdx)));
				return Handle{
					std::move(allocation.memory),
					allocation.offset,
					allocation.flags,
					allocation.pointer,
					*this,
					static_cast<uint32_t>(blocks.size() - 1),
					allocation.leafIdx
				};
			}
		} catch (const vk::OutOfHostMemoryError &e) {
			if (memoryTypeIndex + 1 < memoryProperties.memoryProperties.memoryTypeCount) {
				//  try another memory type
				ngn::log::warn("rn::vki::memory::Pool::alloc({} bytes) => out of host memory for memory type {:#x}", requirements.memoryRequirements.size, memoryTypeIndex);

				start = memoryTypeIndex + 1;
			} else {
				// re-throw
				throw;
			}
		} catch (const vk::OutOfDeviceMemoryError &e) {
			if (memoryTypeIndex + 1 < memoryProperties.memoryProperties.memoryTypeCount) {
				//  try another memory type
				ngn::log::warn("rn::vki::memory::Pool::alloc({} bytes) => out of device memory for memory type {:#x}", requirements.memoryRequirements.size, memoryTypeIndex);

				start = memoryTypeIndex + 1;
			} else {
				// re-throw
				throw;
			}
		}
	}

	throw std::runtime_error{"Could not allocate memory"};
}

void Pool::reset() {
	if ( ! blocks.empty()) {
		// ngn::log::debug("rn::vki::memory::Pool::reset() => reset pool \"{}\"", name);
		blocks.clear();
	}

	name = {};
}

void Pool::free(const Handle &handle) {
	if ( ! handle.memory || handle.blockIdx == std::numeric_limits<uint32_t>::max() || handle.leafIdx == std::numeric_limits<uint32_t>::max()) {
		return;
	}

	if (handle.blockIdx >= blocks.size()) {
		ngn::log::error("rn::vki::memory::Pool::free({:#x}, {}, {}) => unable to free memory handle, block index out-of-range: {} => {}", rn::vki::id(handle.memory.get()), handle.blockIdx, handle.leafIdx, handle.blockIdx, blocks.size());
	} else {
		blocks[handle.blockIdx].free(handle.leafIdx);

		if (blocks[handle.blockIdx].isEmpty()) {
			// ngn::log::debug("rn::vki::memory::Pool::free({:#x} \"{}\") => reset block {}", rn::vki::id(handle.memory.get()), name, handle.blockIdx);
			blocks[handle.blockIdx].reset();
			// ngn::log::debug("rn::vki::memory::Pool::free({:#x}, {}, {}) => memory block reset", rn::vki::id(handle.memory.get()), handle.blockIdx, handle.leafIdx);
		} else {
			// ngn::log::debug("rn::vki::memory::Pool::free({:#x}, {}, {}) => memory block freed", rn::vki::id(handle.memory.get()), handle.blockIdx, handle.leafIdx);
		}
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

uint32_t Pool::findMemoryTypeIndex(vk::MemoryPropertyFlags flags, const vk::MemoryRequirements2 &requirements, uint32_t start) const {
	for (uint32_t i = start; i < memoryProperties.memoryProperties.memoryTypeCount; i++) {
		if (((1u << i) & requirements.memoryRequirements.memoryTypeBits) == 0) {
			continue;
		}

		if ((memoryProperties.memoryProperties.memoryTypes[i].propertyFlags & flags) != flags) {
			continue;
		}

		return i;
	}

	return std::numeric_limits<uint32_t>::max();
}

Block & Pool::createBlock(uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags, const vk::MemoryRequirements2 &requirements, const std::variant<std::monostate, vk::Image, vk::Buffer> &dedicatedFor) {
	vk::DeviceSize size = 0;
	vk::DeviceSize leafSize = 0;
	rn::vki::UniqueDeviceMemory deviceMemory{};

	if (std::holds_alternative<std::monostate>(dedicatedFor)) {
		size = std::max(requirements.memoryRequirements.size, baseMemorySize);

		leafSize = 1u << (baseLevels - 1);

		// round up to be divisible by (1 << (levels - 1))
		size = ((size + leafSize - 1u) / leafSize) * leafSize;

		if (size > baseMemorySize) {
			ngn::log::warn("rn::vki::memory::Pool::createBlock({:#x}, {}, {} bytes) => created bigger block than expected: {} bytes > {} bytes, leaf {} bytes [{:#x}: {}]", memoryTypeIndex, vk::to_string(flags), requirements.memoryRequirements.size, size, baseMemorySize, leafSize);
		} else {
			// ngn::log::debug("rn::vki::memory::Pool::createBlock({:#x}, {}, {} bytes) => created new block: {} bytes, leaf {} bytes", memoryTypeIndex, vk::to_string(flags), requirements.memoryRequirements.size, size, leafSize);
		}

		deviceMemory = rn::vki::UniqueDeviceMemory{
			RN_VKI_TRACE(device->allocateMemoryUnique({ size, memoryTypeIndex }, nullptr, device.table())),
			device.table()
		};
	} else {
		size = requirements.memoryRequirements.size;
		leafSize = requirements.memoryRequirements.size;

		// ngn::log::debug("rn::vki::memory::Pool::createBlock({:#x}, {}, {} bytes) => created new dedicated block: {} bytes", memoryTypeIndex, vk::to_string(flags), requirements.memoryRequirements.size, size);

		auto imageValue = std::get_if<vk::Image>(&dedicatedFor);
		auto bufferValue = std::get_if<vk::Buffer>(&dedicatedFor);

		vk::StructureChain<vk::MemoryAllocateInfo, vk::MemoryDedicatedAllocateInfo> structureChain{
			{
				/*.allocationSize=*/ size,
				/*.memoryTypeIndex=*/ memoryTypeIndex,
			},
			{
				/*.image=*/ imageValue ? *imageValue : vk::Image{},
				/*.buffer=*/ bufferValue ? *bufferValue : vk::Buffer{},
			}
		};

		deviceMemory = rn::vki::UniqueDeviceMemory{
			RN_VKI_TRACE(device->allocateMemoryUnique(structureChain.get<vk::MemoryAllocateInfo>(), nullptr, device.table())),
			device.table()
		};
	}

	Mapping mapping{};
	if ((flags & vk::MemoryPropertyFlagBits::eHostVisible)) {
		// if memory is host visible then automatically map whole block
		mapping = Mapping{
			device.handle(),
			deviceMemory.handle(),
			RN_VKI_TRACE(device->mapMemory(deviceMemory.get(), 0, VK_WHOLE_SIZE, vk::MemoryMapFlags{}, device.table()))
		};
	}

	for (size_t i = 0; i < blocks.size(); i++) {
		auto &block = blocks[i];

		if ( ! block) {
			block = Block{
				std::move(deviceMemory),
				std::move(mapping),
				std::move(size),
				std::move(baseLevels),
				std::move(memoryTypeIndex),
				std::move(flags)
			};

			return block;
		}
	}

	return blocks.emplace_back(std::move(deviceMemory), std::move(mapping), std::move(size), std::move(baseLevels), std::move(memoryTypeIndex), std::move(flags));
}

} // rn::vki::memory
