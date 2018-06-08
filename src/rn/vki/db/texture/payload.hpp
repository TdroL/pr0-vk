#pragma once

#include <future>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../memory/handle.hpp"

namespace rn::vki::db::texture {

struct Payload {
	uint32_t index = 0;
	std::string source{};
	std::promise<bool> promise{};
	vk::ImageCreateFlags flags{};
	vk::Format format{vk::Format::eUndefined};
	vk::ImageType imageType{vk::ImageType::e1D};
	uint32_t layers = 0;
	uint32_t faces = 0;
	uint32_t levels = 0;
	std::vector<vk::Extent3D> extents{};
	std::vector<vk::DeviceSize> sizes{};
	vk::ComponentMapping swizzles{};
	vk::UniqueBuffer buffer{};
	memory::Handle bufferMemory{};

	Payload() = default;

	explicit Payload(uint32_t &&index, std::string &&source, std::promise<bool> &&promise) noexcept :
		index{std::move(index)},
		source{std::move(source)},
		promise{std::move(promise)}
	{}

	explicit Payload(uint32_t &&index, std::string &&source, std::promise<bool> &&promise, vk::ImageCreateFlags &&flags, vk::Format &&format, vk::ImageType &&imageType, uint32_t &&layers, uint32_t &&faces, uint32_t &&levels, std::vector<vk::Extent3D> &&extents, std::vector<vk::DeviceSize> &&sizes, vk::ComponentMapping &&swizzles, vk::UniqueBuffer &&buffer, memory::Handle &&bufferMemory) noexcept :
		index{std::move(index)},
		source{std::move(source)},
		promise{std::move(promise)},
		flags{std::move(flags)},
		format{std::move(format)},
		imageType{std::move(imageType)},
		layers{std::move(layers)},
		faces{std::move(faces)},
		levels{std::move(levels)},
		extents{std::move(extents)},
		sizes{std::move(sizes)},
		swizzles{std::move(swizzles)},
		buffer{std::move(buffer)},
		bufferMemory{std::move(bufferMemory)}
	{}

	Payload(Payload &&other) noexcept :
		index{std::move(other.index)},
		source{std::move(other.source)},
		promise{std::move(other.promise)},
		flags{std::move(other.flags)},
		format{std::move(other.format)},
		imageType{std::move(other.imageType)},
		layers{std::move(other.layers)},
		faces{std::move(other.faces)},
		levels{std::move(other.levels)},
		extents{std::move(other.extents)},
		sizes{std::move(other.sizes)},
		swizzles{std::move(other.swizzles)},
		buffer{std::move(other.buffer)},
		bufferMemory{std::move(other.bufferMemory)}
	{}

	Payload & operator=(Payload &&other) noexcept {
		index = std::move(other.index);
		source = std::move(other.source);
		promise = std::move(other.promise);
		flags = std::move(other.flags);
		format = std::move(other.format);
		imageType = std::move(other.imageType);
		layers = std::move(other.layers);
		faces = std::move(other.faces);
		levels = std::move(other.levels);
		extents = std::move(other.extents);
		sizes = std::move(other.sizes);
		swizzles = std::move(other.swizzles);
		buffer = std::move(other.buffer);
		bufferMemory = std::move(other.bufferMemory);

		return *this;
	}

	Payload(const Payload &other) = delete;
	Payload & operator=(const Payload &other) = delete;
};

} // rn::vki::db::texture
