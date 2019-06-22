#include "mapping.hpp"

namespace rn::vki {

vk::Format fromCommon(rn::PixelFormat pixelFormat) {
	switch (pixelFormat) {
		default:
		case rn::PixelFormat::Undefined: return vk::Format::eUndefined;
		case rn::PixelFormat::R8UNorm: return vk::Format::eR8Unorm;
		case rn::PixelFormat::R8SNorm: return vk::Format::eR8Snorm;
		case rn::PixelFormat::R8UInt: return vk::Format::eR8Uint;
		case rn::PixelFormat::R8SInt: return vk::Format::eR8Sint;
		case rn::PixelFormat::R8G8UNorm: return vk::Format::eR8G8Unorm;
		case rn::PixelFormat::R8G8SNorm: return vk::Format::eR8G8Snorm;
		case rn::PixelFormat::R8G8UInt: return vk::Format::eR8G8Uint;
		case rn::PixelFormat::R8G8SInt: return vk::Format::eR8G8Sint;
		case rn::PixelFormat::R8G8B8A8UNorm: return vk::Format::eR8G8B8A8Unorm;
		case rn::PixelFormat::R8G8B8A8SNorm: return vk::Format::eR8G8B8A8Snorm;
		case rn::PixelFormat::R8G8B8A8UInt: return vk::Format::eR8G8B8A8Uint;
		case rn::PixelFormat::R8G8B8A8SInt: return vk::Format::eR8G8B8A8Sint;
		case rn::PixelFormat::B8G8R8A8UNorm: return vk::Format::eB8G8R8A8Unorm;
		case rn::PixelFormat::R16UNorm: return vk::Format::eR16Unorm;
		case rn::PixelFormat::R16SNorm: return vk::Format::eR16Snorm;
		case rn::PixelFormat::R16UInt: return vk::Format::eR16Uint;
		case rn::PixelFormat::R16SInt: return vk::Format::eR16Sint;
		case rn::PixelFormat::R16Float: return vk::Format::eR16Sfloat;
		case rn::PixelFormat::R16G16UNorm: return vk::Format::eR16G16Unorm;
		case rn::PixelFormat::R16G16SNorm: return vk::Format::eR16G16Snorm;
		case rn::PixelFormat::R16G16UInt: return vk::Format::eR16G16Uint;
		case rn::PixelFormat::R16G16SInt: return vk::Format::eR16G16Sint;
		case rn::PixelFormat::R16G16Float: return vk::Format::eR16G16Sfloat;
		case rn::PixelFormat::R16G16B16A16UNorm: return vk::Format::eR16G16B16A16Unorm;
		case rn::PixelFormat::R16G16B16A16SNorm: return vk::Format::eR16G16B16A16Snorm;
		case rn::PixelFormat::R16G16B16A16UInt: return vk::Format::eR16G16B16A16Uint;
		case rn::PixelFormat::R16G16B16A16SInt: return vk::Format::eR16G16B16A16Sint;
		case rn::PixelFormat::R16G16B16A16Float: return vk::Format::eR16G16B16A16Sfloat;
		case rn::PixelFormat::R32UInt: return vk::Format::eR32Uint;
		case rn::PixelFormat::R32SInt: return vk::Format::eR32Sint;
		case rn::PixelFormat::R32Float: return vk::Format::eR32Sfloat;
		case rn::PixelFormat::R32G32UInt: return vk::Format::eR32G32Uint;
		case rn::PixelFormat::R32G32SInt: return vk::Format::eR32G32Sint;
		case rn::PixelFormat::R32G32Float: return vk::Format::eR32G32Sfloat;
		case rn::PixelFormat::R32G32B32UInt: return vk::Format::eR32G32B32Uint;
		case rn::PixelFormat::R32G32B32SInt: return vk::Format::eR32G32B32Sint;
		case rn::PixelFormat::R32G32B32Float: return vk::Format::eR32G32B32Sfloat;
		case rn::PixelFormat::R32G32B32A32UInt: return vk::Format::eR32G32B32A32Uint;
		case rn::PixelFormat::R32G32B32A32SInt: return vk::Format::eR32G32B32A32Sint;
		case rn::PixelFormat::R32G32B32A32Float: return vk::Format::eR32G32B32A32Sfloat;
		case rn::PixelFormat::D16UNorm: return vk::Format::eD16Unorm;
		case rn::PixelFormat::D32Float: return vk::Format::eD32Sfloat;
		case rn::PixelFormat::D24UNormS8UInt: return vk::Format::eD24UnormS8Uint;
		case rn::PixelFormat::D32FloatS8UInt: return vk::Format::eD32SfloatS8Uint;
		case rn::PixelFormat::BC1UNorm: return vk::Format::eBc1RgbaUnormBlock;
		case rn::PixelFormat::BC1SRGB: return vk::Format::eBc1RgbaSrgbBlock;
		case rn::PixelFormat::BC2UNorm: return vk::Format::eBc2UnormBlock;
		case rn::PixelFormat::BC2SRGB: return vk::Format::eBc2SrgbBlock;
		case rn::PixelFormat::BC3UNorm: return vk::Format::eBc3UnormBlock;
		case rn::PixelFormat::BC3SRGB: return vk::Format::eBc3SrgbBlock;
		case rn::PixelFormat::BC4UNorm: return vk::Format::eBc4UnormBlock;
		case rn::PixelFormat::BC4SNorm: return vk::Format::eBc4SnormBlock;
		case rn::PixelFormat::BC5UNorm: return vk::Format::eBc5UnormBlock;
		case rn::PixelFormat::BC5SNorm: return vk::Format::eBc5SnormBlock;
		case rn::PixelFormat::BC6HUFloat: return vk::Format::eBc6HUfloatBlock;
		case rn::PixelFormat::BC6HSFloat: return vk::Format::eBc6HSfloatBlock;
		case rn::PixelFormat::BC7UNorm: return vk::Format::eBc7UnormBlock;
		case rn::PixelFormat::BC7SRGB: return vk::Format::eBc7SrgbBlock;
		case rn::PixelFormat::B4G4R4A4UNormPack16: return vk::Format::eB4G4R4A4UnormPack16;
		case rn::PixelFormat::B5G6R5UNormPack16: return vk::Format::eB5G6R5UnormPack16;
		case rn::PixelFormat::B5G5R5A1UNormPack16: return vk::Format::eB5G5R5A1UnormPack16;
		case rn::PixelFormat::R10G10B10A2UNorm: return vk::Format::eA2B10G10R10UintPack32;
		case rn::PixelFormat::R10G10B10A2UInt: return vk::Format::eA2B10G10R10UnormPack32;
		case rn::PixelFormat::R11G11B10Float: return vk::Format::eB10G11R11UfloatPack32;
	}
}

vk::Format fromCommon(rn::DataFormat dataFormat) {
	switch (dataFormat) {
		default:
		case rn::DataFormat::Undefined: return vk::Format::eUndefined;
		case rn::DataFormat::UChar1: return vk::Format::eR8Uint;
		case rn::DataFormat::UChar2: return vk::Format::eR8G8Uint;
		// case rn::DataFormat::UChar3: return vk::Format::eR8G8B8Uint;
		case rn::DataFormat::UChar4: return vk::Format::eR8G8B8A8Uint;
		case rn::DataFormat::Char1: return vk::Format::eR8Sint;
		case rn::DataFormat::Char2: return vk::Format::eR8G8Sint;
		// case rn::DataFormat::Char3: return vk::Format::eR8G8B8Sint;
		case rn::DataFormat::Char4: return vk::Format::eR8G8B8A8Sint;
		case rn::DataFormat::UChar1Norm: return vk::Format::eR8Unorm;
		case rn::DataFormat::UChar2Norm: return vk::Format::eR8G8Unorm;
		// case rn::DataFormat::UChar3Norm: return vk::Format::eR8G8B8Unorm;
		case rn::DataFormat::UChar4Norm: return vk::Format::eR8G8B8A8Unorm;
		case rn::DataFormat::Char1Norm: return vk::Format::eR8Snorm;
		case rn::DataFormat::Char2Norm: return vk::Format::eR8G8Snorm;
		// case rn::DataFormat::Char3Norm: return vk::Format::eR8G8B8Snorm;
		case rn::DataFormat::Char4Norm: return vk::Format::eR8G8B8A8Snorm;
		case rn::DataFormat::UShort1: return vk::Format::eR16Uint;
		case rn::DataFormat::UShort2: return vk::Format::eR16G16Uint;
		// case rn::DataFormat::UShort3: return vk::Format::eR16G16B16Uint;
		case rn::DataFormat::UShort4: return vk::Format::eR16G16B16A16Uint;
		case rn::DataFormat::Short1: return vk::Format::eR16Sint;
		case rn::DataFormat::Short2: return vk::Format::eR16G16Sint;
		// case rn::DataFormat::Short3: return vk::Format::eR16G16B16Sint;
		case rn::DataFormat::Short4: return vk::Format::eR16G16B16A16Sint;
		case rn::DataFormat::UShort1Norm: return vk::Format::eR16Unorm;
		case rn::DataFormat::UShort2Norm: return vk::Format::eR16G16Unorm;
		// case rn::DataFormat::UShort3Norm: return vk::Format::eR16G16B16Unorm;
		case rn::DataFormat::UShort4Norm: return vk::Format::eR16G16B16A16Unorm;
		case rn::DataFormat::Short1Norm: return vk::Format::eR16Snorm;
		case rn::DataFormat::Short2Norm: return vk::Format::eR16G16Snorm;
		// case rn::DataFormat::Short3Norm: return vk::Format::eR16G16B16Snorm;
		case rn::DataFormat::Short4Norm: return vk::Format::eR16G16B16A16Snorm;
		case rn::DataFormat::Half1: return vk::Format::eR16Sfloat;
		case rn::DataFormat::Half2: return vk::Format::eR16G16Sfloat;
		// case rn::DataFormat::Half3: return vk::Format::eR16G16B16Sfloat;
		case rn::DataFormat::Half4: return vk::Format::eR16G16B16A16Sfloat;
		case rn::DataFormat::Float1: return vk::Format::eR32Sfloat;
		case rn::DataFormat::Float2: return vk::Format::eR32G32Sfloat;
		case rn::DataFormat::Float3: return vk::Format::eR32G32B32Sfloat;
		case rn::DataFormat::Float4: return vk::Format::eR32G32B32A32Sfloat;
		case rn::DataFormat::UInt1: return vk::Format::eR32Uint;
		case rn::DataFormat::UInt2: return vk::Format::eR32G32Uint;
		case rn::DataFormat::UInt3: return vk::Format::eR32G32B32Uint;
		case rn::DataFormat::UInt4: return vk::Format::eR32G32B32A32Uint;
		case rn::DataFormat::Int1: return vk::Format::eR32Sint;
		case rn::DataFormat::Int2: return vk::Format::eR32G32Sint;
		case rn::DataFormat::Int3: return vk::Format::eR32G32B32Sint;
		case rn::DataFormat::Int4: return vk::Format::eR32G32B32A32Sint;
		// case rn::DataFormat::R32G32B32A32UInt: return vk::Format::eR32G32B32A32Uint;
		// case rn::DataFormat::R32G32B32A32SInt: return vk::Format::eR32G32B32A32Sint;
		// case rn::DataFormat::R32G32B32A32Float: return vk::Format::eR32G32B32A32Sfloat;
		// case rn::DataFormat::R32G32B32UInt: return vk::Format::eR32G32B32Uint;
		// case rn::DataFormat::R32G32B32SInt: return vk::Format::eR32G32B32Sint;
		// case rn::DataFormat::R32G32B32Float: return vk::Format::eR32G32B32Sfloat;
		// case rn::DataFormat::R16G16B16A16UNorm: return vk::Format::eR16G16B16A16Unorm;
		// case rn::DataFormat::R16G16B16A16SNorm: return vk::Format::eR16G16B16A16Snorm;
		// case rn::DataFormat::R16G16B16A16UInt: return vk::Format::eR16G16B16A16Uint;
		// case rn::DataFormat::R16G16B16A16SInt: return vk::Format::eR16G16B16A16Sint;
		// case rn::DataFormat::R16G16B16A16Float: return vk::Format::eR16G16B16A16Sfloat;
		// case rn::DataFormat::R32G32SInt: return vk::Format::eR32G32Sint;
		// case rn::DataFormat::R32G32UInt: return vk::Format::eR32G32Uint;
		// case rn::DataFormat::R32G32Float: return vk::Format::eR32G32Sfloat;
		// case rn::DataFormat::R8G8B8A8UNorm: return vk::Format::eR8G8B8A8Unorm;
		// case rn::DataFormat::R8G8B8A8SNorm: return vk::Format::eR8G8B8A8Snorm;
		// case rn::DataFormat::R8G8B8A8UInt: return vk::Format::eR8G8B8A8Uint;
		// case rn::DataFormat::R8G8B8A8SInt: return vk::Format::eR8G8B8A8Sint;
		// case rn::DataFormat::R16G16UNorm: return vk::Format::eR16G16Unorm;
		// case rn::DataFormat::R16G16SNorm: return vk::Format::eR16G16Snorm;
		// case rn::DataFormat::R16G16UInt: return vk::Format::eR16G16Uint;
		// case rn::DataFormat::R16G16SInt: return vk::Format::eR16G16Sint;
		// case rn::DataFormat::R16G16Float: return vk::Format::eR16G16Sfloat;
		// case rn::DataFormat::R32UInt: return vk::Format::eR32Uint;
		// case rn::DataFormat::R32SInt: return vk::Format::eR32Sint;
		// case rn::DataFormat::R32Float: return vk::Format::eR32Sfloat;
		// case rn::DataFormat::R8G8UNorm: return vk::Format::eR8G8Unorm;
		// case rn::DataFormat::R8G8SNorm: return vk::Format::eR8G8Snorm;
		// case rn::DataFormat::R8G8UInt: return vk::Format::eR8G8Uint;
		// case rn::DataFormat::R8G8SInt: return vk::Format::eR8G8Sint;
		// case rn::DataFormat::R16UNorm: return vk::Format::eR16Unorm;
		// case rn::DataFormat::R16SNorm: return vk::Format::eR16Snorm;
		// case rn::DataFormat::R16UInt: return vk::Format::eR16Uint;
		// case rn::DataFormat::R16SInt: return vk::Format::eR16Sint;
		// case rn::DataFormat::R16Float: return vk::Format::eR16Sfloat;
		// case rn::DataFormat::R8UNorm: return vk::Format::eR8Unorm;
		// case rn::DataFormat::R8SNorm: return vk::Format::eR8Snorm;
		// case rn::DataFormat::R8UInt: return vk::Format::eR8Uint;
		// case rn::DataFormat::R8SInt: return vk::Format::eR8Sint;
	}
}

vk::ImageType fromCommon(rn::ImageType imageType) {
	switch (imageType) {
		default:
		case rn::ImageType::Tex1D: return vk::ImageType::e1D;
		case rn::ImageType::Tex2D: return vk::ImageType::e2D;
		case rn::ImageType::Tex3D: return vk::ImageType::e3D;
	}
}

vk::ImageViewType fromCommon(rn::ImageViewType imageViewType) {
	switch (imageViewType) {
		default:
		case rn::ImageViewType::Tex1D: return vk::ImageViewType::e1D;
		case rn::ImageViewType::Tex2D: return vk::ImageViewType::e2D;
		case rn::ImageViewType::Tex3D: return vk::ImageViewType::e3D;
		case rn::ImageViewType::Cube: return vk::ImageViewType::eCube;
		case rn::ImageViewType::Array1D: return vk::ImageViewType::e1DArray;
		case rn::ImageViewType::Array2D: return vk::ImageViewType::e2DArray;
		case rn::ImageViewType::ArrayCube: return vk::ImageViewType::eCubeArray;
	}
}

vk::PresentModeKHR fromCommon(rn::VSync vsync) {
	switch (vsync) {
		default:
		case rn::VSync::Fifo: return vk::PresentModeKHR::eFifo;
		case rn::VSync::FifoRelaxed: return vk::PresentModeKHR::eFifoRelaxed;
		case rn::VSync::Immediate: return vk::PresentModeKHR::eImmediate;
		case rn::VSync::Mailbox: return vk::PresentModeKHR::eMailbox;
	}
}

vk::ColorSpaceKHR fromCommon(rn::ColorSpace colorSpace) {
	switch (colorSpace) {
		default:
		case rn::ColorSpace::LDR: return vk::ColorSpaceKHR::eSrgbNonlinear;
	}
}

template<>
rn::PixelFormat toCommon<rn::PixelFormat>(vk::Format pixelFormat) {
	switch (pixelFormat) {
		default:
		case vk::Format::eUndefined: return rn::PixelFormat::Undefined;
		case vk::Format::eR8Unorm: return rn::PixelFormat::R8UNorm;
		case vk::Format::eR8Snorm: return rn::PixelFormat::R8SNorm;
		case vk::Format::eR8Uint: return rn::PixelFormat::R8UInt;
		case vk::Format::eR8Sint: return rn::PixelFormat::R8SInt;
		case vk::Format::eR8G8Unorm: return rn::PixelFormat::R8G8UNorm;
		case vk::Format::eR8G8Snorm: return rn::PixelFormat::R8G8SNorm;
		case vk::Format::eR8G8Uint: return rn::PixelFormat::R8G8UInt;
		case vk::Format::eR8G8Sint: return rn::PixelFormat::R8G8SInt;
		case vk::Format::eR8G8B8A8Unorm: return rn::PixelFormat::R8G8B8A8UNorm;
		case vk::Format::eR8G8B8A8Snorm: return rn::PixelFormat::R8G8B8A8SNorm;
		case vk::Format::eR8G8B8A8Uint: return rn::PixelFormat::R8G8B8A8UInt;
		case vk::Format::eR8G8B8A8Sint: return rn::PixelFormat::R8G8B8A8SInt;
		case vk::Format::eB8G8R8A8Unorm: return rn::PixelFormat::B8G8R8A8UNorm;
		case vk::Format::eR16Unorm: return rn::PixelFormat::R16UNorm;
		case vk::Format::eR16Snorm: return rn::PixelFormat::R16SNorm;
		case vk::Format::eR16Uint: return rn::PixelFormat::R16UInt;
		case vk::Format::eR16Sint: return rn::PixelFormat::R16SInt;
		case vk::Format::eR16Sfloat: return rn::PixelFormat::R16Float;
		case vk::Format::eR16G16Unorm: return rn::PixelFormat::R16G16UNorm;
		case vk::Format::eR16G16Snorm: return rn::PixelFormat::R16G16SNorm;
		case vk::Format::eR16G16Uint: return rn::PixelFormat::R16G16UInt;
		case vk::Format::eR16G16Sint: return rn::PixelFormat::R16G16SInt;
		case vk::Format::eR16G16Sfloat: return rn::PixelFormat::R16G16Float;
		case vk::Format::eR16G16B16A16Unorm: return rn::PixelFormat::R16G16B16A16UNorm;
		case vk::Format::eR16G16B16A16Snorm: return rn::PixelFormat::R16G16B16A16SNorm;
		case vk::Format::eR16G16B16A16Uint: return rn::PixelFormat::R16G16B16A16UInt;
		case vk::Format::eR16G16B16A16Sint: return rn::PixelFormat::R16G16B16A16SInt;
		case vk::Format::eR16G16B16A16Sfloat: return rn::PixelFormat::R16G16B16A16Float;
		case vk::Format::eR32Uint: return rn::PixelFormat::R32UInt;
		case vk::Format::eR32Sint: return rn::PixelFormat::R32SInt;
		case vk::Format::eR32Sfloat: return rn::PixelFormat::R32Float;
		case vk::Format::eR32G32Uint: return rn::PixelFormat::R32G32UInt;
		case vk::Format::eR32G32Sint: return rn::PixelFormat::R32G32SInt;
		case vk::Format::eR32G32Sfloat: return rn::PixelFormat::R32G32Float;
		case vk::Format::eR32G32B32Uint: return rn::PixelFormat::R32G32B32UInt;
		case vk::Format::eR32G32B32Sint: return rn::PixelFormat::R32G32B32SInt;
		case vk::Format::eR32G32B32Sfloat: return rn::PixelFormat::R32G32B32Float;
		case vk::Format::eR32G32B32A32Uint: return rn::PixelFormat::R32G32B32A32UInt;
		case vk::Format::eR32G32B32A32Sint: return rn::PixelFormat::R32G32B32A32SInt;
		case vk::Format::eR32G32B32A32Sfloat: return rn::PixelFormat::R32G32B32A32Float;
		case vk::Format::eD16Unorm: return rn::PixelFormat::D16UNorm;
		case vk::Format::eD32Sfloat: return rn::PixelFormat::D32Float;
		case vk::Format::eD24UnormS8Uint: return rn::PixelFormat::D24UNormS8UInt;
		case vk::Format::eD32SfloatS8Uint: return rn::PixelFormat::D32FloatS8UInt;
		case vk::Format::eBc1RgbaUnormBlock: return rn::PixelFormat::BC1UNorm;
		case vk::Format::eBc1RgbaSrgbBlock: return rn::PixelFormat::BC1SRGB;
		case vk::Format::eBc2UnormBlock: return rn::PixelFormat::BC2UNorm;
		case vk::Format::eBc2SrgbBlock: return rn::PixelFormat::BC2SRGB;
		case vk::Format::eBc3UnormBlock: return rn::PixelFormat::BC3UNorm;
		case vk::Format::eBc3SrgbBlock: return rn::PixelFormat::BC3SRGB;
		case vk::Format::eBc4UnormBlock: return rn::PixelFormat::BC4UNorm;
		case vk::Format::eBc4SnormBlock: return rn::PixelFormat::BC4SNorm;
		case vk::Format::eBc5UnormBlock: return rn::PixelFormat::BC5UNorm;
		case vk::Format::eBc5SnormBlock: return rn::PixelFormat::BC5SNorm;
		case vk::Format::eBc6HUfloatBlock: return rn::PixelFormat::BC6HUFloat;
		case vk::Format::eBc6HSfloatBlock: return rn::PixelFormat::BC6HSFloat;
		case vk::Format::eBc7UnormBlock: return rn::PixelFormat::BC7UNorm;
		case vk::Format::eBc7SrgbBlock: return rn::PixelFormat::BC7SRGB;
		case vk::Format::eB4G4R4A4UnormPack16: return rn::PixelFormat::B4G4R4A4UNormPack16;
		case vk::Format::eB5G6R5UnormPack16: return rn::PixelFormat::B5G6R5UNormPack16;
		case vk::Format::eB5G5R5A1UnormPack16: return rn::PixelFormat::B5G5R5A1UNormPack16;
		case vk::Format::eA2B10G10R10UintPack32: return rn::PixelFormat::R10G10B10A2UNorm;
		case vk::Format::eA2B10G10R10UnormPack32: return rn::PixelFormat::R10G10B10A2UInt;
		case vk::Format::eB10G11R11UfloatPack32: return rn::PixelFormat::R11G11B10Float;
	}
}

template<>
rn::DataFormat toCommon<rn::DataFormat>(vk::Format dataFormat) {
	switch (dataFormat) {
		default:
		case vk::Format::eUndefined: return rn::DataFormat::Undefined;
		case vk::Format::eR8Uint: return rn::DataFormat::UChar1;
		case vk::Format::eR8G8Uint: return rn::DataFormat::UChar2;
		// case vk::Format::eR8G8B8Uint: return rn::DataFormat::UChar3;
		case vk::Format::eR8G8B8A8Uint: return rn::DataFormat::UChar4;
		case vk::Format::eR8Sint: return rn::DataFormat::Char1;
		case vk::Format::eR8G8Sint: return rn::DataFormat::Char2;
		// case vk::Format::eR8G8B8Sint: return rn::DataFormat::Char3;
		case vk::Format::eR8G8B8A8Sint: return rn::DataFormat::Char4;
		case vk::Format::eR8Unorm: return rn::DataFormat::UChar1Norm;
		case vk::Format::eR8G8Unorm: return rn::DataFormat::UChar2Norm;
		// case vk::Format::eR8G8B8Unorm: return rn::DataFormat::UChar3Norm;
		case vk::Format::eR8G8B8A8Unorm: return rn::DataFormat::UChar4Norm;
		case vk::Format::eR8Snorm: return rn::DataFormat::Char1Norm;
		case vk::Format::eR8G8Snorm: return rn::DataFormat::Char2Norm;
		// case vk::Format::eR8G8B8Snorm: return rn::DataFormat::Char3Norm;
		case vk::Format::eR8G8B8A8Snorm: return rn::DataFormat::Char4Norm;
		case vk::Format::eR16Uint: return rn::DataFormat::UShort1;
		case vk::Format::eR16G16Uint: return rn::DataFormat::UShort2;
		// case vk::Format::eR16G16B16Uint: return rn::DataFormat::UShort3;
		case vk::Format::eR16G16B16A16Uint: return rn::DataFormat::UShort4;
		case vk::Format::eR16Sint: return rn::DataFormat::Short1;
		case vk::Format::eR16G16Sint: return rn::DataFormat::Short2;
		// case vk::Format::eR16G16B16Sint: return rn::DataFormat::Short3;
		case vk::Format::eR16G16B16A16Sint: return rn::DataFormat::Short4;
		case vk::Format::eR16Unorm: return rn::DataFormat::UShort1Norm;
		case vk::Format::eR16G16Unorm: return rn::DataFormat::UShort2Norm;
		// case vk::Format::eR16G16B16Unorm: return rn::DataFormat::UShort3Norm;
		case vk::Format::eR16G16B16A16Unorm: return rn::DataFormat::UShort4Norm;
		case vk::Format::eR16Snorm: return rn::DataFormat::Short1Norm;
		case vk::Format::eR16G16Snorm: return rn::DataFormat::Short2Norm;
		// case vk::Format::eR16G16B16Snorm: return rn::DataFormat::Short3Norm;
		case vk::Format::eR16G16B16A16Snorm: return rn::DataFormat::Short4Norm;
		case vk::Format::eR16Sfloat: return rn::DataFormat::Half1;
		case vk::Format::eR16G16Sfloat: return rn::DataFormat::Half2;
		// case vk::Format::eR16G16B16Sfloat: return rn::DataFormat::Half3;
		case vk::Format::eR16G16B16A16Sfloat: return rn::DataFormat::Half4;
		case vk::Format::eR32Sfloat: return rn::DataFormat::Float1;
		case vk::Format::eR32G32Sfloat: return rn::DataFormat::Float2;
		case vk::Format::eR32G32B32Sfloat: return rn::DataFormat::Float3;
		case vk::Format::eR32G32B32A32Sfloat: return rn::DataFormat::Float4;
		case vk::Format::eR32Uint: return rn::DataFormat::UInt1;
		case vk::Format::eR32G32Uint: return rn::DataFormat::UInt2;
		case vk::Format::eR32G32B32Uint: return rn::DataFormat::UInt3;
		case vk::Format::eR32G32B32A32Uint: return rn::DataFormat::UInt4;
		case vk::Format::eR32Sint: return rn::DataFormat::Int1;
		case vk::Format::eR32G32Sint: return rn::DataFormat::Int2;
		case vk::Format::eR32G32B32Sint: return rn::DataFormat::Int3;
		case vk::Format::eR32G32B32A32Sint: return rn::DataFormat::Int4;
		// case vk::Format::eUndefined: return rn::DataFormat::Undefined;
		// case vk::Format::eR32G32B32A32Uint: return rn::DataFormat::R32G32B32A32UInt;
		// case vk::Format::eR32G32B32A32Sint: return rn::DataFormat::R32G32B32A32SInt;
		// case vk::Format::eR32G32B32A32Sfloat: return rn::DataFormat::R32G32B32A32Float;
		// case vk::Format::eR32G32B32Uint: return rn::DataFormat::R32G32B32UInt;
		// case vk::Format::eR32G32B32Sint: return rn::DataFormat::R32G32B32SInt;
		// case vk::Format::eR32G32B32Sfloat: return rn::DataFormat::R32G32B32Float;
		// case vk::Format::eR16G16B16A16Unorm: return rn::DataFormat::R16G16B16A16UNorm;
		// case vk::Format::eR16G16B16A16Snorm: return rn::DataFormat::R16G16B16A16SNorm;
		// case vk::Format::eR16G16B16A16Uint: return rn::DataFormat::R16G16B16A16UInt;
		// case vk::Format::eR16G16B16A16Sint: return rn::DataFormat::R16G16B16A16SInt;
		// case vk::Format::eR16G16B16A16Sfloat: return rn::DataFormat::R16G16B16A16Float;
		// case vk::Format::eR32G32Sint: return rn::DataFormat::R32G32SInt;
		// case vk::Format::eR32G32Uint: return rn::DataFormat::R32G32UInt;
		// case vk::Format::eR32G32Sfloat: return rn::DataFormat::R32G32Float;
		// case vk::Format::eR8G8B8A8Unorm: return rn::DataFormat::R8G8B8A8UNorm;
		// case vk::Format::eR8G8B8A8Snorm: return rn::DataFormat::R8G8B8A8SNorm;
		// case vk::Format::eR8G8B8A8Uint: return rn::DataFormat::R8G8B8A8UInt;
		// case vk::Format::eR8G8B8A8Sint: return rn::DataFormat::R8G8B8A8SInt;
		// case vk::Format::eR16G16Unorm: return rn::DataFormat::R16G16UNorm;
		// case vk::Format::eR16G16Snorm: return rn::DataFormat::R16G16SNorm;
		// case vk::Format::eR16G16Uint: return rn::DataFormat::R16G16UInt;
		// case vk::Format::eR16G16Sint: return rn::DataFormat::R16G16SInt;
		// case vk::Format::eR16G16Sfloat: return rn::DataFormat::R16G16Float;
		// case vk::Format::eR32Uint: return rn::DataFormat::R32UInt;
		// case vk::Format::eR32Sint: return rn::DataFormat::R32SInt;
		// case vk::Format::eR32Sfloat: return rn::DataFormat::R32Float;
		// case vk::Format::eR8G8Unorm: return rn::DataFormat::R8G8UNorm;
		// case vk::Format::eR8G8Snorm: return rn::DataFormat::R8G8SNorm;
		// case vk::Format::eR8G8Uint: return rn::DataFormat::R8G8UInt;
		// case vk::Format::eR8G8Sint: return rn::DataFormat::R8G8SInt;
		// case vk::Format::eR16Unorm: return rn::DataFormat::R16UNorm;
		// case vk::Format::eR16Snorm: return rn::DataFormat::R16SNorm;
		// case vk::Format::eR16Uint: return rn::DataFormat::R16UInt;
		// case vk::Format::eR16Sint: return rn::DataFormat::R16SInt;
		// case vk::Format::eR16Sfloat: return rn::DataFormat::R16Float;
		// case vk::Format::eR8Unorm: return rn::DataFormat::R8UNorm;
		// case vk::Format::eR8Snorm: return rn::DataFormat::R8SNorm;
		// case vk::Format::eR8Uint: return rn::DataFormat::R8UInt;
		// case vk::Format::eR8Sint: return rn::DataFormat::R8SInt;
	}
}

template<>
rn::ImageType toCommon<rn::ImageType>(vk::ImageType imageType) {
	switch (imageType) {
		default:
		case vk::ImageType::e1D: return rn::ImageType::Tex1D;
		case vk::ImageType::e2D: return rn::ImageType::Tex2D;
		case vk::ImageType::e3D: return rn::ImageType::Tex3D;
	}
}

template<>
rn::ImageViewType toCommon<rn::ImageViewType>(vk::ImageViewType imageViewType) {
	switch (imageViewType) {
		default:
		case vk::ImageViewType::e1D: return rn::ImageViewType::Tex1D;
		case vk::ImageViewType::e2D: return rn::ImageViewType::Tex2D;
		case vk::ImageViewType::e3D: return rn::ImageViewType::Tex3D;
		case vk::ImageViewType::eCube: return rn::ImageViewType::Cube;
		case vk::ImageViewType::e1DArray: return rn::ImageViewType::Array1D;
		case vk::ImageViewType::e2DArray: return rn::ImageViewType::Array2D;
		case vk::ImageViewType::eCubeArray: return rn::ImageViewType::ArrayCube;
	}
}

template<>
rn::VSync toCommon<rn::VSync>(vk::PresentModeKHR vsync) {
	switch (vsync) {
		default:
		case vk::PresentModeKHR::eFifo: return rn::VSync::Fifo;
		case vk::PresentModeKHR::eFifoRelaxed: return rn::VSync::FifoRelaxed;
		case vk::PresentModeKHR::eImmediate: return rn::VSync::Immediate;
		case vk::PresentModeKHR::eMailbox: return rn::VSync::Mailbox;
	}
}

template<>
rn::ColorSpace toCommon<rn::ColorSpace>(vk::ColorSpaceKHR colorSpace) {
	switch (colorSpace) {
		default: return rn::ColorSpace::Undefined;
		case vk::ColorSpaceKHR::eSrgbNonlinear: return rn::ColorSpace::LDR;
		case vk::ColorSpaceKHR::eDisplayP3NonlinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eDciP3LinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eDciP3NonlinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eBt709LinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eBt709NonlinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eBt2020LinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eHdr10St2084EXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eDolbyvisionEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eHdr10HlgEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eAdobergbLinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eAdobergbNonlinearEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::ePassThroughEXT: [[fallthrough]];
		case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT: return rn::ColorSpace::HDR;
	}
}

} // rn::vki
