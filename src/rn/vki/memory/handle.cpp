#include "handle.hpp"

#include <stdexcept>

#include "../../../ngn/log.hpp"
#include "../id.hpp"
#include "pool.hpp"

namespace rn::vki::memory {

Handle::Handle(rn::vki::HandleDeviceMemory &&memory, vk::DeviceSize offset, vk::MemoryPropertyFlags flags, void *pointer, Pool &pool, uint32_t blockIdx, uint32_t leafIdx) noexcept :
	memory{std::move(memory)},
	offset{offset},
	flags{flags},
	pointer{pointer},
	pool{&pool},
	blockIdx{blockIdx},
	leafIdx{leafIdx}
{}

Handle::Handle(Handle &&other) noexcept :
	memory{std::move(other.memory)},
	offset{std::move(other.offset)},
	flags{std::move(other.flags)},
	pointer{other.pointer},
	pool{std::move(other.pool)},
	blockIdx{other.blockIdx},
	leafIdx{other.leafIdx}
{
	other.memory = rn::vki::HandleDeviceMemory{};
}

Handle & Handle::operator=(Handle &&other) noexcept {
	release();

	memory = std::move(other.memory);
	offset = std::move(other.offset);
	flags = std::move(other.flags);
	pointer = other.pointer;
	pool = std::move(other.pool);
	blockIdx = other.blockIdx;
	leafIdx = other.leafIdx;

	other.memory = rn::vki::HandleDeviceMemory{};

	return *this;
}

Handle::~Handle() {
	release();
}

bool Handle::needsFlushing() {
	return (flags & vk::MemoryPropertyFlagBits::eHostCoherent) != vk::MemoryPropertyFlagBits::eHostCoherent;
}

void Handle::release() {
	if (memory) {
		if (pool == nullptr) {
			ngn::log::error("rn::vki::memory::Handle::release() <{:#x}> => unable to release memory handle, unknown pool", rn::vki::id(memory.get()));
		} else {
			pool->free(*this);
		}

		memory = rn::vki::HandleDeviceMemory{};
	}
}

} // rn::vki::memory
