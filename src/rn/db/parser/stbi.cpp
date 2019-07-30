#include "stbi.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <limits>

#include <fmt/format.h>
#include <stb/stb_image.h>

// #include "../../../ngn/fs.hpp"
// #include "../../../ngn/log.hpp"
// #include "../../../util/scope.hpp"

namespace rn::db::parser {

class StbiTextureDataContainer : public TextureDataContainer {
public:
	std::unique_ptr<std::byte[], void (*)(void *)> dataPtr{nullptr, stbi_image_free};
	size_t dataLength = 0;
	rn::DataFormat dataFormat = rn::DataFormat::Undefined;

	StbiTextureDataContainer(std::unique_ptr<std::byte[], void (*)(void *)> &&dataPtr, size_t dataLength, rn::DataFormat dataFormat) :
		dataPtr{std::move(dataPtr)},
		dataLength{dataLength},
		dataFormat{dataFormat}
	{}

	std::byte * data(uint32_t layer, uint32_t level) override {
		if (layer > 0 || level > 0) {
			return nullptr;
		} else {
			return dataPtr.get();
		}
	}

	size_t size(uint32_t layer, uint32_t level) override {
		if (layer > 0 || level > 0) {
			return 0;
		} else {
			return dataLength;
		}
	}

	rn::DataFormat format() override {
		return dataFormat;
	}

	virtual ~StbiTextureDataContainer() {}
};

// .bmp: image/bmp
// .gif: image/gif
// .hdr: image/vnd.radiance
// .png: image/png
// .jpeg .jpg: image/jpeg
// .pbm .pgm .ppm .pnm: image/x‑portable‑bitmap image/x‑portable‑graymap image/x‑portable‑pixmap image/x‑portable‑anymap
// .tga: image/x-targa image/x-tga
// .psd: image/vnd.adobe.photoshop
// .pic .pict: image/pict

bool Stbi::isParsable(std::string_view mimeType, std::string_view fileName) {
	std::array<std::string_view, 13> mimes{
		/*[0]=*/ "image/bmp",
		/*[1]=*/ "image/gif",
		/*[2]=*/ "image/vnd.radiance",
		/*[3]=*/ "image/png",
		/*[4]=*/ "image/jpeg",
		/*[5]=*/ "image/x‑portable‑bitmap",
		/*[6]=*/ "image/x‑portable‑graymap",
		/*[7]=*/ "image/x‑portable‑pixmap",
		/*[8]=*/ "image/x‑portable‑anymap",
		/*[9]=*/ "image/x-targa",
		/*[10]=*/ "image/x-tga",
		/*[11]=*/ "image/vnd.adobe.photoshop",
		/*[12]=*/ "image/pict"
	};
	std::array<std::string_view, 14> exts{
		/*[0]=*/ ".bmp",
		/*[1]=*/ ".gif",
		/*[2]=*/ ".hdr",
		/*[3]=*/ ".png",
		/*[4]=*/ ".jpeg",
		/*[5]=*/ ".jpg",
		/*[6]=*/ ".pbm",
		/*[7]=*/ ".pgm",
		/*[8]=*/ ".ppm",
		/*[9]=*/ ".pnm",
		/*[10]=*/ ".tga",
		/*[11]=*/ ".psd",
		/*[12]=*/ ".pic",
		/*[13]=*/ ".pict"
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

util::EitherOption<std::string, ParsedTextureResult> Stbi::parse(const std::byte *data, size_t dataLength) {
	// GlTF::Upload &upload = *static_cast<GlTF::Upload *>(userData);

	if (dataLength > std::numeric_limits<int>::max()) {
		// ngn::log::error("rn::db::parser::Stbi::parse({}) => data length is too big: {}", dataLength);
		return fmt::format("data length is too big: {}", dataLength);
	}

	const stbi_uc * dataStbi = reinterpret_cast<const stbi_uc *>(data);
	int dataLengthStbi = static_cast<int>(dataLength);

	if (stbi_is_hdr_from_memory(dataStbi, dataLengthStbi)) {
		int x = -1;
		int y = -1;
		int components = -1;
		// float *parsedData = stbi_loadf_from_memory(dataStbi, dataLengthStbi, &x, &y, &components, 0);
		// const auto scopeOnExitFreeImage = util::createScopeExit([&] () {
		// 	if (parsedData != nullptr) {
		// 		stbi_image_free(parsedData);
		// 	}
		// });

		std::unique_ptr<std::byte[], void (*)(void *)> parsedData{reinterpret_cast<std::byte *>(stbi_loadf_from_memory(dataStbi, dataLengthStbi, &x, &y, &components, 0)), stbi_image_free};

		if (x <= 0 || y <= 0) {
			// ngn::log::error("rn::db::parser::Stbi::parse({}) => (32b) invalid texture dimensions: {}, {}", imageIdx, x, y);
			// *error += fmt::format("invalid texture dimensions: {}, {}\n", imageIdx, x, y);
			// return false;
			return fmt::format("[32b] invalid texture dimensions: {}, {}", x, y);
		}

		rn::PixelFormat pixelFormat{rn::PixelFormat::Undefined};
		rn::DataFormat dataFormat{rn::DataFormat::Undefined};
		size_t parsedDataLength = x * y * /*depth*/ 1 * /*layers*/ 1;
		switch (components) {
			case 1: {
				pixelFormat = rn::PixelFormat::R32Float;
				dataFormat = rn::DataFormat::Float1;
				parsedDataLength *= 4 * 1;
				break;
			}
			case 2: {
				pixelFormat = rn::PixelFormat::R32G32Float;
				dataFormat = rn::DataFormat::Float2;
				parsedDataLength *= 4 * 2;
				break;
			}
			case 3: {
				pixelFormat = rn::PixelFormat::R32G32B32Float;
				dataFormat = rn::DataFormat::Float3;
				parsedDataLength *= 4 * 3;
				break;
			}
			case 4: {
				pixelFormat = rn::PixelFormat::R32G32B32A32Float;
				dataFormat = rn::DataFormat::Float4;
				parsedDataLength *= 4 * 4;
				break;
			}
			default: {
				// ngn::log::error("rn::db::parser::Stbi::parse({}) => (32b) invalid number of texture color components: {}", imageIdx, components);
				// *error += fmt::format("invalid number of texture color components: {}\n", imageIdx, components);
				// return false;
				return fmt::format("[32b] invalid number of texture color components: {}", components);
			}
		}

		// upload.texture(imageIdx, { static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u }, pixelFormat, dataFormat, reinterpret_cast<std::byte *>(parsedData));
		return ParsedTextureResult{
			/*.data=*/ std::unique_ptr<TextureDataContainer>(new StbiTextureDataContainer{std::move(parsedData), parsedDataLength, dataFormat}),
			/*.description=*/ rn::TextureDescription{
				/*.format=*/ pixelFormat,
				/*.dimensions=*/ rn::Extent3D{static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u},
				/*.type=*/ rn::ImageType::Tex2D,
				/*.levels=*/ 1,
				/*.layers=*/ 1,
				/*.usage=*/ rn::TextureUsage::Sampled,
			},
		};
	} else if (stbi_is_16_bit_from_memory(dataStbi, dataLengthStbi)) {
		int x = -1;
		int y = -1;
		int components = -1;
		if ( ! stbi_info_from_memory(dataStbi, dataLengthStbi, &x, &y, &components)) {
			// ngn::log::error("rn::db::parser::Stbi::parse({}) => (16b) failed to read info", imageIdx);
			// *error += fmt::format("failed to read image info\n", imageIdx);
			// return false;
			return fmt::format("[16b] failed to read image info");
		}

		int outputComponents = components == 3 ? 4 : components;

		// stbi_us *parsedData = stbi_load_16_from_memory(dataStbi, dataLengthStbi, &x, &y, &components, outputComponents);
		// const auto scopeOnExitFreeImage = util::createScopeExit([&] () {
		// 	if (parsedData != nullptr) {
		// 		stbi_image_free(parsedData);
		// 	}
		// });

		std::unique_ptr<std::byte[], void (*)(void *)> parsedData{reinterpret_cast<std::byte *>(stbi_load_16_from_memory(dataStbi, dataLengthStbi, &x, &y, &components, outputComponents)), stbi_image_free};

		if (x <= 0 || y <= 0) {
			// ngn::log::error("rn::db::parser::Stbi::parse({}) => (16b) invalid texture dimensions: {}, {}", imageIdx, x, y);
			// *error += fmt::format("invalid texture dimensions: {}, {}\n", imageIdx, x, y);
			// return false;
			return fmt::format("[16b] invalid texture dimensions: {}, {}", x, y);
		}

		rn::PixelFormat pixelFormat{rn::PixelFormat::Undefined};
		rn::DataFormat dataFormat{rn::DataFormat::Undefined};
		size_t parsedDataLength = x * y * /*depth*/ 1 * /*layers*/ 1;
		switch (outputComponents) {
			case 1: {
				pixelFormat = rn::PixelFormat::R16UInt;
				dataFormat = rn::DataFormat::UShort1;
				parsedDataLength *= 2 * 1;
				break;
			}
			case 2: {
				pixelFormat = rn::PixelFormat::R16G16UInt;
				dataFormat = rn::DataFormat::UShort2;
				parsedDataLength *= 2 * 2;
				break;
			}
			case 4: {
				pixelFormat = rn::PixelFormat::R16G16B16A16UInt;
				dataFormat = rn::DataFormat::UShort4;
				parsedDataLength *= 2 * 4;
				break;
			}
			default: {
				// ngn::log::error("rn::db::parser::Stbi::parse({}) => (16b) invalid number of texture color components: {}", imageIdx, outputComponents);
				// *error += fmt::format("invalid number of texture color components: {}\n", imageIdx, outputComponents);
				// return false;
				return fmt::format("[16b] invalid number of texture color components: {}", outputComponents);
			}
		}

		// upload.texture(imageIdx, { static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u }, pixelFormat, dataFormat, reinterpret_cast<std::byte *>(parsedData));
		return ParsedTextureResult{
			/*.data=*/ std::unique_ptr<TextureDataContainer>(new StbiTextureDataContainer{std::move(parsedData), parsedDataLength, dataFormat}),
			/*.description=*/ rn::TextureDescription{
				/*.format=*/ pixelFormat,
				/*.dimensions=*/ rn::Extent3D{static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u},
				/*.type=*/ rn::ImageType::Tex2D,
				/*.levels=*/ 1,
				/*.layers=*/ 1,
				/*.usage=*/ rn::TextureUsage::Sampled,
			},
		};
	} else {
		int x = -1;
		int y = -1;
		int components = -1;
		if ( ! stbi_info_from_memory(dataStbi, dataLengthStbi, &x, &y, &components)) {
			// ngn::log::error("rn::db::parser::Stbi::parse({}) => (8b) failed to read info", imageIdx);
			// *error += fmt::format("failed to read info\n", imageIdx);
			// return false;
			return fmt::format("[8b] failed to read image info");
		}

		int outputComponents = components == 3 ? 4 : components;
		// stbi_uc *parsedData = stbi_load_from_memory(dataStbi, dataLengthStbi, &x, &y, &components, outputComponents);
		// const auto scopeOnExitFreeImage = util::createScopeExit([&] () {
		// 	if (parsedData != nullptr) {
		// 		stbi_image_free(parsedData);
		// 	}
		// });
		std::unique_ptr<std::byte[], void (*)(void *)> parsedData{reinterpret_cast<std::byte *>(stbi_load_from_memory(dataStbi, dataLengthStbi, &x, &y, &components, outputComponents)), stbi_image_free};

		if (x <= 0 || y <= 0) {
			// ngn::log::error("rn::db::parser::Stbi::parse({}) => (8b) invalid texture dimensions: {}, {}", imageIdx, x, y);
			// *error += fmt::format("invalid texture dimensions: {}, {}\n", imageIdx, x, y);
			// return false;
			return fmt::format("[8b] invalid texture dimensions: {}, {}", x, y);
		}

		rn::PixelFormat pixelFormat{rn::PixelFormat::Undefined};
		rn::DataFormat dataFormat{rn::DataFormat::Undefined};
		size_t parsedDataLength = x * y * /*depth*/ 1 * /*layers*/ 1;
		switch (outputComponents) {
			case 1: {
				pixelFormat = rn::PixelFormat::R8UInt;
				dataFormat = rn::DataFormat::UChar1;
				parsedDataLength *= 1 * 1;
				break;
			}
			case 2: {
				pixelFormat = rn::PixelFormat::R8G8UInt;
				dataFormat = rn::DataFormat::UChar2;
				parsedDataLength *= 2 * 1;
				break;
			}
			case 4: {
				pixelFormat = rn::PixelFormat::R8G8B8A8UInt;
				dataFormat = rn::DataFormat::UChar4;
				parsedDataLength *= 4 * 1;
				break;
			}
			default: {
				// ngn::log::error("rn::db::parser::Stbi::parse({}) => (8b) invalid number of texture color components: {}", imageIdx, outputComponents);
				// return false;
				return fmt::format("[8b] invalid number of texture color components: {}", outputComponents);
			}
		}

		// upload.texture(imageIdx, { static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u }, pixelFormat, dataFormat, reinterpret_cast<std::byte *>(parsedData));
		return ParsedTextureResult{
			/*.data=*/ std::unique_ptr<TextureDataContainer>(new StbiTextureDataContainer{std::move(parsedData), parsedDataLength, dataFormat}),
			/*.description=*/ rn::TextureDescription{
				/*.format=*/ pixelFormat,
				/*.dimensions=*/ rn::Extent3D{
					static_cast<uint32_t>(x),
					static_cast<uint32_t>(y),
					1u
				},
				/*.type=*/ rn::ImageType::Tex2D,
				/*.levels=*/ 1,
				/*.layers=*/ 1,
				/*.usage=*/ rn::TextureUsage::Sampled | rn::TextureUsage::TransferDestination,
			},
		};
	}
}

} // rn::db::parser
