#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

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
			retire(*handleO);
		}

		const auto handle = internal.createTexture(description);

		size_t index = handle.index;

		// if (index >= textureSlots.size()) {
		// 	textureSlots.resize(index + 1);
		// }

		// textureSlots[index] = {
		// 	handle,
		// 	description,
		// 	std::string{name},
		// };
		textureSlots.insert(std::begin(textureSlots) + index, {
			handle,
			description,
			std::string{name}
		});
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

		// textureSlots[index] = {};
		textureSlots.insert(std::begin(textureSlots) + index, {});
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
		textureRetireHandleIndices.erase(textureSlots[index].name);

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

	struct BufferSlot {
		rn::BufferHandle handle;
		rn::BufferDescription description;
		std::string name;
	};

	struct BufferRetire {
		rn::BufferHandle handle;
		rn::BufferDescription description;
		std::string name;
		uint64_t tick;
	};

	std::vector<BufferSlot> bufferSlots{};
	std::map<std::string_view, rn::BufferHandle> bufferSlotHandleIndices{};

	std::vector<BufferRetire> bufferRetires{};
	std::map<std::string_view, rn::BufferHandle> bufferRetireHandleIndices{};

	std::optional<rn::BufferHandle> find(std::string_view name) {
		const auto mappingIt = bufferSlotHandleIndices.find(name);

		if (mappingIt == std::end(bufferSlotHandleIndices)) {
			return std::nullopt;
		}

		return mappingIt->second;
	}

	std::optional<rn::BufferHandle> findRetired(std::string_view name) {
		const auto mappingIt = bufferRetireHandleIndices.find(name);

		if (mappingIt == std::end(bufferRetireHandleIndices)) {
			return std::nullopt;
		}

		return mappingIt->second;
	}

	rn::BufferHandle create(std::string_view name, const rn::BufferDescription &description) {
		const auto handleO = find(name);

		if (handleO) {
			retire(*handleO);
		}

		const auto handle = internal.createBuffer(description);

		size_t index = handle.index;

		// if (index >= bufferSlots.size()) {
		// 	bufferSlots.resize(index + 1);
		// }

		// bufferSlots[index] = {
		// 	handle,
		// 	description,
		// 	std::string{name},
		// };
		bufferSlots.insert(std::begin(bufferSlots) + index, {
			handle,
			description,
			std::string{name}
		});
		bufferSlotHandleIndices.insert_or_assign(bufferSlots[index].name, handle);

		return handle;
	}

	bool retire(rn::BufferHandle handle) {
		size_t index = handle.index;

		if (index >= bufferSlots.size() || bufferSlots[index].handle == rn::end<rn::BufferHandle>()) {
			return false;
		}

		if (index >= bufferRetires.size()) {
			bufferRetires.resize(std::max(index + 1, bufferSlots.size()));
		}

		uint64_t tick = 0;
		// bufferRetires[index] = {
		// 	std::move(bufferSlots[index].handle),
		// 	std::move(bufferSlots[index].description),
		// 	std::move(bufferSlots[index].name),
		// 	tick,
		// };
		bufferRetires.insert(std::begin(bufferRetires) + index, {
			std::move(bufferSlots[index].handle),
			std::move(bufferSlots[index].description),
			std::move(bufferSlots[index].name),
			tick,
		});
		bufferRetireHandleIndices.insert_or_assign(bufferRetires[index].name, handle);

		// bufferSlots[index] = {};
		bufferSlots.insert(std::begin(bufferSlots) + index, {});
		bufferSlotHandleIndices.erase(bufferRetires[index].name);

		return true;
	}

	bool restore(rn::BufferHandle handle) {
		size_t index = handle.index;

		if (index >= bufferRetires.size() || bufferRetires[index].handle == rn::end<rn::BufferHandle>()) {
			return false;
		}

		if (index >= bufferSlots.size()) {
			bufferSlots.resize(std::max(index + 1, bufferRetires.size()));
		}

		// bufferSlots[index] = {
		// 	std::move(bufferRetires[index].handle),
		// 	std::move(bufferRetires[index].description),
		// 	std::move(bufferRetires[index].name),
		// };
		bufferSlots.insert(std::begin(bufferSlots) + index, {
			std::move(bufferRetires[index].handle),
			std::move(bufferRetires[index].description),
			std::move(bufferRetires[index].name),
		});
		bufferSlotHandleIndices.insert_or_assign(bufferSlots[index].name, handle);

		// bufferRetires[index] = {};
		bufferRetires.insert(std::begin(bufferRetires) + index, {});
		bufferRetireHandleIndices.erase(bufferSlots[index].name);

		return true;
	}

	std::optional<std::reference_wrapper<rn::BufferDescription>> describe(rn::BufferHandle handle) {
		size_t index = handle.index;

		if (index >= bufferSlots.size() || bufferSlots[index].handle == rn::end<rn::BufferHandle>()) {
			return std::nullopt;
		}

		return bufferSlots[index].description;
	}

	std::optional<std::reference_wrapper<rn::BufferDescription>> describeRetired(rn::BufferHandle handle) {
		size_t index = handle.index;

		if (index >= bufferRetires.size() || bufferRetires[index].handle == rn::end<rn::BufferHandle>()) {
			return std::nullopt;
		}

		return bufferRetires[index].description;
	}

	bool retire(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return retire(*handleO);
		}

		return false;
	}

	std::optional<std::reference_wrapper<rn::BufferDescription>> describe(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return describe(*handleO);
		} else {
			return std::nullopt;
		}
	}

	rn::BufferHandle findOrCreate(std::string_view name, const rn::BufferDescription &description) {
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
