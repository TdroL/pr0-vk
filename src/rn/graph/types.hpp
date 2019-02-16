#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <limits>

#include "../types.hpp"

namespace rn::graph {

struct BufferResourceHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::graph::BufferResourceHandle) == sizeof(uint32_t));

bool operator==(BufferResourceHandle a, BufferResourceHandle b);
bool operator!=(BufferResourceHandle a, BufferResourceHandle b);

struct TextureResourceHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::graph::TextureResourceHandle) == sizeof(uint32_t));

bool operator==(TextureResourceHandle a, TextureResourceHandle b);
bool operator!=(TextureResourceHandle a, TextureResourceHandle b);

constexpr TextureResourceHandle swapchainResourceHandle() {
	return { std::numeric_limits<TextureResourceHandle::InternalType>::max() - 1 };
}

struct DescriptorSetResourceHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::graph::DescriptorSetResourceHandle) == sizeof(uint32_t));

bool operator==(DescriptorSetResourceHandle a, DescriptorSetResourceHandle b);
bool operator!=(DescriptorSetResourceHandle a, DescriptorSetResourceHandle b);

struct SubpassResourceHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::graph::SubpassResourceHandle) == sizeof(uint32_t));

bool operator==(SubpassResourceHandle a, SubpassResourceHandle b);
bool operator!=(SubpassResourceHandle a, SubpassResourceHandle b);

struct PipelineResourceHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::graph::PipelineResourceHandle) == sizeof(uint32_t));

bool operator==(PipelineResourceHandle a, PipelineResourceHandle b);
bool operator!=(PipelineResourceHandle a, PipelineResourceHandle b);

constexpr size_t numberOfComponents(rn::DataFormat format) {
	switch (format) {
		default:
		case rn::DataFormat::Undefined: return 0;
		case rn::DataFormat::R8SInt: return 1;
		case rn::DataFormat::R8SNorm: return 1;
		case rn::DataFormat::R8UInt: return 1;
		case rn::DataFormat::R8UNorm: return 1;
		case rn::DataFormat::R8G8SInt: return 2;
		case rn::DataFormat::R8G8SNorm: return 2;
		case rn::DataFormat::R8G8UInt: return 2;
		case rn::DataFormat::R8G8UNorm: return 2;
		case rn::DataFormat::R8G8B8A8SInt: return 4;
		case rn::DataFormat::R8G8B8A8SNorm: return 4;
		case rn::DataFormat::R8G8B8A8UInt: return 4;
		case rn::DataFormat::R8G8B8A8UNorm: return 4;
		case rn::DataFormat::R16Float: return 1;
		case rn::DataFormat::R16SInt: return 1;
		case rn::DataFormat::R16SNorm: return 1;
		case rn::DataFormat::R16UInt: return 1;
		case rn::DataFormat::R16UNorm: return 1;
		case rn::DataFormat::R16G16Float: return 2;
		case rn::DataFormat::R16G16SInt: return 2;
		case rn::DataFormat::R16G16SNorm: return 2;
		case rn::DataFormat::R16G16UInt: return 2;
		case rn::DataFormat::R16G16UNorm: return 2;
		case rn::DataFormat::R16G16B16A16Float: return 4;
		case rn::DataFormat::R16G16B16A16SInt: return 4;
		case rn::DataFormat::R16G16B16A16SNorm: return 4;
		case rn::DataFormat::R16G16B16A16UInt: return 4;
		case rn::DataFormat::R16G16B16A16UNorm: return 4;
		case rn::DataFormat::R32Float: return 1;
		case rn::DataFormat::R32SInt: return 1;
		case rn::DataFormat::R32UInt: return 1;
		case rn::DataFormat::R32G32Float: return 2;
		case rn::DataFormat::R32G32SInt: return 2;
		case rn::DataFormat::R32G32UInt: return 2;
		case rn::DataFormat::R32G32B32Float: return 3;
		case rn::DataFormat::R32G32B32SInt: return 3;
		case rn::DataFormat::R32G32B32UInt: return 3;
		case rn::DataFormat::R32G32B32A32Float: return 4;
		case rn::DataFormat::R32G32B32A32SInt: return 4;
		case rn::DataFormat::R32G32B32A32UInt: return 4;
	}
}

enum class Access {
	None = 0x00000000,
	Read = 0x00000001,
	Write = 0x00000002,
	Modify = 0x00000004,
};
static_assert(sizeof(rn::graph::Access) == sizeof(uint32_t));

constexpr rn::graph::Access operator|(rn::graph::Access left, rn::graph::Access right);
constexpr rn::graph::Access operator&(rn::graph::Access left, rn::graph::Access right);

struct TextureCreate {
	rn::PixelFormat format;
	std::optional<rn::Extent3D> dimensions{};
	rn::ImageType type{rn::ImageType::Tex2D};
	uint32_t levels{1};
	uint32_t layers{1};
	rn::TextureUsage usage{rn::TextureUsage::None};
};

struct TextureModify {
	std::string source;
};

struct BufferCreate {
	size_t size;
	rn::BufferUsage usage{rn::BufferUsage::None};
	rn::BufferPaging paging{rn::BufferPaging::SwapchainSize};
};

struct BufferModify {
	std::string source;
};

} // rn::graph

namespace rn {

template<>
constexpr rn::graph::BufferResourceHandle end() {
	return { std::numeric_limits<rn::graph::BufferResourceHandle::InternalType>::max() };
}

template<>
constexpr rn::graph::TextureResourceHandle end() {
	return { std::numeric_limits<rn::graph::TextureResourceHandle::InternalType>::max() };
}

template<>
constexpr rn::graph::DescriptorSetResourceHandle end() {
	return { std::numeric_limits<rn::graph::DescriptorSetResourceHandle::InternalType>::max() };
}

template<>
constexpr rn::graph::SubpassResourceHandle end() {
	return { std::numeric_limits<rn::graph::SubpassResourceHandle::InternalType>::max() };
}

template<>
constexpr rn::graph::PipelineResourceHandle end() {
	return { std::numeric_limits<rn::graph::PipelineResourceHandle::InternalType>::max() };
}

} // rn
