#include "gli.hpp"

#include <algorithm>
#include <array>
#include <cctype>

#include <fmt/format.h>

#pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
// #pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wtype-limits"

#include <gli/gli.hpp>

#pragma GCC diagnostic pop

// #include "../../../ngn/fs.hpp"

namespace rn::db::parser {

// .dds: image/vnd-ms.dds
// .ktx: image/ktx

class GLITextureDataContainer : public TextureDataContainer {
public:
	gli::texture texture{};
	rn::DataFormat dataFormat = rn::DataFormat::Undefined;
	bool isCube = false;

	GLITextureDataContainer(gli::texture &&texture, rn::DataFormat dataFormat) :
		texture{std::move(texture)},
		dataFormat{dataFormat},
		isCube{texture.target() == gli::target::TARGET_CUBE || texture.target() == gli::target::TARGET_CUBE_ARRAY}
	{}

	std::byte * data(uint32_t layer, uint32_t level) override {
		if (isCube) {
			// ((Layer * this->Faces) + Face) * this->Levels + Level;
			return static_cast<std::byte *>(texture.data(layer / 6, layer % 6, level));
		} else {
			return static_cast<std::byte *>(texture.data(layer, 0, level));
		}
	}

	size_t size([[maybe_unused]] uint32_t layer, uint32_t level) override {
		if (isCube) {
			return texture.size(level);
		} else {
			return texture.size(level);
		}
	}

	rn::DataFormat format() override {
		return dataFormat;
	}

	virtual ~GLITextureDataContainer() {}
};

std::tuple<rn::PixelFormat, rn::DataFormat> translateGLIPixelDataFormat(gli::format format) {
	switch (format) {
		default:
		case gli::format::FORMAT_UNDEFINED: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RG4_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA4_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // B4G4R4A4UNormPack16 + swizzle?
		case gli::format::FORMAT_BGRA4_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::B4G4R4A4UNormPack16, rn::DataFormat::Char2Norm);
		case gli::format::FORMAT_R5G6B5_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // B5G6R5UNormPack16 + swizzle?
		case gli::format::FORMAT_B5G6R5_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::B5G6R5UNormPack16, rn::DataFormat::Char2Norm);
		case gli::format::FORMAT_RGB5A1_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // B5G5R5A1UNormPack16 + swizzle?
		case gli::format::FORMAT_BGR5A1_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::B5G5R5A1UNormPack16, rn::DataFormat::Char2Norm);
		case gli::format::FORMAT_A1RGB5_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // B5G5R5A1UNormPack16 + swizzle?

		case gli::format::FORMAT_R8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::R8UNorm, rn::DataFormat::Char1Norm);
		case gli::format::FORMAT_R8_SNORM_PACK8: return std::make_tuple(rn::PixelFormat::R8SNorm, rn::DataFormat::Char1Norm);
		case gli::format::FORMAT_R8_USCALED_PACK8: return std::make_tuple(rn::PixelFormat::R8UNorm, rn::DataFormat::Char1);
		case gli::format::FORMAT_R8_SSCALED_PACK8: return std::make_tuple(rn::PixelFormat::R8SNorm, rn::DataFormat::Char1);
		case gli::format::FORMAT_R8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::R8UInt, rn::DataFormat::Char1);
		case gli::format::FORMAT_R8_SINT_PACK8: return std::make_tuple(rn::PixelFormat::R8SInt, rn::DataFormat::Char1);
		case gli::format::FORMAT_R8_SRGB_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RG8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::R8G8UNorm, rn::DataFormat::Char2Norm);
		case gli::format::FORMAT_RG8_SNORM_PACK8: return std::make_tuple(rn::PixelFormat::R8G8SNorm, rn::DataFormat::Char2Norm);
		case gli::format::FORMAT_RG8_USCALED_PACK8: return std::make_tuple(rn::PixelFormat::R8G8UNorm, rn::DataFormat::Char2);
		case gli::format::FORMAT_RG8_SSCALED_PACK8: return std::make_tuple(rn::PixelFormat::R8G8SNorm, rn::DataFormat::Char2);
		case gli::format::FORMAT_RG8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::R8G8UInt, rn::DataFormat::Char2);
		case gli::format::FORMAT_RG8_SINT_PACK8: return std::make_tuple(rn::PixelFormat::R8G8SInt, rn::DataFormat::Char2);
		case gli::format::FORMAT_RG8_SRGB_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGB8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?
		case gli::format::FORMAT_RGB8_SNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?
		case gli::format::FORMAT_RGB8_USCALED_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?
		case gli::format::FORMAT_RGB8_SSCALED_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?
		case gli::format::FORMAT_RGB8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?
		case gli::format::FORMAT_RGB8_SINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?
		case gli::format::FORMAT_RGB8_SRGB_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8?

		case gli::format::FORMAT_BGR8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?
		case gli::format::FORMAT_BGR8_SNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?
		case gli::format::FORMAT_BGR8_USCALED_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?
		case gli::format::FORMAT_BGR8_SSCALED_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?
		case gli::format::FORMAT_BGR8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?
		case gli::format::FORMAT_BGR8_SINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?
		case gli::format::FORMAT_BGR8_SRGB_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert to R8G8B8A8 + swizzle?

		case gli::format::FORMAT_RGBA8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::R8G8B8A8UNorm, rn::DataFormat::Char4Norm);
		case gli::format::FORMAT_RGBA8_SNORM_PACK8: return std::make_tuple(rn::PixelFormat::R8G8B8A8SNorm, rn::DataFormat::Char4Norm);
		case gli::format::FORMAT_RGBA8_USCALED_PACK8: return std::make_tuple(rn::PixelFormat::R8G8B8A8UNorm, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_SSCALED_PACK8: return std::make_tuple(rn::PixelFormat::R8G8B8A8SNorm, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::R8G8B8A8UInt, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_SINT_PACK8: return std::make_tuple(rn::PixelFormat::R8G8B8A8SInt, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_SRGB_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_BGRA8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R8G8B8A8UNorm + swizzle?
		case gli::format::FORMAT_BGRA8_SNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R8G8B8A8SNorm + swizzle?
		case gli::format::FORMAT_BGRA8_USCALED_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R8G8B8A8UNorm + swizzle?
		case gli::format::FORMAT_BGRA8_SSCALED_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R8G8B8A8SNorm + swizzle?
		case gli::format::FORMAT_BGRA8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R8G8B8A8UInt + swizzle?
		case gli::format::FORMAT_BGRA8_SINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R8G8B8A8SInt + swizzle?
		case gli::format::FORMAT_BGRA8_SRGB_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGBA8_UNORM_PACK32: return std::make_tuple(rn::PixelFormat::R8G8B8A8UNorm, rn::DataFormat::Char4Norm);
		case gli::format::FORMAT_RGBA8_SNORM_PACK32: return std::make_tuple(rn::PixelFormat::R8G8B8A8SNorm, rn::DataFormat::Char4Norm);
		case gli::format::FORMAT_RGBA8_USCALED_PACK32: return std::make_tuple(rn::PixelFormat::R8G8B8A8UNorm, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_SSCALED_PACK32: return std::make_tuple(rn::PixelFormat::R8G8B8A8SNorm, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_UINT_PACK32: return std::make_tuple(rn::PixelFormat::R8G8B8A8UInt, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_SINT_PACK32: return std::make_tuple(rn::PixelFormat::R8G8B8A8SInt, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGBA8_SRGB_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGB10A2_UNORM_PACK32: return std::make_tuple(rn::PixelFormat::R10G10B10A2UNorm, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGB10A2_SNORM_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB10A2_USCALED_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB10A2_SSCALED_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB10A2_UINT_PACK32: return std::make_tuple(rn::PixelFormat::R10G10B10A2UInt, rn::DataFormat::Char4);
		case gli::format::FORMAT_RGB10A2_SINT_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_BGR10A2_UNORM_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R10G10B10A2UNorm + swizzle?
		case gli::format::FORMAT_BGR10A2_SNORM_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_BGR10A2_USCALED_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_BGR10A2_SSCALED_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_BGR10A2_UINT_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // R10G10B10A2UInt + swizzle?
		case gli::format::FORMAT_BGR10A2_SINT_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_R16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::R16UNorm, rn::DataFormat::UShort1);
		case gli::format::FORMAT_R16_SNORM_PACK16: return std::make_tuple(rn::PixelFormat::R16SNorm, rn::DataFormat::Short1);
		case gli::format::FORMAT_R16_USCALED_PACK16: return std::make_tuple(rn::PixelFormat::R16UNorm, rn::DataFormat::UShort1);
		case gli::format::FORMAT_R16_SSCALED_PACK16: return std::make_tuple(rn::PixelFormat::R16SNorm, rn::DataFormat::Short1);
		case gli::format::FORMAT_R16_UINT_PACK16: return std::make_tuple(rn::PixelFormat::R16UInt, rn::DataFormat::UShort1);
		case gli::format::FORMAT_R16_SINT_PACK16: return std::make_tuple(rn::PixelFormat::R16SInt, rn::DataFormat::Short1);
		case gli::format::FORMAT_R16_SFLOAT_PACK16: return std::make_tuple(rn::PixelFormat::R16Float, rn::DataFormat::Half1);

		case gli::format::FORMAT_RG16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::R16G16UNorm, rn::DataFormat::UShort2);
		case gli::format::FORMAT_RG16_SNORM_PACK16: return std::make_tuple(rn::PixelFormat::R16G16SNorm, rn::DataFormat::Short2);
		case gli::format::FORMAT_RG16_USCALED_PACK16: return std::make_tuple(rn::PixelFormat::R16G16UNorm, rn::DataFormat::UShort2);
		case gli::format::FORMAT_RG16_SSCALED_PACK16: return std::make_tuple(rn::PixelFormat::R16G16SNorm, rn::DataFormat::Short2);
		case gli::format::FORMAT_RG16_UINT_PACK16: return std::make_tuple(rn::PixelFormat::R16G16UInt, rn::DataFormat::UShort2);
		case gli::format::FORMAT_RG16_SINT_PACK16: return std::make_tuple(rn::PixelFormat::R16G16SInt, rn::DataFormat::Short2);
		case gli::format::FORMAT_RG16_SFLOAT_PACK16: return std::make_tuple(rn::PixelFormat::R16G16Float, rn::DataFormat::Half2);

		case gli::format::FORMAT_RGB16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16UNorm + swizzle
		case gli::format::FORMAT_RGB16_SNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16SNorm + swizzle
		case gli::format::FORMAT_RGB16_USCALED_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16UNorm + swizzle
		case gli::format::FORMAT_RGB16_SSCALED_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16SNorm + swizzle
		case gli::format::FORMAT_RGB16_UINT_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16UInt + swizzle
		case gli::format::FORMAT_RGB16_SINT_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16SInt + swizzle
		case gli::format::FORMAT_RGB16_SFLOAT_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined); // convert R16G16B16A16Float + swizzle

		case gli::format::FORMAT_RGBA16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16UNorm, rn::DataFormat::UShort4);
		case gli::format::FORMAT_RGBA16_SNORM_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16SNorm, rn::DataFormat::Short4);
		case gli::format::FORMAT_RGBA16_USCALED_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16UNorm, rn::DataFormat::UShort4);
		case gli::format::FORMAT_RGBA16_SSCALED_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16SNorm, rn::DataFormat::Short4);
		case gli::format::FORMAT_RGBA16_UINT_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16UInt, rn::DataFormat::UShort4);
		case gli::format::FORMAT_RGBA16_SINT_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16SInt, rn::DataFormat::Short4);
		case gli::format::FORMAT_RGBA16_SFLOAT_PACK16: return std::make_tuple(rn::PixelFormat::R16G16B16A16Float, rn::DataFormat::Half4);

		case gli::format::FORMAT_R32_UINT_PACK32: return std::make_tuple(rn::PixelFormat::R32UInt, rn::DataFormat::UInt1);
		case gli::format::FORMAT_R32_SINT_PACK32: return std::make_tuple(rn::PixelFormat::R32SInt, rn::DataFormat::Int1);
		case gli::format::FORMAT_R32_SFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::R32Float, rn::DataFormat::Float1);

		case gli::format::FORMAT_RG32_UINT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32UInt, rn::DataFormat::UInt2);
		case gli::format::FORMAT_RG32_SINT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32SInt, rn::DataFormat::Int2);
		case gli::format::FORMAT_RG32_SFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32Float, rn::DataFormat::Float2);

		case gli::format::FORMAT_RGB32_UINT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32B32UInt, rn::DataFormat::UInt3);
		case gli::format::FORMAT_RGB32_SINT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32B32SInt, rn::DataFormat::Int3);
		case gli::format::FORMAT_RGB32_SFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32B32Float, rn::DataFormat::Float3);

		case gli::format::FORMAT_RGBA32_UINT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32B32A32UInt, rn::DataFormat::UInt4);
		case gli::format::FORMAT_RGBA32_SINT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32B32A32SInt, rn::DataFormat::Int4);
		case gli::format::FORMAT_RGBA32_SFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::R32G32B32A32Float, rn::DataFormat::Float4);

		case gli::format::FORMAT_R64_UINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_R64_SINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_R64_SFLOAT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RG64_UINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RG64_SINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RG64_SFLOAT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGB64_UINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB64_SINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB64_SFLOAT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGBA64_UINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA64_SINT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA64_SFLOAT_PACK64: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RG11B10_UFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::R11G11B10Float, rn::DataFormat::Float1);
		case gli::format::FORMAT_RGB9E5_UFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_D16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::D16UNorm, rn::DataFormat::UShort1);
		case gli::format::FORMAT_D24_UNORM_PACK32: return std::make_tuple(rn::PixelFormat::D24UNormS8UInt, rn::DataFormat::UInt1);
		case gli::format::FORMAT_D32_SFLOAT_PACK32: return std::make_tuple(rn::PixelFormat::D32Float, rn::DataFormat::Float1);
		case gli::format::FORMAT_S8_UINT_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_D16_UNORM_S8_UINT_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_D24_UNORM_S8_UINT_PACK32: return std::make_tuple(rn::PixelFormat::D24UNormS8UInt, rn::DataFormat::UInt1);
		case gli::format::FORMAT_D32_SFLOAT_S8_UINT_PACK64: return std::make_tuple(rn::PixelFormat::D32FloatS8UInt, rn::DataFormat::Float2);

		case gli::format::FORMAT_RGB_DXT1_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::BC1UNorm, rn::DataFormat::UInt2); // 8 bytes per 4x4 block
		case gli::format::FORMAT_RGB_DXT1_SRGB_BLOCK8: return std::make_tuple(rn::PixelFormat::BC1SRGB, rn::DataFormat::UInt2); // 8 bytes per 4x4 block
		case gli::format::FORMAT_RGBA_DXT1_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_DXT1_SRGB_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_DXT3_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::BC2UNorm, rn::DataFormat::UInt4); // 16 bytes per 4x4 block
		case gli::format::FORMAT_RGBA_DXT3_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::BC2SRGB, rn::DataFormat::UInt4); // 16 bytes per 4x4 block
		case gli::format::FORMAT_RGBA_DXT5_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::BC3UNorm, rn::DataFormat::UInt4); // 16 bytes per 4x4 block
		case gli::format::FORMAT_RGBA_DXT5_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::BC3SRGB, rn::DataFormat::UInt4); // 16 bytes per 4x4 block
		case gli::format::FORMAT_R_ATI1N_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::BC4UNorm, rn::DataFormat::UInt2);
		case gli::format::FORMAT_R_ATI1N_SNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::BC4SNorm, rn::DataFormat::UInt2);
		case gli::format::FORMAT_RG_ATI2N_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::BC5UNorm, rn::DataFormat::UInt4);
		case gli::format::FORMAT_RG_ATI2N_SNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::BC5SNorm, rn::DataFormat::UInt4);
		case gli::format::FORMAT_RGB_BP_UFLOAT_BLOCK16: return std::make_tuple(rn::PixelFormat::BC6HUFloat, rn::DataFormat::UInt4); // is it correct?
		case gli::format::FORMAT_RGB_BP_SFLOAT_BLOCK16: return std::make_tuple(rn::PixelFormat::BC6HSFloat, rn::DataFormat::UInt4); // is it correct?
		case gli::format::FORMAT_RGBA_BP_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::BC7UNorm, rn::DataFormat::UInt4); // is it correct?
		case gli::format::FORMAT_RGBA_BP_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::BC7SRGB, rn::DataFormat::UInt4); // is it correct?

		case gli::format::FORMAT_RGB_ETC2_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB_ETC2_SRGB_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ETC2_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ETC2_SRGB_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ETC2_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ETC2_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_R_EAC_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_R_EAC_SNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RG_EAC_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RG_EAC_SNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGBA_ASTC_4X4_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_4X4_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_5X4_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_5X4_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_5X5_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_5X5_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_6X5_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_6X5_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_6X6_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_6X6_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_8X5_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_8X5_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_8X6_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_8X6_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_8X8_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X5_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X5_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X6_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X6_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X8_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X8_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X10_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_10X10_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_12X10_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_12X10_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_12X12_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ASTC_12X12_SRGB_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RGB_ETC_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGB_ATC_UNORM_BLOCK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ATCA_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_RGBA_ATCI_UNORM_BLOCK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_L8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_A8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_LA8_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_L16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_A16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_LA16_UNORM_PACK16: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_BGR8_UNORM_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
		case gli::format::FORMAT_BGR8_SRGB_PACK32: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);

		case gli::format::FORMAT_RG3B2_UNORM_PACK8: return std::make_tuple(rn::PixelFormat::Undefined, rn::DataFormat::Undefined);
	}
}

bool GLI::isParsable(std::string_view mimeType, std::string_view fileName) {
	std::array<std::string_view, 2> mimes{
		/*[0]=*/ "image/vnd-ms.dds",
		/*[1]=*/ "image/ktx"
	};
	std::array<std::string_view, 3> exts{
		/*[0]=*/ ".dds",
		/*[1]=*/ ".ktx",
		/*[2]=*/ ".kmg",
	};

	if ( ! mimeType.empty()) {
		for (auto &&mime : mimes) {
			if (mime == mimeType) {
				return true;
			}
		}
	}

	auto isSame = [] (char a, char b) {
		return std::tolower(a) == std::tolower(b);
	};

	for (auto &&ext : exts) {
		if (fileName.size() < ext.size()) {
			continue;
		}

		if (std::equal(std::rbegin(ext), std::rend(ext), std::rbegin(fileName), isSame)) {
			return true;
		}
	}

	return false;
}

util::EitherOption<std::string, ParsedTextureResult> GLI::parse(const std::byte *data, size_t dataLength) {
	gli::texture tex = gli::load(reinterpret_cast<const char *>(data), dataLength);

	if (tex.empty()) {
		return fmt::format("unable to parse texture data");
	}

	if (gli::is_compressed(tex.format())) {
		return fmt::format("compressed images not supported");
	}

	if (tex.extent().x <= 0 || tex.extent().y <= 0 || tex.extent().z <= 0) {
		return fmt::format("invalid texture size [{}, {}, {}]", tex.extent().x, tex.extent().y, tex.extent().z);
	}

	if (tex.levels() > std::numeric_limits<uint32_t>::max()) {
		return fmt::format("invalid number of levels ({})", tex.levels());
	}

	if (tex.faces() > std::numeric_limits<uint32_t>::max()) {
		return fmt::format("invalid number of faces ({})", tex.faces());
	}

	if (tex.layers() > std::numeric_limits<uint32_t>::max()) {
		return fmt::format("invalid number of layers ({})", tex.layers());
	}

	rn::PixelFormat pixelFormat = rn::PixelFormat::Undefined;
	rn::DataFormat dataFormat = rn::DataFormat::Undefined;

	std::tie(pixelFormat, dataFormat) = translateGLIPixelDataFormat(tex.format());

	rn::ImageType textureType = rn::ImageType::Tex1D;
	switch (tex.target()) {
		default: [[fallthrough]];
		case gli::target::TARGET_1D: [[fallthrough]];
		case gli::target::TARGET_1D_ARRAY: {
			textureType = rn::ImageType::Tex1D;
			break;
		}
		case gli::target::TARGET_CUBE: [[fallthrough]];
		case gli::target::TARGET_CUBE_ARRAY: [[fallthrough]];
		case gli::target::TARGET_2D: [[fallthrough]];
		case gli::target::TARGET_RECT: [[fallthrough]];
		case gli::target::TARGET_2D_ARRAY: [[fallthrough]];
		case gli::target::TARGET_RECT_ARRAY: {
			textureType = rn::ImageType::Tex2D;
			break;
		}
		case gli::target::TARGET_3D: {
			textureType = rn::ImageType::Tex3D;
			break;
		}
	}

	return ParsedTextureResult{
		/*.data=*/ std::unique_ptr<TextureDataContainer>(new GLITextureDataContainer{std::move(tex), dataFormat}),
		/*.description=*/ rn::TextureDescription{
			/*.format=*/ pixelFormat,
			/*.dimensions=*/ rn::Extent3D{
				static_cast<uint32_t>(tex.extent().x),
				static_cast<uint32_t>(tex.extent().y),
				static_cast<uint32_t>(tex.extent().z)
			},
			/*.type=*/ textureType,
			/*.levels=*/ static_cast<uint32_t>(tex.levels()),
			/*.layers=*/ static_cast<uint32_t>(tex.layers() * tex.faces()),
			/*.usage=*/ rn::TextureUsage::Sampled | rn::TextureUsage::TransferDestination,
		},
	};
}

} // rn::db::parser
