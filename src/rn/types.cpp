#include "types.hpp"

#include <tuple>

namespace rn {

bool operator==(const rn::Extent3D &a, const rn::Extent3D &b) {
	return std::tie(a.width, a.height, a.depth) == std::tie(b.width, b.height, b.depth);
}

bool operator!=(const rn::Extent3D &a, const rn::Extent3D &b) {
	return !(a == b);
}

bool operator==(const rn::Offset3D &a, const rn::Offset3D &b) {
	return std::tie(a.x, a.y, a.z) == std::tie(b.x, b.y, b.z);
}

bool operator!=(const rn::Offset3D &a, const rn::Offset3D &b) {
	return !(a == b);
}

// bool operator==(rn::FenceHandle a, rn::FenceHandle b) {
// 	return a.index == b.index;
// }

// bool operator!=(rn::FenceHandle a, rn::FenceHandle b) {
// 	return a.index != b.index;
// }

bool operator==(rn::BufferHandle a, rn::BufferHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::BufferHandle a, rn::BufferHandle b) {
	return a.index != b.index;
}

rn::BufferUsage operator|(rn::BufferUsage left, rn::BufferUsage right) {
	return static_cast<rn::BufferUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

rn::BufferUsage operator&(rn::BufferUsage left, rn::BufferUsage right) {
	return static_cast<rn::BufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

bool operator==(const rn::BufferDescription &a, const rn::BufferDescription &b) {
	return std::tie(a.size, a.usage, a.paging) == std::tie(b.size, b.usage, b.paging);
}

bool operator!=(const rn::BufferDescription &a, const rn::BufferDescription &b) {
	return !(a == b);
}

bool operator==(const rn::TextureDescription &a, const rn::TextureDescription &b) {
	return std::tie(a.format, a.dimensions, a.type, a.levels, a.layers, a.usage) == std::tie(b.format, b.dimensions, b.type, b.levels, b.layers, b.usage);
}

bool operator!=(const rn::TextureDescription &a, const rn::TextureDescription &b) {
	return !(a == b);
}

bool operator==(rn::TextureHandle a, rn::TextureHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::TextureHandle a, rn::TextureHandle b) {
	return a.index != b.index;
}

rn::TextureUsage operator|(rn::TextureUsage left, rn::TextureUsage right) {
	return static_cast<rn::TextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

rn::TextureUsage operator&(rn::TextureUsage left, rn::TextureUsage right) {
	return static_cast<rn::TextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

rn::ShaderStage operator|(rn::ShaderStage left, rn::ShaderStage right) {
	return static_cast<rn::ShaderStage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

rn::ShaderStage operator&(rn::ShaderStage left, rn::ShaderStage right) {
	return static_cast<rn::ShaderStage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

std::string_view toString(PixelFormat format) {
	switch (format) {
		default: [[fallthrough]];
		case rn::PixelFormat::Undefined: return "rn::PixelFormat::Undefined";
		case rn::PixelFormat::R8UNorm: return "rn::PixelFormat::R8UNorm";
		case rn::PixelFormat::R8SNorm: return "rn::PixelFormat::R8SNorm";
		case rn::PixelFormat::R8UInt: return "rn::PixelFormat::R8UInt";
		case rn::PixelFormat::R8SInt: return "rn::PixelFormat::R8SInt";
		case rn::PixelFormat::R8G8UNorm: return "rn::PixelFormat::R8G8UNorm";
		case rn::PixelFormat::R8G8SNorm: return "rn::PixelFormat::R8G8SNorm";
		case rn::PixelFormat::R8G8UInt: return "rn::PixelFormat::R8G8UInt";
		case rn::PixelFormat::R8G8SInt: return "rn::PixelFormat::R8G8SInt";
		case rn::PixelFormat::R8G8B8A8UNorm: return "rn::PixelFormat::R8G8B8A8UNorm";
		case rn::PixelFormat::R8G8B8A8SNorm: return "rn::PixelFormat::R8G8B8A8SNorm";
		case rn::PixelFormat::R8G8B8A8UInt: return "rn::PixelFormat::R8G8B8A8UInt";
		case rn::PixelFormat::R8G8B8A8SInt: return "rn::PixelFormat::R8G8B8A8SInt";
		case rn::PixelFormat::B8G8R8A8UNorm: return "rn::PixelFormat::B8G8R8A8UNorm";
		case rn::PixelFormat::R16UNorm: return "rn::PixelFormat::R16UNorm";
		case rn::PixelFormat::R16SNorm: return "rn::PixelFormat::R16SNorm";
		case rn::PixelFormat::R16UInt: return "rn::PixelFormat::R16UInt";
		case rn::PixelFormat::R16SInt: return "rn::PixelFormat::R16SInt";
		case rn::PixelFormat::R16Float: return "rn::PixelFormat::R16Float";
		case rn::PixelFormat::R16G16UNorm: return "rn::PixelFormat::R16G16UNorm";
		case rn::PixelFormat::R16G16SNorm: return "rn::PixelFormat::R16G16SNorm";
		case rn::PixelFormat::R16G16UInt: return "rn::PixelFormat::R16G16UInt";
		case rn::PixelFormat::R16G16SInt: return "rn::PixelFormat::R16G16SInt";
		case rn::PixelFormat::R16G16Float: return "rn::PixelFormat::R16G16Float";
		case rn::PixelFormat::R16G16B16A16UNorm: return "rn::PixelFormat::R16G16B16A16UNorm";
		case rn::PixelFormat::R16G16B16A16SNorm: return "rn::PixelFormat::R16G16B16A16SNorm";
		case rn::PixelFormat::R16G16B16A16UInt: return "rn::PixelFormat::R16G16B16A16UInt";
		case rn::PixelFormat::R16G16B16A16SInt: return "rn::PixelFormat::R16G16B16A16SInt";
		case rn::PixelFormat::R16G16B16A16Float: return "rn::PixelFormat::R16G16B16A16Float";
		case rn::PixelFormat::R32UInt: return "rn::PixelFormat::R32UInt";
		case rn::PixelFormat::R32SInt: return "rn::PixelFormat::R32SInt";
		case rn::PixelFormat::R32Float: return "rn::PixelFormat::R32Float";
		case rn::PixelFormat::R32G32UInt: return "rn::PixelFormat::R32G32UInt";
		case rn::PixelFormat::R32G32SInt: return "rn::PixelFormat::R32G32SInt";
		case rn::PixelFormat::R32G32Float: return "rn::PixelFormat::R32G32Float";
		case rn::PixelFormat::R32G32B32UInt: return "rn::PixelFormat::R32G32B32UInt";
		case rn::PixelFormat::R32G32B32SInt: return "rn::PixelFormat::R32G32B32SInt";
		case rn::PixelFormat::R32G32B32Float: return "rn::PixelFormat::R32G32B32Float";
		case rn::PixelFormat::R32G32B32A32UInt: return "rn::PixelFormat::R32G32B32A32UInt";
		case rn::PixelFormat::R32G32B32A32SInt: return "rn::PixelFormat::R32G32B32A32SInt";
		case rn::PixelFormat::R32G32B32A32Float: return "rn::PixelFormat::R32G32B32A32Float";
		case rn::PixelFormat::D16UNorm: return "rn::PixelFormat::D16UNorm";
		case rn::PixelFormat::D32Float: return "rn::PixelFormat::D32Float";
		case rn::PixelFormat::D24UNormS8UInt: return "rn::PixelFormat::D24UNormS8UInt";
		case rn::PixelFormat::D32FloatS8UInt: return "rn::PixelFormat::D32FloatS8UInt";
		case rn::PixelFormat::BC1UNorm: return "rn::PixelFormat::BC1UNorm";
		case rn::PixelFormat::BC1SRGB: return "rn::PixelFormat::BC1SRGB";
		case rn::PixelFormat::BC2UNorm: return "rn::PixelFormat::BC2UNorm";
		case rn::PixelFormat::BC2SRGB: return "rn::PixelFormat::BC2SRGB";
		case rn::PixelFormat::BC3UNorm: return "rn::PixelFormat::BC3UNorm";
		case rn::PixelFormat::BC3SRGB: return "rn::PixelFormat::BC3SRGB";
		case rn::PixelFormat::BC4UNorm: return "rn::PixelFormat::BC4UNorm";
		case rn::PixelFormat::BC4SNorm: return "rn::PixelFormat::BC4SNorm";
		case rn::PixelFormat::BC5UNorm: return "rn::PixelFormat::BC5UNorm";
		case rn::PixelFormat::BC5SNorm: return "rn::PixelFormat::BC5SNorm";
		case rn::PixelFormat::BC6HUFloat: return "rn::PixelFormat::BC6HUFloat";
		case rn::PixelFormat::BC6HSFloat: return "rn::PixelFormat::BC6HSFloat";
		case rn::PixelFormat::BC7UNorm: return "rn::PixelFormat::BC7UNorm";
		case rn::PixelFormat::BC7SRGB: return "rn::PixelFormat::BC7SRGB";
		case rn::PixelFormat::B4G4R4A4UNormPack16: return "rn::PixelFormat::B4G4R4A4UNormPack16";
		case rn::PixelFormat::B5G6R5UNormPack16: return "rn::PixelFormat::B5G6R5UNormPack16";
		case rn::PixelFormat::B5G5R5A1UNormPack16: return "rn::PixelFormat::B5G5R5A1UNormPack16";
		case rn::PixelFormat::R10G10B10A2UNorm: return "rn::PixelFormat::R10G10B10A2UNorm";
		case rn::PixelFormat::R10G10B10A2UInt: return "rn::PixelFormat::R10G10B10A2UInt";
		case rn::PixelFormat::R11G11B10Float: return "rn::PixelFormat::R11G11B10Float";
	}
}
std::string_view toString(DataFormat format) {
	switch (format) {
		default: [[fallthrough]];
		case rn::DataFormat::Undefined: return "rn::DataFormat::Undefined";
		case rn::DataFormat::UChar1: return "rn::DataFormat::UChar1";
		case rn::DataFormat::UChar2: return "rn::DataFormat::UChar2";
		// case rn::DataFormat::UChar3: return "rn::DataFormat::UChar3";
		case rn::DataFormat::UChar4: return "rn::DataFormat::UChar4";
		case rn::DataFormat::Char1: return "rn::DataFormat::Char1";
		case rn::DataFormat::Char2: return "rn::DataFormat::Char2";
		// case rn::DataFormat::Char3: return "rn::DataFormat::Char3";
		case rn::DataFormat::Char4: return "rn::DataFormat::Char4";
		case rn::DataFormat::UChar1Norm: return "rn::DataFormat::UChar1Norm";
		case rn::DataFormat::UChar2Norm: return "rn::DataFormat::UChar2Norm";
		// case rn::DataFormat::UChar3Norm: return "rn::DataFormat::UChar3Norm";
		case rn::DataFormat::UChar4Norm: return "rn::DataFormat::UChar4Norm";
		case rn::DataFormat::Char1Norm: return "rn::DataFormat::Char1Norm";
		case rn::DataFormat::Char2Norm: return "rn::DataFormat::Char2Norm";
		// case rn::DataFormat::Char3Norm: return "rn::DataFormat::Char3Norm";
		case rn::DataFormat::Char4Norm: return "rn::DataFormat::Char4Norm";
		case rn::DataFormat::UShort1: return "rn::DataFormat::UShort1";
		case rn::DataFormat::UShort2: return "rn::DataFormat::UShort2";
		// case rn::DataFormat::UShort3: return "rn::DataFormat::UShort3";
		case rn::DataFormat::UShort4: return "rn::DataFormat::UShort4";
		case rn::DataFormat::Short1: return "rn::DataFormat::Short1";
		case rn::DataFormat::Short2: return "rn::DataFormat::Short2";
		// case rn::DataFormat::Short3: return "rn::DataFormat::Short3";
		case rn::DataFormat::Short4: return "rn::DataFormat::Short4";
		case rn::DataFormat::UShort1Norm: return "rn::DataFormat::UShort1Norm";
		case rn::DataFormat::UShort2Norm: return "rn::DataFormat::UShort2Norm";
		// case rn::DataFormat::UShort3Norm: return "rn::DataFormat::UShort3Norm";
		case rn::DataFormat::UShort4Norm: return "rn::DataFormat::UShort4Norm";
		case rn::DataFormat::Short1Norm: return "rn::DataFormat::Short1Norm";
		case rn::DataFormat::Short2Norm: return "rn::DataFormat::Short2Norm";
		// case rn::DataFormat::Short3Norm: return "rn::DataFormat::Short3Norm";
		case rn::DataFormat::Short4Norm: return "rn::DataFormat::Short4Norm";
		case rn::DataFormat::Half1: return "rn::DataFormat::Half1";
		case rn::DataFormat::Half2: return "rn::DataFormat::Half2";
		// case rn::DataFormat::Half3: return "rn::DataFormat::Half3";
		case rn::DataFormat::Half4: return "rn::DataFormat::Half4";
		case rn::DataFormat::Float1: return "rn::DataFormat::Float1";
		case rn::DataFormat::Float2: return "rn::DataFormat::Float2";
		case rn::DataFormat::Float3: return "rn::DataFormat::Float3";
		case rn::DataFormat::Float4: return "rn::DataFormat::Float4";
		case rn::DataFormat::UInt1: return "rn::DataFormat::UInt1";
		case rn::DataFormat::UInt2: return "rn::DataFormat::UInt2";
		case rn::DataFormat::UInt3: return "rn::DataFormat::UInt3";
		case rn::DataFormat::UInt4: return "rn::DataFormat::UInt4";
		case rn::DataFormat::Int1: return "rn::DataFormat::Int1";
		case rn::DataFormat::Int2: return "rn::DataFormat::Int2";
		case rn::DataFormat::Int3: return "rn::DataFormat::Int3";
		case rn::DataFormat::Int4: return "rn::DataFormat::Int4";
	}
}

} // rn
