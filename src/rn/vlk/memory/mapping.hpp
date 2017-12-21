#pragma once

#include <cstddef>

#include <vulkan/vulkan.hpp>

namespace rn::vlk::memory {

class Mapping {
public:
	vk::Device device{};
	vk::DeviceMemory memory{};
	void *pointer = nullptr;

	Mapping() = default;

	explicit Mapping(vk::Device device, vk::DeviceMemory memory, void *pointer = nullptr) noexcept :
		device{device},
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

} // rn::vlk::memory
