#pragma once

#include <mutex>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../memory/pool.hpp"
#include "texture/handle.hpp"
#include "texture/loader.hpp"

namespace rn::vki::db {

enum class TextureEntryStatus {
	UNUSED,
	PENDING,
	STAGED,
	TRANSFERING,
	READY,
	INVALID_FILE,
	INVALID_FORMAT,
	INVALID_DATA,
	CANCELLED,
	READER_FAILED,
	TRANSFERRER_FAILED,
};

struct TextureEntry {
	std::string source{};
	TextureEntryStatus status{TextureEntryStatus::UNUSED};

	vk::Format format{vk::Format::eUndefined};
	vk::ImageType imageType{vk::ImageType::e1D};
	bool cubeCompatible = false;
	uint32_t layers = 0;
	uint32_t faces = 0;
	uint32_t levels = 0;
	std::vector<vk::Extent3D> extents{};
	vk::ComponentMapping swizzles{};

	vk::UniqueImage image{};
	memory::Handle imageMemory{};

	// ref counter
	uint32_t counter = 0;
};

class Texture {
public:
	std::vector<TextureEntry> entries{};
	std::mutex entriesMutex{};

	texture::Handle emplace(const std::string_view &source);

	void acquire(uint32_t index);
	void release(uint32_t index);

	template<class T>
	auto map(uint32_t index, T callback) -> decltype(callback(entries[index])) {
		std::lock_guard lock{entriesMutex};

		if (entries.size() < index) {
			entries.resize(index + 1);
		}

		return callback(entries[index]);
	}

	template<class T>
	auto map(const texture::Handle &handle, T callback) -> decltype(map(handle.index, std::forward<T>(callback))) {
		return map(handle.index, std::forward<T>(callback));
	}

	template<class T>
	auto map(T callback) -> decltype(callback()) {
		std::lock_guard lock{entriesMutex};

		return callback();
	}
};

} // rn::vki::db
