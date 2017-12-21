#include "core.hpp"

#include <json.hpp>
#include "../fs.hpp"

using json = nlohmann::json;

namespace ngn {

namespace config {

std::string toString(const VSync &value) {
	switch (value) {
		default:
		case VSync::Immediate: { return "immediate"; }
		case VSync::Fifo: { return "fifo"; }
		case VSync::FifoRelaxed: { return "fiforelaxed"; }
		case VSync::Mailbox: { return "mailbox"; }
	}
}

VSync fromString(const std::string_view &name, const VSync &defaultValue) {
	if (name == "immediate") { return VSync::Immediate; }
	if (name == "fifo") { return VSync::Fifo; }
	if (name == "fiforelaxed") { return VSync::FifoRelaxed; }
	if (name == "mailbox") { return VSync::Mailbox; }
	return defaultValue;
}

std::string toString(const WindowMode &value) {
	switch (value) {
		default:
		case WindowMode::Windowed: { return "windowed"; }
		case WindowMode::Borderless: { return "borderless"; }
		case WindowMode::Fullscreen: { return "fullscreen"; }
	}
}

WindowMode fromString(const std::string_view &name, const WindowMode &defaultValue) {
	if (name == "windowed") { return WindowMode::Windowed; }
	if (name == "borderless") { return WindowMode::Borderless; }
	if (name == "fullscreen") { return WindowMode::Fullscreen; }
	return defaultValue;
}

std::string toString(const ImageFormat &value) {
	switch (value) {
		default:
		case ImageFormat::Undefined: { return "undefined"; }
		case ImageFormat::R4G4UnormPack8: { return "r4g4unormpack8"; }
		case ImageFormat::R4G4B4A4UnormPack16: { return "r4g4b4a4unormpack16"; }
		case ImageFormat::B4G4R4A4UnormPack16: { return "b4g4r4a4unormpack16"; }
		case ImageFormat::R5G6B5UnormPack16: { return "r5g6b5unormpack16"; }
		case ImageFormat::B5G6R5UnormPack16: { return "b5g6r5unormpack16"; }
		case ImageFormat::R5G5B5A1UnormPack16: { return "r5g5b5a1unormpack16"; }
		case ImageFormat::B5G5R5A1UnormPack16: { return "b5g5r5a1unormpack16"; }
		case ImageFormat::A1R5G5B5UnormPack16: { return "a1r5g5b5unormpack16"; }
		case ImageFormat::R8Unorm: { return "r8unorm"; }
		case ImageFormat::R8Snorm: { return "r8snorm"; }
		case ImageFormat::R8Uscaled: { return "r8uscaled"; }
		case ImageFormat::R8Sscaled: { return "r8sscaled"; }
		case ImageFormat::R8Uint: { return "r8uint"; }
		case ImageFormat::R8Sint: { return "r8sint"; }
		case ImageFormat::R8Srgb: { return "r8srgb"; }
		case ImageFormat::R8G8Unorm: { return "r8g8unorm"; }
		case ImageFormat::R8G8Snorm: { return "r8g8snorm"; }
		case ImageFormat::R8G8Uscaled: { return "r8g8uscaled"; }
		case ImageFormat::R8G8Sscaled: { return "r8g8sscaled"; }
		case ImageFormat::R8G8Uint: { return "r8g8uint"; }
		case ImageFormat::R8G8Sint: { return "r8g8sint"; }
		case ImageFormat::R8G8Srgb: { return "r8g8srgb"; }
		case ImageFormat::R8G8B8Unorm: { return "r8g8b8unorm"; }
		case ImageFormat::R8G8B8Snorm: { return "r8g8b8snorm"; }
		case ImageFormat::R8G8B8Uscaled: { return "r8g8b8uscaled"; }
		case ImageFormat::R8G8B8Sscaled: { return "r8g8b8sscaled"; }
		case ImageFormat::R8G8B8Uint: { return "r8g8b8uint"; }
		case ImageFormat::R8G8B8Sint: { return "r8g8b8sint"; }
		case ImageFormat::R8G8B8Srgb: { return "r8g8b8srgb"; }
		case ImageFormat::B8G8R8Unorm: { return "b8g8r8unorm"; }
		case ImageFormat::B8G8R8Snorm: { return "b8g8r8snorm"; }
		case ImageFormat::B8G8R8Uscaled: { return "b8g8r8uscaled"; }
		case ImageFormat::B8G8R8Sscaled: { return "b8g8r8sscaled"; }
		case ImageFormat::B8G8R8Uint: { return "b8g8r8uint"; }
		case ImageFormat::B8G8R8Sint: { return "b8g8r8sint"; }
		case ImageFormat::B8G8R8Srgb: { return "b8g8r8srgb"; }
		case ImageFormat::R8G8B8A8Unorm: { return "r8g8b8a8unorm"; }
		case ImageFormat::R8G8B8A8Snorm: { return "r8g8b8a8snorm"; }
		case ImageFormat::R8G8B8A8Uscaled: { return "r8g8b8a8uscaled"; }
		case ImageFormat::R8G8B8A8Sscaled: { return "r8g8b8a8sscaled"; }
		case ImageFormat::R8G8B8A8Uint: { return "r8g8b8a8uint"; }
		case ImageFormat::R8G8B8A8Sint: { return "r8g8b8a8sint"; }
		case ImageFormat::R8G8B8A8Srgb: { return "r8g8b8a8srgb"; }
		case ImageFormat::B8G8R8A8Unorm: { return "b8g8r8a8unorm"; }
		case ImageFormat::B8G8R8A8Snorm: { return "b8g8r8a8snorm"; }
		case ImageFormat::B8G8R8A8Uscaled: { return "b8g8r8a8uscaled"; }
		case ImageFormat::B8G8R8A8Sscaled: { return "b8g8r8a8sscaled"; }
		case ImageFormat::B8G8R8A8Uint: { return "b8g8r8a8uint"; }
		case ImageFormat::B8G8R8A8Sint: { return "b8g8r8a8sint"; }
		case ImageFormat::B8G8R8A8Srgb: { return "b8g8r8a8srgb"; }
		case ImageFormat::A8B8G8R8UnormPack32: { return "a8b8g8r8unormpack32"; }
		case ImageFormat::A8B8G8R8SnormPack32: { return "a8b8g8r8snormpack32"; }
		case ImageFormat::A8B8G8R8UscaledPack32: { return "a8b8g8r8uscaledpack32"; }
		case ImageFormat::A8B8G8R8SscaledPack32: { return "a8b8g8r8sscaledpack32"; }
		case ImageFormat::A8B8G8R8UintPack32: { return "a8b8g8r8uintpack32"; }
		case ImageFormat::A8B8G8R8SintPack32: { return "a8b8g8r8sintpack32"; }
		case ImageFormat::A8B8G8R8SrgbPack32: { return "a8b8g8r8srgbpack32"; }
		case ImageFormat::A2R10G10B10UnormPack32: { return "a2r10g10b10unormpack32"; }
		case ImageFormat::A2R10G10B10SnormPack32: { return "a2r10g10b10snormpack32"; }
		case ImageFormat::A2R10G10B10UscaledPack32: { return "a2r10g10b10uscaledpack32"; }
		case ImageFormat::A2R10G10B10SscaledPack32: { return "a2r10g10b10sscaledpack32"; }
		case ImageFormat::A2R10G10B10UintPack32: { return "a2r10g10b10uintpack32"; }
		case ImageFormat::A2R10G10B10SintPack32: { return "a2r10g10b10sintpack32"; }
		case ImageFormat::A2B10G10R10UnormPack32: { return "a2b10g10r10unormpack32"; }
		case ImageFormat::A2B10G10R10SnormPack32: { return "a2b10g10r10snormpack32"; }
		case ImageFormat::A2B10G10R10UscaledPack32: { return "a2b10g10r10uscaledpack32"; }
		case ImageFormat::A2B10G10R10SscaledPack32: { return "a2b10g10r10sscaledpack32"; }
		case ImageFormat::A2B10G10R10UintPack32: { return "a2b10g10r10uintpack32"; }
		case ImageFormat::A2B10G10R10SintPack32: { return "a2b10g10r10sintpack32"; }
		case ImageFormat::R16Unorm: { return "r16unorm"; }
		case ImageFormat::R16Snorm: { return "r16snorm"; }
		case ImageFormat::R16Uscaled: { return "r16uscaled"; }
		case ImageFormat::R16Sscaled: { return "r16sscaled"; }
		case ImageFormat::R16Uint: { return "r16uint"; }
		case ImageFormat::R16Sint: { return "r16sint"; }
		case ImageFormat::R16Sfloat: { return "r16sfloat"; }
		case ImageFormat::R16G16Unorm: { return "r16g16unorm"; }
		case ImageFormat::R16G16Snorm: { return "r16g16snorm"; }
		case ImageFormat::R16G16Uscaled: { return "r16g16uscaled"; }
		case ImageFormat::R16G16Sscaled: { return "r16g16sscaled"; }
		case ImageFormat::R16G16Uint: { return "r16g16uint"; }
		case ImageFormat::R16G16Sint: { return "r16g16sint"; }
		case ImageFormat::R16G16Sfloat: { return "r16g16sfloat"; }
		case ImageFormat::R16G16B16Unorm: { return "r16g16b16unorm"; }
		case ImageFormat::R16G16B16Snorm: { return "r16g16b16snorm"; }
		case ImageFormat::R16G16B16Uscaled: { return "r16g16b16uscaled"; }
		case ImageFormat::R16G16B16Sscaled: { return "r16g16b16sscaled"; }
		case ImageFormat::R16G16B16Uint: { return "r16g16b16uint"; }
		case ImageFormat::R16G16B16Sint: { return "r16g16b16sint"; }
		case ImageFormat::R16G16B16Sfloat: { return "r16g16b16sfloat"; }
		case ImageFormat::R16G16B16A16Unorm: { return "r16g16b16a16unorm"; }
		case ImageFormat::R16G16B16A16Snorm: { return "r16g16b16a16snorm"; }
		case ImageFormat::R16G16B16A16Uscaled: { return "r16g16b16a16uscaled"; }
		case ImageFormat::R16G16B16A16Sscaled: { return "r16g16b16a16sscaled"; }
		case ImageFormat::R16G16B16A16Uint: { return "r16g16b16a16uint"; }
		case ImageFormat::R16G16B16A16Sint: { return "r16g16b16a16sint"; }
		case ImageFormat::R16G16B16A16Sfloat: { return "r16g16b16a16sfloat"; }
		case ImageFormat::R32Uint: { return "r32uint"; }
		case ImageFormat::R32Sint: { return "r32sint"; }
		case ImageFormat::R32Sfloat: { return "r32sfloat"; }
		case ImageFormat::R32G32Uint: { return "r32g32uint"; }
		case ImageFormat::R32G32Sint: { return "r32g32sint"; }
		case ImageFormat::R32G32Sfloat: { return "r32g32sfloat"; }
		case ImageFormat::R32G32B32Uint: { return "r32g32b32uint"; }
		case ImageFormat::R32G32B32Sint: { return "r32g32b32sint"; }
		case ImageFormat::R32G32B32Sfloat: { return "r32g32b32sfloat"; }
		case ImageFormat::R32G32B32A32Uint: { return "r32g32b32a32uint"; }
		case ImageFormat::R32G32B32A32Sint: { return "r32g32b32a32sint"; }
		case ImageFormat::R32G32B32A32Sfloat: { return "r32g32b32a32sfloat"; }
		case ImageFormat::R64Uint: { return "r64uint"; }
		case ImageFormat::R64Sint: { return "r64sint"; }
		case ImageFormat::R64Sfloat: { return "r64sfloat"; }
		case ImageFormat::R64G64Uint: { return "r64g64uint"; }
		case ImageFormat::R64G64Sint: { return "r64g64sint"; }
		case ImageFormat::R64G64Sfloat: { return "r64g64sfloat"; }
		case ImageFormat::R64G64B64Uint: { return "r64g64b64uint"; }
		case ImageFormat::R64G64B64Sint: { return "r64g64b64sint"; }
		case ImageFormat::R64G64B64Sfloat: { return "r64g64b64sfloat"; }
		case ImageFormat::R64G64B64A64Uint: { return "r64g64b64a64uint"; }
		case ImageFormat::R64G64B64A64Sint: { return "r64g64b64a64sint"; }
		case ImageFormat::R64G64B64A64Sfloat: { return "r64g64b64a64sfloat"; }
		case ImageFormat::B10G11R11UfloatPack32: { return "b10g11r11ufloatpack32"; }
		case ImageFormat::E5B9G9R9UfloatPack32: { return "e5b9g9r9ufloatpack32"; }
		case ImageFormat::D16Unorm: { return "d16unorm"; }
		case ImageFormat::X8D24UnormPack32: { return "x8d24unormpack32"; }
		case ImageFormat::D32Sfloat: { return "d32sfloat"; }
		case ImageFormat::S8Uint: { return "s8uint"; }
		case ImageFormat::D16UnormS8Uint: { return "d16unorms8uint"; }
		case ImageFormat::D24UnormS8Uint: { return "d24unorms8uint"; }
		case ImageFormat::D32SfloatS8Uint: { return "d32sfloats8uint"; }
		case ImageFormat::Bc1RgbUnormBlock: { return "bc1rgbunormblock"; }
		case ImageFormat::Bc1RgbSrgbBlock: { return "bc1rgbsrgbblock"; }
		case ImageFormat::Bc1RgbaUnormBlock: { return "bc1rgbaunormblock"; }
		case ImageFormat::Bc1RgbaSrgbBlock: { return "bc1rgbasrgbblock"; }
		case ImageFormat::Bc2UnormBlock: { return "bc2unormblock"; }
		case ImageFormat::Bc2SrgbBlock: { return "bc2srgbblock"; }
		case ImageFormat::Bc3UnormBlock: { return "bc3unormblock"; }
		case ImageFormat::Bc3SrgbBlock: { return "bc3srgbblock"; }
		case ImageFormat::Bc4UnormBlock: { return "bc4unormblock"; }
		case ImageFormat::Bc4SnormBlock: { return "bc4snormblock"; }
		case ImageFormat::Bc5UnormBlock: { return "bc5unormblock"; }
		case ImageFormat::Bc5SnormBlock: { return "bc5snormblock"; }
		case ImageFormat::Bc6HUfloatBlock: { return "bc6hufloatblock"; }
		case ImageFormat::Bc6HSfloatBlock: { return "bc6hsfloatblock"; }
		case ImageFormat::Bc7UnormBlock: { return "bc7unormblock"; }
		case ImageFormat::Bc7SrgbBlock: { return "bc7srgbblock"; }
		case ImageFormat::Etc2R8G8B8UnormBlock: { return "etc2r8g8b8unormblock"; }
		case ImageFormat::Etc2R8G8B8SrgbBlock: { return "etc2r8g8b8srgbblock"; }
		case ImageFormat::Etc2R8G8B8A1UnormBlock: { return "etc2r8g8b8a1unormblock"; }
		case ImageFormat::Etc2R8G8B8A1SrgbBlock: { return "etc2r8g8b8a1srgbblock"; }
		case ImageFormat::Etc2R8G8B8A8UnormBlock: { return "etc2r8g8b8a8unormblock"; }
		case ImageFormat::Etc2R8G8B8A8SrgbBlock: { return "etc2r8g8b8a8srgbblock"; }
		case ImageFormat::EacR11UnormBlock: { return "eacr11unormblock"; }
		case ImageFormat::EacR11SnormBlock: { return "eacr11snormblock"; }
		case ImageFormat::EacR11G11UnormBlock: { return "eacr11g11unormblock"; }
		case ImageFormat::EacR11G11SnormBlock: { return "eacr11g11snormblock"; }
		case ImageFormat::Astc4x4UnormBlock: { return "astc4x4unormblock"; }
		case ImageFormat::Astc4x4SrgbBlock: { return "astc4x4srgbblock"; }
		case ImageFormat::Astc5x4UnormBlock: { return "astc5x4unormblock"; }
		case ImageFormat::Astc5x4SrgbBlock: { return "astc5x4srgbblock"; }
		case ImageFormat::Astc5x5UnormBlock: { return "astc5x5unormblock"; }
		case ImageFormat::Astc5x5SrgbBlock: { return "astc5x5srgbblock"; }
		case ImageFormat::Astc6x5UnormBlock: { return "astc6x5unormblock"; }
		case ImageFormat::Astc6x5SrgbBlock: { return "astc6x5srgbblock"; }
		case ImageFormat::Astc6x6UnormBlock: { return "astc6x6unormblock"; }
		case ImageFormat::Astc6x6SrgbBlock: { return "astc6x6srgbblock"; }
		case ImageFormat::Astc8x5UnormBlock: { return "astc8x5unormblock"; }
		case ImageFormat::Astc8x5SrgbBlock: { return "astc8x5srgbblock"; }
		case ImageFormat::Astc8x6UnormBlock: { return "astc8x6unormblock"; }
		case ImageFormat::Astc8x6SrgbBlock: { return "astc8x6srgbblock"; }
		case ImageFormat::Astc8x8UnormBlock: { return "astc8x8unormblock"; }
		case ImageFormat::Astc8x8SrgbBlock: { return "astc8x8srgbblock"; }
		case ImageFormat::Astc10x5UnormBlock: { return "astc10x5unormblock"; }
		case ImageFormat::Astc10x5SrgbBlock: { return "astc10x5srgbblock"; }
		case ImageFormat::Astc10x6UnormBlock: { return "astc10x6unormblock"; }
		case ImageFormat::Astc10x6SrgbBlock: { return "astc10x6srgbblock"; }
		case ImageFormat::Astc10x8UnormBlock: { return "astc10x8unormblock"; }
		case ImageFormat::Astc10x8SrgbBlock: { return "astc10x8srgbblock"; }
		case ImageFormat::Astc10x10UnormBlock: { return "astc10x10unormblock"; }
		case ImageFormat::Astc10x10SrgbBlock: { return "astc10x10srgbblock"; }
		case ImageFormat::Astc12x10UnormBlock: { return "astc12x10unormblock"; }
		case ImageFormat::Astc12x10SrgbBlock: { return "astc12x10srgbblock"; }
		case ImageFormat::Astc12x12UnormBlock: { return "astc12x12unormblock"; }
		case ImageFormat::Astc12x12SrgbBlock: { return "astc12x12srgbblock"; }
		case ImageFormat::Pvrtc12BppUnormBlockIMG: { return "pvrtc12bppunormblockimg"; }
		case ImageFormat::Pvrtc14BppUnormBlockIMG: { return "pvrtc14bppunormblockimg"; }
		case ImageFormat::Pvrtc22BppUnormBlockIMG: { return "pvrtc22bppunormblockimg"; }
		case ImageFormat::Pvrtc24BppUnormBlockIMG: { return "pvrtc24bppunormblockimg"; }
		case ImageFormat::Pvrtc12BppSrgbBlockIMG: { return "pvrtc12bppsrgbblockimg"; }
		case ImageFormat::Pvrtc14BppSrgbBlockIMG: { return "pvrtc14bppsrgbblockimg"; }
		case ImageFormat::Pvrtc22BppSrgbBlockIMG: { return "pvrtc22bppsrgbblockimg"; }
		case ImageFormat::Pvrtc24BppSrgbBlockIMG: { return "pvrtc24bppsrgbblockimg"; }
		case ImageFormat::G8B8G8R8422UnormKHR: { return "g8b8g8r8422unormkhr"; }
		case ImageFormat::B8G8R8G8422UnormKHR: { return "b8g8r8g8422unormkhr"; }
		case ImageFormat::G8B8R83Plane420UnormKHR: { return "g8b8r83plane420unormkhr"; }
		case ImageFormat::G8B8R82Plane420UnormKHR: { return "g8b8r82plane420unormkhr"; }
		case ImageFormat::G8B8R83Plane422UnormKHR: { return "g8b8r83plane422unormkhr"; }
		case ImageFormat::G8B8R82Plane422UnormKHR: { return "g8b8r82plane422unormkhr"; }
		case ImageFormat::G8B8R83Plane444UnormKHR: { return "g8b8r83plane444unormkhr"; }
		case ImageFormat::R10X6UnormPack16KHR: { return "r10x6unormpack16khr"; }
		case ImageFormat::R10X6G10X6Unorm2Pack16KHR: { return "r10x6g10x6unorm2pack16khr"; }
		case ImageFormat::R10X6G10X6B10X6A10X6Unorm4Pack16KHR: { return "r10x6g10x6b10x6a10x6unorm4pack16khr"; }
		case ImageFormat::G10X6B10X6G10X6R10X6422Unorm4Pack16KHR: { return "g10x6b10x6g10x6r10x6422unorm4pack16khr"; }
		case ImageFormat::B10X6G10X6R10X6G10X6422Unorm4Pack16KHR: { return "b10x6g10x6r10x6g10x6422unorm4pack16khr"; }
		case ImageFormat::G10X6B10X6R10X63Plane420Unorm3Pack16KHR: { return "g10x6b10x6r10x63plane420unorm3pack16khr"; }
		case ImageFormat::G10X6B10X6R10X62Plane420Unorm3Pack16KHR: { return "g10x6b10x6r10x62plane420unorm3pack16khr"; }
		case ImageFormat::G10X6B10X6R10X63Plane422Unorm3Pack16KHR: { return "g10x6b10x6r10x63plane422unorm3pack16khr"; }
		case ImageFormat::G10X6B10X6R10X62Plane422Unorm3Pack16KHR: { return "g10x6b10x6r10x62plane422unorm3pack16khr"; }
		case ImageFormat::G10X6B10X6R10X63Plane444Unorm3Pack16KHR: { return "g10x6b10x6r10x63plane444unorm3pack16khr"; }
		case ImageFormat::R12X4UnormPack16KHR: { return "r12x4unormpack16khr"; }
		case ImageFormat::R12X4G12X4Unorm2Pack16KHR: { return "r12x4g12x4unorm2pack16khr"; }
		case ImageFormat::R12X4G12X4B12X4A12X4Unorm4Pack16KHR: { return "r12x4g12x4b12x4a12x4unorm4pack16khr"; }
		case ImageFormat::G12X4B12X4G12X4R12X4422Unorm4Pack16KHR: { return "g12x4b12x4g12x4r12x4422unorm4pack16khr"; }
		case ImageFormat::B12X4G12X4R12X4G12X4422Unorm4Pack16KHR: { return "b12x4g12x4r12x4g12x4422unorm4pack16khr"; }
		case ImageFormat::G12X4B12X4R12X43Plane420Unorm3Pack16KHR: { return "g12x4b12x4r12x43plane420unorm3pack16khr"; }
		case ImageFormat::G12X4B12X4R12X42Plane420Unorm3Pack16KHR: { return "g12x4b12x4r12x42plane420unorm3pack16khr"; }
		case ImageFormat::G12X4B12X4R12X43Plane422Unorm3Pack16KHR: { return "g12x4b12x4r12x43plane422unorm3pack16khr"; }
		case ImageFormat::G12X4B12X4R12X42Plane422Unorm3Pack16KHR: { return "g12x4b12x4r12x42plane422unorm3pack16khr"; }
		case ImageFormat::G12X4B12X4R12X43Plane444Unorm3Pack16KHR: { return "g12x4b12x4r12x43plane444unorm3pack16khr"; }
		case ImageFormat::G16B16G16R16422UnormKHR: { return "g16b16g16r16422unormkhr"; }
		case ImageFormat::B16G16R16G16422UnormKHR: { return "b16g16r16g16422unormkhr"; }
		case ImageFormat::G16B16R163Plane420UnormKHR: { return "g16b16r163plane420unormkhr"; }
		case ImageFormat::G16B16R162Plane420UnormKHR: { return "g16b16r162plane420unormkhr"; }
		case ImageFormat::G16B16R163Plane422UnormKHR: { return "g16b16r163plane422unormkhr"; }
		case ImageFormat::G16B16R162Plane422UnormKHR: { return "g16b16r162plane422unormkhr"; }
		case ImageFormat::G16B16R163Plane444UnormKHR: { return "g16b16r163plane444unormkhr"; }
	}
}

ImageFormat fromString(const std::string_view &name, const ImageFormat &defaultValue) {
	if (name == "undefined") { return ImageFormat::Undefined; }
	if (name == "r4g4unormpack8") { return ImageFormat::R4G4UnormPack8; }
	if (name == "r4g4b4a4unormpack16") { return ImageFormat::R4G4B4A4UnormPack16; }
	if (name == "b4g4r4a4unormpack16") { return ImageFormat::B4G4R4A4UnormPack16; }
	if (name == "r5g6b5unormpack16") { return ImageFormat::R5G6B5UnormPack16; }
	if (name == "b5g6r5unormpack16") { return ImageFormat::B5G6R5UnormPack16; }
	if (name == "r5g5b5a1unormpack16") { return ImageFormat::R5G5B5A1UnormPack16; }
	if (name == "b5g5r5a1unormpack16") { return ImageFormat::B5G5R5A1UnormPack16; }
	if (name == "a1r5g5b5unormpack16") { return ImageFormat::A1R5G5B5UnormPack16; }
	if (name == "r8unorm") { return ImageFormat::R8Unorm; }
	if (name == "r8snorm") { return ImageFormat::R8Snorm; }
	if (name == "r8uscaled") { return ImageFormat::R8Uscaled; }
	if (name == "r8sscaled") { return ImageFormat::R8Sscaled; }
	if (name == "r8uint") { return ImageFormat::R8Uint; }
	if (name == "r8sint") { return ImageFormat::R8Sint; }
	if (name == "r8srgb") { return ImageFormat::R8Srgb; }
	if (name == "r8g8unorm") { return ImageFormat::R8G8Unorm; }
	if (name == "r8g8snorm") { return ImageFormat::R8G8Snorm; }
	if (name == "r8g8uscaled") { return ImageFormat::R8G8Uscaled; }
	if (name == "r8g8sscaled") { return ImageFormat::R8G8Sscaled; }
	if (name == "r8g8uint") { return ImageFormat::R8G8Uint; }
	if (name == "r8g8sint") { return ImageFormat::R8G8Sint; }
	if (name == "r8g8srgb") { return ImageFormat::R8G8Srgb; }
	if (name == "r8g8b8unorm") { return ImageFormat::R8G8B8Unorm; }
	if (name == "r8g8b8snorm") { return ImageFormat::R8G8B8Snorm; }
	if (name == "r8g8b8uscaled") { return ImageFormat::R8G8B8Uscaled; }
	if (name == "r8g8b8sscaled") { return ImageFormat::R8G8B8Sscaled; }
	if (name == "r8g8b8uint") { return ImageFormat::R8G8B8Uint; }
	if (name == "r8g8b8sint") { return ImageFormat::R8G8B8Sint; }
	if (name == "r8g8b8srgb") { return ImageFormat::R8G8B8Srgb; }
	if (name == "b8g8r8unorm") { return ImageFormat::B8G8R8Unorm; }
	if (name == "b8g8r8snorm") { return ImageFormat::B8G8R8Snorm; }
	if (name == "b8g8r8uscaled") { return ImageFormat::B8G8R8Uscaled; }
	if (name == "b8g8r8sscaled") { return ImageFormat::B8G8R8Sscaled; }
	if (name == "b8g8r8uint") { return ImageFormat::B8G8R8Uint; }
	if (name == "b8g8r8sint") { return ImageFormat::B8G8R8Sint; }
	if (name == "b8g8r8srgb") { return ImageFormat::B8G8R8Srgb; }
	if (name == "r8g8b8a8unorm") { return ImageFormat::R8G8B8A8Unorm; }
	if (name == "r8g8b8a8snorm") { return ImageFormat::R8G8B8A8Snorm; }
	if (name == "r8g8b8a8uscaled") { return ImageFormat::R8G8B8A8Uscaled; }
	if (name == "r8g8b8a8sscaled") { return ImageFormat::R8G8B8A8Sscaled; }
	if (name == "r8g8b8a8uint") { return ImageFormat::R8G8B8A8Uint; }
	if (name == "r8g8b8a8sint") { return ImageFormat::R8G8B8A8Sint; }
	if (name == "r8g8b8a8srgb") { return ImageFormat::R8G8B8A8Srgb; }
	if (name == "b8g8r8a8unorm") { return ImageFormat::B8G8R8A8Unorm; }
	if (name == "b8g8r8a8snorm") { return ImageFormat::B8G8R8A8Snorm; }
	if (name == "b8g8r8a8uscaled") { return ImageFormat::B8G8R8A8Uscaled; }
	if (name == "b8g8r8a8sscaled") { return ImageFormat::B8G8R8A8Sscaled; }
	if (name == "b8g8r8a8uint") { return ImageFormat::B8G8R8A8Uint; }
	if (name == "b8g8r8a8sint") { return ImageFormat::B8G8R8A8Sint; }
	if (name == "b8g8r8a8srgb") { return ImageFormat::B8G8R8A8Srgb; }
	if (name == "a8b8g8r8unormpack32") { return ImageFormat::A8B8G8R8UnormPack32; }
	if (name == "a8b8g8r8snormpack32") { return ImageFormat::A8B8G8R8SnormPack32; }
	if (name == "a8b8g8r8uscaledpack32") { return ImageFormat::A8B8G8R8UscaledPack32; }
	if (name == "a8b8g8r8sscaledpack32") { return ImageFormat::A8B8G8R8SscaledPack32; }
	if (name == "a8b8g8r8uintpack32") { return ImageFormat::A8B8G8R8UintPack32; }
	if (name == "a8b8g8r8sintpack32") { return ImageFormat::A8B8G8R8SintPack32; }
	if (name == "a8b8g8r8srgbpack32") { return ImageFormat::A8B8G8R8SrgbPack32; }
	if (name == "a2r10g10b10unormpack32") { return ImageFormat::A2R10G10B10UnormPack32; }
	if (name == "a2r10g10b10snormpack32") { return ImageFormat::A2R10G10B10SnormPack32; }
	if (name == "a2r10g10b10uscaledpack32") { return ImageFormat::A2R10G10B10UscaledPack32; }
	if (name == "a2r10g10b10sscaledpack32") { return ImageFormat::A2R10G10B10SscaledPack32; }
	if (name == "a2r10g10b10uintpack32") { return ImageFormat::A2R10G10B10UintPack32; }
	if (name == "a2r10g10b10sintpack32") { return ImageFormat::A2R10G10B10SintPack32; }
	if (name == "a2b10g10r10unormpack32") { return ImageFormat::A2B10G10R10UnormPack32; }
	if (name == "a2b10g10r10snormpack32") { return ImageFormat::A2B10G10R10SnormPack32; }
	if (name == "a2b10g10r10uscaledpack32") { return ImageFormat::A2B10G10R10UscaledPack32; }
	if (name == "a2b10g10r10sscaledpack32") { return ImageFormat::A2B10G10R10SscaledPack32; }
	if (name == "a2b10g10r10uintpack32") { return ImageFormat::A2B10G10R10UintPack32; }
	if (name == "a2b10g10r10sintpack32") { return ImageFormat::A2B10G10R10SintPack32; }
	if (name == "r16unorm") { return ImageFormat::R16Unorm; }
	if (name == "r16snorm") { return ImageFormat::R16Snorm; }
	if (name == "r16uscaled") { return ImageFormat::R16Uscaled; }
	if (name == "r16sscaled") { return ImageFormat::R16Sscaled; }
	if (name == "r16uint") { return ImageFormat::R16Uint; }
	if (name == "r16sint") { return ImageFormat::R16Sint; }
	if (name == "r16sfloat") { return ImageFormat::R16Sfloat; }
	if (name == "r16g16unorm") { return ImageFormat::R16G16Unorm; }
	if (name == "r16g16snorm") { return ImageFormat::R16G16Snorm; }
	if (name == "r16g16uscaled") { return ImageFormat::R16G16Uscaled; }
	if (name == "r16g16sscaled") { return ImageFormat::R16G16Sscaled; }
	if (name == "r16g16uint") { return ImageFormat::R16G16Uint; }
	if (name == "r16g16sint") { return ImageFormat::R16G16Sint; }
	if (name == "r16g16sfloat") { return ImageFormat::R16G16Sfloat; }
	if (name == "r16g16b16unorm") { return ImageFormat::R16G16B16Unorm; }
	if (name == "r16g16b16snorm") { return ImageFormat::R16G16B16Snorm; }
	if (name == "r16g16b16uscaled") { return ImageFormat::R16G16B16Uscaled; }
	if (name == "r16g16b16sscaled") { return ImageFormat::R16G16B16Sscaled; }
	if (name == "r16g16b16uint") { return ImageFormat::R16G16B16Uint; }
	if (name == "r16g16b16sint") { return ImageFormat::R16G16B16Sint; }
	if (name == "r16g16b16sfloat") { return ImageFormat::R16G16B16Sfloat; }
	if (name == "r16g16b16a16unorm") { return ImageFormat::R16G16B16A16Unorm; }
	if (name == "r16g16b16a16snorm") { return ImageFormat::R16G16B16A16Snorm; }
	if (name == "r16g16b16a16uscaled") { return ImageFormat::R16G16B16A16Uscaled; }
	if (name == "r16g16b16a16sscaled") { return ImageFormat::R16G16B16A16Sscaled; }
	if (name == "r16g16b16a16uint") { return ImageFormat::R16G16B16A16Uint; }
	if (name == "r16g16b16a16sint") { return ImageFormat::R16G16B16A16Sint; }
	if (name == "r16g16b16a16sfloat") { return ImageFormat::R16G16B16A16Sfloat; }
	if (name == "r32uint") { return ImageFormat::R32Uint; }
	if (name == "r32sint") { return ImageFormat::R32Sint; }
	if (name == "r32sfloat") { return ImageFormat::R32Sfloat; }
	if (name == "r32g32uint") { return ImageFormat::R32G32Uint; }
	if (name == "r32g32sint") { return ImageFormat::R32G32Sint; }
	if (name == "r32g32sfloat") { return ImageFormat::R32G32Sfloat; }
	if (name == "r32g32b32uint") { return ImageFormat::R32G32B32Uint; }
	if (name == "r32g32b32sint") { return ImageFormat::R32G32B32Sint; }
	if (name == "r32g32b32sfloat") { return ImageFormat::R32G32B32Sfloat; }
	if (name == "r32g32b32a32uint") { return ImageFormat::R32G32B32A32Uint; }
	if (name == "r32g32b32a32sint") { return ImageFormat::R32G32B32A32Sint; }
	if (name == "r32g32b32a32sfloat") { return ImageFormat::R32G32B32A32Sfloat; }
	if (name == "r64uint") { return ImageFormat::R64Uint; }
	if (name == "r64sint") { return ImageFormat::R64Sint; }
	if (name == "r64sfloat") { return ImageFormat::R64Sfloat; }
	if (name == "r64g64uint") { return ImageFormat::R64G64Uint; }
	if (name == "r64g64sint") { return ImageFormat::R64G64Sint; }
	if (name == "r64g64sfloat") { return ImageFormat::R64G64Sfloat; }
	if (name == "r64g64b64uint") { return ImageFormat::R64G64B64Uint; }
	if (name == "r64g64b64sint") { return ImageFormat::R64G64B64Sint; }
	if (name == "r64g64b64sfloat") { return ImageFormat::R64G64B64Sfloat; }
	if (name == "r64g64b64a64uint") { return ImageFormat::R64G64B64A64Uint; }
	if (name == "r64g64b64a64sint") { return ImageFormat::R64G64B64A64Sint; }
	if (name == "r64g64b64a64sfloat") { return ImageFormat::R64G64B64A64Sfloat; }
	if (name == "b10g11r11ufloatpack32") { return ImageFormat::B10G11R11UfloatPack32; }
	if (name == "e5b9g9r9ufloatpack32") { return ImageFormat::E5B9G9R9UfloatPack32; }
	if (name == "d16unorm") { return ImageFormat::D16Unorm; }
	if (name == "x8d24unormpack32") { return ImageFormat::X8D24UnormPack32; }
	if (name == "d32sfloat") { return ImageFormat::D32Sfloat; }
	if (name == "s8uint") { return ImageFormat::S8Uint; }
	if (name == "d16unorms8uint") { return ImageFormat::D16UnormS8Uint; }
	if (name == "d24unorms8uint") { return ImageFormat::D24UnormS8Uint; }
	if (name == "d32sfloats8uint") { return ImageFormat::D32SfloatS8Uint; }
	if (name == "bc1rgbunormblock") { return ImageFormat::Bc1RgbUnormBlock; }
	if (name == "bc1rgbsrgbblock") { return ImageFormat::Bc1RgbSrgbBlock; }
	if (name == "bc1rgbaunormblock") { return ImageFormat::Bc1RgbaUnormBlock; }
	if (name == "bc1rgbasrgbblock") { return ImageFormat::Bc1RgbaSrgbBlock; }
	if (name == "bc2unormblock") { return ImageFormat::Bc2UnormBlock; }
	if (name == "bc2srgbblock") { return ImageFormat::Bc2SrgbBlock; }
	if (name == "bc3unormblock") { return ImageFormat::Bc3UnormBlock; }
	if (name == "bc3srgbblock") { return ImageFormat::Bc3SrgbBlock; }
	if (name == "bc4unormblock") { return ImageFormat::Bc4UnormBlock; }
	if (name == "bc4snormblock") { return ImageFormat::Bc4SnormBlock; }
	if (name == "bc5unormblock") { return ImageFormat::Bc5UnormBlock; }
	if (name == "bc5snormblock") { return ImageFormat::Bc5SnormBlock; }
	if (name == "bc6hufloatblock") { return ImageFormat::Bc6HUfloatBlock; }
	if (name == "bc6hsfloatblock") { return ImageFormat::Bc6HSfloatBlock; }
	if (name == "bc7unormblock") { return ImageFormat::Bc7UnormBlock; }
	if (name == "bc7srgbblock") { return ImageFormat::Bc7SrgbBlock; }
	if (name == "etc2r8g8b8unormblock") { return ImageFormat::Etc2R8G8B8UnormBlock; }
	if (name == "etc2r8g8b8srgbblock") { return ImageFormat::Etc2R8G8B8SrgbBlock; }
	if (name == "etc2r8g8b8a1unormblock") { return ImageFormat::Etc2R8G8B8A1UnormBlock; }
	if (name == "etc2r8g8b8a1srgbblock") { return ImageFormat::Etc2R8G8B8A1SrgbBlock; }
	if (name == "etc2r8g8b8a8unormblock") { return ImageFormat::Etc2R8G8B8A8UnormBlock; }
	if (name == "etc2r8g8b8a8srgbblock") { return ImageFormat::Etc2R8G8B8A8SrgbBlock; }
	if (name == "eacr11unormblock") { return ImageFormat::EacR11UnormBlock; }
	if (name == "eacr11snormblock") { return ImageFormat::EacR11SnormBlock; }
	if (name == "eacr11g11unormblock") { return ImageFormat::EacR11G11UnormBlock; }
	if (name == "eacr11g11snormblock") { return ImageFormat::EacR11G11SnormBlock; }
	if (name == "astc4x4unormblock") { return ImageFormat::Astc4x4UnormBlock; }
	if (name == "astc4x4srgbblock") { return ImageFormat::Astc4x4SrgbBlock; }
	if (name == "astc5x4unormblock") { return ImageFormat::Astc5x4UnormBlock; }
	if (name == "astc5x4srgbblock") { return ImageFormat::Astc5x4SrgbBlock; }
	if (name == "astc5x5unormblock") { return ImageFormat::Astc5x5UnormBlock; }
	if (name == "astc5x5srgbblock") { return ImageFormat::Astc5x5SrgbBlock; }
	if (name == "astc6x5unormblock") { return ImageFormat::Astc6x5UnormBlock; }
	if (name == "astc6x5srgbblock") { return ImageFormat::Astc6x5SrgbBlock; }
	if (name == "astc6x6unormblock") { return ImageFormat::Astc6x6UnormBlock; }
	if (name == "astc6x6srgbblock") { return ImageFormat::Astc6x6SrgbBlock; }
	if (name == "astc8x5unormblock") { return ImageFormat::Astc8x5UnormBlock; }
	if (name == "astc8x5srgbblock") { return ImageFormat::Astc8x5SrgbBlock; }
	if (name == "astc8x6unormblock") { return ImageFormat::Astc8x6UnormBlock; }
	if (name == "astc8x6srgbblock") { return ImageFormat::Astc8x6SrgbBlock; }
	if (name == "astc8x8unormblock") { return ImageFormat::Astc8x8UnormBlock; }
	if (name == "astc8x8srgbblock") { return ImageFormat::Astc8x8SrgbBlock; }
	if (name == "astc10x5unormblock") { return ImageFormat::Astc10x5UnormBlock; }
	if (name == "astc10x5srgbblock") { return ImageFormat::Astc10x5SrgbBlock; }
	if (name == "astc10x6unormblock") { return ImageFormat::Astc10x6UnormBlock; }
	if (name == "astc10x6srgbblock") { return ImageFormat::Astc10x6SrgbBlock; }
	if (name == "astc10x8unormblock") { return ImageFormat::Astc10x8UnormBlock; }
	if (name == "astc10x8srgbblock") { return ImageFormat::Astc10x8SrgbBlock; }
	if (name == "astc10x10unormblock") { return ImageFormat::Astc10x10UnormBlock; }
	if (name == "astc10x10srgbblock") { return ImageFormat::Astc10x10SrgbBlock; }
	if (name == "astc12x10unormblock") { return ImageFormat::Astc12x10UnormBlock; }
	if (name == "astc12x10srgbblock") { return ImageFormat::Astc12x10SrgbBlock; }
	if (name == "astc12x12unormblock") { return ImageFormat::Astc12x12UnormBlock; }
	if (name == "astc12x12srgbblock") { return ImageFormat::Astc12x12SrgbBlock; }
	if (name == "pvrtc12bppunormblockimg") { return ImageFormat::Pvrtc12BppUnormBlockIMG; }
	if (name == "pvrtc14bppunormblockimg") { return ImageFormat::Pvrtc14BppUnormBlockIMG; }
	if (name == "pvrtc22bppunormblockimg") { return ImageFormat::Pvrtc22BppUnormBlockIMG; }
	if (name == "pvrtc24bppunormblockimg") { return ImageFormat::Pvrtc24BppUnormBlockIMG; }
	if (name == "pvrtc12bppsrgbblockimg") { return ImageFormat::Pvrtc12BppSrgbBlockIMG; }
	if (name == "pvrtc14bppsrgbblockimg") { return ImageFormat::Pvrtc14BppSrgbBlockIMG; }
	if (name == "pvrtc22bppsrgbblockimg") { return ImageFormat::Pvrtc22BppSrgbBlockIMG; }
	if (name == "pvrtc24bppsrgbblockimg") { return ImageFormat::Pvrtc24BppSrgbBlockIMG; }
	if (name == "g8b8g8r8422unormkhr") { return ImageFormat::G8B8G8R8422UnormKHR; }
	if (name == "b8g8r8g8422unormkhr") { return ImageFormat::B8G8R8G8422UnormKHR; }
	if (name == "g8b8r83plane420unormkhr") { return ImageFormat::G8B8R83Plane420UnormKHR; }
	if (name == "g8b8r82plane420unormkhr") { return ImageFormat::G8B8R82Plane420UnormKHR; }
	if (name == "g8b8r83plane422unormkhr") { return ImageFormat::G8B8R83Plane422UnormKHR; }
	if (name == "g8b8r82plane422unormkhr") { return ImageFormat::G8B8R82Plane422UnormKHR; }
	if (name == "g8b8r83plane444unormkhr") { return ImageFormat::G8B8R83Plane444UnormKHR; }
	if (name == "r10x6unormpack16khr") { return ImageFormat::R10X6UnormPack16KHR; }
	if (name == "r10x6g10x6unorm2pack16khr") { return ImageFormat::R10X6G10X6Unorm2Pack16KHR; }
	if (name == "r10x6g10x6b10x6a10x6unorm4pack16khr") { return ImageFormat::R10X6G10X6B10X6A10X6Unorm4Pack16KHR; }
	if (name == "g10x6b10x6g10x6r10x6422unorm4pack16khr") { return ImageFormat::G10X6B10X6G10X6R10X6422Unorm4Pack16KHR; }
	if (name == "b10x6g10x6r10x6g10x6422unorm4pack16khr") { return ImageFormat::B10X6G10X6R10X6G10X6422Unorm4Pack16KHR; }
	if (name == "g10x6b10x6r10x63plane420unorm3pack16khr") { return ImageFormat::G10X6B10X6R10X63Plane420Unorm3Pack16KHR; }
	if (name == "g10x6b10x6r10x62plane420unorm3pack16khr") { return ImageFormat::G10X6B10X6R10X62Plane420Unorm3Pack16KHR; }
	if (name == "g10x6b10x6r10x63plane422unorm3pack16khr") { return ImageFormat::G10X6B10X6R10X63Plane422Unorm3Pack16KHR; }
	if (name == "g10x6b10x6r10x62plane422unorm3pack16khr") { return ImageFormat::G10X6B10X6R10X62Plane422Unorm3Pack16KHR; }
	if (name == "g10x6b10x6r10x63plane444unorm3pack16khr") { return ImageFormat::G10X6B10X6R10X63Plane444Unorm3Pack16KHR; }
	if (name == "r12x4unormpack16khr") { return ImageFormat::R12X4UnormPack16KHR; }
	if (name == "r12x4g12x4unorm2pack16khr") { return ImageFormat::R12X4G12X4Unorm2Pack16KHR; }
	if (name == "r12x4g12x4b12x4a12x4unorm4pack16khr") { return ImageFormat::R12X4G12X4B12X4A12X4Unorm4Pack16KHR; }
	if (name == "g12x4b12x4g12x4r12x4422unorm4pack16khr") { return ImageFormat::G12X4B12X4G12X4R12X4422Unorm4Pack16KHR; }
	if (name == "b12x4g12x4r12x4g12x4422unorm4pack16khr") { return ImageFormat::B12X4G12X4R12X4G12X4422Unorm4Pack16KHR; }
	if (name == "g12x4b12x4r12x43plane420unorm3pack16khr") { return ImageFormat::G12X4B12X4R12X43Plane420Unorm3Pack16KHR; }
	if (name == "g12x4b12x4r12x42plane420unorm3pack16khr") { return ImageFormat::G12X4B12X4R12X42Plane420Unorm3Pack16KHR; }
	if (name == "g12x4b12x4r12x43plane422unorm3pack16khr") { return ImageFormat::G12X4B12X4R12X43Plane422Unorm3Pack16KHR; }
	if (name == "g12x4b12x4r12x42plane422unorm3pack16khr") { return ImageFormat::G12X4B12X4R12X42Plane422Unorm3Pack16KHR; }
	if (name == "g12x4b12x4r12x43plane444unorm3pack16khr") { return ImageFormat::G12X4B12X4R12X43Plane444Unorm3Pack16KHR; }
	if (name == "g16b16g16r16422unormkhr") { return ImageFormat::G16B16G16R16422UnormKHR; }
	if (name == "b16g16r16g16422unormkhr") { return ImageFormat::B16G16R16G16422UnormKHR; }
	if (name == "g16b16r163plane420unormkhr") { return ImageFormat::G16B16R163Plane420UnormKHR; }
	if (name == "g16b16r162plane420unormkhr") { return ImageFormat::G16B16R162Plane420UnormKHR; }
	if (name == "g16b16r163plane422unormkhr") { return ImageFormat::G16B16R163Plane422UnormKHR; }
	if (name == "g16b16r162plane422unormkhr") { return ImageFormat::G16B16R162Plane422UnormKHR; }
	if (name == "g16b16r163plane444unormkhr") { return ImageFormat::G16B16R163Plane444UnormKHR; }
	return defaultValue;
}

std::string toString(const ColorSpace &value) {
	switch (value) {
		default:
		case ColorSpace::SrgbNonlinear: { return "srgbnonlinear"; }
		case ColorSpace::DisplayP3NonlinearEXT: { return "displayp3nonlinearext"; }
		case ColorSpace::ExtendedSrgbLinearEXT: { return "extendedsrgblinearext"; }
		case ColorSpace::DciP3LinearEXT: { return "dcip3linearext"; }
		case ColorSpace::DciP3NonlinearEXT: { return "dcip3nonlinearext"; }
		case ColorSpace::Bt709LinearEXT: { return "bt709linearext"; }
		case ColorSpace::Bt709NonlinearEXT: { return "bt709nonlinearext"; }
		case ColorSpace::Bt2020LinearEXT: { return "bt2020linearext"; }
		case ColorSpace::Hdr10St2084EXT: { return "hdr10st2084ext"; }
		case ColorSpace::DolbyvisionEXT: { return "dolbyvisionext"; }
		case ColorSpace::Hdr10HlgEXT: { return "hdr10hlgext"; }
		case ColorSpace::AdobergbLinearEXT: { return "adobergblinearext"; }
		case ColorSpace::AdobergbNonlinearEXT: { return "adobergbnonlinearext"; }
		case ColorSpace::PassThroughEXT: { return "passthroughext"; }
		case ColorSpace::ExtendedSrgbNonlinearEXT: { return "extendedsrgbnonlinearext"; }
	}
}

ColorSpace fromString(const std::string_view &name, const ColorSpace &defaultValue) {
	if (name == "srgbnonlinear") { return ColorSpace::SrgbNonlinear; }
	if (name == "displayp3nonlinearext") { return ColorSpace::DisplayP3NonlinearEXT; }
	if (name == "extendedsrgblinearext") { return ColorSpace::ExtendedSrgbLinearEXT; }
	if (name == "dcip3linearext") { return ColorSpace::DciP3LinearEXT; }
	if (name == "dcip3nonlinearext") { return ColorSpace::DciP3NonlinearEXT; }
	if (name == "bt709linearext") { return ColorSpace::Bt709LinearEXT; }
	if (name == "bt709nonlinearext") { return ColorSpace::Bt709NonlinearEXT; }
	if (name == "bt2020linearext") { return ColorSpace::Bt2020LinearEXT; }
	if (name == "hdr10st2084ext") { return ColorSpace::Hdr10St2084EXT; }
	if (name == "dolbyvisionext") { return ColorSpace::DolbyvisionEXT; }
	if (name == "hdr10hlgext") { return ColorSpace::Hdr10HlgEXT; }
	if (name == "adobergblinearext") { return ColorSpace::AdobergbLinearEXT; }
	if (name == "adobergbnonlinearext") { return ColorSpace::AdobergbNonlinearEXT; }
	if (name == "passthroughext") { return ColorSpace::PassThroughEXT; }
	if (name == "extendedsrgbnonlinearext") { return ColorSpace::ExtendedSrgbNonlinearEXT; }
	return defaultValue;
}

void Core::load() {
	std::string contents = ngn::fs::contents(source(), false);
	if (contents.empty()) {
		contents = "{}";
		dirty(true);
	}

	json body = json::parse(contents);

	try {
		physicalDevice.deviceId(body.at("/physicalDevice/deviceId"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		physicalDevice.deviceId(0);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		physicalDevice.deviceId(0);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		physicalDevice.deviceId(0);
		dirty(true);
	}

	try {
		physicalDevice.vendorId(body.at("/physicalDevice/vendorId"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		physicalDevice.vendorId(0);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		physicalDevice.vendorId(0);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		physicalDevice.vendorId(0);
		dirty(true);
	}

	try {
		db.mesh.forceSynchronusLoading(body.at("/db/mesh/forceSynchronusLoading"_json_pointer).get<bool>());
	} catch (const std::out_of_range &/*e*/) {
		db.mesh.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		db.mesh.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		db.mesh.forceSynchronusLoading(false);
		dirty(true);
	}

	try {
		db.texture.forceSynchronusLoading(body.at("/db/texture/forceSynchronusLoading"_json_pointer).get<bool>());
	} catch (const std::out_of_range &/*e*/) {
		db.texture.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		db.texture.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		db.texture.forceSynchronusLoading(false);
		dirty(true);
	}

	try {
		window.width(body.at("/window/width"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.width(1600);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.width(1600);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.width(1600);
		dirty(true);
	}

	try {
		window.height(body.at("/window/height"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.height(900);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.height(900);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.height(900);
		dirty(true);
	}

	try {
		window.mode(fromString(body.at("/window/mode"_json_pointer).get<std::string>(), WindowMode::Borderless));
	} catch (const std::out_of_range &/*e*/) {
		window.mode(WindowMode::Borderless);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.mode(WindowMode::Borderless);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.mode(WindowMode::Borderless);
		dirty(true);
	}

	try {
		window.monitor(body.at("/window/monitor"_json_pointer).get<int32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.monitor(-1);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.monitor(-1);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.monitor(-1);
		dirty(true);
	}

	try {
		window.vsync(fromString(body.at("/window/vsync"_json_pointer).get<std::string>(), VSync::Mailbox));
	} catch (const std::out_of_range &/*e*/) {
		window.vsync(VSync::Mailbox);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.vsync(VSync::Mailbox);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.vsync(VSync::Mailbox);
		dirty(true);
	}

	try {
		window.imageBuffers(body.at("/window/imageBuffers"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.imageBuffers(3);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.imageBuffers(3);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.imageBuffers(3);
		dirty(true);
	}

	try {
		window.surfaceFormat(fromString(body.at("/window/surfaceFormat"_json_pointer).get<std::string>(), ImageFormat::B8G8R8A8Unorm));
	} catch (const std::out_of_range &/*e*/) {
		window.surfaceFormat(ImageFormat::B8G8R8A8Unorm);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.surfaceFormat(ImageFormat::B8G8R8A8Unorm);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.surfaceFormat(ImageFormat::B8G8R8A8Unorm);
		dirty(true);
	}

	try {
		window.surfaceColorSpace(fromString(body.at("/window/surfaceColorSpace"_json_pointer).get<std::string>(), ColorSpace::SrgbNonlinear));
	} catch (const std::out_of_range &/*e*/) {
		window.surfaceColorSpace(ColorSpace::SrgbNonlinear);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.surfaceColorSpace(ColorSpace::SrgbNonlinear);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.surfaceColorSpace(ColorSpace::SrgbNonlinear);
		dirty(true);
	}

	try {
		debug.vk.useRenderDoc(body.at("/debug/vk/useRenderDoc"_json_pointer).get<bool>());
	} catch (const std::out_of_range &/*e*/) {
		debug.vk.useRenderDoc(false);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		debug.vk.useRenderDoc(false);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		debug.vk.useRenderDoc(false);
		dirty(true);
	}

	try {
		debug.vk.logLevel(body.at("/debug/vk/logLevel"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		debug.vk.logLevel(1);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		debug.vk.logLevel(1);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		debug.vk.logLevel(1);
		dirty(true);
	}
}

std::string Core::dump(const int indent) {
	json body = {
		{"physicalDevice", {
			{"deviceId", physicalDevice.deviceId()},
			{"vendorId", physicalDevice.vendorId()}
		}},
		{"db", {
			{"mesh", {
				{"forceSynchronusLoading", db.mesh.forceSynchronusLoading()}
			}},
			{"texture", {
				{"forceSynchronusLoading", db.texture.forceSynchronusLoading()}
			}}
		}},
		{"window", {
			{"width", window.width()},
			{"height", window.height()},
			{"mode", toString(window.mode())},
			{"monitor", window.monitor()},
			{"vsync", toString(window.vsync())},
			{"imageBuffers", window.imageBuffers()},
			{"surfaceFormat", toString(window.surfaceFormat())},
			{"surfaceColorSpace", toString(window.surfaceColorSpace())}
		}},
		{"debug", {
			{"vk", {
				{"useRenderDoc", debug.vk.useRenderDoc()},
				{"logLevel", debug.vk.logLevel()}
			}}
		}}
	};

	return body.dump(indent);
}

bool Core::store() {
	if (ngn::fs::write(source(), dump(2), false)) {
		dirty(false);
		return true;
	} else {
		return false;
	}
}

} // config

} // ngn
