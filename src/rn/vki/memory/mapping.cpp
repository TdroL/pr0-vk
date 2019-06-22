#include "mapping.hpp"

#include "../../../ngn/log.hpp"
#include "../id.hpp"
#include "../trace.hpp"

namespace rn::vki::memory {

Mapping::Mapping(Mapping &&other) noexcept :
	device{std::move(other.device)},
	memory{std::move(other.memory)},
	pointer{std::move(other.pointer)}
{
	other.pointer = nullptr;
}

Mapping & Mapping::operator=(Mapping &&other) noexcept {
	reset();

	device = std::move(other.device);
	memory = std::move(other.memory);
	pointer = std::move(other.pointer);

	other.pointer = nullptr;

	return *this;
}

Mapping::~Mapping() {
	reset();
}

void Mapping::reset() {
	if (pointer != nullptr) {
		if ( ! device) {
			ngn::log::error("rn::vki::memory::Mapping::reset() <{}> => unable to unmap pointer, device is missing", pointer);
			return;
		}

		if ( ! memory) {
			ngn::log::error("rn::vki::memory::Mapping::reset() <{}, {:#x}> => unable to unmap pointer, device memory is missing", pointer, rn::vki::id(memory.get()));
			return;
		}

		RN_VKI_TRACE(device->unmapMemory(memory.get(), device.table()));
		pointer = nullptr;
	}
}

void * Mapping::get(vk::DeviceSize offset) const {
	if (pointer == nullptr) {
		return nullptr;
	}

	return reinterpret_cast<void *>(reinterpret_cast<std::byte *>(pointer) + offset);
}

} // rn::vki::memory
