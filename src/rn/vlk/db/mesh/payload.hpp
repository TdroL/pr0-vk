#pragma once

#include <future>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../memory/handle.hpp"

namespace rn::vlk::db::mesh {

struct Payload {
	uint32_t index = 0;
	std::string source{};
	std::promise<bool> promise{};
	vk::UniqueBuffer buffer{};
	memory::Handle bufferMemory{};

	Payload() = default;

	explicit Payload(uint32_t &&index, std::string &&source, std::promise<bool> &&promise) noexcept :
		index{std::move(index)},
		source{std::move(source)},
		promise{std::move(promise)}
	{}

	explicit Payload(uint32_t &&index, std::string &&source, std::promise<bool> &&promise, vk::UniqueBuffer &&buffer, memory::Handle &&bufferMemory) noexcept :
		index{std::move(index)},
		source{std::move(source)},
		promise{std::move(promise)},
		buffer{std::move(buffer)},
		bufferMemory{std::move(bufferMemory)}
	{}

	Payload(Payload &&other) noexcept :
		index{std::move(other.index)},
		source{std::move(other.source)},
		promise{std::move(other.promise)},
		buffer{std::move(other.buffer)},
		bufferMemory{std::move(other.bufferMemory)}
	{}

	Payload & operator=(Payload &&other) {
		index = std::move(other.index);
		source = std::move(other.source);
		promise = std::move(other.promise);
		buffer = std::move(other.buffer);
		bufferMemory = std::move(other.bufferMemory);

		return *this;
	}

	Payload(const Payload &other) = delete;
	Payload & operator=(const Payload &other) = delete;
};

} // rn::vlk::db::mesh
