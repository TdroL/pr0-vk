#pragma once

#include <functional>
#include <string_view>

#include "../../resources.hpp"
#include "../../types.hpp"

namespace rn::db::loader {

class GlTF {
public:
	struct Upload {
		std::function<void (uint32_t index, std::byte *data, size_t length)> buffer; // upload.buffer(index, size, data)
		std::function<void (uint32_t index, rn::TextureDescription description, rn::TextureDataAccessor &accessor)> texture; // upload.texture(index, { width, height, depth }, pixelFormat, dataFormat, data)
	};

	template<typename T>
	void loadModel(std::string_view filePath, [[maybe_unused]] rn::Resources<T> &resources) {
		Upload upload{
			/*.buffer=*/ [&] (uint32_t index, [[maybe_unused]] std::byte *data, [[maybe_unused]] size_t dataLength) {
				std::string name = std::string{filePath} + "#" + std::to_string(index);
				//
			},
			/*.texture=*/ [&] (uint32_t index, rn::TextureDescription description, rn::TextureDataAccessor &accessor) {
				std::string name = std::string{filePath} + "#" + std::to_string(index);

				[[maybe_unused]] const auto textureHandle = resources.texture.create(name, description);
				[[maybe_unused]] const auto fenceHandle = resources.enqueueTextureUpload(textureHandle, accessor);
			}
		};

		readFile(filePath, upload);
	}

	void readFile(std::string_view filePath, Upload &upload);
};

} // rn::db::loader