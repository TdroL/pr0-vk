#include "handle.hpp"

#include <stdexcept>

#include "../../ngn/log.hpp"
#include "../id.hpp"
#include "pool.hpp"

namespace rn::vlk::memory {

Handle::Handle(vk::DeviceMemory deviceMemory, vk::DeviceSize offset, vk::MemoryPropertyFlags flags, void *pointer, Pool *pool, uint32_t blockIdx, uint32_t leafIdx) noexcept :
	deviceMemory{deviceMemory},
	offset{offset},
	flags{flags},
	pointer{pointer},
	pool{pool},
	blockIdx{blockIdx},
	leafIdx{leafIdx}
{}

Handle::Handle(Handle &&other) noexcept :
	deviceMemory{std::move(other.deviceMemory)},
	offset{std::move(other.offset)},
	flags{std::move(other.flags)},
	pointer{std::move(other.pointer)},
	pool{std::move(other.pool)},
	blockIdx{std::move(other.blockIdx)},
	leafIdx{std::move(other.leafIdx)}
{
	other.deviceMemory = vk::DeviceMemory{};
}

Handle & Handle::operator=(Handle &&other) noexcept {
	destroy();

	deviceMemory = std::move(other.deviceMemory);
	offset = std::move(other.offset);
	flags = std::move(other.flags);
	pointer = std::move(other.pointer);
	pool = std::move(other.pool);
	blockIdx = std::move(other.blockIdx);
	leafIdx = std::move(other.leafIdx);

	other.deviceMemory = vk::DeviceMemory{};

	return *this;
}

Handle::~Handle() {
	destroy();
}

void Handle::destroy() {
	if (deviceMemory != vk::DeviceMemory{}) {
		if (pool == nullptr) {
			ngn::log::error("Unable to destroy memory handle -- unknown pool: {:x}", rn::vlk::id(deviceMemory));
		} else {
			if (blockIdx >= pool->blocks.size()) {
				ngn::log::error("Unable to destroy memory handle -- block index out-of-range {} >= {}: {:x}", blockIdx, pool->blocks.size(), rn::vlk::id(deviceMemory));
			} else {
				pool->blocks[blockIdx].free(leafIdx);
			}
		}

		deviceMemory = vk::DeviceMemory{};
	}
}

} // rn::vlk::memory
