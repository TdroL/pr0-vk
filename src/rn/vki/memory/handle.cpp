#include "handle.hpp"

#include <stdexcept>

#include "../../ngn/log.hpp"
#include "../id.hpp"
#include "pool.hpp"

namespace rn::vki::memory {

Handle::Handle(vk::DeviceMemory memory, vk::DeviceSize offset, vk::MemoryPropertyFlags flags, void *pointer, Pool *pool, uint32_t blockIdx, uint32_t leafIdx) noexcept :
	memory{memory},
	offset{offset},
	flags{flags},
	pointer{pointer},
	pool{pool},
	blockIdx{blockIdx},
	leafIdx{leafIdx}
{}

Handle::Handle(Handle &&other) noexcept :
	memory{std::move(other.memory)},
	offset{std::move(other.offset)},
	flags{std::move(other.flags)},
	pointer{std::move(other.pointer)},
	pool{std::move(other.pool)},
	blockIdx{std::move(other.blockIdx)},
	leafIdx{std::move(other.leafIdx)}
{
	other.memory = vk::DeviceMemory{};
}

Handle & Handle::operator=(Handle &&other) noexcept {
	release();

	memory = std::move(other.memory);
	offset = std::move(other.offset);
	flags = std::move(other.flags);
	pointer = std::move(other.pointer);
	pool = std::move(other.pool);
	blockIdx = std::move(other.blockIdx);
	leafIdx = std::move(other.leafIdx);

	other.memory = vk::DeviceMemory{};

	return *this;
}

Handle::~Handle() {
	release();
}

bool Handle::needsFlushing() {
	return (flags & vk::MemoryPropertyFlagBits::eHostCoherent) != vk::MemoryPropertyFlagBits::eHostCoherent;
}

void Handle::release() {
	if (memory != vk::DeviceMemory{}) {
		if (pool == nullptr) {
			ngn::log::error("rn::vki::memory::Handle::release() <{:x}> => unable to release memory handle, unknown pool", rn::vki::id(memory));
		} else {
			pool->free(*this);
		}

		memory = vk::DeviceMemory{};
	}
}

} // rn::vki::memory
