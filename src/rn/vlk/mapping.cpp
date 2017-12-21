#include "mapping.hpp"

namespace rn::vlk {

vk::Format fromCore(rn::ImageFormat imageFormat) {
	return fromConfig(imageFormat);
}

vk::ImageType fromCore(rn::ImageType imageType) {
	switch (imageType) {
		default:
		case rn::ImageType::Tex1D: return vk::ImageType::e1D;
		case rn::ImageType::Tex2D: return vk::ImageType::e2D;
		case rn::ImageType::Tex3D: return vk::ImageType::e3D;
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

rn::ImageFormat toCore(vk::Format imageFormat) {
	return toConfig(imageFormat);
}

rn::ImageType toCore(vk::ImageType imageType) {
	switch (imageType) {
		default:
		case vk::ImageType::e1D: return rn::ImageType::Tex1D;
		case vk::ImageType::e2D: return rn::ImageType::Tex2D;
		case vk::ImageType::e3D: return rn::ImageType::Tex3D;
	}
}

rn::ImageViewType toCore(vk::ImageViewType imageViewType) {
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

vk::Format fromConfig(ngn::config::ImageFormat imageFormat) {
	switch (imageFormat) {
		case ngn::config::ImageFormat::Undefined: return vk::Format::eUndefined;
		case ngn::config::ImageFormat::R4G4UnormPack8: return vk::Format::eR4G4UnormPack8;
		case ngn::config::ImageFormat::R4G4B4A4UnormPack16: return vk::Format::eR4G4B4A4UnormPack16;
		case ngn::config::ImageFormat::B4G4R4A4UnormPack16: return vk::Format::eB4G4R4A4UnormPack16;
		case ngn::config::ImageFormat::R5G6B5UnormPack16: return vk::Format::eR5G6B5UnormPack16;
		case ngn::config::ImageFormat::B5G6R5UnormPack16: return vk::Format::eB5G6R5UnormPack16;
		case ngn::config::ImageFormat::R5G5B5A1UnormPack16: return vk::Format::eR5G5B5A1UnormPack16;
		case ngn::config::ImageFormat::B5G5R5A1UnormPack16: return vk::Format::eB5G5R5A1UnormPack16;
		case ngn::config::ImageFormat::A1R5G5B5UnormPack16: return vk::Format::eA1R5G5B5UnormPack16;
		case ngn::config::ImageFormat::R8Unorm: return vk::Format::eR8Unorm;
		case ngn::config::ImageFormat::R8Snorm: return vk::Format::eR8Snorm;
		case ngn::config::ImageFormat::R8Uscaled: return vk::Format::eR8Uscaled;
		case ngn::config::ImageFormat::R8Sscaled: return vk::Format::eR8Sscaled;
		case ngn::config::ImageFormat::R8Uint: return vk::Format::eR8Uint;
		case ngn::config::ImageFormat::R8Sint: return vk::Format::eR8Sint;
		case ngn::config::ImageFormat::R8Srgb: return vk::Format::eR8Srgb;
		case ngn::config::ImageFormat::R8G8Unorm: return vk::Format::eR8G8Unorm;
		case ngn::config::ImageFormat::R8G8Snorm: return vk::Format::eR8G8Snorm;
		case ngn::config::ImageFormat::R8G8Uscaled: return vk::Format::eR8G8Uscaled;
		case ngn::config::ImageFormat::R8G8Sscaled: return vk::Format::eR8G8Sscaled;
		case ngn::config::ImageFormat::R8G8Uint: return vk::Format::eR8G8Uint;
		case ngn::config::ImageFormat::R8G8Sint: return vk::Format::eR8G8Sint;
		case ngn::config::ImageFormat::R8G8Srgb: return vk::Format::eR8G8Srgb;
		case ngn::config::ImageFormat::R8G8B8Unorm: return vk::Format::eR8G8B8Unorm;
		case ngn::config::ImageFormat::R8G8B8Snorm: return vk::Format::eR8G8B8Snorm;
		case ngn::config::ImageFormat::R8G8B8Uscaled: return vk::Format::eR8G8B8Uscaled;
		case ngn::config::ImageFormat::R8G8B8Sscaled: return vk::Format::eR8G8B8Sscaled;
		case ngn::config::ImageFormat::R8G8B8Uint: return vk::Format::eR8G8B8Uint;
		case ngn::config::ImageFormat::R8G8B8Sint: return vk::Format::eR8G8B8Sint;
		case ngn::config::ImageFormat::R8G8B8Srgb: return vk::Format::eR8G8B8Srgb;
		case ngn::config::ImageFormat::B8G8R8Unorm: return vk::Format::eB8G8R8Unorm;
		case ngn::config::ImageFormat::B8G8R8Snorm: return vk::Format::eB8G8R8Snorm;
		case ngn::config::ImageFormat::B8G8R8Uscaled: return vk::Format::eB8G8R8Uscaled;
		case ngn::config::ImageFormat::B8G8R8Sscaled: return vk::Format::eB8G8R8Sscaled;
		case ngn::config::ImageFormat::B8G8R8Uint: return vk::Format::eB8G8R8Uint;
		case ngn::config::ImageFormat::B8G8R8Sint: return vk::Format::eB8G8R8Sint;
		case ngn::config::ImageFormat::B8G8R8Srgb: return vk::Format::eB8G8R8Srgb;
		case ngn::config::ImageFormat::R8G8B8A8Unorm: return vk::Format::eR8G8B8A8Unorm;
		case ngn::config::ImageFormat::R8G8B8A8Snorm: return vk::Format::eR8G8B8A8Snorm;
		case ngn::config::ImageFormat::R8G8B8A8Uscaled: return vk::Format::eR8G8B8A8Uscaled;
		case ngn::config::ImageFormat::R8G8B8A8Sscaled: return vk::Format::eR8G8B8A8Sscaled;
		case ngn::config::ImageFormat::R8G8B8A8Uint: return vk::Format::eR8G8B8A8Uint;
		case ngn::config::ImageFormat::R8G8B8A8Sint: return vk::Format::eR8G8B8A8Sint;
		case ngn::config::ImageFormat::R8G8B8A8Srgb: return vk::Format::eR8G8B8A8Srgb;
		case ngn::config::ImageFormat::B8G8R8A8Unorm: return vk::Format::eB8G8R8A8Unorm;
		case ngn::config::ImageFormat::B8G8R8A8Snorm: return vk::Format::eB8G8R8A8Snorm;
		case ngn::config::ImageFormat::B8G8R8A8Uscaled: return vk::Format::eB8G8R8A8Uscaled;
		case ngn::config::ImageFormat::B8G8R8A8Sscaled: return vk::Format::eB8G8R8A8Sscaled;
		case ngn::config::ImageFormat::B8G8R8A8Uint: return vk::Format::eB8G8R8A8Uint;
		case ngn::config::ImageFormat::B8G8R8A8Sint: return vk::Format::eB8G8R8A8Sint;
		case ngn::config::ImageFormat::B8G8R8A8Srgb: return vk::Format::eB8G8R8A8Srgb;
		case ngn::config::ImageFormat::A8B8G8R8UnormPack32: return vk::Format::eA8B8G8R8UnormPack32;
		case ngn::config::ImageFormat::A8B8G8R8SnormPack32: return vk::Format::eA8B8G8R8SnormPack32;
		case ngn::config::ImageFormat::A8B8G8R8UscaledPack32: return vk::Format::eA8B8G8R8UscaledPack32;
		case ngn::config::ImageFormat::A8B8G8R8SscaledPack32: return vk::Format::eA8B8G8R8SscaledPack32;
		case ngn::config::ImageFormat::A8B8G8R8UintPack32: return vk::Format::eA8B8G8R8UintPack32;
		case ngn::config::ImageFormat::A8B8G8R8SintPack32: return vk::Format::eA8B8G8R8SintPack32;
		case ngn::config::ImageFormat::A8B8G8R8SrgbPack32: return vk::Format::eA8B8G8R8SrgbPack32;
		case ngn::config::ImageFormat::A2R10G10B10UnormPack32: return vk::Format::eA2R10G10B10UnormPack32;
		case ngn::config::ImageFormat::A2R10G10B10SnormPack32: return vk::Format::eA2R10G10B10SnormPack32;
		case ngn::config::ImageFormat::A2R10G10B10UscaledPack32: return vk::Format::eA2R10G10B10UscaledPack32;
		case ngn::config::ImageFormat::A2R10G10B10SscaledPack32: return vk::Format::eA2R10G10B10SscaledPack32;
		case ngn::config::ImageFormat::A2R10G10B10UintPack32: return vk::Format::eA2R10G10B10UintPack32;
		case ngn::config::ImageFormat::A2R10G10B10SintPack32: return vk::Format::eA2R10G10B10SintPack32;
		case ngn::config::ImageFormat::A2B10G10R10UnormPack32: return vk::Format::eA2B10G10R10UnormPack32;
		case ngn::config::ImageFormat::A2B10G10R10SnormPack32: return vk::Format::eA2B10G10R10SnormPack32;
		case ngn::config::ImageFormat::A2B10G10R10UscaledPack32: return vk::Format::eA2B10G10R10UscaledPack32;
		case ngn::config::ImageFormat::A2B10G10R10SscaledPack32: return vk::Format::eA2B10G10R10SscaledPack32;
		case ngn::config::ImageFormat::A2B10G10R10UintPack32: return vk::Format::eA2B10G10R10UintPack32;
		case ngn::config::ImageFormat::A2B10G10R10SintPack32: return vk::Format::eA2B10G10R10SintPack32;
		case ngn::config::ImageFormat::R16Unorm: return vk::Format::eR16Unorm;
		case ngn::config::ImageFormat::R16Snorm: return vk::Format::eR16Snorm;
		case ngn::config::ImageFormat::R16Uscaled: return vk::Format::eR16Uscaled;
		case ngn::config::ImageFormat::R16Sscaled: return vk::Format::eR16Sscaled;
		case ngn::config::ImageFormat::R16Uint: return vk::Format::eR16Uint;
		case ngn::config::ImageFormat::R16Sint: return vk::Format::eR16Sint;
		case ngn::config::ImageFormat::R16Sfloat: return vk::Format::eR16Sfloat;
		case ngn::config::ImageFormat::R16G16Unorm: return vk::Format::eR16G16Unorm;
		case ngn::config::ImageFormat::R16G16Snorm: return vk::Format::eR16G16Snorm;
		case ngn::config::ImageFormat::R16G16Uscaled: return vk::Format::eR16G16Uscaled;
		case ngn::config::ImageFormat::R16G16Sscaled: return vk::Format::eR16G16Sscaled;
		case ngn::config::ImageFormat::R16G16Uint: return vk::Format::eR16G16Uint;
		case ngn::config::ImageFormat::R16G16Sint: return vk::Format::eR16G16Sint;
		case ngn::config::ImageFormat::R16G16Sfloat: return vk::Format::eR16G16Sfloat;
		case ngn::config::ImageFormat::R16G16B16Unorm: return vk::Format::eR16G16B16Unorm;
		case ngn::config::ImageFormat::R16G16B16Snorm: return vk::Format::eR16G16B16Snorm;
		case ngn::config::ImageFormat::R16G16B16Uscaled: return vk::Format::eR16G16B16Uscaled;
		case ngn::config::ImageFormat::R16G16B16Sscaled: return vk::Format::eR16G16B16Sscaled;
		case ngn::config::ImageFormat::R16G16B16Uint: return vk::Format::eR16G16B16Uint;
		case ngn::config::ImageFormat::R16G16B16Sint: return vk::Format::eR16G16B16Sint;
		case ngn::config::ImageFormat::R16G16B16Sfloat: return vk::Format::eR16G16B16Sfloat;
		case ngn::config::ImageFormat::R16G16B16A16Unorm: return vk::Format::eR16G16B16A16Unorm;
		case ngn::config::ImageFormat::R16G16B16A16Snorm: return vk::Format::eR16G16B16A16Snorm;
		case ngn::config::ImageFormat::R16G16B16A16Uscaled: return vk::Format::eR16G16B16A16Uscaled;
		case ngn::config::ImageFormat::R16G16B16A16Sscaled: return vk::Format::eR16G16B16A16Sscaled;
		case ngn::config::ImageFormat::R16G16B16A16Uint: return vk::Format::eR16G16B16A16Uint;
		case ngn::config::ImageFormat::R16G16B16A16Sint: return vk::Format::eR16G16B16A16Sint;
		case ngn::config::ImageFormat::R16G16B16A16Sfloat: return vk::Format::eR16G16B16A16Sfloat;
		case ngn::config::ImageFormat::R32Uint: return vk::Format::eR32Uint;
		case ngn::config::ImageFormat::R32Sint: return vk::Format::eR32Sint;
		case ngn::config::ImageFormat::R32Sfloat: return vk::Format::eR32Sfloat;
		case ngn::config::ImageFormat::R32G32Uint: return vk::Format::eR32G32Uint;
		case ngn::config::ImageFormat::R32G32Sint: return vk::Format::eR32G32Sint;
		case ngn::config::ImageFormat::R32G32Sfloat: return vk::Format::eR32G32Sfloat;
		case ngn::config::ImageFormat::R32G32B32Uint: return vk::Format::eR32G32B32Uint;
		case ngn::config::ImageFormat::R32G32B32Sint: return vk::Format::eR32G32B32Sint;
		case ngn::config::ImageFormat::R32G32B32Sfloat: return vk::Format::eR32G32B32Sfloat;
		case ngn::config::ImageFormat::R32G32B32A32Uint: return vk::Format::eR32G32B32A32Uint;
		case ngn::config::ImageFormat::R32G32B32A32Sint: return vk::Format::eR32G32B32A32Sint;
		case ngn::config::ImageFormat::R32G32B32A32Sfloat: return vk::Format::eR32G32B32A32Sfloat;
		case ngn::config::ImageFormat::R64Uint: return vk::Format::eR64Uint;
		case ngn::config::ImageFormat::R64Sint: return vk::Format::eR64Sint;
		case ngn::config::ImageFormat::R64Sfloat: return vk::Format::eR64Sfloat;
		case ngn::config::ImageFormat::R64G64Uint: return vk::Format::eR64G64Uint;
		case ngn::config::ImageFormat::R64G64Sint: return vk::Format::eR64G64Sint;
		case ngn::config::ImageFormat::R64G64Sfloat: return vk::Format::eR64G64Sfloat;
		case ngn::config::ImageFormat::R64G64B64Uint: return vk::Format::eR64G64B64Uint;
		case ngn::config::ImageFormat::R64G64B64Sint: return vk::Format::eR64G64B64Sint;
		case ngn::config::ImageFormat::R64G64B64Sfloat: return vk::Format::eR64G64B64Sfloat;
		case ngn::config::ImageFormat::R64G64B64A64Uint: return vk::Format::eR64G64B64A64Uint;
		case ngn::config::ImageFormat::R64G64B64A64Sint: return vk::Format::eR64G64B64A64Sint;
		case ngn::config::ImageFormat::R64G64B64A64Sfloat: return vk::Format::eR64G64B64A64Sfloat;
		case ngn::config::ImageFormat::B10G11R11UfloatPack32: return vk::Format::eB10G11R11UfloatPack32;
		case ngn::config::ImageFormat::E5B9G9R9UfloatPack32: return vk::Format::eE5B9G9R9UfloatPack32;
		case ngn::config::ImageFormat::D16Unorm: return vk::Format::eD16Unorm;
		case ngn::config::ImageFormat::X8D24UnormPack32: return vk::Format::eX8D24UnormPack32;
		case ngn::config::ImageFormat::D32Sfloat: return vk::Format::eD32Sfloat;
		case ngn::config::ImageFormat::S8Uint: return vk::Format::eS8Uint;
		case ngn::config::ImageFormat::D16UnormS8Uint: return vk::Format::eD16UnormS8Uint;
		case ngn::config::ImageFormat::D24UnormS8Uint: return vk::Format::eD24UnormS8Uint;
		case ngn::config::ImageFormat::D32SfloatS8Uint: return vk::Format::eD32SfloatS8Uint;
		case ngn::config::ImageFormat::Bc1RgbUnormBlock: return vk::Format::eBc1RgbUnormBlock;
		case ngn::config::ImageFormat::Bc1RgbSrgbBlock: return vk::Format::eBc1RgbSrgbBlock;
		case ngn::config::ImageFormat::Bc1RgbaUnormBlock: return vk::Format::eBc1RgbaUnormBlock;
		case ngn::config::ImageFormat::Bc1RgbaSrgbBlock: return vk::Format::eBc1RgbaSrgbBlock;
		case ngn::config::ImageFormat::Bc2UnormBlock: return vk::Format::eBc2UnormBlock;
		case ngn::config::ImageFormat::Bc2SrgbBlock: return vk::Format::eBc2SrgbBlock;
		case ngn::config::ImageFormat::Bc3UnormBlock: return vk::Format::eBc3UnormBlock;
		case ngn::config::ImageFormat::Bc3SrgbBlock: return vk::Format::eBc3SrgbBlock;
		case ngn::config::ImageFormat::Bc4UnormBlock: return vk::Format::eBc4UnormBlock;
		case ngn::config::ImageFormat::Bc4SnormBlock: return vk::Format::eBc4SnormBlock;
		case ngn::config::ImageFormat::Bc5UnormBlock: return vk::Format::eBc5UnormBlock;
		case ngn::config::ImageFormat::Bc5SnormBlock: return vk::Format::eBc5SnormBlock;
		case ngn::config::ImageFormat::Bc6HUfloatBlock: return vk::Format::eBc6HUfloatBlock;
		case ngn::config::ImageFormat::Bc6HSfloatBlock: return vk::Format::eBc6HSfloatBlock;
		case ngn::config::ImageFormat::Bc7UnormBlock: return vk::Format::eBc7UnormBlock;
		case ngn::config::ImageFormat::Bc7SrgbBlock: return vk::Format::eBc7SrgbBlock;
		case ngn::config::ImageFormat::Etc2R8G8B8UnormBlock: return vk::Format::eEtc2R8G8B8UnormBlock;
		case ngn::config::ImageFormat::Etc2R8G8B8SrgbBlock: return vk::Format::eEtc2R8G8B8SrgbBlock;
		case ngn::config::ImageFormat::Etc2R8G8B8A1UnormBlock: return vk::Format::eEtc2R8G8B8A1UnormBlock;
		case ngn::config::ImageFormat::Etc2R8G8B8A1SrgbBlock: return vk::Format::eEtc2R8G8B8A1SrgbBlock;
		case ngn::config::ImageFormat::Etc2R8G8B8A8UnormBlock: return vk::Format::eEtc2R8G8B8A8UnormBlock;
		case ngn::config::ImageFormat::Etc2R8G8B8A8SrgbBlock: return vk::Format::eEtc2R8G8B8A8SrgbBlock;
		case ngn::config::ImageFormat::EacR11UnormBlock: return vk::Format::eEacR11UnormBlock;
		case ngn::config::ImageFormat::EacR11SnormBlock: return vk::Format::eEacR11SnormBlock;
		case ngn::config::ImageFormat::EacR11G11UnormBlock: return vk::Format::eEacR11G11UnormBlock;
		case ngn::config::ImageFormat::EacR11G11SnormBlock: return vk::Format::eEacR11G11SnormBlock;
		case ngn::config::ImageFormat::Astc4x4UnormBlock: return vk::Format::eAstc4x4UnormBlock;
		case ngn::config::ImageFormat::Astc4x4SrgbBlock: return vk::Format::eAstc4x4SrgbBlock;
		case ngn::config::ImageFormat::Astc5x4UnormBlock: return vk::Format::eAstc5x4UnormBlock;
		case ngn::config::ImageFormat::Astc5x4SrgbBlock: return vk::Format::eAstc5x4SrgbBlock;
		case ngn::config::ImageFormat::Astc5x5UnormBlock: return vk::Format::eAstc5x5UnormBlock;
		case ngn::config::ImageFormat::Astc5x5SrgbBlock: return vk::Format::eAstc5x5SrgbBlock;
		case ngn::config::ImageFormat::Astc6x5UnormBlock: return vk::Format::eAstc6x5UnormBlock;
		case ngn::config::ImageFormat::Astc6x5SrgbBlock: return vk::Format::eAstc6x5SrgbBlock;
		case ngn::config::ImageFormat::Astc6x6UnormBlock: return vk::Format::eAstc6x6UnormBlock;
		case ngn::config::ImageFormat::Astc6x6SrgbBlock: return vk::Format::eAstc6x6SrgbBlock;
		case ngn::config::ImageFormat::Astc8x5UnormBlock: return vk::Format::eAstc8x5UnormBlock;
		case ngn::config::ImageFormat::Astc8x5SrgbBlock: return vk::Format::eAstc8x5SrgbBlock;
		case ngn::config::ImageFormat::Astc8x6UnormBlock: return vk::Format::eAstc8x6UnormBlock;
		case ngn::config::ImageFormat::Astc8x6SrgbBlock: return vk::Format::eAstc8x6SrgbBlock;
		case ngn::config::ImageFormat::Astc8x8UnormBlock: return vk::Format::eAstc8x8UnormBlock;
		case ngn::config::ImageFormat::Astc8x8SrgbBlock: return vk::Format::eAstc8x8SrgbBlock;
		case ngn::config::ImageFormat::Astc10x5UnormBlock: return vk::Format::eAstc10x5UnormBlock;
		case ngn::config::ImageFormat::Astc10x5SrgbBlock: return vk::Format::eAstc10x5SrgbBlock;
		case ngn::config::ImageFormat::Astc10x6UnormBlock: return vk::Format::eAstc10x6UnormBlock;
		case ngn::config::ImageFormat::Astc10x6SrgbBlock: return vk::Format::eAstc10x6SrgbBlock;
		case ngn::config::ImageFormat::Astc10x8UnormBlock: return vk::Format::eAstc10x8UnormBlock;
		case ngn::config::ImageFormat::Astc10x8SrgbBlock: return vk::Format::eAstc10x8SrgbBlock;
		case ngn::config::ImageFormat::Astc10x10UnormBlock: return vk::Format::eAstc10x10UnormBlock;
		case ngn::config::ImageFormat::Astc10x10SrgbBlock: return vk::Format::eAstc10x10SrgbBlock;
		case ngn::config::ImageFormat::Astc12x10UnormBlock: return vk::Format::eAstc12x10UnormBlock;
		case ngn::config::ImageFormat::Astc12x10SrgbBlock: return vk::Format::eAstc12x10SrgbBlock;
		case ngn::config::ImageFormat::Astc12x12UnormBlock: return vk::Format::eAstc12x12UnormBlock;
		case ngn::config::ImageFormat::Astc12x12SrgbBlock: return vk::Format::eAstc12x12SrgbBlock;
		case ngn::config::ImageFormat::Pvrtc12BppUnormBlockIMG: return vk::Format::ePvrtc12BppUnormBlockIMG;
		case ngn::config::ImageFormat::Pvrtc14BppUnormBlockIMG: return vk::Format::ePvrtc14BppUnormBlockIMG;
		case ngn::config::ImageFormat::Pvrtc22BppUnormBlockIMG: return vk::Format::ePvrtc22BppUnormBlockIMG;
		case ngn::config::ImageFormat::Pvrtc24BppUnormBlockIMG: return vk::Format::ePvrtc24BppUnormBlockIMG;
		case ngn::config::ImageFormat::Pvrtc12BppSrgbBlockIMG: return vk::Format::ePvrtc12BppSrgbBlockIMG;
		case ngn::config::ImageFormat::Pvrtc14BppSrgbBlockIMG: return vk::Format::ePvrtc14BppSrgbBlockIMG;
		case ngn::config::ImageFormat::Pvrtc22BppSrgbBlockIMG: return vk::Format::ePvrtc22BppSrgbBlockIMG;
		case ngn::config::ImageFormat::Pvrtc24BppSrgbBlockIMG: return vk::Format::ePvrtc24BppSrgbBlockIMG;
		case ngn::config::ImageFormat::G8B8G8R8422UnormKHR: return vk::Format::eG8B8G8R8422UnormKHR;
		case ngn::config::ImageFormat::B8G8R8G8422UnormKHR: return vk::Format::eB8G8R8G8422UnormKHR;
		case ngn::config::ImageFormat::G8B8R83Plane420UnormKHR: return vk::Format::eG8B8R83Plane420UnormKHR;
		case ngn::config::ImageFormat::G8B8R82Plane420UnormKHR: return vk::Format::eG8B8R82Plane420UnormKHR;
		case ngn::config::ImageFormat::G8B8R83Plane422UnormKHR: return vk::Format::eG8B8R83Plane422UnormKHR;
		case ngn::config::ImageFormat::G8B8R82Plane422UnormKHR: return vk::Format::eG8B8R82Plane422UnormKHR;
		case ngn::config::ImageFormat::G8B8R83Plane444UnormKHR: return vk::Format::eG8B8R83Plane444UnormKHR;
		case ngn::config::ImageFormat::R10X6UnormPack16KHR: return vk::Format::eR10X6UnormPack16KHR;
		case ngn::config::ImageFormat::R10X6G10X6Unorm2Pack16KHR: return vk::Format::eR10X6G10X6Unorm2Pack16KHR;
		case ngn::config::ImageFormat::R10X6G10X6B10X6A10X6Unorm4Pack16KHR: return vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR;
		case ngn::config::ImageFormat::G10X6B10X6G10X6R10X6422Unorm4Pack16KHR: return vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR;
		case ngn::config::ImageFormat::B10X6G10X6R10X6G10X6422Unorm4Pack16KHR: return vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR;
		case ngn::config::ImageFormat::G10X6B10X6R10X63Plane420Unorm3Pack16KHR: return vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G10X6B10X6R10X62Plane420Unorm3Pack16KHR: return vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G10X6B10X6R10X63Plane422Unorm3Pack16KHR: return vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G10X6B10X6R10X62Plane422Unorm3Pack16KHR: return vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G10X6B10X6R10X63Plane444Unorm3Pack16KHR: return vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR;
		case ngn::config::ImageFormat::R12X4UnormPack16KHR: return vk::Format::eR12X4UnormPack16KHR;
		case ngn::config::ImageFormat::R12X4G12X4Unorm2Pack16KHR: return vk::Format::eR12X4G12X4Unorm2Pack16KHR;
		case ngn::config::ImageFormat::R12X4G12X4B12X4A12X4Unorm4Pack16KHR: return vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR;
		case ngn::config::ImageFormat::G12X4B12X4G12X4R12X4422Unorm4Pack16KHR: return vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR;
		case ngn::config::ImageFormat::B12X4G12X4R12X4G12X4422Unorm4Pack16KHR: return vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR;
		case ngn::config::ImageFormat::G12X4B12X4R12X43Plane420Unorm3Pack16KHR: return vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G12X4B12X4R12X42Plane420Unorm3Pack16KHR: return vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G12X4B12X4R12X43Plane422Unorm3Pack16KHR: return vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G12X4B12X4R12X42Plane422Unorm3Pack16KHR: return vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G12X4B12X4R12X43Plane444Unorm3Pack16KHR: return vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR;
		case ngn::config::ImageFormat::G16B16G16R16422UnormKHR: return vk::Format::eG16B16G16R16422UnormKHR;
		case ngn::config::ImageFormat::B16G16R16G16422UnormKHR: return vk::Format::eB16G16R16G16422UnormKHR;
		case ngn::config::ImageFormat::G16B16R163Plane420UnormKHR: return vk::Format::eG16B16R163Plane420UnormKHR;
		case ngn::config::ImageFormat::G16B16R162Plane420UnormKHR: return vk::Format::eG16B16R162Plane420UnormKHR;
		case ngn::config::ImageFormat::G16B16R163Plane422UnormKHR: return vk::Format::eG16B16R163Plane422UnormKHR;
		case ngn::config::ImageFormat::G16B16R162Plane422UnormKHR: return vk::Format::eG16B16R162Plane422UnormKHR;
		case ngn::config::ImageFormat::G16B16R163Plane444UnormKHR: return vk::Format::eG16B16R163Plane444UnormKHR;
		default: return vk::Format::eUndefined;
	}
}

vk::ColorSpaceKHR fromConfig(ngn::config::ColorSpace colorSpace) {
	switch (colorSpace) {
		case ngn::config::ColorSpace::SrgbNonlinear: return vk::ColorSpaceKHR::eSrgbNonlinear;
		case ngn::config::ColorSpace::DisplayP3NonlinearEXT: return vk::ColorSpaceKHR::eDisplayP3NonlinearEXT;
		case ngn::config::ColorSpace::ExtendedSrgbLinearEXT: return vk::ColorSpaceKHR::eExtendedSrgbLinearEXT;
		case ngn::config::ColorSpace::DciP3LinearEXT: return vk::ColorSpaceKHR::eDciP3LinearEXT;
		case ngn::config::ColorSpace::DciP3NonlinearEXT: return vk::ColorSpaceKHR::eDciP3NonlinearEXT;
		case ngn::config::ColorSpace::Bt709LinearEXT: return vk::ColorSpaceKHR::eBt709LinearEXT;
		case ngn::config::ColorSpace::Bt709NonlinearEXT: return vk::ColorSpaceKHR::eBt709NonlinearEXT;
		case ngn::config::ColorSpace::Bt2020LinearEXT: return vk::ColorSpaceKHR::eBt2020LinearEXT;
		case ngn::config::ColorSpace::Hdr10St2084EXT: return vk::ColorSpaceKHR::eHdr10St2084EXT;
		case ngn::config::ColorSpace::DolbyvisionEXT: return vk::ColorSpaceKHR::eDolbyvisionEXT;
		case ngn::config::ColorSpace::Hdr10HlgEXT: return vk::ColorSpaceKHR::eHdr10HlgEXT;
		case ngn::config::ColorSpace::AdobergbLinearEXT: return vk::ColorSpaceKHR::eAdobergbLinearEXT;
		case ngn::config::ColorSpace::AdobergbNonlinearEXT: return vk::ColorSpaceKHR::eAdobergbNonlinearEXT;
		case ngn::config::ColorSpace::PassThroughEXT: return vk::ColorSpaceKHR::ePassThroughEXT;
		case ngn::config::ColorSpace::ExtendedSrgbNonlinearEXT: return vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT;
		default: return vk::ColorSpaceKHR::eSrgbNonlinear;
	}
}

vk::PresentModeKHR fromConfig(ngn::config::VSync vsync) {
	switch (vsync) {
		case ngn::config::VSync::Immediate: return vk::PresentModeKHR::eImmediate;
		case ngn::config::VSync::Fifo: return vk::PresentModeKHR::eFifo;
		case ngn::config::VSync::FifoRelaxed: return vk::PresentModeKHR::eFifoRelaxed;
		case ngn::config::VSync::Mailbox: return vk::PresentModeKHR::eMailbox;
		default: return vk::PresentModeKHR::eFifo;
	}
}

ngn::config::ImageFormat toConfig(vk::Format imageFormat) {
	switch (imageFormat) {
		case vk::Format::eUndefined: return ngn::config::ImageFormat::Undefined;
		case vk::Format::eR4G4UnormPack8: return ngn::config::ImageFormat::R4G4UnormPack8;
		case vk::Format::eR4G4B4A4UnormPack16: return ngn::config::ImageFormat::R4G4B4A4UnormPack16;
		case vk::Format::eB4G4R4A4UnormPack16: return ngn::config::ImageFormat::B4G4R4A4UnormPack16;
		case vk::Format::eR5G6B5UnormPack16: return ngn::config::ImageFormat::R5G6B5UnormPack16;
		case vk::Format::eB5G6R5UnormPack16: return ngn::config::ImageFormat::B5G6R5UnormPack16;
		case vk::Format::eR5G5B5A1UnormPack16: return ngn::config::ImageFormat::R5G5B5A1UnormPack16;
		case vk::Format::eB5G5R5A1UnormPack16: return ngn::config::ImageFormat::B5G5R5A1UnormPack16;
		case vk::Format::eA1R5G5B5UnormPack16: return ngn::config::ImageFormat::A1R5G5B5UnormPack16;
		case vk::Format::eR8Unorm: return ngn::config::ImageFormat::R8Unorm;
		case vk::Format::eR8Snorm: return ngn::config::ImageFormat::R8Snorm;
		case vk::Format::eR8Uscaled: return ngn::config::ImageFormat::R8Uscaled;
		case vk::Format::eR8Sscaled: return ngn::config::ImageFormat::R8Sscaled;
		case vk::Format::eR8Uint: return ngn::config::ImageFormat::R8Uint;
		case vk::Format::eR8Sint: return ngn::config::ImageFormat::R8Sint;
		case vk::Format::eR8Srgb: return ngn::config::ImageFormat::R8Srgb;
		case vk::Format::eR8G8Unorm: return ngn::config::ImageFormat::R8G8Unorm;
		case vk::Format::eR8G8Snorm: return ngn::config::ImageFormat::R8G8Snorm;
		case vk::Format::eR8G8Uscaled: return ngn::config::ImageFormat::R8G8Uscaled;
		case vk::Format::eR8G8Sscaled: return ngn::config::ImageFormat::R8G8Sscaled;
		case vk::Format::eR8G8Uint: return ngn::config::ImageFormat::R8G8Uint;
		case vk::Format::eR8G8Sint: return ngn::config::ImageFormat::R8G8Sint;
		case vk::Format::eR8G8Srgb: return ngn::config::ImageFormat::R8G8Srgb;
		case vk::Format::eR8G8B8Unorm: return ngn::config::ImageFormat::R8G8B8Unorm;
		case vk::Format::eR8G8B8Snorm: return ngn::config::ImageFormat::R8G8B8Snorm;
		case vk::Format::eR8G8B8Uscaled: return ngn::config::ImageFormat::R8G8B8Uscaled;
		case vk::Format::eR8G8B8Sscaled: return ngn::config::ImageFormat::R8G8B8Sscaled;
		case vk::Format::eR8G8B8Uint: return ngn::config::ImageFormat::R8G8B8Uint;
		case vk::Format::eR8G8B8Sint: return ngn::config::ImageFormat::R8G8B8Sint;
		case vk::Format::eR8G8B8Srgb: return ngn::config::ImageFormat::R8G8B8Srgb;
		case vk::Format::eB8G8R8Unorm: return ngn::config::ImageFormat::B8G8R8Unorm;
		case vk::Format::eB8G8R8Snorm: return ngn::config::ImageFormat::B8G8R8Snorm;
		case vk::Format::eB8G8R8Uscaled: return ngn::config::ImageFormat::B8G8R8Uscaled;
		case vk::Format::eB8G8R8Sscaled: return ngn::config::ImageFormat::B8G8R8Sscaled;
		case vk::Format::eB8G8R8Uint: return ngn::config::ImageFormat::B8G8R8Uint;
		case vk::Format::eB8G8R8Sint: return ngn::config::ImageFormat::B8G8R8Sint;
		case vk::Format::eB8G8R8Srgb: return ngn::config::ImageFormat::B8G8R8Srgb;
		case vk::Format::eR8G8B8A8Unorm: return ngn::config::ImageFormat::R8G8B8A8Unorm;
		case vk::Format::eR8G8B8A8Snorm: return ngn::config::ImageFormat::R8G8B8A8Snorm;
		case vk::Format::eR8G8B8A8Uscaled: return ngn::config::ImageFormat::R8G8B8A8Uscaled;
		case vk::Format::eR8G8B8A8Sscaled: return ngn::config::ImageFormat::R8G8B8A8Sscaled;
		case vk::Format::eR8G8B8A8Uint: return ngn::config::ImageFormat::R8G8B8A8Uint;
		case vk::Format::eR8G8B8A8Sint: return ngn::config::ImageFormat::R8G8B8A8Sint;
		case vk::Format::eR8G8B8A8Srgb: return ngn::config::ImageFormat::R8G8B8A8Srgb;
		case vk::Format::eB8G8R8A8Unorm: return ngn::config::ImageFormat::B8G8R8A8Unorm;
		case vk::Format::eB8G8R8A8Snorm: return ngn::config::ImageFormat::B8G8R8A8Snorm;
		case vk::Format::eB8G8R8A8Uscaled: return ngn::config::ImageFormat::B8G8R8A8Uscaled;
		case vk::Format::eB8G8R8A8Sscaled: return ngn::config::ImageFormat::B8G8R8A8Sscaled;
		case vk::Format::eB8G8R8A8Uint: return ngn::config::ImageFormat::B8G8R8A8Uint;
		case vk::Format::eB8G8R8A8Sint: return ngn::config::ImageFormat::B8G8R8A8Sint;
		case vk::Format::eB8G8R8A8Srgb: return ngn::config::ImageFormat::B8G8R8A8Srgb;
		case vk::Format::eA8B8G8R8UnormPack32: return ngn::config::ImageFormat::A8B8G8R8UnormPack32;
		case vk::Format::eA8B8G8R8SnormPack32: return ngn::config::ImageFormat::A8B8G8R8SnormPack32;
		case vk::Format::eA8B8G8R8UscaledPack32: return ngn::config::ImageFormat::A8B8G8R8UscaledPack32;
		case vk::Format::eA8B8G8R8SscaledPack32: return ngn::config::ImageFormat::A8B8G8R8SscaledPack32;
		case vk::Format::eA8B8G8R8UintPack32: return ngn::config::ImageFormat::A8B8G8R8UintPack32;
		case vk::Format::eA8B8G8R8SintPack32: return ngn::config::ImageFormat::A8B8G8R8SintPack32;
		case vk::Format::eA8B8G8R8SrgbPack32: return ngn::config::ImageFormat::A8B8G8R8SrgbPack32;
		case vk::Format::eA2R10G10B10UnormPack32: return ngn::config::ImageFormat::A2R10G10B10UnormPack32;
		case vk::Format::eA2R10G10B10SnormPack32: return ngn::config::ImageFormat::A2R10G10B10SnormPack32;
		case vk::Format::eA2R10G10B10UscaledPack32: return ngn::config::ImageFormat::A2R10G10B10UscaledPack32;
		case vk::Format::eA2R10G10B10SscaledPack32: return ngn::config::ImageFormat::A2R10G10B10SscaledPack32;
		case vk::Format::eA2R10G10B10UintPack32: return ngn::config::ImageFormat::A2R10G10B10UintPack32;
		case vk::Format::eA2R10G10B10SintPack32: return ngn::config::ImageFormat::A2R10G10B10SintPack32;
		case vk::Format::eA2B10G10R10UnormPack32: return ngn::config::ImageFormat::A2B10G10R10UnormPack32;
		case vk::Format::eA2B10G10R10SnormPack32: return ngn::config::ImageFormat::A2B10G10R10SnormPack32;
		case vk::Format::eA2B10G10R10UscaledPack32: return ngn::config::ImageFormat::A2B10G10R10UscaledPack32;
		case vk::Format::eA2B10G10R10SscaledPack32: return ngn::config::ImageFormat::A2B10G10R10SscaledPack32;
		case vk::Format::eA2B10G10R10UintPack32: return ngn::config::ImageFormat::A2B10G10R10UintPack32;
		case vk::Format::eA2B10G10R10SintPack32: return ngn::config::ImageFormat::A2B10G10R10SintPack32;
		case vk::Format::eR16Unorm: return ngn::config::ImageFormat::R16Unorm;
		case vk::Format::eR16Snorm: return ngn::config::ImageFormat::R16Snorm;
		case vk::Format::eR16Uscaled: return ngn::config::ImageFormat::R16Uscaled;
		case vk::Format::eR16Sscaled: return ngn::config::ImageFormat::R16Sscaled;
		case vk::Format::eR16Uint: return ngn::config::ImageFormat::R16Uint;
		case vk::Format::eR16Sint: return ngn::config::ImageFormat::R16Sint;
		case vk::Format::eR16Sfloat: return ngn::config::ImageFormat::R16Sfloat;
		case vk::Format::eR16G16Unorm: return ngn::config::ImageFormat::R16G16Unorm;
		case vk::Format::eR16G16Snorm: return ngn::config::ImageFormat::R16G16Snorm;
		case vk::Format::eR16G16Uscaled: return ngn::config::ImageFormat::R16G16Uscaled;
		case vk::Format::eR16G16Sscaled: return ngn::config::ImageFormat::R16G16Sscaled;
		case vk::Format::eR16G16Uint: return ngn::config::ImageFormat::R16G16Uint;
		case vk::Format::eR16G16Sint: return ngn::config::ImageFormat::R16G16Sint;
		case vk::Format::eR16G16Sfloat: return ngn::config::ImageFormat::R16G16Sfloat;
		case vk::Format::eR16G16B16Unorm: return ngn::config::ImageFormat::R16G16B16Unorm;
		case vk::Format::eR16G16B16Snorm: return ngn::config::ImageFormat::R16G16B16Snorm;
		case vk::Format::eR16G16B16Uscaled: return ngn::config::ImageFormat::R16G16B16Uscaled;
		case vk::Format::eR16G16B16Sscaled: return ngn::config::ImageFormat::R16G16B16Sscaled;
		case vk::Format::eR16G16B16Uint: return ngn::config::ImageFormat::R16G16B16Uint;
		case vk::Format::eR16G16B16Sint: return ngn::config::ImageFormat::R16G16B16Sint;
		case vk::Format::eR16G16B16Sfloat: return ngn::config::ImageFormat::R16G16B16Sfloat;
		case vk::Format::eR16G16B16A16Unorm: return ngn::config::ImageFormat::R16G16B16A16Unorm;
		case vk::Format::eR16G16B16A16Snorm: return ngn::config::ImageFormat::R16G16B16A16Snorm;
		case vk::Format::eR16G16B16A16Uscaled: return ngn::config::ImageFormat::R16G16B16A16Uscaled;
		case vk::Format::eR16G16B16A16Sscaled: return ngn::config::ImageFormat::R16G16B16A16Sscaled;
		case vk::Format::eR16G16B16A16Uint: return ngn::config::ImageFormat::R16G16B16A16Uint;
		case vk::Format::eR16G16B16A16Sint: return ngn::config::ImageFormat::R16G16B16A16Sint;
		case vk::Format::eR16G16B16A16Sfloat: return ngn::config::ImageFormat::R16G16B16A16Sfloat;
		case vk::Format::eR32Uint: return ngn::config::ImageFormat::R32Uint;
		case vk::Format::eR32Sint: return ngn::config::ImageFormat::R32Sint;
		case vk::Format::eR32Sfloat: return ngn::config::ImageFormat::R32Sfloat;
		case vk::Format::eR32G32Uint: return ngn::config::ImageFormat::R32G32Uint;
		case vk::Format::eR32G32Sint: return ngn::config::ImageFormat::R32G32Sint;
		case vk::Format::eR32G32Sfloat: return ngn::config::ImageFormat::R32G32Sfloat;
		case vk::Format::eR32G32B32Uint: return ngn::config::ImageFormat::R32G32B32Uint;
		case vk::Format::eR32G32B32Sint: return ngn::config::ImageFormat::R32G32B32Sint;
		case vk::Format::eR32G32B32Sfloat: return ngn::config::ImageFormat::R32G32B32Sfloat;
		case vk::Format::eR32G32B32A32Uint: return ngn::config::ImageFormat::R32G32B32A32Uint;
		case vk::Format::eR32G32B32A32Sint: return ngn::config::ImageFormat::R32G32B32A32Sint;
		case vk::Format::eR32G32B32A32Sfloat: return ngn::config::ImageFormat::R32G32B32A32Sfloat;
		case vk::Format::eR64Uint: return ngn::config::ImageFormat::R64Uint;
		case vk::Format::eR64Sint: return ngn::config::ImageFormat::R64Sint;
		case vk::Format::eR64Sfloat: return ngn::config::ImageFormat::R64Sfloat;
		case vk::Format::eR64G64Uint: return ngn::config::ImageFormat::R64G64Uint;
		case vk::Format::eR64G64Sint: return ngn::config::ImageFormat::R64G64Sint;
		case vk::Format::eR64G64Sfloat: return ngn::config::ImageFormat::R64G64Sfloat;
		case vk::Format::eR64G64B64Uint: return ngn::config::ImageFormat::R64G64B64Uint;
		case vk::Format::eR64G64B64Sint: return ngn::config::ImageFormat::R64G64B64Sint;
		case vk::Format::eR64G64B64Sfloat: return ngn::config::ImageFormat::R64G64B64Sfloat;
		case vk::Format::eR64G64B64A64Uint: return ngn::config::ImageFormat::R64G64B64A64Uint;
		case vk::Format::eR64G64B64A64Sint: return ngn::config::ImageFormat::R64G64B64A64Sint;
		case vk::Format::eR64G64B64A64Sfloat: return ngn::config::ImageFormat::R64G64B64A64Sfloat;
		case vk::Format::eB10G11R11UfloatPack32: return ngn::config::ImageFormat::B10G11R11UfloatPack32;
		case vk::Format::eE5B9G9R9UfloatPack32: return ngn::config::ImageFormat::E5B9G9R9UfloatPack32;
		case vk::Format::eD16Unorm: return ngn::config::ImageFormat::D16Unorm;
		case vk::Format::eX8D24UnormPack32: return ngn::config::ImageFormat::X8D24UnormPack32;
		case vk::Format::eD32Sfloat: return ngn::config::ImageFormat::D32Sfloat;
		case vk::Format::eS8Uint: return ngn::config::ImageFormat::S8Uint;
		case vk::Format::eD16UnormS8Uint: return ngn::config::ImageFormat::D16UnormS8Uint;
		case vk::Format::eD24UnormS8Uint: return ngn::config::ImageFormat::D24UnormS8Uint;
		case vk::Format::eD32SfloatS8Uint: return ngn::config::ImageFormat::D32SfloatS8Uint;
		case vk::Format::eBc1RgbUnormBlock: return ngn::config::ImageFormat::Bc1RgbUnormBlock;
		case vk::Format::eBc1RgbSrgbBlock: return ngn::config::ImageFormat::Bc1RgbSrgbBlock;
		case vk::Format::eBc1RgbaUnormBlock: return ngn::config::ImageFormat::Bc1RgbaUnormBlock;
		case vk::Format::eBc1RgbaSrgbBlock: return ngn::config::ImageFormat::Bc1RgbaSrgbBlock;
		case vk::Format::eBc2UnormBlock: return ngn::config::ImageFormat::Bc2UnormBlock;
		case vk::Format::eBc2SrgbBlock: return ngn::config::ImageFormat::Bc2SrgbBlock;
		case vk::Format::eBc3UnormBlock: return ngn::config::ImageFormat::Bc3UnormBlock;
		case vk::Format::eBc3SrgbBlock: return ngn::config::ImageFormat::Bc3SrgbBlock;
		case vk::Format::eBc4UnormBlock: return ngn::config::ImageFormat::Bc4UnormBlock;
		case vk::Format::eBc4SnormBlock: return ngn::config::ImageFormat::Bc4SnormBlock;
		case vk::Format::eBc5UnormBlock: return ngn::config::ImageFormat::Bc5UnormBlock;
		case vk::Format::eBc5SnormBlock: return ngn::config::ImageFormat::Bc5SnormBlock;
		case vk::Format::eBc6HUfloatBlock: return ngn::config::ImageFormat::Bc6HUfloatBlock;
		case vk::Format::eBc6HSfloatBlock: return ngn::config::ImageFormat::Bc6HSfloatBlock;
		case vk::Format::eBc7UnormBlock: return ngn::config::ImageFormat::Bc7UnormBlock;
		case vk::Format::eBc7SrgbBlock: return ngn::config::ImageFormat::Bc7SrgbBlock;
		case vk::Format::eEtc2R8G8B8UnormBlock: return ngn::config::ImageFormat::Etc2R8G8B8UnormBlock;
		case vk::Format::eEtc2R8G8B8SrgbBlock: return ngn::config::ImageFormat::Etc2R8G8B8SrgbBlock;
		case vk::Format::eEtc2R8G8B8A1UnormBlock: return ngn::config::ImageFormat::Etc2R8G8B8A1UnormBlock;
		case vk::Format::eEtc2R8G8B8A1SrgbBlock: return ngn::config::ImageFormat::Etc2R8G8B8A1SrgbBlock;
		case vk::Format::eEtc2R8G8B8A8UnormBlock: return ngn::config::ImageFormat::Etc2R8G8B8A8UnormBlock;
		case vk::Format::eEtc2R8G8B8A8SrgbBlock: return ngn::config::ImageFormat::Etc2R8G8B8A8SrgbBlock;
		case vk::Format::eEacR11UnormBlock: return ngn::config::ImageFormat::EacR11UnormBlock;
		case vk::Format::eEacR11SnormBlock: return ngn::config::ImageFormat::EacR11SnormBlock;
		case vk::Format::eEacR11G11UnormBlock: return ngn::config::ImageFormat::EacR11G11UnormBlock;
		case vk::Format::eEacR11G11SnormBlock: return ngn::config::ImageFormat::EacR11G11SnormBlock;
		case vk::Format::eAstc4x4UnormBlock: return ngn::config::ImageFormat::Astc4x4UnormBlock;
		case vk::Format::eAstc4x4SrgbBlock: return ngn::config::ImageFormat::Astc4x4SrgbBlock;
		case vk::Format::eAstc5x4UnormBlock: return ngn::config::ImageFormat::Astc5x4UnormBlock;
		case vk::Format::eAstc5x4SrgbBlock: return ngn::config::ImageFormat::Astc5x4SrgbBlock;
		case vk::Format::eAstc5x5UnormBlock: return ngn::config::ImageFormat::Astc5x5UnormBlock;
		case vk::Format::eAstc5x5SrgbBlock: return ngn::config::ImageFormat::Astc5x5SrgbBlock;
		case vk::Format::eAstc6x5UnormBlock: return ngn::config::ImageFormat::Astc6x5UnormBlock;
		case vk::Format::eAstc6x5SrgbBlock: return ngn::config::ImageFormat::Astc6x5SrgbBlock;
		case vk::Format::eAstc6x6UnormBlock: return ngn::config::ImageFormat::Astc6x6UnormBlock;
		case vk::Format::eAstc6x6SrgbBlock: return ngn::config::ImageFormat::Astc6x6SrgbBlock;
		case vk::Format::eAstc8x5UnormBlock: return ngn::config::ImageFormat::Astc8x5UnormBlock;
		case vk::Format::eAstc8x5SrgbBlock: return ngn::config::ImageFormat::Astc8x5SrgbBlock;
		case vk::Format::eAstc8x6UnormBlock: return ngn::config::ImageFormat::Astc8x6UnormBlock;
		case vk::Format::eAstc8x6SrgbBlock: return ngn::config::ImageFormat::Astc8x6SrgbBlock;
		case vk::Format::eAstc8x8UnormBlock: return ngn::config::ImageFormat::Astc8x8UnormBlock;
		case vk::Format::eAstc8x8SrgbBlock: return ngn::config::ImageFormat::Astc8x8SrgbBlock;
		case vk::Format::eAstc10x5UnormBlock: return ngn::config::ImageFormat::Astc10x5UnormBlock;
		case vk::Format::eAstc10x5SrgbBlock: return ngn::config::ImageFormat::Astc10x5SrgbBlock;
		case vk::Format::eAstc10x6UnormBlock: return ngn::config::ImageFormat::Astc10x6UnormBlock;
		case vk::Format::eAstc10x6SrgbBlock: return ngn::config::ImageFormat::Astc10x6SrgbBlock;
		case vk::Format::eAstc10x8UnormBlock: return ngn::config::ImageFormat::Astc10x8UnormBlock;
		case vk::Format::eAstc10x8SrgbBlock: return ngn::config::ImageFormat::Astc10x8SrgbBlock;
		case vk::Format::eAstc10x10UnormBlock: return ngn::config::ImageFormat::Astc10x10UnormBlock;
		case vk::Format::eAstc10x10SrgbBlock: return ngn::config::ImageFormat::Astc10x10SrgbBlock;
		case vk::Format::eAstc12x10UnormBlock: return ngn::config::ImageFormat::Astc12x10UnormBlock;
		case vk::Format::eAstc12x10SrgbBlock: return ngn::config::ImageFormat::Astc12x10SrgbBlock;
		case vk::Format::eAstc12x12UnormBlock: return ngn::config::ImageFormat::Astc12x12UnormBlock;
		case vk::Format::eAstc12x12SrgbBlock: return ngn::config::ImageFormat::Astc12x12SrgbBlock;
		case vk::Format::ePvrtc12BppUnormBlockIMG: return ngn::config::ImageFormat::Pvrtc12BppUnormBlockIMG;
		case vk::Format::ePvrtc14BppUnormBlockIMG: return ngn::config::ImageFormat::Pvrtc14BppUnormBlockIMG;
		case vk::Format::ePvrtc22BppUnormBlockIMG: return ngn::config::ImageFormat::Pvrtc22BppUnormBlockIMG;
		case vk::Format::ePvrtc24BppUnormBlockIMG: return ngn::config::ImageFormat::Pvrtc24BppUnormBlockIMG;
		case vk::Format::ePvrtc12BppSrgbBlockIMG: return ngn::config::ImageFormat::Pvrtc12BppSrgbBlockIMG;
		case vk::Format::ePvrtc14BppSrgbBlockIMG: return ngn::config::ImageFormat::Pvrtc14BppSrgbBlockIMG;
		case vk::Format::ePvrtc22BppSrgbBlockIMG: return ngn::config::ImageFormat::Pvrtc22BppSrgbBlockIMG;
		case vk::Format::ePvrtc24BppSrgbBlockIMG: return ngn::config::ImageFormat::Pvrtc24BppSrgbBlockIMG;
		case vk::Format::eG8B8G8R8422UnormKHR: return ngn::config::ImageFormat::G8B8G8R8422UnormKHR;
		case vk::Format::eB8G8R8G8422UnormKHR: return ngn::config::ImageFormat::B8G8R8G8422UnormKHR;
		case vk::Format::eG8B8R83Plane420UnormKHR: return ngn::config::ImageFormat::G8B8R83Plane420UnormKHR;
		case vk::Format::eG8B8R82Plane420UnormKHR: return ngn::config::ImageFormat::G8B8R82Plane420UnormKHR;
		case vk::Format::eG8B8R83Plane422UnormKHR: return ngn::config::ImageFormat::G8B8R83Plane422UnormKHR;
		case vk::Format::eG8B8R82Plane422UnormKHR: return ngn::config::ImageFormat::G8B8R82Plane422UnormKHR;
		case vk::Format::eG8B8R83Plane444UnormKHR: return ngn::config::ImageFormat::G8B8R83Plane444UnormKHR;
		case vk::Format::eR10X6UnormPack16KHR: return ngn::config::ImageFormat::R10X6UnormPack16KHR;
		case vk::Format::eR10X6G10X6Unorm2Pack16KHR: return ngn::config::ImageFormat::R10X6G10X6Unorm2Pack16KHR;
		case vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR: return ngn::config::ImageFormat::R10X6G10X6B10X6A10X6Unorm4Pack16KHR;
		case vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR: return ngn::config::ImageFormat::G10X6B10X6G10X6R10X6422Unorm4Pack16KHR;
		case vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR: return ngn::config::ImageFormat::B10X6G10X6R10X6G10X6422Unorm4Pack16KHR;
		case vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR: return ngn::config::ImageFormat::G10X6B10X6R10X63Plane420Unorm3Pack16KHR;
		case vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR: return ngn::config::ImageFormat::G10X6B10X6R10X62Plane420Unorm3Pack16KHR;
		case vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR: return ngn::config::ImageFormat::G10X6B10X6R10X63Plane422Unorm3Pack16KHR;
		case vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR: return ngn::config::ImageFormat::G10X6B10X6R10X62Plane422Unorm3Pack16KHR;
		case vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR: return ngn::config::ImageFormat::G10X6B10X6R10X63Plane444Unorm3Pack16KHR;
		case vk::Format::eR12X4UnormPack16KHR: return ngn::config::ImageFormat::R12X4UnormPack16KHR;
		case vk::Format::eR12X4G12X4Unorm2Pack16KHR: return ngn::config::ImageFormat::R12X4G12X4Unorm2Pack16KHR;
		case vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR: return ngn::config::ImageFormat::R12X4G12X4B12X4A12X4Unorm4Pack16KHR;
		case vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR: return ngn::config::ImageFormat::G12X4B12X4G12X4R12X4422Unorm4Pack16KHR;
		case vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR: return ngn::config::ImageFormat::B12X4G12X4R12X4G12X4422Unorm4Pack16KHR;
		case vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR: return ngn::config::ImageFormat::G12X4B12X4R12X43Plane420Unorm3Pack16KHR;
		case vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR: return ngn::config::ImageFormat::G12X4B12X4R12X42Plane420Unorm3Pack16KHR;
		case vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR: return ngn::config::ImageFormat::G12X4B12X4R12X43Plane422Unorm3Pack16KHR;
		case vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR: return ngn::config::ImageFormat::G12X4B12X4R12X42Plane422Unorm3Pack16KHR;
		case vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR: return ngn::config::ImageFormat::G12X4B12X4R12X43Plane444Unorm3Pack16KHR;
		case vk::Format::eG16B16G16R16422UnormKHR: return ngn::config::ImageFormat::G16B16G16R16422UnormKHR;
		case vk::Format::eB16G16R16G16422UnormKHR: return ngn::config::ImageFormat::B16G16R16G16422UnormKHR;
		case vk::Format::eG16B16R163Plane420UnormKHR: return ngn::config::ImageFormat::G16B16R163Plane420UnormKHR;
		case vk::Format::eG16B16R162Plane420UnormKHR: return ngn::config::ImageFormat::G16B16R162Plane420UnormKHR;
		case vk::Format::eG16B16R163Plane422UnormKHR: return ngn::config::ImageFormat::G16B16R163Plane422UnormKHR;
		case vk::Format::eG16B16R162Plane422UnormKHR: return ngn::config::ImageFormat::G16B16R162Plane422UnormKHR;
		case vk::Format::eG16B16R163Plane444UnormKHR: return ngn::config::ImageFormat::G16B16R163Plane444UnormKHR;
		default: return ngn::config::ImageFormat::Undefined;
	}
}

ngn::config::ColorSpace toConfig(vk::ColorSpaceKHR colorSpace) {
	switch (colorSpace) {
		case vk::ColorSpaceKHR::eSrgbNonlinear: return ngn::config::ColorSpace::SrgbNonlinear;
		case vk::ColorSpaceKHR::eDisplayP3NonlinearEXT: return ngn::config::ColorSpace::DisplayP3NonlinearEXT;
		case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT: return ngn::config::ColorSpace::ExtendedSrgbLinearEXT;
		case vk::ColorSpaceKHR::eDciP3LinearEXT: return ngn::config::ColorSpace::DciP3LinearEXT;
		case vk::ColorSpaceKHR::eDciP3NonlinearEXT: return ngn::config::ColorSpace::DciP3NonlinearEXT;
		case vk::ColorSpaceKHR::eBt709LinearEXT: return ngn::config::ColorSpace::Bt709LinearEXT;
		case vk::ColorSpaceKHR::eBt709NonlinearEXT: return ngn::config::ColorSpace::Bt709NonlinearEXT;
		case vk::ColorSpaceKHR::eBt2020LinearEXT: return ngn::config::ColorSpace::Bt2020LinearEXT;
		case vk::ColorSpaceKHR::eHdr10St2084EXT: return ngn::config::ColorSpace::Hdr10St2084EXT;
		case vk::ColorSpaceKHR::eDolbyvisionEXT: return ngn::config::ColorSpace::DolbyvisionEXT;
		case vk::ColorSpaceKHR::eHdr10HlgEXT: return ngn::config::ColorSpace::Hdr10HlgEXT;
		case vk::ColorSpaceKHR::eAdobergbLinearEXT: return ngn::config::ColorSpace::AdobergbLinearEXT;
		case vk::ColorSpaceKHR::eAdobergbNonlinearEXT: return ngn::config::ColorSpace::AdobergbNonlinearEXT;
		case vk::ColorSpaceKHR::ePassThroughEXT: return ngn::config::ColorSpace::PassThroughEXT;
		case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT: return ngn::config::ColorSpace::ExtendedSrgbNonlinearEXT;
		default: return ngn::config::ColorSpace::SrgbNonlinear;
	}
}

ngn::config::VSync toConfig(vk::PresentModeKHR vsync) {
	switch (vsync) {
		case vk::PresentModeKHR::eImmediate: return ngn::config::VSync::Immediate;
		case vk::PresentModeKHR::eFifo: return ngn::config::VSync::Fifo;
		case vk::PresentModeKHR::eFifoRelaxed: return ngn::config::VSync::FifoRelaxed;
		case vk::PresentModeKHR::eMailbox: return ngn::config::VSync::Mailbox;
		default: return ngn::config::VSync::Fifo;
	}
}

} // rn::vlk
