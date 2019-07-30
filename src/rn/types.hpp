#pragma once

#include <cstdint>
#include <functional>
#include <limits>
#include <string_view>

namespace rn {

template<class T>
constexpr T end();

struct Extent3D {
	uint32_t width{0};
	uint32_t height{0};
	uint32_t depth{0};
};

bool operator==(const rn::Extent3D &a, const rn::Extent3D &b);
bool operator!=(const rn::Extent3D &a, const rn::Extent3D &b);

struct Offset3D {
	uint32_t x{0};
	uint32_t y{0};
	uint32_t z{0};
};

bool operator==(const rn::Offset3D &a, const rn::Offset3D &b);
bool operator!=(const rn::Offset3D &a, const rn::Offset3D &b);

enum class PixelFormat {
	Undefined = 0,
	R8UNorm,
	R8SNorm,
	R8UInt,
	R8SInt,
	R8G8UNorm,
	R8G8SNorm,
	R8G8UInt,
	R8G8SInt,
	R8G8B8A8UNorm,
	R8G8B8A8SNorm,
	R8G8B8A8UInt,
	R8G8B8A8SInt,
	B8G8R8A8UNorm,
	R16UNorm,
	R16SNorm,
	R16UInt,
	R16SInt,
	R16Float,
	R16G16UNorm,
	R16G16SNorm,
	R16G16UInt,
	R16G16SInt,
	R16G16Float,
	R16G16B16A16UNorm,
	R16G16B16A16SNorm,
	R16G16B16A16UInt,
	R16G16B16A16SInt,
	R16G16B16A16Float,
	R32UInt,
	R32SInt,
	R32Float,
	R32G32UInt,
	R32G32SInt,
	R32G32Float,
	R32G32B32UInt,
	R32G32B32SInt,
	R32G32B32Float,
	R32G32B32A32UInt,
	R32G32B32A32SInt,
	R32G32B32A32Float,
	D16UNorm,
	D32Float,
	D24UNormS8UInt,
	D32FloatS8UInt,
	BC1UNorm,
	BC1SRGB,
	BC2UNorm,
	BC2SRGB,
	BC3UNorm,
	BC3SRGB,
	BC4UNorm,
	BC4SNorm,
	BC5UNorm,
	BC5SNorm,
	BC6HUFloat,
	BC6HSFloat,
	BC7UNorm,
	BC7SRGB,
	B4G4R4A4UNormPack16,
	B5G6R5UNormPack16,
	B5G5R5A1UNormPack16,
	R10G10B10A2UNorm,
	R10G10B10A2UInt,
	R11G11B10Float,
};

enum class DataFormat {
	Undefined = 0,
	// R8SInt,
	// R8SNorm,
	// R8UInt,
	// R8UNorm,
	// R8G8SInt,
	// R8G8SNorm,
	// R8G8UInt,
	// R8G8UNorm,
	// R8G8B8A8SInt,
	// R8G8B8A8SNorm,
	// R8G8B8A8UInt,
	// R8G8B8A8UNorm,
	// R16Float,
	// R16SInt,
	// R16SNorm,
	// R16UInt,
	// R16UNorm,
	// R16G16Float,
	// R16G16SInt,
	// R16G16SNorm,
	// R16G16UInt,
	// R16G16UNorm,
	// R16G16B16A16Float,
	// R16G16B16A16SInt,
	// R16G16B16A16SNorm,
	// R16G16B16A16UInt,
	// R16G16B16A16UNorm,
	// R32Float,
	// R32SInt,
	// R32UInt,
	// R32G32Float,
	// R32G32SInt,
	// R32G32UInt,
	// R32G32B32Float,
	// R32G32B32SInt,
	// R32G32B32UInt,
	// R32G32B32A32Float,
	// R32G32B32A32SInt,
	// R32G32B32A32UInt,
	UChar1,
	UChar2,
	// UChar3,
	UChar4,
	Char1,
	Char2,
	// Char3,
	Char4,
	UChar1Norm,
	UChar2Norm,
	// UChar3Norm,
	UChar4Norm,
	Char1Norm,
	Char2Norm,
	// Char3Norm,
	Char4Norm,
	UShort1,
	UShort2,
	// UShort3,
	UShort4,
	Short1,
	Short2,
	// Short3,
	Short4,
	UShort1Norm,
	UShort2Norm,
	// UShort3Norm,
	UShort4Norm,
	Short1Norm,
	Short2Norm,
	// Short3Norm,
	Short4Norm,
	Half1,
	Half2,
	// Half3,
	Half4,
	Float1,
	Float2,
	Float3,
	Float4,
	UInt1,
	UInt2,
	UInt3,
	UInt4,
	Int1,
	Int2,
	Int3,
	Int4,
};

constexpr size_t sizeOf(rn::DataFormat format) {
	switch (format) {
		default: [[fallthrough]];
		case rn::DataFormat::Undefined: return 0;
		case rn::DataFormat::UChar1: return 1 * 1;
		case rn::DataFormat::UChar2: return 2 * 1;
		// case rn::DataFormat::UChar3: return 3 * 1;
		case rn::DataFormat::UChar4: return 4 * 1;
		case rn::DataFormat::Char1: return 1 * 1;
		case rn::DataFormat::Char2: return 2 * 1;
		// case rn::DataFormat::Char3: return 3 * 1;
		case rn::DataFormat::Char4: return 4 * 1;
		case rn::DataFormat::UChar1Norm: return 1 * 1;
		case rn::DataFormat::UChar2Norm: return 2 * 1;
		// case rn::DataFormat::UChar3Norm: return 3 * 1;
		case rn::DataFormat::UChar4Norm: return 4 * 1;
		case rn::DataFormat::Char1Norm: return 1 * 1;
		case rn::DataFormat::Char2Norm: return 2 * 1;
		// case rn::DataFormat::Char3Norm: return 3 * 1;
		case rn::DataFormat::Char4Norm: return 4 * 1;
		case rn::DataFormat::UShort1: return 1 * 2;
		case rn::DataFormat::UShort2: return 2 * 2;
		// case rn::DataFormat::UShort3: return 3 * 2;
		case rn::DataFormat::UShort4: return 4 * 2;
		case rn::DataFormat::Short1: return 1 * 2;
		case rn::DataFormat::Short2: return 2 * 2;
		// case rn::DataFormat::Short3: return 3 * 2;
		case rn::DataFormat::Short4: return 4 * 2;
		case rn::DataFormat::UShort1Norm: return 1 * 2;
		case rn::DataFormat::UShort2Norm: return 2 * 2;
		// case rn::DataFormat::UShort3Norm: return 3 * 2;
		case rn::DataFormat::UShort4Norm: return 4 * 2;
		case rn::DataFormat::Short1Norm: return 1 * 2;
		case rn::DataFormat::Short2Norm: return 2 * 2;
		// case rn::DataFormat::Short3Norm: return 3 * 2;
		case rn::DataFormat::Short4Norm: return 4 * 2;
		case rn::DataFormat::Half1: return 1 * 2;
		case rn::DataFormat::Half2: return 2 * 2;
		// case rn::DataFormat::Half3: return 3 * 2;
		case rn::DataFormat::Half4: return 4 * 2;
		case rn::DataFormat::Float1: return 1 * 4;
		case rn::DataFormat::Float2: return 2 * 4;
		case rn::DataFormat::Float3: return 3 * 4;
		case rn::DataFormat::Float4: return 4 * 4;
		case rn::DataFormat::UInt1: return 1 * 4;
		case rn::DataFormat::UInt2: return 2 * 4;
		case rn::DataFormat::UInt3: return 3 * 4;
		case rn::DataFormat::UInt4: return 4 * 4;
		case rn::DataFormat::Int1: return 1 * 4;
		case rn::DataFormat::Int2: return 2 * 4;
		case rn::DataFormat::Int3: return 3 * 4;
		case rn::DataFormat::Int4: return 4 * 4;
		// case rn::DataFormat::R8SInt: return 1;
		// case rn::DataFormat::R8SNorm: return 1;
		// case rn::DataFormat::R8UInt: return 1;
		// case rn::DataFormat::R8UNorm: return 1;
		// case rn::DataFormat::R8G8SInt: return 2;
		// case rn::DataFormat::R8G8SNorm: return 2;
		// case rn::DataFormat::R8G8UInt: return 2;
		// case rn::DataFormat::R8G8UNorm: return 2;
		// case rn::DataFormat::R8G8B8A8SInt: return 4;
		// case rn::DataFormat::R8G8B8A8SNorm: return 4;
		// case rn::DataFormat::R8G8B8A8UInt: return 4;
		// case rn::DataFormat::R8G8B8A8UNorm: return 4;
		// case rn::DataFormat::R16Float: return 1;
		// case rn::DataFormat::R16SInt: return 1;
		// case rn::DataFormat::R16SNorm: return 1;
		// case rn::DataFormat::R16UInt: return 1;
		// case rn::DataFormat::R16UNorm: return 1;
		// case rn::DataFormat::R16G16Float: return 2;
		// case rn::DataFormat::R16G16SInt: return 2;
		// case rn::DataFormat::R16G16SNorm: return 2;
		// case rn::DataFormat::R16G16UInt: return 2;
		// case rn::DataFormat::R16G16UNorm: return 2;
		// case rn::DataFormat::R16G16B16A16Float: return 4;
		// case rn::DataFormat::R16G16B16A16SInt: return 4;
		// case rn::DataFormat::R16G16B16A16SNorm: return 4;
		// case rn::DataFormat::R16G16B16A16UInt: return 4;
		// case rn::DataFormat::R16G16B16A16UNorm: return 4;
		// case rn::DataFormat::R32Float: return 1;
		// case rn::DataFormat::R32SInt: return 1;
		// case rn::DataFormat::R32UInt: return 1;
		// case rn::DataFormat::R32G32Float: return 2;
		// case rn::DataFormat::R32G32SInt: return 2;
		// case rn::DataFormat::R32G32UInt: return 2;
		// case rn::DataFormat::R32G32B32Float: return 3;
		// case rn::DataFormat::R32G32B32SInt: return 3;
		// case rn::DataFormat::R32G32B32UInt: return 3;
		// case rn::DataFormat::R32G32B32A32Float: return 4;
		// case rn::DataFormat::R32G32B32A32SInt: return 4;
		// case rn::DataFormat::R32G32B32A32UInt: return 4;
	}
}

constexpr size_t numberOfComponents(rn::DataFormat format) {
	switch (format) {
		default: [[fallthrough]];
		case rn::DataFormat::Undefined: return 0;
		case rn::DataFormat::UChar1: return 1;
		case rn::DataFormat::UChar2: return 2;
		// case rn::DataFormat::UChar3: return 3;
		case rn::DataFormat::UChar4: return 4;
		case rn::DataFormat::Char1: return 1;
		case rn::DataFormat::Char2: return 2;
		// case rn::DataFormat::Char3: return 3;
		case rn::DataFormat::Char4: return 4;
		case rn::DataFormat::UChar1Norm: return 1;
		case rn::DataFormat::UChar2Norm: return 2;
		// case rn::DataFormat::UChar3Norm: return 3;
		case rn::DataFormat::UChar4Norm: return 4;
		case rn::DataFormat::Char1Norm: return 1;
		case rn::DataFormat::Char2Norm: return 2;
		// case rn::DataFormat::Char3Norm: return 3;
		case rn::DataFormat::Char4Norm: return 4;
		case rn::DataFormat::UShort1: return 1;
		case rn::DataFormat::UShort2: return 2;
		// case rn::DataFormat::UShort3: return 3;
		case rn::DataFormat::UShort4: return 4;
		case rn::DataFormat::Short1: return 1;
		case rn::DataFormat::Short2: return 2;
		// case rn::DataFormat::Short3: return 3;
		case rn::DataFormat::Short4: return 4;
		case rn::DataFormat::UShort1Norm: return 1;
		case rn::DataFormat::UShort2Norm: return 2;
		// case rn::DataFormat::UShort3Norm: return 3;
		case rn::DataFormat::UShort4Norm: return 4;
		case rn::DataFormat::Short1Norm: return 1;
		case rn::DataFormat::Short2Norm: return 2;
		// case rn::DataFormat::Short3Norm: return 3;
		case rn::DataFormat::Short4Norm: return 4;
		case rn::DataFormat::Half1: return 1;
		case rn::DataFormat::Half2: return 2;
		// case rn::DataFormat::Half3: return 3;
		case rn::DataFormat::Half4: return 4;
		case rn::DataFormat::Float1: return 1;
		case rn::DataFormat::Float2: return 2;
		case rn::DataFormat::Float3: return 3;
		case rn::DataFormat::Float4: return 4;
		case rn::DataFormat::UInt1: return 1;
		case rn::DataFormat::UInt2: return 2;
		case rn::DataFormat::UInt3: return 3;
		case rn::DataFormat::UInt4: return 4;
		case rn::DataFormat::Int1: return 1;
		case rn::DataFormat::Int2: return 2;
		case rn::DataFormat::Int3: return 3;
		case rn::DataFormat::Int4: return 4;
		// case rn::DataFormat::R8SInt: return 1;
		// case rn::DataFormat::R8SNorm: return 1;
		// case rn::DataFormat::R8UInt: return 1;
		// case rn::DataFormat::R8UNorm: return 1;
		// case rn::DataFormat::R8G8SInt: return 2;
		// case rn::DataFormat::R8G8SNorm: return 2;
		// case rn::DataFormat::R8G8UInt: return 2;
		// case rn::DataFormat::R8G8UNorm: return 2;
		// case rn::DataFormat::R8G8B8A8SInt: return 4;
		// case rn::DataFormat::R8G8B8A8SNorm: return 4;
		// case rn::DataFormat::R8G8B8A8UInt: return 4;
		// case rn::DataFormat::R8G8B8A8UNorm: return 4;
		// case rn::DataFormat::R16Float: return 1;
		// case rn::DataFormat::R16SInt: return 1;
		// case rn::DataFormat::R16SNorm: return 1;
		// case rn::DataFormat::R16UInt: return 1;
		// case rn::DataFormat::R16UNorm: return 1;
		// case rn::DataFormat::R16G16Float: return 2;
		// case rn::DataFormat::R16G16SInt: return 2;
		// case rn::DataFormat::R16G16SNorm: return 2;
		// case rn::DataFormat::R16G16UInt: return 2;
		// case rn::DataFormat::R16G16UNorm: return 2;
		// case rn::DataFormat::R16G16B16A16Float: return 4;
		// case rn::DataFormat::R16G16B16A16SInt: return 4;
		// case rn::DataFormat::R16G16B16A16SNorm: return 4;
		// case rn::DataFormat::R16G16B16A16UInt: return 4;
		// case rn::DataFormat::R16G16B16A16UNorm: return 4;
		// case rn::DataFormat::R32Float: return 1;
		// case rn::DataFormat::R32SInt: return 1;
		// case rn::DataFormat::R32UInt: return 1;
		// case rn::DataFormat::R32G32Float: return 2;
		// case rn::DataFormat::R32G32SInt: return 2;
		// case rn::DataFormat::R32G32UInt: return 2;
		// case rn::DataFormat::R32G32B32Float: return 3;
		// case rn::DataFormat::R32G32B32SInt: return 3;
		// case rn::DataFormat::R32G32B32UInt: return 3;
		// case rn::DataFormat::R32G32B32A32Float: return 4;
		// case rn::DataFormat::R32G32B32A32SInt: return 4;
		// case rn::DataFormat::R32G32B32A32UInt: return 4;
	}
}

// enum class DataFormat {
// 	Undefined = 0,
// 	// R8SInt,
// 	// R8SNorm,
// 	// R8UInt,
// 	// R8UNorm,
// 	// R8G8SInt,
// 	// R8G8SNorm,
// 	// R8G8UInt,
// 	// R8G8UNorm,
// 	// R8G8B8A8SInt,
// 	// R8G8B8A8SNorm,
// 	// R8G8B8A8UInt,
// 	// R8G8B8A8UNorm,
// 	// R16Float,
// 	// R16SInt,
// 	// R16SNorm,
// 	// R16UInt,
// 	// R16UNorm,
// 	// R16G16Float,
// 	// R16G16SInt,
// 	// R16G16SNorm,
// 	// R16G16UInt,
// 	// R16G16UNorm,
// 	// R16G16B16A16Float,
// 	// R16G16B16A16SInt,
// 	// R16G16B16A16SNorm,
// 	// R16G16B16A16UInt,
// 	// R16G16B16A16UNorm,
// 	// R32Float,
// 	// R32SInt,
// 	// R32UInt,
// 	// R32G32Float,
// 	// R32G32SInt,
// 	// R32G32UInt,
// 	// R32G32B32Float,
// 	// R32G32B32SInt,
// 	// R32G32B32UInt,
// 	// R32G32B32A32Float,
// 	// R32G32B32A32SInt,
// 	// R32G32B32A32UInt,
// 	UChar1,
// 	UChar2,
// 	UChar3,
// 	UChar4,
// 	Char1,
// 	Char2,
// 	Char3,
// 	Char4,
// 	UChar1Norm,
// 	UChar2Norm,
// 	UChar3Norm,
// 	UChar4Norm,
// 	Char1Norm,
// 	Char2Norm,
// 	Char3Norm,
// 	Char4Norm,
// 	UShort1,
// 	UShort2,
// 	UShort3,
// 	UShort4,
// 	Short1,
// 	Short2,
// 	Short3,
// 	Short4,
// 	UShort1Norm,
// 	UShort2Norm,
// 	UShort3Norm,
// 	UShort4Norm,
// 	Short1Norm,
// 	Short2Norm,
// 	Short3Norm,
// 	Short4Norm,
// 	Half1,
// 	Half2,
// 	Half3,
// 	Half4,
// 	Float1,
// 	Float2,
// 	Float3,
// 	Float4,
// 	UInt1,
// 	UInt2,
// 	UInt3,
// 	UInt4,
// 	Int1,
// 	Int2,
// 	Int3,
// 	Int4,
// };

enum class ImageType {
	Tex1D,
	Tex2D,
	Tex3D,
};

enum class ImageViewType {
	Tex1D,
	Tex2D,
	Tex3D,
	Cube,
	Array1D,
	Array2D,
	ArrayCube,
};

enum class ColorSpace {
	Undefined,
	LDR,
	HDR,
};

enum class VSync {
	Fifo,
	FifoRelaxed,
	Mailbox,
	Immediate,
};

enum class WindowMode {
	Windowed,
	Borderless,
	Fullscreen,
};

// fence
// struct FenceHandle {
// 	using InternalType = uint32_t;

// 	InternalType index{std::numeric_limits<InternalType>::max()};
// };
// static_assert(sizeof(rn::FenceHandle) == sizeof(uint32_t));

// bool operator==(rn::FenceHandle a, rn::FenceHandle b);
// bool operator!=(rn::FenceHandle a, rn::FenceHandle b);

// template<>
// constexpr rn::FenceHandle end() {
// 	return { std::numeric_limits<rn::FenceHandle::InternalType>::max() };
// }

using FenceStamp = size_t;

template<>
constexpr rn::FenceStamp end() {
	return std::numeric_limits<rn::FenceStamp>::max();
}

// buffer
enum class BufferUsage : uint32_t {
	None = 0x00000000,
	TransferSource = 0x00000001,
	TransferDestination = 0x00000002,
	Index = 0x00000004,
	Vertex = 0x00000008,
	Uniform = 0x00000010,
	Storage = 0x00000020,
	Indirect = 0x00000040,
	Staging = 0x10000000,
};
static_assert(sizeof(rn::BufferUsage) == sizeof(uint32_t));

rn::BufferUsage operator|(rn::BufferUsage left, rn::BufferUsage right);
rn::BufferUsage operator&(rn::BufferUsage left, rn::BufferUsage right);

enum class BufferPaging {
	Static,
	SwapchainRelative,
};

struct BufferDescription {
	size_t size;
	rn::BufferUsage usage{rn::BufferUsage::None};
	rn::BufferPaging paging{rn::BufferPaging::SwapchainRelative};
};

bool operator==(const rn::BufferDescription &a, const rn::BufferDescription &b);
bool operator!=(const rn::BufferDescription &a, const rn::BufferDescription &b);

struct BufferHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::BufferHandle) == sizeof(uint32_t));

bool operator==(rn::BufferHandle a, rn::BufferHandle b);
bool operator!=(rn::BufferHandle a, rn::BufferHandle b);

template<>
constexpr rn::BufferHandle end() {
	return { std::numeric_limits<rn::BufferHandle::InternalType>::max() };
}

enum class BufferAccess : uint32_t {
	None = 0,
	// read
	IndirectBufferRead = 1, // Read as an indirect buffer for drawing or dispatch (THSVS_ACCESS_INDIRECT_BUFFER)
	IndexBufferRead = 2, // Read as an index buffer for drawing (THSVS_ACCESS_INDEX_BUFFER)
	VertexBufferRead = 3, // Read as a vertex buffer for drawing (THSVS_ACCESS_VERTEX_BUFFER)
	VertexShaderUniformBufferRead = 4, // Read as a uniform buffer in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_READ_UNIFORM_BUFFER)
	VertexShaderUniformTexelBufferRead = 5, // Read as a sampled image/uniform texel buffer in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	TessellationControlShaderUniformBufferRead = 6, // Read as a uniform buffer in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_READ_UNIFORM_BUFFER)
	TessellationControlShaderUniformTexelBufferRead = 7, // Read as a sampled image/uniform texel buffer  in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	TessellationEvaluationShaderUniformBufferRead = 8, // Read as a uniform buffer in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_READ_UNIFORM_BUFFER)
	TessellationEvaluationShaderUniformTexelBufferRead = 9, // Read as a sampled image/uniform texel buffer in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	GeometryShaderUniformBufferRead = 10, // Read as a uniform buffer in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_READ_UNIFORM_BUFFER)
	GeometryShaderUniformTexelBufferRead = 11, // Read as a sampled image/uniform texel buffer  in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	FragmentShaderUniformBufferRead = 12, // Read as a uniform buffer in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_UNIFORM_BUFFER)
	FragmentShaderUniformTexelBufferRead = 13, // Read as a sampled image/uniform texel buffer  in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	ComputeShaderUniformBufferRead = 14, // Read as a uniform buffer in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_READ_UNIFORM_BUFFER)
	ComputeShaderUniformTexelBufferRead = 15, // Read as a sampled image/uniform texel buffer in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	AnyShaderUniformBufferRead = 16, // Read as a uniform buffer in any shader (THSVS_ACCESS_ANY_SHADER_READ_UNIFORM_BUFFER)
	AnyShaderUniformBufferOrVertexBufferRead = 17, // Read as a uniform buffer in any shader, or a vertex buffer (THSVS_ACCESS_ANY_SHADER_READ_UNIFORM_BUFFER_OR_VERTEX_BUFFER)
	AnyShaderUniformTexelBufferRead = 18, // Read as a sampled image in any shader (THSVS_ACCESS_ANY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	TransferRead = 19, // Read as the source of a transfer operation (THSVS_ACCESS_TRANSFER_READ)
	HostRead = 20, // Read on the host (THSVS_ACCESS_HOST_READ)
	// write
	VertexShaderWrite = 21, // Written as any resource in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_WRITE)
	TessellationControlShaderWrite = 22, // Written as any resource in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_WRITE)
	TessellationEvaluationShaderWrite = 23, // Written as any resource in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_WRITE)
	GeometryShaderWrite = 24, // Written as any resource in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_WRITE)
	FragmentShaderWrite = 25, // Written as any resource in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_WRITE)
	ComputeShaderWrite = 26, // Written as any resource in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_WRITE)
	AnyShaderWrite = 27, // Written as any resource in any shader (THSVS_ACCESS_ANY_SHADER_WRITE)
	TransferWrite = 28, // Written as the destination of a transfer operation (THSVS_ACCESS_TRANSFER_WRITE)
	HostPreinitialized = 29, // Data pre-filled by host before device access starts (THSVS_ACCESS_HOST_PREINITIALIZED)
	HostWrite = 30, // Written on the host (THSVS_ACCESS_HOST_WRITE)
	General = 31, // Covers any access - useful for debug, generally avoid for performance reasons (THSVS_ACCESS_GENERAL)
};

// texture
enum class TextureUsage : uint32_t {
	None = 0x00000000,
	TransferSource = 0x00000001,
	TransferDestination = 0x00000002,
	Sampled = 0x00000004,
	Storage = 0x00000008,
	OutputAttachment = 0x00000010,
	DepthStencilAttachment = 0x00000020,
	TransientAttachment = 0x00000040,
	InputAttachment = 0x00000080,
};
static_assert(sizeof(rn::TextureUsage) == sizeof(uint32_t));

rn::TextureUsage operator|(rn::TextureUsage left, rn::TextureUsage right);
rn::TextureUsage operator&(rn::TextureUsage left, rn::TextureUsage right);

struct TextureDescription {
	rn::PixelFormat format;
	rn::Extent3D dimensions;
	rn::ImageType type{rn::ImageType::Tex2D};
	uint32_t levels{1};
	uint32_t layers{1};
	rn::TextureUsage usage{rn::TextureUsage::None};
};

bool operator==(const rn::TextureDescription &a, const rn::TextureDescription &b);
bool operator!=(const rn::TextureDescription &a, const rn::TextureDescription &b);

struct TextureHandle {
	using InternalType = uint32_t;

	InternalType index{std::numeric_limits<InternalType>::max()};
};
static_assert(sizeof(rn::TextureHandle) == sizeof(uint32_t));

bool operator==(rn::TextureHandle a, rn::TextureHandle b);
bool operator!=(rn::TextureHandle a, rn::TextureHandle b);

template<>
constexpr rn::TextureHandle end() {
	return { std::numeric_limits<rn::TextureHandle::InternalType>::max() };
}

constexpr rn::TextureHandle swapchainHandle() {
	return { std::numeric_limits<rn::TextureHandle::InternalType>::max() - 1 };
}

// TextureAccess based on https://github.com/Tobski/simple_vulkan_synchronization
enum class TextureAccess : uint32_t {
	None = 0,
	// read
	VertexShaderSampledRead = 1, // Read as a sampled image/uniform texel buffer in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	TessellationControlShaderSampledRead = 2, // Read as a sampled image/uniform texel buffer  in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	TessellationEvaluationShaderSampledRead = 3, // Read as a sampled image/uniform texel buffer in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	GeometryShaderSampledRead = 4, // Read as a sampled image/uniform texel buffer  in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	FragmentShaderSampledRead = 5, // Read as a sampled image/uniform texel buffer  in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	FragmentShaderColorInputAttachmentRead = 6, // Read as an input attachment with a color format in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_COLOR_INPUT_ATTACHMENT)
	FragmentShaderDepthStencilInputAttachmentRead = 7, // Read as an input attachment with a depth/stencil format in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_DEPTH_STENCIL_INPUT_ATTACHMENT)
	ColorAttachmentRead = 8, // Read by standard blending/logic operations or subpass load operations (THSVS_ACCESS_COLOR_ATTACHMENT_READ)
	DepthStencilAttachmentRead = 9, // Read by depth/stencil tests or subpass load operations (THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ)
	ComputeShaderSampledRead = 10, // Read as a sampled image/uniform texel buffer in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	AnyShaderSampledRead = 11, // Read as a sampled image in any shader (THSVS_ACCESS_ANY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
	TransferRead = 12, // Read as the source of a transfer operation (THSVS_ACCESS_TRANSFER_READ)
	// write
	Present = 13, // Read by the presentation engine (i.e. vkQueuePresentKHR) (THSVS_ACCESS_PRESENT)
	FragmentShaderWrite = 14, // Written as any resource in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_WRITE)
	ColorAttachmentWrite = 15, // Written as a color attachment during rendering, or via a subpass store op (THSVS_ACCESS_COLOR_ATTACHMENT_WRITE)
	DepthStencilAttachmentWrite = 16, // Written as a depth/stencil attachment during rendering, or via a subpass store op (THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE)
	ComputeShaderWrite = 17, // Written as any resource in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_WRITE)
	TransferWrite = 18, // Written as the destination of a transfer operation (THSVS_ACCESS_TRANSFER_WRITE)
	HostPreinitialized = 19, // Data pre-filled by host before device access starts (THSVS_ACCESS_HOST_PREINITIALIZED)
	HostWrite = 20, // Written on the host (THSVS_ACCESS_HOST_WRITE)
	// read-write
	DepthAttachmentWriteStencilRead = 21, // Written as a depth aspect of a depth/stencil attachment during rendering, whilst the stencil aspect is read-only (THSVS_ACCESS_DEPTH_ATTACHMENT_WRITE_STENCIL_READ_ONLY)
	StencilAttachmentWriteDepthRead = 22, // Written as a stencil aspect of a depth/stencil attachment during rendering, whilst the depth aspect is read-only (THSVS_ACCESS_STENCIL_ATTACHMENT_WRITE_DEPTH_READ_ONLY)
	ColorAttachmentWriteRead = 23, // Read or written as a color attachment during rendering (THSVS_ACCESS_COLOR_ATTACHMENT_READ_WRITE)
	General = 24, // Covers any access - useful for debug, generally avoid for performance reasons (THSVS_ACCESS_GENERAL)
};

enum class ShaderStage : uint32_t {
	None = 0x00000000,
	Vertex = 0x00000001,
	Fragment = 0x00000002,
	Compute = 0x00000004,
};

rn::ShaderStage operator|(rn::ShaderStage left, rn::ShaderStage right);
rn::ShaderStage operator&(rn::ShaderStage left, rn::ShaderStage right);

enum class DescriptorType : uint32_t {
	UniformBuffer,
	DynamicUniformBuffer,
	Sampler,
	SampledTexture,
	StorageBuffer,
	DynamicStorageBuffer,
};

// std::string_view toString();
// std::string_view toString();
std::string_view toString(PixelFormat);
std::string_view toString(DataFormat format);

struct BufferCopyRange {
	size_t bufferOffset;
	std::byte *data;
	size_t length;
};

struct BufferDataAccessor {
	std::function<std::byte * ()> data;
	std::function<size_t ()> size;
	std::function<rn::DataFormat ()> format;
};

struct TextureDataAccessor {
	std::function<std::byte * (uint32_t layer, uint32_t level)> data;
	std::function<size_t (uint32_t level)> size;
	std::function<rn::DataFormat ()> format;
};

enum class QueueType : uint32_t {
	None = 0,
	Graphic = 1,
	Compute = 2,
	Transfer = 3,
	Presentation = 4,
};

} // rn
