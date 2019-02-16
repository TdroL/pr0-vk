// #pragma once

// #include <cstdint>
// #include <optional>
// #include <vector>

// #include "../../util/flatStorage.hpp"
// #include "../context.hpp"
// #include "../types.hpp"

// namespace rn::db::texture {

// struct Handle {
// 	using HandleType = uint32_t;

// 	HandleType handle;
// };
// static_assert(sizeof(rn::db::texture::Handle) == sizeof(uint32_t));

// struct Description {
// 	rn::PixelFormat format;
// 	rn::Extent3D dimensions;
// 	rn::ImageType type{rn::ImageType::Tex2D};
// 	uint32_t levels{1};
// 	uint32_t layers{1};
// 	rn::TextureUsage usage{rn::TextureUsage::None};
// };

// bool operator==(const rn::db::texture::Description &a, const rn::db::texture::Description &b);
// bool operator!=(const rn::db::texture::Description &a, const rn::db::texture::Description &b);

// struct Key {
// 	std::string name;
// 	rn::db::texture::Description description;
// };

// bool operator==(const rn::db::texture::Key &a, const rn::db::texture::Key &b);
// bool operator!=(const rn::db::texture::Key &a, const rn::db::texture::Key &b);

// } // rn::db::texture

// namespace rn::db {

// template<class T>
// class Texture {
// public:
// 	struct Value {
// 		uint64_t lastTouchedAtTick = 0;
// 		// typename T::Texture
// 	};

// 	uint64_t currentTick = 0;
// 	rn::Context<T> &context;

// 	util::FlatStorage<rn::db::texture::Key, rn::db::texture::Texture<T>::Value> storage{};
// 	// std::vector<rn::db::texture::Handle> retireHeap{};

// 	Texture(rn::Context<T> &context) :
// 		context(context)
// 	{}

// 	std::optional<rn::TextureHandle> find(const std::string &name, const rn::db::texture::Description &description) {
// 		auto it = std::find_if(std::begin(storage), std::end(storage), [&] (const auto &pair) {
// 			return pair.first->name == name && pair.first->description == description;
// 		});

// 		if (it != std::end(storage)) {
// 			return it.second->handle;
// 		}

// 		return std::nullopt;
// 	}

// 	void retire([[maybe_unused]] rn::TextureHandle handle) {

// 	}

// 	void refresh([[maybe_unused]] rn::TextureHandle handle) {
// 		auto entryO = storage.access(handle.handle);

// 		if (entryO) {
// 			auto &value = entryO->get().second;

// 			value.lastTouchedAtTick = value.lastTouchedAtTick > currentTick ? value.lastTouchedAtTick : currentTick;
// 		}
// 	}

// 	rn::TextureHandle tryCreate(const std::string &name, const rn::db::texture::Description &description) {
// 		auto it = std::find_if(std::begin(storage), std::end(storage), [&] (const auto &pair) {
// 			return pair.first->name == name && pair.first->description == description;
// 		});

// 		if (it != std::end(storage)) {
// 			refresh(it.second->handle);

// 			return it.second->handle;
// 		}

// 		return storage.assign({ name, description }, context.texture.create());

// 		return { 0 };
// 	}
// };

// } // rn
