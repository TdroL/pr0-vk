#pragma once

#include <cstdint>
#include <limits>

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
	R8SInt,
	R8SNorm,
	R8UInt,
	R8UNorm,
	R8G8SInt,
	R8G8SNorm,
	R8G8UInt,
	R8G8UNorm,
	R8G8B8A8SInt,
	R8G8B8A8SNorm,
	R8G8B8A8UInt,
	R8G8B8A8UNorm,
	R16Float,
	R16SInt,
	R16SNorm,
	R16UInt,
	R16UNorm,
	R16G16Float,
	R16G16SInt,
	R16G16SNorm,
	R16G16UInt,
	R16G16UNorm,
	R16G16B16A16Float,
	R16G16B16A16SInt,
	R16G16B16A16SNorm,
	R16G16B16A16UInt,
	R16G16B16A16UNorm,
	R32Float,
	R32SInt,
	R32UInt,
	R32G32Float,
	R32G32SInt,
	R32G32UInt,
	R32G32B32Float,
	R32G32B32SInt,
	R32G32B32UInt,
	R32G32B32A32Float,
	R32G32B32A32SInt,
	R32G32B32A32UInt,
};

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

// buffer
enum class BufferUsage : uint32_t {
	None = 0x00000000,
	TransferSource = 0x00000001,
	TransferDestination = 0x00000002,
	Index = 0x00000004,
	Vertex = 0x00000008,
	Uniform = 0x00000010,
	Storage = 0x00000020,
};
static_assert(sizeof(rn::BufferUsage) == sizeof(uint32_t));

rn::BufferUsage operator|(rn::BufferUsage left, rn::BufferUsage right);
rn::BufferUsage operator&(rn::BufferUsage left, rn::BufferUsage right);

enum class BufferPaging {
	Single,
	SwapchainSize,
};

struct BufferDescription {
	size_t size;
	rn::BufferUsage usage{rn::BufferUsage::None};
	rn::BufferPaging paging{rn::BufferPaging::SwapchainSize};
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

// texture
enum class TextureUsage : uint32_t {
	None = 0x00000000,
	TransferSource = 0x00000001,
	TransferDestination = 0x00000002,
	Sampled = 0x00000004,
	Storage = 0x00000008,
	ColorAttachment = 0x00000010,
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

} // rn
