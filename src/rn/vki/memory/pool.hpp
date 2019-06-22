#pragma once

#include <vector>
#include <variant>

#include <vulkan/vulkan.hpp>

#include "../dispatch.hpp"
#include "block.hpp"
#include "handle.hpp"

namespace rn::vki::memory {

enum class Usage {
	GPU,
	CPU,
	CPU_TO_GPU,
	GPU_TO_CPU
};

class Pool {
public:
	rn::vki::HandleDevice device{};
	vk::PhysicalDeviceMemoryProperties2 memoryProperties{};
	uint32_t baseLevels{};
	vk::DeviceSize baseMemorySize{};

	std::vector<Block> blocks{};

	Pool() = default;

	Pool(rn::vki::HandleDevice &&device, vk::PhysicalDeviceMemoryProperties2 memoryProperties, uint32_t baseLevels, vk::DeviceSize baseMemorySize) noexcept;

	Pool(Pool &&other) noexcept;
	Pool & operator=(Pool &&other) noexcept;

	Pool(const Pool &) = delete;
	Pool & operator=(const Pool &) = delete;

	void reset();

	Handle alloc(const Usage usage, const vk::Image &image);
	Handle alloc(const Usage usage, const vk::Buffer &buffer);
	Handle alloc(const Usage usage, const vk::MemoryRequirements2 &requirements, const std::variant<std::monostate, vk::Image, vk::Buffer> &dedicatedFor = std::monostate{});
	void free(const Handle &handle);

	vk::MemoryPropertyFlags getPreferredPropertyFlags(const Usage usage) const;
	vk::MemoryPropertyFlags getRequiredPropertyFlags(const Usage usage) const;

	uint32_t findMemoryTypeIndex(vk::MemoryPropertyFlags flags, const vk::MemoryRequirements2 &requirements, uint32_t start = 0) const;
	Block & createBlock(uint32_t memoryTypeIndex, vk::MemoryPropertyFlags flags, const vk::MemoryRequirements2 &requirements, const std::variant<std::monostate, vk::Image, vk::Buffer> &dedicatedFor);
};

} // rn::vki::memory
