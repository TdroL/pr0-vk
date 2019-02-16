#pragma once

#include <functional>
#include <optional>
#include <string>

#include "types.hpp"

namespace rn {

template<class T>
class TextureResources {
public:
	T &internal;

	struct TextureSlot {
		rn::TextureHandle handle;
		rn::TextureDescription description;
		std::string name;
	};

	struct TextureRetire {
		rn::TextureHandle handle;
		rn::TextureDescription description;
		std::string name;
		uint64_t tick;
	};

	std::vector<TextureSlot> textureSlots{};
	std::map<std::string_view, rn::TextureHandle> textureSlotHandleIndices{};

	std::vector<TextureRetire> textureRetires{};
	std::map<std::string_view, rn::TextureHandle> textureRetireHandleIndices{};

	std::optional<rn::TextureHandle> find(std::string_view name) {
		const auto mappingIt = textureSlotHandleIndices.find(name);

		if (mappingIt == std::end(textureSlotHandleIndices)) {
			return std::nullopt;
		}

		return mappingIt->second;
	}

	std::optional<rn::TextureHandle> findRetired(std::string_view name) {
		const auto mappingIt = textureRetireHandleIndices.find(name);

		if (mappingIt == std::end(textureRetireHandleIndices)) {
			return std::nullopt;
		}

		return mappingIt->second;
	}

	rn::TextureHandle create(std::string_view name, const rn::TextureDescription &description) {
		const auto handleO = find(name);

		if (handleO) {
			return retire(*handleO);
		}

		const auto handle = internal.createTexture(description);

		size_t index = handle.index;

		if (index >= textureSlots.size()) {
			textureSlots.resize(index);
		}

		textureSlots[index] = {
			handle,
			description,
			std::string{name},
		};
		textureSlotHandleIndices.insert_or_assign(textureSlots[index].name, handle);

		return handle;
	}

	bool retire(rn::TextureHandle handle) {
		size_t index = handle.index;

		if (index >= textureSlots.size() || textureSlots[index].handle == rn::end<rn::TextureHandle>()) {
			return false;
		}

		if (index >= textureRetires.size()) {
			textureRetires.resize(std::max(index, textureSlots.size()));
		}

		uint64_t tick = 0;
		textureRetires[index] = {
			std::move(textureSlots[index].handle),
			std::move(textureSlots[index].description),
			std::move(textureSlots[index].name),
			tick,
		};
		textureRetireHandleIndices.insert_or_assign(textureRetires[index].name, handle);

		textureSlots[index] = {};
		textureSlotHandleIndices.erase(textureRetires[index].name);

		return true;
	}

	bool restore(rn::TextureHandle handle) {
		size_t index = handle.index;

		if (index >= textureRetires.size() || textureRetires[index].handle == rn::end<rn::TextureHandle>()) {
			return false;
		}

		if (index >= textureSlots.size()) {
			textureSlots.resize(std::max(index, textureRetires.size()));
		}

		textureSlots[index] = {
			std::move(textureRetires[index].handle),
			std::move(textureRetires[index].description),
			std::move(textureRetires[index].name),
		};
		textureSlotHandleIndices.insert_or_assign(textureSlots[index].name, handle);

		textureRetires[index] = {};
		textureRetireHandleIndices.erase(textureSlots[index].name, handle);

		return true;
	}

	std::optional<std::reference_wrapper<rn::TextureDescription>> describe(rn::TextureHandle handle) {
		size_t index = handle.index;

		if (index >= textureSlots.size() || textureSlots[index].handle == rn::end<rn::TextureHandle>()) {
			return std::nullopt;
		}

		return textureSlots[index].description;
	}

	std::optional<std::reference_wrapper<rn::TextureDescription>> describeRetired(rn::TextureHandle handle) {
		size_t index = handle.index;

		if (index >= textureRetires.size() || textureRetires[index].handle == rn::end<rn::TextureHandle>()) {
			return std::nullopt;
		}

		return textureRetires[index].description;
	}

	bool retire(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return retire(*handleO);
		}

		return false;
	}

	std::optional<std::reference_wrapper<rn::TextureDescription>> describe(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return describe(*handleO);
		} else {
			return std::nullopt;
		}
	}

	rn::TextureHandle findOrCreate(std::string_view name, const rn::TextureDescription &description) {
		const auto handleO = find(name);

		if (handleO) {
			auto descriptionO = describe(*handleO);

			if (descriptionO) {
				if (descriptionO->get() == description) {
					return *handleO;
				} else {
					retire(*handleO);
				}
			} else {
				descriptionO = describeRetired(*handleO);

				if (descriptionO->get() == description) {
					restore(*handleO);
					return *handleO;
				}
			}
		}

		return create(name, description);
	}
};

template<class T>
class BufferResources {
public:
	T &internal;

};

template<class T>
class Resources {
public:
	T internal;

	TextureResources<T> texture;
	BufferResources<T> buffer;

	Resources() = default;

	Resources(T &&internal) :
		internal{std::move(internal)},
		texture{this->internal},
		buffer{this->internal}
	{}

	Resources(const Resources &other) = delete;
	Resources(Resources &&other) = default;

	Resources & operator=(const Resources &other) = delete;
	Resources & operator=(Resources &&other) = delete;
};

} // rn