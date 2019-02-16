#pragma once

#include <cstddef>

#include <vulkan/vulkan.hpp>

#include "../dispatch.hpp"

namespace rn::vki::memory {

class Mapping {
public:
	rn::vki::HandleDevice device{};
	rn::vki::HandleDeviceMemory memory{};
	void *pointer = nullptr;

	Mapping() = default;

	Mapping(rn::vki::HandleDevice &&device, rn::vki::HandleDeviceMemory &&memory, void *pointer = nullptr) noexcept :
		device{std::move(device)},
		memory{std::move(memory)},
		pointer{pointer}
	{}

	Mapping(Mapping &&other) noexcept;
	Mapping & operator=(Mapping &&other) noexcept;

	Mapping(const Mapping &) = delete;
	Mapping & operator=(const Mapping &) = delete;

	~Mapping();

	void reset();

	void * get(vk::DeviceSize offset) const;
};

} // rn::vki::memory
