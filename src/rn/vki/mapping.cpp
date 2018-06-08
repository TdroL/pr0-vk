#include "mapping.hpp"

namespace rn::vki {

vk::Format fromCore(rn::PixelFormat pixelFormat) {
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

vk::Format fromCore(rn::VertexFormat vertexFormat) {
	switch (vertexFormat) {
		default:
		case rn::VertexFormat::Undefined: return vk::Format::eUndefined;
		case rn::VertexFormat::R32G32B32A32UInt: return vk::Format::eR32G32B32A32Uint;
		case rn::VertexFormat::R32G32B32A32SInt: return vk::Format::eR32G32B32A32Sint;
		case rn::VertexFormat::R32G32B32A32Float: return vk::Format::eR32G32B32A32Sfloat;
		case rn::VertexFormat::R32G32B32UInt: return vk::Format::eR32G32B32Uint;
		case rn::VertexFormat::R32G32B32SInt: return vk::Format::eR32G32B32Sint;
		case rn::VertexFormat::R32G32B32Float: return vk::Format::eR32G32B32Sfloat;
		case rn::VertexFormat::R16G16B16A16UNorm: return vk::Format::eR16G16B16A16Unorm;
		case rn::VertexFormat::R16G16B16A16SNorm: return vk::Format::eR16G16B16A16Snorm;
		case rn::VertexFormat::R16G16B16A16UInt: return vk::Format::eR16G16B16A16Uint;
		case rn::VertexFormat::R16G16B16A16SInt: return vk::Format::eR16G16B16A16Sint;
		case rn::VertexFormat::R16G16B16A16Float: return vk::Format::eR16G16B16A16Sfloat;
		case rn::VertexFormat::R32G32SInt: return vk::Format::eR32G32Sint;
		case rn::VertexFormat::R32G32UInt: return vk::Format::eR32G32Uint;
		case rn::VertexFormat::R32G32Float: return vk::Format::eR32G32Sfloat;
		case rn::VertexFormat::R8G8B8A8UNorm: return vk::Format::eR8G8B8A8Unorm;
		case rn::VertexFormat::R8G8B8A8SNorm: return vk::Format::eR8G8B8A8Snorm;
		case rn::VertexFormat::R8G8B8A8UInt: return vk::Format::eR8G8B8A8Uint;
		case rn::VertexFormat::R8G8B8A8SInt: return vk::Format::eR8G8B8A8Sint;
		case rn::VertexFormat::R16G16UNorm: return vk::Format::eR16G16Unorm;
		case rn::VertexFormat::R16G16SNorm: return vk::Format::eR16G16Snorm;
		case rn::VertexFormat::R16G16UInt: return vk::Format::eR16G16Uint;
		case rn::VertexFormat::R16G16SInt: return vk::Format::eR16G16Sint;
		case rn::VertexFormat::R16G16Float: return vk::Format::eR16G16Sfloat;
		case rn::VertexFormat::R32UInt: return vk::Format::eR32Uint;
		case rn::VertexFormat::R32SInt: return vk::Format::eR32Sint;
		case rn::VertexFormat::R32Float: return vk::Format::eR32Sfloat;
		case rn::VertexFormat::R8G8UNorm: return vk::Format::eR8G8Unorm;
		case rn::VertexFormat::R8G8SNorm: return vk::Format::eR8G8Snorm;
		case rn::VertexFormat::R8G8UInt: return vk::Format::eR8G8Uint;
		case rn::VertexFormat::R8G8SInt: return vk::Format::eR8G8Sint;
		case rn::VertexFormat::R16UNorm: return vk::Format::eR16Unorm;
		case rn::VertexFormat::R16SNorm: return vk::Format::eR16Snorm;
		case rn::VertexFormat::R16UInt: return vk::Format::eR16Uint;
		case rn::VertexFormat::R16SInt: return vk::Format::eR16Sint;
		case rn::VertexFormat::R16Float: return vk::Format::eR16Sfloat;
		case rn::VertexFormat::R8UNorm: return vk::Format::eR8Unorm;
		case rn::VertexFormat::R8SNorm: return vk::Format::eR8Snorm;
		case rn::VertexFormat::R8UInt: return vk::Format::eR8Uint;
		case rn::VertexFormat::R8SInt: return vk::Format::eR8Sint;
	}
}

vk::ImageType fromCore(rn::ImageType imageType) {
	switch (imageType) {
		default:
		case rn::ImageType::Image1D: return vk::ImageType::e1D;
		case rn::ImageType::Image2D: return vk::ImageType::e2D;
		case rn::ImageType::Image3D: return vk::ImageType::e3D;
	}
}

vk::ImageViewType fromCore(rn::ImageViewType imageViewType) {
	switch (imageViewType) {
		default:
		case rn::ImageViewType::View1D: return vk::ImageViewType::e1D;
		case rn::ImageViewType::View2D: return vk::ImageViewType::e2D;
		case rn::ImageViewType::View3D: return vk::ImageViewType::e3D;
		case rn::ImageViewType::ViewCube: return vk::ImageViewType::eCube;
		case rn::ImageViewType::ViewArray1D: return vk::ImageViewType::e1DArray;
		case rn::ImageViewType::ViewArray2D: return vk::ImageViewType::e2DArray;
		case rn::ImageViewType::ViewArrayCube: return vk::ImageViewType::eCubeArray;
	}
}

vk::PresentModeKHR fromCore(rn::VSync vsync) {
	switch (vsync) {
		default:
		case rn::VSync::Fifo: return vk::PresentModeKHR::eFifo;
		case rn::VSync::FifoRelaxed: return vk::PresentModeKHR::eFifoRelaxed;
		case rn::VSync::Immediate: return vk::PresentModeKHR::eImmediate;
		case rn::VSync::Mailbox: return vk::PresentModeKHR::eMailbox;
	}
}

vk::ColorSpaceKHR fromCore(rn::ColorSpace colorSpace) {
	switch (colorSpace) {
		default:
		case rn::ColorSpace::LDR: return vk::ColorSpaceKHR::eSrgbNonlinear;
	}
}

template<>
rn::PixelFormat toCore<rn::PixelFormat>(vk::Format pixelFormat) {
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
rn::VertexFormat toCore<rn::VertexFormat>(vk::Format vertexFormat) {
	switch (vertexFormat) {
		default:
		case vk::Format::eUndefined: return rn::VertexFormat::Undefined;
		case vk::Format::eR32G32B32A32Uint: return rn::VertexFormat::R32G32B32A32UInt;
		case vk::Format::eR32G32B32A32Sint: return rn::VertexFormat::R32G32B32A32SInt;
		case vk::Format::eR32G32B32A32Sfloat: return rn::VertexFormat::R32G32B32A32Float;
		case vk::Format::eR32G32B32Uint: return rn::VertexFormat::R32G32B32UInt;
		case vk::Format::eR32G32B32Sint: return rn::VertexFormat::R32G32B32SInt;
		case vk::Format::eR32G32B32Sfloat: return rn::VertexFormat::R32G32B32Float;
		case vk::Format::eR16G16B16A16Unorm: return rn::VertexFormat::R16G16B16A16UNorm;
		case vk::Format::eR16G16B16A16Snorm: return rn::VertexFormat::R16G16B16A16SNorm;
		case vk::Format::eR16G16B16A16Uint: return rn::VertexFormat::R16G16B16A16UInt;
		case vk::Format::eR16G16B16A16Sint: return rn::VertexFormat::R16G16B16A16SInt;
		case vk::Format::eR16G16B16A16Sfloat: return rn::VertexFormat::R16G16B16A16Float;
		case vk::Format::eR32G32Sint: return rn::VertexFormat::R32G32SInt;
		case vk::Format::eR32G32Uint: return rn::VertexFormat::R32G32UInt;
		case vk::Format::eR32G32Sfloat: return rn::VertexFormat::R32G32Float;
		case vk::Format::eR8G8B8A8Unorm: return rn::VertexFormat::R8G8B8A8UNorm;
		case vk::Format::eR8G8B8A8Snorm: return rn::VertexFormat::R8G8B8A8SNorm;
		case vk::Format::eR8G8B8A8Uint: return rn::VertexFormat::R8G8B8A8UInt;
		case vk::Format::eR8G8B8A8Sint: return rn::VertexFormat::R8G8B8A8SInt;
		case vk::Format::eR16G16Unorm: return rn::VertexFormat::R16G16UNorm;
		case vk::Format::eR16G16Snorm: return rn::VertexFormat::R16G16SNorm;
		case vk::Format::eR16G16Uint: return rn::VertexFormat::R16G16UInt;
		case vk::Format::eR16G16Sint: return rn::VertexFormat::R16G16SInt;
		case vk::Format::eR16G16Sfloat: return rn::VertexFormat::R16G16Float;
		case vk::Format::eR32Uint: return rn::VertexFormat::R32UInt;
		case vk::Format::eR32Sint: return rn::VertexFormat::R32SInt;
		case vk::Format::eR32Sfloat: return rn::VertexFormat::R32Float;
		case vk::Format::eR8G8Unorm: return rn::VertexFormat::R8G8UNorm;
		case vk::Format::eR8G8Snorm: return rn::VertexFormat::R8G8SNorm;
		case vk::Format::eR8G8Uint: return rn::VertexFormat::R8G8UInt;
		case vk::Format::eR8G8Sint: return rn::VertexFormat::R8G8SInt;
		case vk::Format::eR16Unorm: return rn::VertexFormat::R16UNorm;
		case vk::Format::eR16Snorm: return rn::VertexFormat::R16SNorm;
		case vk::Format::eR16Uint: return rn::VertexFormat::R16UInt;
		case vk::Format::eR16Sint: return rn::VertexFormat::R16SInt;
		case vk::Format::eR16Sfloat: return rn::VertexFormat::R16Float;
		case vk::Format::eR8Unorm: return rn::VertexFormat::R8UNorm;
		case vk::Format::eR8Snorm: return rn::VertexFormat::R8SNorm;
		case vk::Format::eR8Uint: return rn::VertexFormat::R8UInt;
		case vk::Format::eR8Sint: return rn::VertexFormat::R8SInt;
	}
}

template<>
rn::ImageType toCore<rn::ImageType>(vk::ImageType imageType) {
	switch (imageType) {
		default:
		case vk::ImageType::e1D: return rn::ImageType::Image1D;
		case vk::ImageType::e2D: return rn::ImageType::Image2D;
		case vk::ImageType::e3D: return rn::ImageType::Image3D;
	}
}

template<>
rn::ImageViewType toCore<rn::ImageViewType>(vk::ImageViewType imageViewType) {
	switch (imageViewType) {
		default:
		case vk::ImageViewType::e1D: return rn::ImageViewType::View1D;
		case vk::ImageViewType::e2D: return rn::ImageViewType::View2D;
		case vk::ImageViewType::e3D: return rn::ImageViewType::View3D;
		case vk::ImageViewType::eCube: return rn::ImageViewType::ViewCube;
		case vk::ImageViewType::e1DArray: return rn::ImageViewType::ViewArray1D;
		case vk::ImageViewType::e2DArray: return rn::ImageViewType::ViewArray2D;
		case vk::ImageViewType::eCubeArray: return rn::ImageViewType::ViewArrayCube;
	}
}

template<>
rn::VSync toCore<rn::VSync>(vk::PresentModeKHR vsync) {
	switch (vsync) {
		default:
		case vk::PresentModeKHR::eFifo: return rn::VSync::Fifo;
		case vk::PresentModeKHR::eFifoRelaxed: return rn::VSync::FifoRelaxed;
		case vk::PresentModeKHR::eImmediate: return rn::VSync::Immediate;
		case vk::PresentModeKHR::eMailbox: return rn::VSync::Mailbox;
	}
}

template<>
rn::ColorSpace toCore<rn::ColorSpace>(vk::ColorSpaceKHR colorSpace) {
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
