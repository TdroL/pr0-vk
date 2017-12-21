#pragma once

#include <cstddef>
#include <limits>

#include <vulkan/vulkan.hpp>

namespace rn::vlk::memory {

class Pool;

class Handle {
public:
	vk::DeviceMemory memory{};
	vk::DeviceSize offset{std::numeric_limits<vk::DeviceSize>::max()};
	vk::MemoryPropertyFlags flags{};
	void *pointer = nullptr;

	// deallocation info
	Pool *pool = nullptr;
	uint32_t blockIdx = std::numeric_limits<uint32_t>::max();
	uint32_t leafIdx = std::numeric_limits<uint32_t>::max();

	Handle() = default;

	explicit Handle(vk::DeviceMemory memory, vk::DeviceSize offset, vk::MemoryPropertyFlags flags, void *pointer, Pool *pool, uint32_t blockIdx, uint32_t leafIdx) noexcept;

	Handle(Handle &&other) noexcept;
	Handle & operator=(Handle &&other) noexcept;

	Handle(const Handle &) = delete;
	Handle & operator=(const Handle &) = delete;

	~Handle();

	bool needsFlushing();

	void release();
};

} // rn::vlk::memory

