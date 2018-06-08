#include "mapping.hpp"

#include "../../ngn/log.hpp"
#include "../id.hpp"
#include "../trace.hpp"

namespace rn::vki::memory {

namespace {

Device dummyDevice{};

}

Mapping::Mapping() noexcept :
	device(dummyDevice)
{}

Mapping::Mapping(Mapping &&other) noexcept :
	device(other.device),
	memory{other.memory},
	pointer{other.pointer}
{
	other.pointer = nullptr;
}

Mapping & Mapping::operator=(Mapping &&other) noexcept {
	unmap();

	device = other.device;
	memory = other.memory;
	pointer = other.pointer;

	other.pointer = nullptr;

	return *this;
}

Mapping::~Mapping() {
	unmap();
}

void Mapping::unmap() {
	if (pointer != nullptr) {
		if ( ! device) {
			ngn::log::error("rn::vki::memory::Mapping::unmap() <{}> => unable to unmap pointer, device is missing", pointer);
			return;
		}

		if (memory == vk::DeviceMemory{}) {
			ngn::log::error("rn::vki::memory::Mapping::unmap() <{}, {}> => unable to unmap pointer, device memory is missing", pointer, rn::vki::id(memory));
			return;
		}

		RN_VLK_TRACE(device.unmapMemory(memory));
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
