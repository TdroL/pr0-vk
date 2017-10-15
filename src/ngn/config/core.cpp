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

std::string toString(const SurfaceFormat &value) {
	switch (value) {
		default:
		case SurfaceFormat::R4G4UnormPack8: { return "r4g4unormpack8"; }
		case SurfaceFormat::R4G4B4A4UnormPack16: { return "r4g4b4a4unormpack16"; }
		case SurfaceFormat::B4G4R4A4UnormPack16: { return "b4g4r4a4unormpack16"; }
		case SurfaceFormat::R5G6B5UnormPack16: { return "r5g6b5unormpack16"; }
		case SurfaceFormat::B5G6R5UnormPack16: { return "b5g6r5unormpack16"; }
		case SurfaceFormat::R5G5B5A1UnormPack16: { return "r5g5b5a1unormpack16"; }
		case SurfaceFormat::B5G5R5A1UnormPack16: { return "b5g5r5a1unormpack16"; }
		case SurfaceFormat::A1R5G5B5UnormPack16: { return "a1r5g5b5unormpack16"; }
		case SurfaceFormat::R8Unorm: { return "r8unorm"; }
		case SurfaceFormat::R8Snorm: { return "r8snorm"; }
		case SurfaceFormat::R8Uscaled: { return "r8uscaled"; }
		case SurfaceFormat::R8Sscaled: { return "r8sscaled"; }
		case SurfaceFormat::R8Uint: { return "r8uint"; }
		case SurfaceFormat::R8Sint: { return "r8sint"; }
		case SurfaceFormat::R8Srgb: { return "r8srgb"; }
		case SurfaceFormat::R8G8Unorm: { return "r8g8unorm"; }
		case SurfaceFormat::R8G8Snorm: { return "r8g8snorm"; }
		case SurfaceFormat::R8G8Uscaled: { return "r8g8uscaled"; }
		case SurfaceFormat::R8G8Sscaled: { return "r8g8sscaled"; }
		case SurfaceFormat::R8G8Uint: { return "r8g8uint"; }
		case SurfaceFormat::R8G8Sint: { return "r8g8sint"; }
		case SurfaceFormat::R8G8Srgb: { return "r8g8srgb"; }
		case SurfaceFormat::R8G8B8Unorm: { return "r8g8b8unorm"; }
		case SurfaceFormat::R8G8B8Snorm: { return "r8g8b8snorm"; }
		case SurfaceFormat::R8G8B8Uscaled: { return "r8g8b8uscaled"; }
		case SurfaceFormat::R8G8B8Sscaled: { return "r8g8b8sscaled"; }
		case SurfaceFormat::R8G8B8Uint: { return "r8g8b8uint"; }
		case SurfaceFormat::R8G8B8Sint: { return "r8g8b8sint"; }
		case SurfaceFormat::R8G8B8Srgb: { return "r8g8b8srgb"; }
		case SurfaceFormat::B8G8R8Unorm: { return "b8g8r8unorm"; }
		case SurfaceFormat::B8G8R8Snorm: { return "b8g8r8snorm"; }
		case SurfaceFormat::B8G8R8Uscaled: { return "b8g8r8uscaled"; }
		case SurfaceFormat::B8G8R8Sscaled: { return "b8g8r8sscaled"; }
		case SurfaceFormat::B8G8R8Uint: { return "b8g8r8uint"; }
		case SurfaceFormat::B8G8R8Sint: { return "b8g8r8sint"; }
		case SurfaceFormat::B8G8R8Srgb: { return "b8g8r8srgb"; }
		case SurfaceFormat::R8G8B8A8Unorm: { return "r8g8b8a8unorm"; }
		case SurfaceFormat::R8G8B8A8Snorm: { return "r8g8b8a8snorm"; }
		case SurfaceFormat::R8G8B8A8Uscaled: { return "r8g8b8a8uscaled"; }
		case SurfaceFormat::R8G8B8A8Sscaled: { return "r8g8b8a8sscaled"; }
		case SurfaceFormat::R8G8B8A8Uint: { return "r8g8b8a8uint"; }
		case SurfaceFormat::R8G8B8A8Sint: { return "r8g8b8a8sint"; }
		case SurfaceFormat::R8G8B8A8Srgb: { return "r8g8b8a8srgb"; }
		case SurfaceFormat::B8G8R8A8Unorm: { return "b8g8r8a8unorm"; }
		case SurfaceFormat::B8G8R8A8Snorm: { return "b8g8r8a8snorm"; }
		case SurfaceFormat::B8G8R8A8Uscaled: { return "b8g8r8a8uscaled"; }
		case SurfaceFormat::B8G8R8A8Sscaled: { return "b8g8r8a8sscaled"; }
		case SurfaceFormat::B8G8R8A8Uint: { return "b8g8r8a8uint"; }
		case SurfaceFormat::B8G8R8A8Sint: { return "b8g8r8a8sint"; }
		case SurfaceFormat::B8G8R8A8Srgb: { return "b8g8r8a8srgb"; }
		case SurfaceFormat::A8B8G8R8UnormPack32: { return "a8b8g8r8unormpack32"; }
		case SurfaceFormat::A8B8G8R8SnormPack32: { return "a8b8g8r8snormpack32"; }
		case SurfaceFormat::A8B8G8R8UscaledPack32: { return "a8b8g8r8uscaledpack32"; }
		case SurfaceFormat::A8B8G8R8SscaledPack32: { return "a8b8g8r8sscaledpack32"; }
		case SurfaceFormat::A8B8G8R8UintPack32: { return "a8b8g8r8uintpack32"; }
		case SurfaceFormat::A8B8G8R8SintPack32: { return "a8b8g8r8sintpack32"; }
		case SurfaceFormat::A8B8G8R8SrgbPack32: { return "a8b8g8r8srgbpack32"; }
		case SurfaceFormat::A2R10G10B10UnormPack32: { return "a2r10g10b10unormpack32"; }
		case SurfaceFormat::A2R10G10B10SnormPack32: { return "a2r10g10b10snormpack32"; }
		case SurfaceFormat::A2R10G10B10UscaledPack32: { return "a2r10g10b10uscaledpack32"; }
		case SurfaceFormat::A2R10G10B10SscaledPack32: { return "a2r10g10b10sscaledpack32"; }
		case SurfaceFormat::A2R10G10B10UintPack32: { return "a2r10g10b10uintpack32"; }
		case SurfaceFormat::A2R10G10B10SintPack32: { return "a2r10g10b10sintpack32"; }
		case SurfaceFormat::A2B10G10R10UnormPack32: { return "a2b10g10r10unormpack32"; }
		case SurfaceFormat::A2B10G10R10SnormPack32: { return "a2b10g10r10snormpack32"; }
		case SurfaceFormat::A2B10G10R10UscaledPack32: { return "a2b10g10r10uscaledpack32"; }
		case SurfaceFormat::A2B10G10R10SscaledPack32: { return "a2b10g10r10sscaledpack32"; }
		case SurfaceFormat::A2B10G10R10UintPack32: { return "a2b10g10r10uintpack32"; }
		case SurfaceFormat::A2B10G10R10SintPack32: { return "a2b10g10r10sintpack32"; }
		case SurfaceFormat::R16Unorm: { return "r16unorm"; }
		case SurfaceFormat::R16Snorm: { return "r16snorm"; }
		case SurfaceFormat::R16Uscaled: { return "r16uscaled"; }
		case SurfaceFormat::R16Sscaled: { return "r16sscaled"; }
		case SurfaceFormat::R16Uint: { return "r16uint"; }
		case SurfaceFormat::R16Sint: { return "r16sint"; }
		case SurfaceFormat::R16Sfloat: { return "r16sfloat"; }
		case SurfaceFormat::R16G16Unorm: { return "r16g16unorm"; }
		case SurfaceFormat::R16G16Snorm: { return "r16g16snorm"; }
		case SurfaceFormat::R16G16Uscaled: { return "r16g16uscaled"; }
		case SurfaceFormat::R16G16Sscaled: { return "r16g16sscaled"; }
		case SurfaceFormat::R16G16Uint: { return "r16g16uint"; }
		case SurfaceFormat::R16G16Sint: { return "r16g16sint"; }
		case SurfaceFormat::R16G16Sfloat: { return "r16g16sfloat"; }
		case SurfaceFormat::R16G16B16Unorm: { return "r16g16b16unorm"; }
		case SurfaceFormat::R16G16B16Snorm: { return "r16g16b16snorm"; }
		case SurfaceFormat::R16G16B16Uscaled: { return "r16g16b16uscaled"; }
		case SurfaceFormat::R16G16B16Sscaled: { return "r16g16b16sscaled"; }
		case SurfaceFormat::R16G16B16Uint: { return "r16g16b16uint"; }
		case SurfaceFormat::R16G16B16Sint: { return "r16g16b16sint"; }
		case SurfaceFormat::R16G16B16Sfloat: { return "r16g16b16sfloat"; }
		case SurfaceFormat::R16G16B16A16Unorm: { return "r16g16b16a16unorm"; }
		case SurfaceFormat::R16G16B16A16Snorm: { return "r16g16b16a16snorm"; }
		case SurfaceFormat::R16G16B16A16Uscaled: { return "r16g16b16a16uscaled"; }
		case SurfaceFormat::R16G16B16A16Sscaled: { return "r16g16b16a16sscaled"; }
		case SurfaceFormat::R16G16B16A16Uint: { return "r16g16b16a16uint"; }
		case SurfaceFormat::R16G16B16A16Sint: { return "r16g16b16a16sint"; }
		case SurfaceFormat::R16G16B16A16Sfloat: { return "r16g16b16a16sfloat"; }
		case SurfaceFormat::R32Uint: { return "r32uint"; }
		case SurfaceFormat::R32Sint: { return "r32sint"; }
		case SurfaceFormat::R32Sfloat: { return "r32sfloat"; }
		case SurfaceFormat::R32G32Uint: { return "r32g32uint"; }
		case SurfaceFormat::R32G32Sint: { return "r32g32sint"; }
		case SurfaceFormat::R32G32Sfloat: { return "r32g32sfloat"; }
		case SurfaceFormat::R32G32B32Uint: { return "r32g32b32uint"; }
		case SurfaceFormat::R32G32B32Sint: { return "r32g32b32sint"; }
		case SurfaceFormat::R32G32B32Sfloat: { return "r32g32b32sfloat"; }
		case SurfaceFormat::R32G32B32A32Uint: { return "r32g32b32a32uint"; }
		case SurfaceFormat::R32G32B32A32Sint: { return "r32g32b32a32sint"; }
		case SurfaceFormat::R32G32B32A32Sfloat: { return "r32g32b32a32sfloat"; }
		case SurfaceFormat::R64Uint: { return "r64uint"; }
		case SurfaceFormat::R64Sint: { return "r64sint"; }
		case SurfaceFormat::R64Sfloat: { return "r64sfloat"; }
		case SurfaceFormat::R64G64Uint: { return "r64g64uint"; }
		case SurfaceFormat::R64G64Sint: { return "r64g64sint"; }
		case SurfaceFormat::R64G64Sfloat: { return "r64g64sfloat"; }
		case SurfaceFormat::R64G64B64Uint: { return "r64g64b64uint"; }
		case SurfaceFormat::R64G64B64Sint: { return "r64g64b64sint"; }
		case SurfaceFormat::R64G64B64Sfloat: { return "r64g64b64sfloat"; }
		case SurfaceFormat::R64G64B64A64Uint: { return "r64g64b64a64uint"; }
		case SurfaceFormat::R64G64B64A64Sint: { return "r64g64b64a64sint"; }
		case SurfaceFormat::R64G64B64A64Sfloat: { return "r64g64b64a64sfloat"; }
		case SurfaceFormat::B10G11R11UfloatPack32: { return "b10g11r11ufloatpack32"; }
		case SurfaceFormat::E5B9G9R9UfloatPack32: { return "e5b9g9r9ufloatpack32"; }
		case SurfaceFormat::D16Unorm: { return "d16unorm"; }
		case SurfaceFormat::X8D24UnormPack32: { return "x8d24unormpack32"; }
		case SurfaceFormat::D32Sfloat: { return "d32sfloat"; }
		case SurfaceFormat::S8Uint: { return "s8uint"; }
		case SurfaceFormat::D16UnormS8Uint: { return "d16unorms8uint"; }
		case SurfaceFormat::D24UnormS8Uint: { return "d24unorms8uint"; }
		case SurfaceFormat::D32SfloatS8Uint: { return "d32sfloats8uint"; }
		case SurfaceFormat::Bc1RgbUnormBlock: { return "bc1rgbunormblock"; }
		case SurfaceFormat::Bc1RgbSrgbBlock: { return "bc1rgbsrgbblock"; }
		case SurfaceFormat::Bc1RgbaUnormBlock: { return "bc1rgbaunormblock"; }
		case SurfaceFormat::Bc1RgbaSrgbBlock: { return "bc1rgbasrgbblock"; }
		case SurfaceFormat::Bc2UnormBlock: { return "bc2unormblock"; }
		case SurfaceFormat::Bc2SrgbBlock: { return "bc2srgbblock"; }
		case SurfaceFormat::Bc3UnormBlock: { return "bc3unormblock"; }
		case SurfaceFormat::Bc3SrgbBlock: { return "bc3srgbblock"; }
		case SurfaceFormat::Bc4UnormBlock: { return "bc4unormblock"; }
		case SurfaceFormat::Bc4SnormBlock: { return "bc4snormblock"; }
		case SurfaceFormat::Bc5UnormBlock: { return "bc5unormblock"; }
		case SurfaceFormat::Bc5SnormBlock: { return "bc5snormblock"; }
		case SurfaceFormat::Bc6HUfloatBlock: { return "bc6hufloatblock"; }
		case SurfaceFormat::Bc6HSfloatBlock: { return "bc6hsfloatblock"; }
		case SurfaceFormat::Bc7UnormBlock: { return "bc7unormblock"; }
		case SurfaceFormat::Bc7SrgbBlock: { return "bc7srgbblock"; }
		case SurfaceFormat::Etc2R8G8B8UnormBlock: { return "etc2r8g8b8unormblock"; }
		case SurfaceFormat::Etc2R8G8B8SrgbBlock: { return "etc2r8g8b8srgbblock"; }
		case SurfaceFormat::Etc2R8G8B8A1UnormBlock: { return "etc2r8g8b8a1unormblock"; }
		case SurfaceFormat::Etc2R8G8B8A1SrgbBlock: { return "etc2r8g8b8a1srgbblock"; }
		case SurfaceFormat::Etc2R8G8B8A8UnormBlock: { return "etc2r8g8b8a8unormblock"; }
		case SurfaceFormat::Etc2R8G8B8A8SrgbBlock: { return "etc2r8g8b8a8srgbblock"; }
		case SurfaceFormat::EacR11UnormBlock: { return "eacr11unormblock"; }
		case SurfaceFormat::EacR11SnormBlock: { return "eacr11snormblock"; }
		case SurfaceFormat::EacR11G11UnormBlock: { return "eacr11g11unormblock"; }
		case SurfaceFormat::EacR11G11SnormBlock: { return "eacr11g11snormblock"; }
		case SurfaceFormat::Astc4x4UnormBlock: { return "astc4x4unormblock"; }
		case SurfaceFormat::Astc4x4SrgbBlock: { return "astc4x4srgbblock"; }
		case SurfaceFormat::Astc5x4UnormBlock: { return "astc5x4unormblock"; }
		case SurfaceFormat::Astc5x4SrgbBlock: { return "astc5x4srgbblock"; }
		case SurfaceFormat::Astc5x5UnormBlock: { return "astc5x5unormblock"; }
		case SurfaceFormat::Astc5x5SrgbBlock: { return "astc5x5srgbblock"; }
		case SurfaceFormat::Astc6x5UnormBlock: { return "astc6x5unormblock"; }
		case SurfaceFormat::Astc6x5SrgbBlock: { return "astc6x5srgbblock"; }
		case SurfaceFormat::Astc6x6UnormBlock: { return "astc6x6unormblock"; }
		case SurfaceFormat::Astc6x6SrgbBlock: { return "astc6x6srgbblock"; }
		case SurfaceFormat::Astc8x5UnormBlock: { return "astc8x5unormblock"; }
		case SurfaceFormat::Astc8x5SrgbBlock: { return "astc8x5srgbblock"; }
		case SurfaceFormat::Astc8x6UnormBlock: { return "astc8x6unormblock"; }
		case SurfaceFormat::Astc8x6SrgbBlock: { return "astc8x6srgbblock"; }
		case SurfaceFormat::Astc8x8UnormBlock: { return "astc8x8unormblock"; }
		case SurfaceFormat::Astc8x8SrgbBlock: { return "astc8x8srgbblock"; }
		case SurfaceFormat::Astc10x5UnormBlock: { return "astc10x5unormblock"; }
		case SurfaceFormat::Astc10x5SrgbBlock: { return "astc10x5srgbblock"; }
		case SurfaceFormat::Astc10x6UnormBlock: { return "astc10x6unormblock"; }
		case SurfaceFormat::Astc10x6SrgbBlock: { return "astc10x6srgbblock"; }
		case SurfaceFormat::Astc10x8UnormBlock: { return "astc10x8unormblock"; }
		case SurfaceFormat::Astc10x8SrgbBlock: { return "astc10x8srgbblock"; }
		case SurfaceFormat::Astc10x10UnormBlock: { return "astc10x10unormblock"; }
		case SurfaceFormat::Astc10x10SrgbBlock: { return "astc10x10srgbblock"; }
		case SurfaceFormat::Astc12x10UnormBlock: { return "astc12x10unormblock"; }
		case SurfaceFormat::Astc12x10SrgbBlock: { return "astc12x10srgbblock"; }
		case SurfaceFormat::Astc12x12UnormBlock: { return "astc12x12unormblock"; }
		case SurfaceFormat::Astc12x12SrgbBlock: { return "astc12x12srgbblock"; }
		case SurfaceFormat::Pvrtc12BppUnormBlockIMG: { return "pvrtc12bppunormblockimg"; }
		case SurfaceFormat::Pvrtc14BppUnormBlockIMG: { return "pvrtc14bppunormblockimg"; }
		case SurfaceFormat::Pvrtc22BppUnormBlockIMG: { return "pvrtc22bppunormblockimg"; }
		case SurfaceFormat::Pvrtc24BppUnormBlockIMG: { return "pvrtc24bppunormblockimg"; }
		case SurfaceFormat::Pvrtc12BppSrgbBlockIMG: { return "pvrtc12bppsrgbblockimg"; }
		case SurfaceFormat::Pvrtc14BppSrgbBlockIMG: { return "pvrtc14bppsrgbblockimg"; }
		case SurfaceFormat::Pvrtc22BppSrgbBlockIMG: { return "pvrtc22bppsrgbblockimg"; }
		case SurfaceFormat::Pvrtc24BppSrgbBlockIMG: { return "pvrtc24bppsrgbblockimg"; }
	}
}

SurfaceFormat fromString(const std::string_view &name, const SurfaceFormat &defaultValue) {
	if (name == "r4g4unormpack8") { return SurfaceFormat::R4G4UnormPack8; }
	if (name == "r4g4b4a4unormpack16") { return SurfaceFormat::R4G4B4A4UnormPack16; }
	if (name == "b4g4r4a4unormpack16") { return SurfaceFormat::B4G4R4A4UnormPack16; }
	if (name == "r5g6b5unormpack16") { return SurfaceFormat::R5G6B5UnormPack16; }
	if (name == "b5g6r5unormpack16") { return SurfaceFormat::B5G6R5UnormPack16; }
	if (name == "r5g5b5a1unormpack16") { return SurfaceFormat::R5G5B5A1UnormPack16; }
	if (name == "b5g5r5a1unormpack16") { return SurfaceFormat::B5G5R5A1UnormPack16; }
	if (name == "a1r5g5b5unormpack16") { return SurfaceFormat::A1R5G5B5UnormPack16; }
	if (name == "r8unorm") { return SurfaceFormat::R8Unorm; }
	if (name == "r8snorm") { return SurfaceFormat::R8Snorm; }
	if (name == "r8uscaled") { return SurfaceFormat::R8Uscaled; }
	if (name == "r8sscaled") { return SurfaceFormat::R8Sscaled; }
	if (name == "r8uint") { return SurfaceFormat::R8Uint; }
	if (name == "r8sint") { return SurfaceFormat::R8Sint; }
	if (name == "r8srgb") { return SurfaceFormat::R8Srgb; }
	if (name == "r8g8unorm") { return SurfaceFormat::R8G8Unorm; }
	if (name == "r8g8snorm") { return SurfaceFormat::R8G8Snorm; }
	if (name == "r8g8uscaled") { return SurfaceFormat::R8G8Uscaled; }
	if (name == "r8g8sscaled") { return SurfaceFormat::R8G8Sscaled; }
	if (name == "r8g8uint") { return SurfaceFormat::R8G8Uint; }
	if (name == "r8g8sint") { return SurfaceFormat::R8G8Sint; }
	if (name == "r8g8srgb") { return SurfaceFormat::R8G8Srgb; }
	if (name == "r8g8b8unorm") { return SurfaceFormat::R8G8B8Unorm; }
	if (name == "r8g8b8snorm") { return SurfaceFormat::R8G8B8Snorm; }
	if (name == "r8g8b8uscaled") { return SurfaceFormat::R8G8B8Uscaled; }
	if (name == "r8g8b8sscaled") { return SurfaceFormat::R8G8B8Sscaled; }
	if (name == "r8g8b8uint") { return SurfaceFormat::R8G8B8Uint; }
	if (name == "r8g8b8sint") { return SurfaceFormat::R8G8B8Sint; }
	if (name == "r8g8b8srgb") { return SurfaceFormat::R8G8B8Srgb; }
	if (name == "b8g8r8unorm") { return SurfaceFormat::B8G8R8Unorm; }
	if (name == "b8g8r8snorm") { return SurfaceFormat::B8G8R8Snorm; }
	if (name == "b8g8r8uscaled") { return SurfaceFormat::B8G8R8Uscaled; }
	if (name == "b8g8r8sscaled") { return SurfaceFormat::B8G8R8Sscaled; }
	if (name == "b8g8r8uint") { return SurfaceFormat::B8G8R8Uint; }
	if (name == "b8g8r8sint") { return SurfaceFormat::B8G8R8Sint; }
	if (name == "b8g8r8srgb") { return SurfaceFormat::B8G8R8Srgb; }
	if (name == "r8g8b8a8unorm") { return SurfaceFormat::R8G8B8A8Unorm; }
	if (name == "r8g8b8a8snorm") { return SurfaceFormat::R8G8B8A8Snorm; }
	if (name == "r8g8b8a8uscaled") { return SurfaceFormat::R8G8B8A8Uscaled; }
	if (name == "r8g8b8a8sscaled") { return SurfaceFormat::R8G8B8A8Sscaled; }
	if (name == "r8g8b8a8uint") { return SurfaceFormat::R8G8B8A8Uint; }
	if (name == "r8g8b8a8sint") { return SurfaceFormat::R8G8B8A8Sint; }
	if (name == "r8g8b8a8srgb") { return SurfaceFormat::R8G8B8A8Srgb; }
	if (name == "b8g8r8a8unorm") { return SurfaceFormat::B8G8R8A8Unorm; }
	if (name == "b8g8r8a8snorm") { return SurfaceFormat::B8G8R8A8Snorm; }
	if (name == "b8g8r8a8uscaled") { return SurfaceFormat::B8G8R8A8Uscaled; }
	if (name == "b8g8r8a8sscaled") { return SurfaceFormat::B8G8R8A8Sscaled; }
	if (name == "b8g8r8a8uint") { return SurfaceFormat::B8G8R8A8Uint; }
	if (name == "b8g8r8a8sint") { return SurfaceFormat::B8G8R8A8Sint; }
	if (name == "b8g8r8a8srgb") { return SurfaceFormat::B8G8R8A8Srgb; }
	if (name == "a8b8g8r8unormpack32") { return SurfaceFormat::A8B8G8R8UnormPack32; }
	if (name == "a8b8g8r8snormpack32") { return SurfaceFormat::A8B8G8R8SnormPack32; }
	if (name == "a8b8g8r8uscaledpack32") { return SurfaceFormat::A8B8G8R8UscaledPack32; }
	if (name == "a8b8g8r8sscaledpack32") { return SurfaceFormat::A8B8G8R8SscaledPack32; }
	if (name == "a8b8g8r8uintpack32") { return SurfaceFormat::A8B8G8R8UintPack32; }
	if (name == "a8b8g8r8sintpack32") { return SurfaceFormat::A8B8G8R8SintPack32; }
	if (name == "a8b8g8r8srgbpack32") { return SurfaceFormat::A8B8G8R8SrgbPack32; }
	if (name == "a2r10g10b10unormpack32") { return SurfaceFormat::A2R10G10B10UnormPack32; }
	if (name == "a2r10g10b10snormpack32") { return SurfaceFormat::A2R10G10B10SnormPack32; }
	if (name == "a2r10g10b10uscaledpack32") { return SurfaceFormat::A2R10G10B10UscaledPack32; }
	if (name == "a2r10g10b10sscaledpack32") { return SurfaceFormat::A2R10G10B10SscaledPack32; }
	if (name == "a2r10g10b10uintpack32") { return SurfaceFormat::A2R10G10B10UintPack32; }
	if (name == "a2r10g10b10sintpack32") { return SurfaceFormat::A2R10G10B10SintPack32; }
	if (name == "a2b10g10r10unormpack32") { return SurfaceFormat::A2B10G10R10UnormPack32; }
	if (name == "a2b10g10r10snormpack32") { return SurfaceFormat::A2B10G10R10SnormPack32; }
	if (name == "a2b10g10r10uscaledpack32") { return SurfaceFormat::A2B10G10R10UscaledPack32; }
	if (name == "a2b10g10r10sscaledpack32") { return SurfaceFormat::A2B10G10R10SscaledPack32; }
	if (name == "a2b10g10r10uintpack32") { return SurfaceFormat::A2B10G10R10UintPack32; }
	if (name == "a2b10g10r10sintpack32") { return SurfaceFormat::A2B10G10R10SintPack32; }
	if (name == "r16unorm") { return SurfaceFormat::R16Unorm; }
	if (name == "r16snorm") { return SurfaceFormat::R16Snorm; }
	if (name == "r16uscaled") { return SurfaceFormat::R16Uscaled; }
	if (name == "r16sscaled") { return SurfaceFormat::R16Sscaled; }
	if (name == "r16uint") { return SurfaceFormat::R16Uint; }
	if (name == "r16sint") { return SurfaceFormat::R16Sint; }
	if (name == "r16sfloat") { return SurfaceFormat::R16Sfloat; }
	if (name == "r16g16unorm") { return SurfaceFormat::R16G16Unorm; }
	if (name == "r16g16snorm") { return SurfaceFormat::R16G16Snorm; }
	if (name == "r16g16uscaled") { return SurfaceFormat::R16G16Uscaled; }
	if (name == "r16g16sscaled") { return SurfaceFormat::R16G16Sscaled; }
	if (name == "r16g16uint") { return SurfaceFormat::R16G16Uint; }
	if (name == "r16g16sint") { return SurfaceFormat::R16G16Sint; }
	if (name == "r16g16sfloat") { return SurfaceFormat::R16G16Sfloat; }
	if (name == "r16g16b16unorm") { return SurfaceFormat::R16G16B16Unorm; }
	if (name == "r16g16b16snorm") { return SurfaceFormat::R16G16B16Snorm; }
	if (name == "r16g16b16uscaled") { return SurfaceFormat::R16G16B16Uscaled; }
	if (name == "r16g16b16sscaled") { return SurfaceFormat::R16G16B16Sscaled; }
	if (name == "r16g16b16uint") { return SurfaceFormat::R16G16B16Uint; }
	if (name == "r16g16b16sint") { return SurfaceFormat::R16G16B16Sint; }
	if (name == "r16g16b16sfloat") { return SurfaceFormat::R16G16B16Sfloat; }
	if (name == "r16g16b16a16unorm") { return SurfaceFormat::R16G16B16A16Unorm; }
	if (name == "r16g16b16a16snorm") { return SurfaceFormat::R16G16B16A16Snorm; }
	if (name == "r16g16b16a16uscaled") { return SurfaceFormat::R16G16B16A16Uscaled; }
	if (name == "r16g16b16a16sscaled") { return SurfaceFormat::R16G16B16A16Sscaled; }
	if (name == "r16g16b16a16uint") { return SurfaceFormat::R16G16B16A16Uint; }
	if (name == "r16g16b16a16sint") { return SurfaceFormat::R16G16B16A16Sint; }
	if (name == "r16g16b16a16sfloat") { return SurfaceFormat::R16G16B16A16Sfloat; }
	if (name == "r32uint") { return SurfaceFormat::R32Uint; }
	if (name == "r32sint") { return SurfaceFormat::R32Sint; }
	if (name == "r32sfloat") { return SurfaceFormat::R32Sfloat; }
	if (name == "r32g32uint") { return SurfaceFormat::R32G32Uint; }
	if (name == "r32g32sint") { return SurfaceFormat::R32G32Sint; }
	if (name == "r32g32sfloat") { return SurfaceFormat::R32G32Sfloat; }
	if (name == "r32g32b32uint") { return SurfaceFormat::R32G32B32Uint; }
	if (name == "r32g32b32sint") { return SurfaceFormat::R32G32B32Sint; }
	if (name == "r32g32b32sfloat") { return SurfaceFormat::R32G32B32Sfloat; }
	if (name == "r32g32b32a32uint") { return SurfaceFormat::R32G32B32A32Uint; }
	if (name == "r32g32b32a32sint") { return SurfaceFormat::R32G32B32A32Sint; }
	if (name == "r32g32b32a32sfloat") { return SurfaceFormat::R32G32B32A32Sfloat; }
	if (name == "r64uint") { return SurfaceFormat::R64Uint; }
	if (name == "r64sint") { return SurfaceFormat::R64Sint; }
	if (name == "r64sfloat") { return SurfaceFormat::R64Sfloat; }
	if (name == "r64g64uint") { return SurfaceFormat::R64G64Uint; }
	if (name == "r64g64sint") { return SurfaceFormat::R64G64Sint; }
	if (name == "r64g64sfloat") { return SurfaceFormat::R64G64Sfloat; }
	if (name == "r64g64b64uint") { return SurfaceFormat::R64G64B64Uint; }
	if (name == "r64g64b64sint") { return SurfaceFormat::R64G64B64Sint; }
	if (name == "r64g64b64sfloat") { return SurfaceFormat::R64G64B64Sfloat; }
	if (name == "r64g64b64a64uint") { return SurfaceFormat::R64G64B64A64Uint; }
	if (name == "r64g64b64a64sint") { return SurfaceFormat::R64G64B64A64Sint; }
	if (name == "r64g64b64a64sfloat") { return SurfaceFormat::R64G64B64A64Sfloat; }
	if (name == "b10g11r11ufloatpack32") { return SurfaceFormat::B10G11R11UfloatPack32; }
	if (name == "e5b9g9r9ufloatpack32") { return SurfaceFormat::E5B9G9R9UfloatPack32; }
	if (name == "d16unorm") { return SurfaceFormat::D16Unorm; }
	if (name == "x8d24unormpack32") { return SurfaceFormat::X8D24UnormPack32; }
	if (name == "d32sfloat") { return SurfaceFormat::D32Sfloat; }
	if (name == "s8uint") { return SurfaceFormat::S8Uint; }
	if (name == "d16unorms8uint") { return SurfaceFormat::D16UnormS8Uint; }
	if (name == "d24unorms8uint") { return SurfaceFormat::D24UnormS8Uint; }
	if (name == "d32sfloats8uint") { return SurfaceFormat::D32SfloatS8Uint; }
	if (name == "bc1rgbunormblock") { return SurfaceFormat::Bc1RgbUnormBlock; }
	if (name == "bc1rgbsrgbblock") { return SurfaceFormat::Bc1RgbSrgbBlock; }
	if (name == "bc1rgbaunormblock") { return SurfaceFormat::Bc1RgbaUnormBlock; }
	if (name == "bc1rgbasrgbblock") { return SurfaceFormat::Bc1RgbaSrgbBlock; }
	if (name == "bc2unormblock") { return SurfaceFormat::Bc2UnormBlock; }
	if (name == "bc2srgbblock") { return SurfaceFormat::Bc2SrgbBlock; }
	if (name == "bc3unormblock") { return SurfaceFormat::Bc3UnormBlock; }
	if (name == "bc3srgbblock") { return SurfaceFormat::Bc3SrgbBlock; }
	if (name == "bc4unormblock") { return SurfaceFormat::Bc4UnormBlock; }
	if (name == "bc4snormblock") { return SurfaceFormat::Bc4SnormBlock; }
	if (name == "bc5unormblock") { return SurfaceFormat::Bc5UnormBlock; }
	if (name == "bc5snormblock") { return SurfaceFormat::Bc5SnormBlock; }
	if (name == "bc6hufloatblock") { return SurfaceFormat::Bc6HUfloatBlock; }
	if (name == "bc6hsfloatblock") { return SurfaceFormat::Bc6HSfloatBlock; }
	if (name == "bc7unormblock") { return SurfaceFormat::Bc7UnormBlock; }
	if (name == "bc7srgbblock") { return SurfaceFormat::Bc7SrgbBlock; }
	if (name == "etc2r8g8b8unormblock") { return SurfaceFormat::Etc2R8G8B8UnormBlock; }
	if (name == "etc2r8g8b8srgbblock") { return SurfaceFormat::Etc2R8G8B8SrgbBlock; }
	if (name == "etc2r8g8b8a1unormblock") { return SurfaceFormat::Etc2R8G8B8A1UnormBlock; }
	if (name == "etc2r8g8b8a1srgbblock") { return SurfaceFormat::Etc2R8G8B8A1SrgbBlock; }
	if (name == "etc2r8g8b8a8unormblock") { return SurfaceFormat::Etc2R8G8B8A8UnormBlock; }
	if (name == "etc2r8g8b8a8srgbblock") { return SurfaceFormat::Etc2R8G8B8A8SrgbBlock; }
	if (name == "eacr11unormblock") { return SurfaceFormat::EacR11UnormBlock; }
	if (name == "eacr11snormblock") { return SurfaceFormat::EacR11SnormBlock; }
	if (name == "eacr11g11unormblock") { return SurfaceFormat::EacR11G11UnormBlock; }
	if (name == "eacr11g11snormblock") { return SurfaceFormat::EacR11G11SnormBlock; }
	if (name == "astc4x4unormblock") { return SurfaceFormat::Astc4x4UnormBlock; }
	if (name == "astc4x4srgbblock") { return SurfaceFormat::Astc4x4SrgbBlock; }
	if (name == "astc5x4unormblock") { return SurfaceFormat::Astc5x4UnormBlock; }
	if (name == "astc5x4srgbblock") { return SurfaceFormat::Astc5x4SrgbBlock; }
	if (name == "astc5x5unormblock") { return SurfaceFormat::Astc5x5UnormBlock; }
	if (name == "astc5x5srgbblock") { return SurfaceFormat::Astc5x5SrgbBlock; }
	if (name == "astc6x5unormblock") { return SurfaceFormat::Astc6x5UnormBlock; }
	if (name == "astc6x5srgbblock") { return SurfaceFormat::Astc6x5SrgbBlock; }
	if (name == "astc6x6unormblock") { return SurfaceFormat::Astc6x6UnormBlock; }
	if (name == "astc6x6srgbblock") { return SurfaceFormat::Astc6x6SrgbBlock; }
	if (name == "astc8x5unormblock") { return SurfaceFormat::Astc8x5UnormBlock; }
	if (name == "astc8x5srgbblock") { return SurfaceFormat::Astc8x5SrgbBlock; }
	if (name == "astc8x6unormblock") { return SurfaceFormat::Astc8x6UnormBlock; }
	if (name == "astc8x6srgbblock") { return SurfaceFormat::Astc8x6SrgbBlock; }
	if (name == "astc8x8unormblock") { return SurfaceFormat::Astc8x8UnormBlock; }
	if (name == "astc8x8srgbblock") { return SurfaceFormat::Astc8x8SrgbBlock; }
	if (name == "astc10x5unormblock") { return SurfaceFormat::Astc10x5UnormBlock; }
	if (name == "astc10x5srgbblock") { return SurfaceFormat::Astc10x5SrgbBlock; }
	if (name == "astc10x6unormblock") { return SurfaceFormat::Astc10x6UnormBlock; }
	if (name == "astc10x6srgbblock") { return SurfaceFormat::Astc10x6SrgbBlock; }
	if (name == "astc10x8unormblock") { return SurfaceFormat::Astc10x8UnormBlock; }
	if (name == "astc10x8srgbblock") { return SurfaceFormat::Astc10x8SrgbBlock; }
	if (name == "astc10x10unormblock") { return SurfaceFormat::Astc10x10UnormBlock; }
	if (name == "astc10x10srgbblock") { return SurfaceFormat::Astc10x10SrgbBlock; }
	if (name == "astc12x10unormblock") { return SurfaceFormat::Astc12x10UnormBlock; }
	if (name == "astc12x10srgbblock") { return SurfaceFormat::Astc12x10SrgbBlock; }
	if (name == "astc12x12unormblock") { return SurfaceFormat::Astc12x12UnormBlock; }
	if (name == "astc12x12srgbblock") { return SurfaceFormat::Astc12x12SrgbBlock; }
	if (name == "pvrtc12bppunormblockimg") { return SurfaceFormat::Pvrtc12BppUnormBlockIMG; }
	if (name == "pvrtc14bppunormblockimg") { return SurfaceFormat::Pvrtc14BppUnormBlockIMG; }
	if (name == "pvrtc22bppunormblockimg") { return SurfaceFormat::Pvrtc22BppUnormBlockIMG; }
	if (name == "pvrtc24bppunormblockimg") { return SurfaceFormat::Pvrtc24BppUnormBlockIMG; }
	if (name == "pvrtc12bppsrgbblockimg") { return SurfaceFormat::Pvrtc12BppSrgbBlockIMG; }
	if (name == "pvrtc14bppsrgbblockimg") { return SurfaceFormat::Pvrtc14BppSrgbBlockIMG; }
	if (name == "pvrtc22bppsrgbblockimg") { return SurfaceFormat::Pvrtc22BppSrgbBlockIMG; }
	if (name == "pvrtc24bppsrgbblockimg") { return SurfaceFormat::Pvrtc24BppSrgbBlockIMG; }
	return defaultValue;
}

std::string toString(const SurfaceColorSpace &value) {
	switch (value) {
		default:
		case SurfaceColorSpace::SrgbNonlinear: { return "srgbnonlinear"; }
		case SurfaceColorSpace::DisplayP3NonlinearEXT: { return "displayp3nonlinearext"; }
		case SurfaceColorSpace::ExtendedSrgbLinearEXT: { return "extendedsrgblinearext"; }
		case SurfaceColorSpace::DciP3LinearEXT: { return "dcip3linearext"; }
		case SurfaceColorSpace::DciP3NonlinearEXT: { return "dcip3nonlinearext"; }
		case SurfaceColorSpace::Bt709LinearEXT: { return "bt709linearext"; }
		case SurfaceColorSpace::Bt709NonlinearEXT: { return "bt709nonlinearext"; }
		case SurfaceColorSpace::Bt2020LinearEXT: { return "bt2020linearext"; }
		case SurfaceColorSpace::Hdr10St2084EXT: { return "hdr10st2084ext"; }
		case SurfaceColorSpace::DolbyvisionEXT: { return "dolbyvisionext"; }
		case SurfaceColorSpace::Hdr10HlgEXT: { return "hdr10hlgext"; }
		case SurfaceColorSpace::AdobergbLinearEXT: { return "adobergblinearext"; }
		case SurfaceColorSpace::AdobergbNonlinearEXT: { return "adobergbnonlinearext"; }
		case SurfaceColorSpace::PassThroughEXT: { return "passthroughext"; }
	}
}

SurfaceColorSpace fromString(const std::string_view &name, const SurfaceColorSpace &defaultValue) {
	if (name == "srgbnonlinear") { return SurfaceColorSpace::SrgbNonlinear; }
	if (name == "displayp3nonlinearext") { return SurfaceColorSpace::DisplayP3NonlinearEXT; }
	if (name == "extendedsrgblinearext") { return SurfaceColorSpace::ExtendedSrgbLinearEXT; }
	if (name == "dcip3linearext") { return SurfaceColorSpace::DciP3LinearEXT; }
	if (name == "dcip3nonlinearext") { return SurfaceColorSpace::DciP3NonlinearEXT; }
	if (name == "bt709linearext") { return SurfaceColorSpace::Bt709LinearEXT; }
	if (name == "bt709nonlinearext") { return SurfaceColorSpace::Bt709NonlinearEXT; }
	if (name == "bt2020linearext") { return SurfaceColorSpace::Bt2020LinearEXT; }
	if (name == "hdr10st2084ext") { return SurfaceColorSpace::Hdr10St2084EXT; }
	if (name == "dolbyvisionext") { return SurfaceColorSpace::DolbyvisionEXT; }
	if (name == "hdr10hlgext") { return SurfaceColorSpace::Hdr10HlgEXT; }
	if (name == "adobergblinearext") { return SurfaceColorSpace::AdobergbLinearEXT; }
	if (name == "adobergbnonlinearext") { return SurfaceColorSpace::AdobergbNonlinearEXT; }
	if (name == "passthroughext") { return SurfaceColorSpace::PassThroughEXT; }
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
		window.surfaceFormat(fromString(body.at("/window/surfaceFormat"_json_pointer).get<std::string>(), SurfaceFormat::B8G8R8A8Unorm));
	} catch (const std::out_of_range &/*e*/) {
		window.surfaceFormat(SurfaceFormat::B8G8R8A8Unorm);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.surfaceFormat(SurfaceFormat::B8G8R8A8Unorm);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.surfaceFormat(SurfaceFormat::B8G8R8A8Unorm);
		dirty(true);
	}

	try {
		window.surfaceColorSpace(fromString(body.at("/window/surfaceColorSpace"_json_pointer).get<std::string>(), SurfaceColorSpace::SrgbNonlinear));
	} catch (const std::out_of_range &/*e*/) {
		window.surfaceColorSpace(SurfaceColorSpace::SrgbNonlinear);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.surfaceColorSpace(SurfaceColorSpace::SrgbNonlinear);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.surfaceColorSpace(SurfaceColorSpace::SrgbNonlinear);
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
