#include "gltf.hpp"

#include <nlohmann/json.hpp>

// tinygltf and Draco generate too many warnings...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"

#if defined(ERROR)
	// wingdi.h defines "ERROR" macro and it breaks draco :/
	#undef ERROR
	#define WINGDI_ERROR_UNDEFINED
#endif

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
// #define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_ENABLE_DRACO
#include <tiny_gltf.h>

#if defined(WINGDI_ERROR_UNDEFINED)
	// restore ERROR macro
	#define ERROR 0
	#undef WINGDI_ERROR_UNDEFINED
#endif

#pragma GCC diagnostic pop

#include <ngn/fs.hpp>
#include <ngn/log.hpp>
#include <util/scope.hpp>

#include "../../../ngn/prof.hpp"
#include "../parser/gli.hpp"
#include "../parser/stbi.hpp"

namespace rn::db::loader {

std::string indentValue(const int indent) {
	return std::string(static_cast<size_t>(indent * 2), ' ');
	// std::string s{};
	// for (int i = 0; i < indent; i++) {
	// 	s += "  ";
	// }

	// return s;
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
	GlTF::Uploader &uploader = *static_cast<GlTF::Uploader *>(userData);

	if (rn::db::parser::GLI::isParsable(image->mimeType, image->uri)) {
		double startParseTime = ngn::prof::now();
		rn::db::parser::GLI gliParser{};
		auto resultEither = gliParser.parse(reinterpret_cast<const std::byte *>(data), dataLength);

		if (resultEither.isLeft()) {
			double stopParseTime = ngn::prof::now();
			ngn::log::error("rn::db::loader::details::parseImage({}) => GLI parser error: {} ({}ms)", imageIdx, resultEither.left(), 1000.0 * (stopParseTime - startParseTime));
			*error += fmt::format("image #{} GLI parser error: {}\n", imageIdx, resultEither.left());
			return false;
		} else {
			const auto &stagedTextureResult = resultEither.right();

			rn::TextureDataAccessor accessor{
				/*.data=*/ [&stagedTextureResult] (uint32_t layer, uint32_t level) {
					return stagedTextureResult.data->data(layer, level);
				},
				/*.size=*/ [&stagedTextureResult] (uint32_t level) {
					return stagedTextureResult.data->size(0, level);
				},
				/*.format=*/ [&stagedTextureResult] () {
					return stagedTextureResult.data->format();
				},
			};

			uploader.texture(static_cast<uint32_t>(imageIdx), stagedTextureResult.description, accessor);

			// double stopParseTime = ngn::prof::now();
			// ngn::log::debug("rn::db::loader::details::parseImage({}) => GLI parser done ({}ms)", imageIdx, 1000.0 * (stopParseTime - startParseTime));
			return true;
		}
	} else if (rn::db::parser::Stbi::isParsable(image->mimeType, image->uri)) {
		double startParseTime = ngn::prof::now();
		rn::db::parser::Stbi stbiParser{};
		auto resultEither = stbiParser.parse(reinterpret_cast<const std::byte *>(data), dataLength);

		if (resultEither.isLeft()) {
			double stopParseTime = ngn::prof::now();
			ngn::log::error("rn::db::loader::details::parseImage({}) => stbi parser error: {} ({}ms)", imageIdx, resultEither.left(), 1000.0 * (stopParseTime - startParseTime));
			*error += fmt::format("image #{} stbi parser error: {}\n", imageIdx, resultEither.left());
			return false;
		} else {
			const auto &stagedTextureResult = resultEither.right();

			rn::TextureDataAccessor accessor{
				/*.data=*/ [&stagedTextureResult] (uint32_t layer, uint32_t level) {
					return stagedTextureResult.data->data(layer, level);
				},
				/*.size=*/ [&stagedTextureResult] (uint32_t level) {
					return stagedTextureResult.data->size(0, level);
				},
				/*.format=*/ [&stagedTextureResult] () {
					return stagedTextureResult.data->format();
				},
			};

			uploader.texture(static_cast<uint32_t>(imageIdx), stagedTextureResult.description, accessor);

			// double stopParseTime = ngn::prof::now();
			// ngn::log::debug("rn::db::loader::details::parseImage({}) => stbi parser done ({}ms)", imageIdx, 1000.0 * (stopParseTime - startParseTime));
			return true;
		}
	} else {
		ngn::log::error("rn::db::loader::details::parseImage({}) => unsupported file format: {} ({})", imageIdx, image->mimeType, image->uri);
		*error += fmt::format("image #{} parsing: unsupported file format: {} ({})\n", imageIdx, image->mimeType, image->uri);
		return false;
	}
}

} // details

bool GlTF::readFile(std::string_view filePath, Uploader &uploader) {
	ngn::log::info("rn::db::loader::GlTF::readFile({}) => start", filePath);
	double startParseTime = ngn::prof::now();

	tinygltf::Model model{};
	tinygltf::TinyGLTF loader{};

	loader.SetImageLoader(details::parseImage, &uploader);

	std::string fileExt = ngn::fs::ext(filePath);

	std::string error;
	std::string warning;
	bool result;
	if (fileExt == ".gltf") {
		result = loader.LoadASCIIFromFile(&model, &error, &warning, std::string{filePath});
	} else if (fileExt == ".glb") {
		result = loader.LoadBinaryFromFile(&model, &error, &warning, std::string{filePath});
	} else {
		ngn::log::error("rn::db::loader::GlTF::readFile({}) => unsupported file extension \"{}\"", filePath, fileExt);
		return false;
	}

	if ( ! warning.empty()) {
		ngn::log::warn("rn::db::loader::GlTF::readFile({}) => {}", filePath, warning);
	}

	if ( ! error.empty()) {
		ngn::log::error("rn::db::loader::GlTF::readFile({}) => {}", filePath, error);
	}

	if ( ! result) {
		ngn::log::error("rn::db::loader::GlTF::readFile({}) => failed to load file", filePath);
		return false;
	}

	for (uint32_t index = 0; index < model.buffers.size(); index++) {
		auto &buffer = model.buffers[index];

		rn::BufferDescription description{
			/*.size=*/ buffer.data.size(),
			/*.usage=*/ rn::BufferUsage::Index | rn::BufferUsage::Vertex | rn::BufferUsage::TransferDestination,
			/*.paging=*/ rn::BufferPaging::Static,
		};

		rn::BufferDataAccessor accessor{
				/*.data=*/ [&buffer] () {
					return reinterpret_cast<std::byte *>(buffer.data.data());
				},
				/*.size=*/ [&buffer] () {
					return buffer.data.size();
				},
				/*.format=*/ [] () {
					return rn::DataFormat::Char1;
				},
		};

		uploader.buffer(index, description, accessor);
	}

	// for (size_t i = 0; i < model.buffers.size(); i++) {
	// 	ngn::log::debug("buffer#{}: name={}, data.length={}", i, model.buffers[i].name, model.buffers[i].data.size()); // printValue("extras", model.buffers[i].extras, 2));
	// }

	// for (size_t i = 0; i < model.images.size(); i++) {
	// 	ngn::log::debug("image#{}: name={}, width={}, height={}", i, model.images[i].name, model.images[i].width, model.images[i].height); // printValue("extras", model.buffers[i].extras, 2));
	// }

	double stopParseTime = ngn::prof::now();

	ngn::log::info("rn::db::loader::GlTF::readFile({}) => done ({}ms)", filePath, 1000.0 * (stopParseTime - startParseTime));

	return true;
}

} // rn::db::loader