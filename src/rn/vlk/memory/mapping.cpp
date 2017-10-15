#include "mapping.hpp"

#include "../../ngn/log.hpp"
#include "../id.hpp"

namespace rn::vlk::memory {

Mapping::Mapping(Mapping &&other) noexcept :
	device{other.device},
	deviceMemory{other.deviceMemory},
	pointer{other.pointer}
{
	other.pointer = nullptr;
}

Mapping & Mapping::operator=(Mapping &&other) noexcept {
	unmap();

	device = other.device;
	deviceMemory = other.deviceMemory;
	pointer = other.pointer;

	other.pointer = nullptr;

	return *this;
}

Mapping::~Mapping() {
	unmap();
}

void Mapping::unmap() {
	if (pointer != nullptr) {
		if (device == vk::Device{}) {
			ngn::log::error("Unable to unmap pointer -- device is missing: {}", pointer);
			return;
		}

		if (deviceMemory == vk::DeviceMemory{}) {
			ngn::log::error("Unable to unmap pointer -- device memory is missing: {}, {}", pointer, rn::vlk::id(deviceMemory));
			return;
		}

		device.unmapMemory(deviceMemory);
		pointer = nullptr;
	}
}

void * Mapping::get(vk::DeviceSize offset) const {
	if (pointer == nullptr) {
		return nullptr;
	}

	return reinterpret_cast<void *>(reinterpret_cast<std::byte *>(pointer) + offset);
}

} // rn::vlk::memory
