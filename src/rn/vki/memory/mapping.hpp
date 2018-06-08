#pragma once

#include <cstddef>

#include "../device.hpp"

namespace rn::vki::memory {

class Mapping {
public:
	Device &device;
	vk::DeviceMemory memory{};
	void *pointer = nullptr;

	Mapping() noexcept;

	explicit Mapping(Device &device, vk::DeviceMemory memory, void *pointer = nullptr) noexcept :
		device(device),
		memory{memory},
		pointer{pointer}
	{}

	Mapping(Mapping &&other) noexcept;
	Mapping & operator=(Mapping &&other) noexcept;

	Mapping(const Mapping &) = delete;
	Mapping & operator=(const Mapping &) = delete;

	~Mapping();

	void unmap();

	void * get(vk::DeviceSize offset) const;
};

} // rn::vki::memory
