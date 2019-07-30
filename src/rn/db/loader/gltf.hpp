#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "../../../ngn/prof.hpp"
#include "../../../ngn/log.hpp"
#include "../../resources.hpp"
#include "../../types.hpp"
#include "../model.hpp"
#include "../buffer.hpp"
#include "../texture.hpp"
#include "types.hpp"

namespace rn::db::loader {

class GlTF {
public:
	struct Uploader {
		std::function<void (uint32_t index, const rn::TextureDescription &description, rn::TextureDataAccessor &accessor)> texture; // upload.texture(index, { width, height, depth }, pixelFormat, dataFormat, data)
		std::function<void (uint32_t index, const rn::BufferDescription &description, rn::BufferDataAccessor &accessor)> buffer; // upload.buffer(index, size, data)
	};

	template<typename T>
	std::optional<rn::db::Model<T>> loadModel(std::string_view filePath, rn::Resources<T> &resources) {
		std::vector<rn::db::Texture<T>> textures{};
		std::vector<rn::db::Buffer<T>> buffers{};

		Uploader uploader{
			/*.texture=*/ [&] (uint32_t index, const rn::TextureDescription &description, rn::TextureDataAccessor &accessor) {
				// double startTime = ngn::prof::now();
				std::string name = std::string{filePath} + "#" + std::to_string(index);

				uint32_t resultIdx = index;
				if (resultIdx >= textures.size()) {
					if (resultIdx >= textures.capacity()) {
						textures.reserve(textures.capacity() * 2);
					}

					textures.resize(resultIdx + 1);
				}

				textures[resultIdx] = { resources, resources.texture.create(name, description) };
				textures[resultIdx].fenceStamp = resources.enqueueTextureUpload(textures[resultIdx].handle, accessor);
			},
			/*.buffer=*/ [&] (uint32_t index, const rn::BufferDescription &description, rn::BufferDataAccessor &accessor) {
				// double startTime = ngn::prof::now();
				std::string name = std::string{filePath} + "#" + std::to_string(index);

				uint32_t resultIdx = index;
				if (resultIdx >= buffers.size()) {
					if (resultIdx >= buffers.capacity()) {
						buffers.reserve(buffers.capacity() * 2);
					}

					buffers.resize(resultIdx + 1);
				}

				buffers[resultIdx] = { resources, resources.buffer.create(name, description) };
				buffers[resultIdx].fenceStamp = resources.enqueueBufferUpload(buffers[resultIdx].handle, accessor);
			}
		};

		if (readFile(filePath, uploader)) {
			return rn::db::Model<T>{ std::string{filePath}, std::move(textures), std::move(buffers) };
		} else {
			return std::nullopt;
		}
	}

	bool readFile(std::string_view filePath, Uploader &uploader);
};

} // rn::db::loader