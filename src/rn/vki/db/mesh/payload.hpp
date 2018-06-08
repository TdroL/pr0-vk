#pragma once

#include <future>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../memory/handle.hpp"

namespace rn::vki::db::mesh {

struct Payload {
	uint32_t index = 0;
	std::string source{};
	std::promise<bool> promise{};
	vk::UniqueBuffer vertexBuffer{};
	memory::Handle vertexBufferMemory{};
	vk::DeviceSize vertexBufferSize = 0;
	vk::UniqueBuffer indexBuffer{};
	memory::Handle indexBufferMemory{};
	vk::DeviceSize indexBufferSize = 0;

	Payload() = default;

	explicit Payload(uint32_t &&index, std::string &&source, std::promise<bool> &&promise) noexcept :
		index{std::move(index)},
		source{std::move(source)},
		promise{std::move(promise)}
	{}

	explicit Payload(uint32_t &&index, std::string &&source, std::promise<bool> &&promise, vk::UniqueBuffer &&vertexBuffer, memory::Handle &&vertexBufferMemory, vk::DeviceSize &&vertexBufferSize, vk::UniqueBuffer &&indexBuffer, memory::Handle &&indexBufferMemory, vk::DeviceSize &&indexBufferSize) noexcept :
		index{std::move(index)},
		source{std::move(source)},
		promise{std::move(promise)},
		vertexBuffer{std::move(vertexBuffer)},
		vertexBufferMemory{std::move(vertexBufferMemory)},
		vertexBufferSize{std::move(vertexBufferSize)},
		indexBuffer{std::move(indexBuffer)},
		indexBufferMemory{std::move(indexBufferMemory)},
		indexBufferSize{std::move(indexBufferSize)}
	{}

	Payload(Payload &&other) noexcept :
		index{std::move(other.index)},
		source{std::move(other.source)},
		promise{std::move(other.promise)},
		vertexBuffer{std::move(other.vertexBuffer)},
		vertexBufferMemory{std::move(other.vertexBufferMemory)},
		vertexBufferSize{std::move(other.vertexBufferSize)},
		indexBuffer{std::move(other.indexBuffer)},
		indexBufferMemory{std::move(other.indexBufferMemory)},
		indexBufferSize{std::move(other.indexBufferSize)}
	{}

	Payload & operator=(Payload &&other) noexcept {
		index = std::move(other.index);
		source = std::move(other.source);
		promise = std::move(other.promise);
		vertexBuffer = std::move(vertexBuffer);
		vertexBufferMemory = std::move(vertexBufferMemory);
		vertexBufferSize = std::move(vertexBufferSize);
		indexBuffer = std::move(indexBuffer);
		indexBufferMemory = std::move(indexBufferMemory);
		indexBufferSize = std::move(indexBufferSize);

		return *this;
	}

	Payload(const Payload &other) = delete;
	Payload & operator=(const Payload &other) = delete;
};

} // rn::vki::db::mesh
