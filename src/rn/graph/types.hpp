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

template<typename T>
struct BufferResourceHandleWithUsage {
	rn::graph::BufferResourceHandle buffer{};
	T usage{T::None};
};

template<typename T>
bool operator==(const BufferResourceHandleWithUsage<T> &a, const BufferResourceHandleWithUsage<T> &b) {
	return a.buffer == b.buffer && a.usage == b.usage;
}

template<typename T>
bool operator!=(const BufferResourceHandleWithUsage<T> &a, const BufferResourceHandleWithUsage<T> &b) {
	return a.buffer != b.buffer || a.usage != b.usage;
}

template<typename T, typename U>
struct BufferResourceHandleWithUsageWithStages {
	rn::graph::BufferResourceHandle buffer{};
	T usage{T::None};
	U stages{U::None};
};

template<typename T, typename U>
bool operator==(const BufferResourceHandleWithUsageWithStages<T, U> &a, const BufferResourceHandleWithUsageWithStages<T, U> &b) {
	return a.buffer == b.buffer && a.usage == b.usage && a.stages == b.stages;
}

template<typename T, typename U>
bool operator!=(const BufferResourceHandleWithUsageWithStages<T, U> &a, const BufferResourceHandleWithUsageWithStages<T, U> &b) {
	return a.buffer != b.buffer || a.usage != b.usage || a.stages != b.stages;
}

template<typename T>
struct TextureResourceHandleWithUsage {
	rn::graph::TextureResourceHandle texture{};
	T usage{T::None};
};

template<typename T>
bool operator==(const TextureResourceHandleWithUsage<T> &a, const TextureResourceHandleWithUsage<T> &b) {
	return a.texture == b.texture && a.usage == b.usage;
}

template<typename T>
bool operator!=(const TextureResourceHandleWithUsage<T> &a, const TextureResourceHandleWithUsage<T> &b) {
	return a.texture != b.texture || a.usage != b.usage;
}

template<typename T, typename U>
struct TextureResourceHandleWithUsageWithStages {
	rn::graph::TextureResourceHandle texture{};
	T usage{T::None};
	U stages{U::None};
};

template<typename T, typename U>
bool operator==(const TextureResourceHandleWithUsageWithStages<T, U> &a, const TextureResourceHandleWithUsageWithStages<T, U> &b) {
	return a.texture == b.texture && a.usage == b.usage && a.stages == b.stages;
}

template<typename T, typename U>
bool operator!=(const TextureResourceHandleWithUsageWithStages<T, U> &a, const TextureResourceHandleWithUsageWithStages<T, U> &b) {
	return a.texture != b.texture || a.usage != b.usage || a.stages != b.stages;
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

enum class DepthStencilTextureUsage : uint32_t {
	None = 0x00000000,
	Read = 0x00000001,
	Write = 0x00000002,
	Modify = 0x00000004,
};
static_assert(sizeof(rn::graph::DepthStencilTextureUsage) == sizeof(uint32_t));

constexpr rn::graph::DepthStencilTextureUsage operator|(rn::graph::DepthStencilTextureUsage left, rn::graph::DepthStencilTextureUsage right);
constexpr rn::graph::DepthStencilTextureUsage operator&(rn::graph::DepthStencilTextureUsage left, rn::graph::DepthStencilTextureUsage right);

enum class GraphicTextureUsage : uint32_t {
	None = 0x00000000,
	Sampled = 0x00000001,
	Storage = 0x00000002,
};
static_assert(sizeof(rn::graph::GraphicTextureUsage) == sizeof(uint32_t));

constexpr rn::graph::GraphicTextureUsage operator|(rn::graph::GraphicTextureUsage left, rn::graph::GraphicTextureUsage right);
constexpr rn::graph::GraphicTextureUsage operator&(rn::graph::GraphicTextureUsage left, rn::graph::GraphicTextureUsage right);

enum class GraphicStage : uint32_t {
	None = 0x00000000,
	DrawIndirect = 0x00000001,
	VertexInput = 0x00000002,
	Vertex = 0x00000004,
	TessellationControl = 0x00000008,
	TessellationEvaluation = 0x00000010,
	Geometry = 0x00000020,
	Fragment = 0x00000040,
	EarlyFragmentTests = 0x00000080,
	LateFragmentTests = 0x00000100,
	Compute = 0x00000200,
	Transfer = 0x00000400,
};
static_assert(sizeof(rn::graph::GraphicStage) == sizeof(uint32_t));

constexpr rn::graph::GraphicStage operator|(rn::graph::GraphicStage left, rn::graph::GraphicStage right);
constexpr rn::graph::GraphicStage operator&(rn::graph::GraphicStage left, rn::graph::GraphicStage right);

enum class ComputeTextureUsage : uint32_t {
	None = 0x00000000,
	Sampled = 0x00000001,
	Storage = 0x00000002,
};
static_assert(sizeof(rn::graph::ComputeTextureUsage) == sizeof(uint32_t));

constexpr rn::graph::ComputeTextureUsage operator|(rn::graph::ComputeTextureUsage left, rn::graph::ComputeTextureUsage right);
constexpr rn::graph::ComputeTextureUsage operator&(rn::graph::ComputeTextureUsage left, rn::graph::ComputeTextureUsage right);

enum class TransferTextureUsage : uint32_t {
	None = 0x00000000,
	Destination = 0x00000001,
	Source = 0x00000002,
};
static_assert(sizeof(rn::graph::TransferTextureUsage) == sizeof(uint32_t));

constexpr rn::graph::TransferTextureUsage operator|(rn::graph::TransferTextureUsage left, rn::graph::TransferTextureUsage right);
constexpr rn::graph::TransferTextureUsage operator&(rn::graph::TransferTextureUsage left, rn::graph::TransferTextureUsage right);

struct TextureCreate {
	rn::PixelFormat format;
	rn::Extent3D dimensions{0, 0, 0};
	rn::ImageType type{rn::ImageType::Tex2D};
	uint32_t levels{1};
	uint32_t layers{1};
	rn::TextureUsage usage{rn::TextureUsage::None};
};

struct TextureModify {
	std::string source;
};

enum class GraphicBufferUsage : uint32_t {
	None = 0x00000000,
	Storage = 0x00000001,
	Uniform = 0x00000002,
	Indirect = 0x00000004,
	Index = 0x00000008,
	Vertex = 0x00000010,
};
static_assert(sizeof(rn::graph::GraphicBufferUsage) == sizeof(uint32_t));

constexpr rn::graph::GraphicBufferUsage operator|(rn::graph::GraphicBufferUsage left, rn::graph::GraphicBufferUsage right);
constexpr rn::graph::GraphicBufferUsage operator&(rn::graph::GraphicBufferUsage left, rn::graph::GraphicBufferUsage right);

enum class ComputeBufferUsage : uint32_t {
	None = 0x00000000,
	Storage = 0x00000001,
	Uniform = 0x00000002,
	Indirect = 0x00000004,
};
static_assert(sizeof(rn::graph::ComputeBufferUsage) == sizeof(uint32_t));

constexpr rn::graph::ComputeBufferUsage operator|(rn::graph::ComputeBufferUsage left, rn::graph::ComputeBufferUsage right);
constexpr rn::graph::ComputeBufferUsage operator&(rn::graph::ComputeBufferUsage left, rn::graph::ComputeBufferUsage right);

enum class TransferBufferUsage : uint32_t {
	None = 0x00000000,
	Destination = 0x00000001,
	Source = 0x00000002,
};
static_assert(sizeof(rn::graph::TransferBufferUsage) == sizeof(uint32_t));

constexpr rn::graph::TransferBufferUsage operator|(rn::graph::TransferBufferUsage left, rn::graph::TransferBufferUsage right);
constexpr rn::graph::TransferBufferUsage operator&(rn::graph::TransferBufferUsage left, rn::graph::TransferBufferUsage right);

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
