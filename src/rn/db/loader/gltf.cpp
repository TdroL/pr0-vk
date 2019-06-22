#include "gltf.hpp"

#include <nlohmann/json.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wconversion"

// something defines "ERROR" macro and it breaks tinygltf :/
#undef ERROR

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
// #define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_ENABLE_DRACO
#include <tiny_gltf.h>

#pragma GCC diagnostic pop

#include <ngn/fs.hpp>
#include <ngn/log.hpp>
#include <util/scope.hpp>

#include "../parser/gli.hpp"
#include "../parser/stbi.hpp"

namespace rn::db::loader {

std::string indentValue(const int indent) {
	std::string s;
	for (int i = 0; i < indent; i++) {
		s += "  ";
	}

	return s;
}

std::string printValue(const std::string &name, const tinygltf::Value &value, const int indent, const bool tag = true) {
	std::stringstream ss;

	if (value.IsObject()) {
		const tinygltf::Value::Object &o = value.Get<tinygltf::Value::Object>();
		tinygltf::Value::Object::const_iterator it(o.begin());
		tinygltf::Value::Object::const_iterator itEnd(o.end());
		for (; it != itEnd; it++) {
			ss << printValue(it->first, it->second, indent + 1) << std::endl;
		}
	} else if (value.IsString()) {
		if (tag) {
			ss << indentValue(indent) << name << " : " << value.Get<std::string>();
		} else {
			ss << " " << value.Get<std::string>() << " ";
		}
	} else if (value.IsBool()) {
		if (tag) {
			ss << indentValue(indent) << name << " : " << value.Get<bool>();
		} else {
			ss << " " << value.Get<bool>() << " ";
		}
	} else if (value.IsNumber()) {
		if (tag) {
			ss << indentValue(indent) << name << " : " << value.Get<double>();
		} else {
			ss << " " << value.Get<double>() << " ";
		}
	} else if (value.IsInt()) {
		if (tag) {
			ss << indentValue(indent) << name << " : " << value.Get<int>();
		} else {
			ss << " " << value.Get<int>() << " ";
		}
	} else if (value.IsArray()) {
		ss << indentValue(indent) << name << " [ ";
		for (size_t i = 0; i < value.Size(); i++) {
			ss << printValue("", value.Get(int(i)), indent + 1, /* tag */ false);
			if (i != (value.ArrayLen() - 1)) {
				ss << ", ";
			}
		}
		ss << indentValue(indent) << "] ";
	}

	// @todo { binary }

	return ss.str();
}

namespace details {

bool parseImage(tinygltf::Image *image, const int imageIdx, std::string *error, [[maybe_unused]] std::string *warning, [[maybe_unused]] int width, [[maybe_unused]] int height, const unsigned char *data, int dataLength, void *userData) {
 if ( ! image->mimeType.empty()) {
		if (image->mimeType != "image/jpeg" && image->mimeType != "image/png" && image->mimeType != "image/bmp" && image->mimeType != "image/gif") {
			ngn::log::error("rn::db::loader::details::parseImage({}) => unsupported mimeType \"{}\"", imageIdx, image->mimeType);
			*error += fmt::format("image #{} parsing: Unsupported mimeType \"{}\"\n", imageIdx, image->mimeType);
			return false;
		}
	} else if ( ! image->uri.empty()) {
		std::string uriExt = ngn::fs::ext(image->uri);
		if (uriExt != ".jpeg" && uriExt != ".jpg" && uriExt != ".png" && uriExt != ".tga" && uriExt != ".bmp" && uriExt != ".psd" && uriExt != ".gif" && uriExt != ".hdr" && uriExt != ".pic" && uriExt != ".pnm") {
			ngn::log::error("rn::db::loader::details::parseImage({}) => unsupported uri extension \"{}\"", imageIdx, uriExt);
			*error += fmt::format("image #{} parsing: unsupported uri extension \"{}\"\n", imageIdx, uriExt);
			return false;
		}
	} else {
		ngn::log::error("rn::db::loader::details::parseImage({}) => uri is empty", imageIdx);
		*error += fmt::format("image #{} parsing: uri is empty\n", imageIdx);
		return false;
	}

	GlTF::Upload &upload = *static_cast<GlTF::Upload *>(userData);

	if (rn::db::parser::GLI::isParsable(image->mimeType, image->uri)) {
		rn::db::parser::GLI gliParser{};
		auto resultEither = gliParser.parse(reinterpret_cast<const std::byte *>(data), dataLength);

		if (resultEither.isLeft()) {
			ngn::log::error("rn::db::loader::details::parseImage({}) => GLI parser error: {}", imageIdx, resultEither.left());
			*error += fmt::format("image #{} GLI parser error: {}\n", imageIdx, resultEither.left());
			return false;
		} else {
			const auto &stagedTextureResult = resultEither.right();

			rn::TextureDataAccessor accessor{
				/*.data=*/ [&] (uint32_t layer, uint32_t level) {
					return stagedTextureResult.data->data(layer, level);
				},
				/*.size=*/ [&] (uint32_t level) {
					return stagedTextureResult.data->size(0, level);
				},
				/*.format=*/ [&] () {
					return stagedTextureResult.data->format();
				},
			};

			upload.texture(imageIdx, stagedTextureResult.description, accessor);

			return true;
		}
	} else if (rn::db::parser::Stbi::isParsable(image->mimeType, image->uri)) {
		rn::db::parser::Stbi stbiParser{};
		auto resultEither = stbiParser.parse(reinterpret_cast<const std::byte *>(data), dataLength);

		if (resultEither.isLeft()) {
			ngn::log::error("rn::db::loader::details::parseImage({}) => stbi parser error: {}", imageIdx, resultEither.left());
			*error += fmt::format("image #{} stbi parser error: {}\n", imageIdx, resultEither.left());
			return false;
		} else {
			const auto &stagedTextureResult = resultEither.right();

			rn::TextureDataAccessor accessor{
				/*.data=*/ [&] (uint32_t layer, uint32_t level) {
					return stagedTextureResult.data->data(layer, level);
				},
				/*.size=*/ [&] (uint32_t level) {
					return stagedTextureResult.data->size(0, level);
				},
				/*.format=*/ [&] () {
					return stagedTextureResult.data->format();
				},
			};

			upload.texture(imageIdx, stagedTextureResult.description, accessor);

			return true;
		}
	} else {
		ngn::log::error("rn::db::loader::details::parseImage({}) => unsupported file format: {} ({})", imageIdx, image->mimeType, image->uri);
		*error += fmt::format("image #{} parsing: unsupported file format: {} ({})\n", imageIdx, image->mimeType, image->uri);
		return false;
	}

	// if (stbi_is_hdr_from_memory(data, dataLength)) {
	// 	int x = -1;
	// 	int y = -1;
	// 	int components = -1;
	// 	float *parsedData = stbi_loadf_from_memory(data, dataLength, &x, &y, &components, 0);
	// 	const auto scopeOnExitFreeImage = util::createScopeExit([&] () {
	// 		if (parsedData != nullptr) {
	// 			stbi_image_free(parsedData);
	// 		}
	// 	});

	// 	if (x <= 0 || y <= 0) {
	// 		ngn::log::error("rn::db::loader::details::parseImage({}) => (32b) invalid texture dimensions: {}, {}", imageIdx, x, y);
	// 		*error += fmt::format("invalid texture dimensions: {}, {}\n", imageIdx, x, y);
	// 		return false;
	// 	}

	// 	rn::PixelFormat pixelFormat{rn::PixelFormat::Undefined};
	// 	rn::DataFormat dataFormat{rn::DataFormat::Undefined};
	// 	switch (components) {
	// 		case 1: {
	// 			pixelFormat = rn::PixelFormat::R32Float;
	// 			dataFormat = rn::DataFormat::Float1;
	// 			break;
	// 		}
	// 		case 2: {
	// 			pixelFormat = rn::PixelFormat::R32G32Float;
	// 			dataFormat = rn::DataFormat::Float2;
	// 			break;
	// 		}
	// 		case 3: {
	// 			pixelFormat = rn::PixelFormat::R32G32B32Float;
	// 			dataFormat = rn::DataFormat::Float3;
	// 			break;
	// 		}
	// 		case 4: {
	// 			pixelFormat = rn::PixelFormat::R32G32B32A32Float;
	// 			dataFormat = rn::DataFormat::Float4;
	// 			break;
	// 		}
	// 		default: {
	// 			ngn::log::error("rn::db::loader::details::parseImage({}) => (32b) invalid number of texture color components: {}", imageIdx, components);
	// 			*error += fmt::format("invalid number of texture color components: {}\n", imageIdx, components);
	// 			return false;
	// 		}
	// 	}

	// 	upload.texture(imageIdx, { static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u }, pixelFormat, dataFormat, reinterpret_cast<std::byte *>(parsedData));
	// } else if (stbi_is_16_bit_from_memory(data, dataLength)) {
	// 	int x = -1;
	// 	int y = -1;
	// 	int components = -1;
	// 	if ( ! stbi_info_from_memory(data, dataLength, &x, &y, &components)) {
	// 		ngn::log::error("rn::db::loader::details::parseImage({}) => (16b) failed to read info", imageIdx);
	// 		*error += fmt::format("failed to read info\n", imageIdx);
	// 		return false;
	// 	}

	// 	int outputComponents = components == 3 ? 4 : components;

	// 	stbi_us *parsedData = stbi_load_16_from_memory(data, dataLength, &x, &y, &components, outputComponents);
	// 	const auto scopeOnExitFreeImage = util::createScopeExit([&] () {
	// 		if (parsedData != nullptr) {
	// 			stbi_image_free(parsedData);
	// 		}
	// 	});

	// 	if (x <= 0 || y <= 0) {
	// 		ngn::log::error("rn::db::loader::details::parseImage({}) => (16b) invalid texture dimensions: {}, {}", imageIdx, x, y);
	// 		*error += fmt::format("invalid texture dimensions: {}, {}\n", imageIdx, x, y);
	// 		return false;
	// 	}

	// 	rn::PixelFormat pixelFormat{rn::PixelFormat::Undefined};
	// 	rn::DataFormat dataFormat{rn::DataFormat::Undefined};
	// 	switch (outputComponents) {
	// 		case 1: {
	// 			pixelFormat = rn::PixelFormat::R16UInt;
	// 			dataFormat = rn::DataFormat::UShort1;
	// 			break;
	// 		}
	// 		case 2: {
	// 			pixelFormat = rn::PixelFormat::R16G16UInt;
	// 			dataFormat = rn::DataFormat::UShort2;
	// 			break;
	// 		}
	// 		case 4: {
	// 			pixelFormat = rn::PixelFormat::R16G16B16A16UInt;
	// 			dataFormat = rn::DataFormat::UShort4;
	// 			break;
	// 		}
	// 		default: {
	// 			ngn::log::error("rn::db::loader::details::parseImage({}) => (16b) invalid number of texture color components: {}", imageIdx, outputComponents);
	// 			*error += fmt::format("invalid number of texture color components: {}\n", imageIdx, outputComponents);
	// 			return false;
	// 		}
	// 	}

	// 	upload.texture(imageIdx, { static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u }, pixelFormat, dataFormat, reinterpret_cast<std::byte *>(parsedData));
	// } else {
	// 	int x = -1;
	// 	int y = -1;
	// 	int components = -1;
	// 	if ( ! stbi_info_from_memory(data, dataLength, &x, &y, &components)) {
	// 		ngn::log::error("rn::db::loader::details::parseImage({}) => (8b) failed to read info", imageIdx);
	// 		*error += fmt::format("failed to read info\n", imageIdx);
	// 		return false;
	// 	}

	// 	int outputComponents = components == 3 ? 4 : components;
	// 	stbi_uc *parsedData = stbi_load_from_memory(data, dataLength, &x, &y, &components, outputComponents);
	// 	const auto scopeOnExitFreeImage = util::createScopeExit([&] () {
	// 		if (parsedData != nullptr) {
	// 			stbi_image_free(parsedData);
	// 		}
	// 	});

	// 	if (x <= 0 || y <= 0) {
	// 		ngn::log::error("rn::db::loader::details::parseImage({}) => (8b) invalid texture dimensions: {}, {}", imageIdx, x, y);
	// 		*error += fmt::format("invalid texture dimensions: {}, {}\n", imageIdx, x, y);
	// 		return false;
	// 	}

	// 	rn::PixelFormat pixelFormat{rn::PixelFormat::Undefined};
	// 	rn::DataFormat dataFormat{rn::DataFormat::Undefined};
	// 	switch (outputComponents) {
	// 		case 1: {
	// 			pixelFormat = rn::PixelFormat::R8UInt;
	// 			dataFormat = rn::DataFormat::UChar1;
	// 			break;
	// 		}
	// 		case 2: {
	// 			pixelFormat = rn::PixelFormat::R8G8UInt;
	// 			dataFormat = rn::DataFormat::UChar2;
	// 			break;
	// 		}
	// 		case 4: {
	// 			pixelFormat = rn::PixelFormat::R8G8B8A8UInt;
	// 			dataFormat = rn::DataFormat::UChar4;
	// 			break;
	// 		}
	// 		default: {
	// 			ngn::log::error("rn::db::loader::details::parseImage({}) => (8b) invalid number of texture color components: {}", imageIdx, outputComponents);
	// 			*error += fmt::format("invalid number of texture color components: {}\n", imageIdx, outputComponents);
	// 			return false;
	// 		}
	// 	}

	// 	upload.texture(imageIdx, { static_cast<uint32_t>(x), static_cast<uint32_t>(y), 1u }, pixelFormat, dataFormat, reinterpret_cast<std::byte *>(parsedData));
	// }

	return true;
}

} // details

void GlTF::readFile(std::string_view filePath, Upload &upload) {
	ngn::log::info("rn::db::loader::GlTF::readFile({}) => start", filePath);

	tinygltf::Model model{};
	tinygltf::TinyGLTF loader{};

	loader.SetImageLoader(details::parseImage, &upload);
	// loader.SetImageLoader([] ([[maybe_unused]]  tinygltf::Image *image, [[maybe_unused]] const int imageIdx, [[maybe_unused]] std::string *error, [[maybe_unused]] std::string *warning, [[maybe_unused]] int width, [[maybe_unused]] int height, [[maybe_unused]] const unsigned char *data, [[maybe_unused]] int dataLength, [[maybe_unused]] void *userData) {
	// 	ngn::log::debug("parseImage: image={}", fmt::ptr(image));
	// 	if (image) {
	// 		ngn::log::debug("parseImage: image->mimeType={}", image->mimeType);
	// 		ngn::log::debug("parseImage: image->pixel_type={}", image->pixel_type);
	// 		ngn::log::debug("parseImage: image->width={}", image->width);
	// 		ngn::log::debug("parseImage: image->height={}", image->height);
	// 	}

	// 	int x, y, comp;
	// 	int infoResult = stbi_info_from_memory(data, dataLength, &x, &y, &comp);
	// 	ngn::log::debug("parseImage: stbi_info_from_memory={} {} {} {}", infoResult, x, y, comp);
	// 	ngn::log::debug("parseImage: imageIdx={}", imageIdx);
	// 	ngn::log::debug("parseImage: width={}", width);
	// 	ngn::log::debug("parseImage: height={}", height);
	// 	ngn::log::debug("parseImage: data={}", fmt::ptr(data));
	// 	ngn::log::debug("parseImage: dataLength={}", dataLength);
	// 	ngn::log::debug("parseImage: userData={}", fmt::ptr(userData));
	// 	return true;
	// }, nullptr);

	std::string_view fileExt = ngn::fs::ext(filePath);

	std::string error;
	std::string warning;

	bool result;
	if (fileExt == ".gltf") {
		result = loader.LoadASCIIFromFile(&model, &error, &warning, std::string{filePath});
	} else if (fileExt == ".glb") {
		result = loader.LoadBinaryFromFile(&model, &error, &warning, std::string{filePath});
	} else {
		ngn::log::error("rn::db::loader::GlTF::readFile({}) => unsupported file extension \"{}\"", filePath, fileExt);
		return;
	}

	if ( ! warning.empty()) {
		ngn::log::warn("rn::db::loader::GlTF::readFile({}) => {}", filePath, warning);
	}

	if ( ! error.empty()) {
		ngn::log::error("rn::db::loader::GlTF::readFile({}) => {}", filePath, error);
	}

	if ( ! result) {
		ngn::log::error("rn::db::loader::GlTF::readFile({}) => failed to load file", filePath);
		return;
	}

	for (size_t i = 0; i < model.buffers.size(); i++) {
		ngn::log::debug("buffer#{}: name={}, data.length={}", i, model.buffers[i].name, model.buffers[i].data.size()); // printValue("extras", model.buffers[i].extras, 2));
	}

	for (size_t i = 0; i < model.images.size(); i++) {
		ngn::log::debug("image#{}: name={}, width={}, height={}", i, model.images[i].name, model.images[i].width, model.images[i].height); // printValue("extras", model.buffers[i].extras, 2));
	}

	ngn::log::info("rn::db::loader::GlTF::readFile({}) => done", filePath);
}

} // rn::db::loader