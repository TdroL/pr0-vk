#pragma once

#include <cmath>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "../ngn/log.hpp"
#include "../util/span.hpp"
#include "commands.hpp"
#include "types.hpp"

namespace rn {

template<class T>
class TextureResources {
public:
	T *context{nullptr};

	struct TextureSlot {
		rn::TextureHandle handle;
		rn::TextureDescription description;
		std::string name;
	};

	struct TextureRetire {
		rn::TextureHandle handle;
		rn::TextureDescription description;
		std::string name;
		rn::FenceStamp fenceStamp;
	};

	rn::FenceStamp pendingFenceStamp{};

	std::vector<TextureSlot> textureSlots{};
	// std::map<std::string_view, rn::TextureHandle> textureSlotHandleIndices{};

	std::vector<TextureRetire> textureRetires{};
	// std::map<std::string_view, rn::TextureHandle> textureRetireHandleIndices{};

	TextureResources() = default;
	explicit TextureResources(T &context) noexcept :
		context{&context}
	{}
	TextureResources(const TextureResources<T> &other) = delete;
	TextureResources(TextureResources<T> &&other) = delete;
	TextureResources & operator=(const TextureResources<T> &other) = delete;
	TextureResources & operator=(TextureResources<T> &&other) = delete;

	std::optional<rn::TextureHandle> find(std::string_view name) {
		const auto slotIt = std::find_if(std::begin(textureSlots), std::end(textureSlots), [&] (const auto &entry) {
			return entry.name == name;
		});

		if (slotIt == std::end(textureSlots)) {
			return std::nullopt;
		}

		return slotIt->handle;
	}

	std::optional<rn::TextureHandle> findRetired(std::string_view name) {
		const auto retireIt = std::find_if(std::begin(textureRetires), std::end(textureRetires), [&] (const auto &entry) {
			return entry.name == name;
		});

		if (retireIt == std::end(textureRetires)) {
			return std::nullopt;
		}

		return retireIt->handle;
	}

	rn::TextureHandle create(std::string_view name, const rn::TextureDescription &description) {
		const auto handleO = find(name);

		if (handleO) {
			retire(*handleO);
		}

		const auto handle = context->createTexture(description);

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
		// textureSlotHandleIndices.insert_or_assign(textureSlots[index].name, handle);

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

		textureRetires[index] = {
			std::move(textureSlots[index].handle),
			std::move(textureSlots[index].description),
			std::move(textureSlots[index].name),
			pendingFenceStamp,
		};
		// textureRetireHandleIndices.insert_or_assign(textureRetires[index].name, handle);

		// textureSlots[index] = {};
		textureSlots.insert(std::begin(textureSlots) + index, {});
		// textureSlotHandleIndices.erase(textureRetires[index].name);

		return true;
	}

	bool retire(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return retire(*handleO);
		}

		return false;
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
		// textureSlotHandleIndices.insert_or_assign(textureSlots[index].name, handle);

		textureRetires[index] = {};
		// textureRetireHandleIndices.erase(textureSlots[index].name);

		return true;
	}

	std::optional<std::reference_wrapper<TextureSlot>> describe(rn::TextureHandle handle) {
		size_t index = handle.index;

		if (index >= textureSlots.size() || textureSlots[index].handle == rn::end<rn::TextureHandle>()) {
			return std::nullopt;
		}

		return std::ref(textureSlots[index]);
	}

	std::optional<std::reference_wrapper<TextureSlot>> describe(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return describe(*handleO);
		} else {
			return std::nullopt;
		}
	}

	std::optional<std::reference_wrapper<TextureSlot>> describeRetired(rn::TextureHandle handle) {
		size_t index = handle.index;

		if (index >= textureRetires.size() || textureRetires[index].handle == rn::end<rn::TextureHandle>()) {
			return std::nullopt;
		}

		return std::ref(textureRetires[index]);
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

	void flushRetired() {
		rn::FenceStamp resolvedFenceStamp = context->resolvedFenceStamp(rn::QueueType::Transfer);

		textureRetires.erase(std::remove_if(std::begin(textureRetires), std::end(textureRetires), [=] (const auto &entry) {
			return entry.fenceStamp < resolvedFenceStamp;
		}), std::end(textureRetires));
	}

	void advance() {
		pendingFenceStamp = context->pendingFenceStamp(rn::QueueType::Transfer);
	}
};

template<class T>
class BufferResources {
public:
	T *context{nullptr};

	struct BufferSlot {
		rn::BufferHandle handle;
		rn::BufferDescription description;
		std::string name;
	};

	struct BufferRetire {
		rn::BufferHandle handle;
		rn::BufferDescription description;
		std::string name;
		rn::FenceStamp fenceStamp;
	};

	rn::FenceStamp pendingFenceStamp{};

	std::vector<BufferSlot> bufferSlots{};
	// std::map<std::string_view, rn::BufferHandle> bufferSlotHandleIndices{};

	std::vector<BufferRetire> bufferRetires{};
	// std::map<std::string_view, rn::BufferHandle> bufferRetireHandleIndices{};

	BufferResources() = default;
	explicit BufferResources(T &context) noexcept :
		context{&context}
	{}
	BufferResources(const BufferResources<T> &other) = delete;
	BufferResources(BufferResources<T> &&other) = delete;
	BufferResources & operator=(const BufferResources<T> &other) = delete;
	BufferResources & operator=(BufferResources<T> &&other) = delete;

	std::optional<rn::BufferHandle> find(std::string_view name) {
		const auto slotIt = std::find_if(std::begin(bufferSlots), std::end(bufferSlots), [&] (const auto &entry) {
			return entry.name == name;
		});

		if (slotIt == std::end(bufferSlots)) {
			return std::nullopt;
		}

		return slotIt->handle;
	}

	std::optional<rn::BufferHandle> findRetired(std::string_view name) {
		const auto retireIt = std::find_if(std::begin(bufferRetires), std::end(bufferRetires), [&] (const auto &entry) {
			return entry.name == name;
		});

		if (retireIt == std::end(bufferRetires)) {
			return std::nullopt;
		}

		return retireIt->handle;
	}

	rn::BufferHandle create(std::string_view name, const rn::BufferDescription &description) {
		const auto handleO = find(name);

		if (handleO) {
			retire(*handleO);
		}

		const auto handle = context->createBuffer(description);

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
		// bufferSlotHandleIndices.insert_or_assign(bufferSlots[index].name, handle);

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
			pendingFenceStamp,
		});
		// bufferRetireHandleIndices.insert_or_assign(bufferRetires[index].name, handle);

		// bufferSlots[index] = {};
		bufferSlots.insert(std::begin(bufferSlots) + index, {});
		// bufferSlotHandleIndices.erase(bufferRetires[index].name);

		return true;
	}

	bool retire(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return retire(*handleO);
		}

		return false;
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
		// bufferSlotHandleIndices.insert_or_assign(bufferSlots[index].name, handle);

		// bufferRetires[index] = {};
		bufferRetires.insert(std::begin(bufferRetires) + index, {});
		// bufferRetireHandleIndices.erase(bufferSlots[index].name);

		return true;
	}

	std::optional<std::reference_wrapper<BufferSlot>> describe(rn::BufferHandle handle) {
		size_t index = handle.index;

		if (index >= bufferSlots.size() || bufferSlots[index].handle == rn::end<rn::BufferHandle>()) {
			return std::nullopt;
		}

		return std::ref(bufferSlots[index]);
	}

	std::optional<std::reference_wrapper<BufferSlot>> describe(std::string_view name) {
		const auto handleO = find(name);

		if (handleO) {
			return describe(*handleO);
		} else {
			return std::nullopt;
		}
	}

	std::optional<std::reference_wrapper<BufferSlot>> describeRetired(rn::BufferHandle handle) {
		size_t index = handle.index;

		if (index >= bufferRetires.size() || bufferRetires[index].handle == rn::end<rn::BufferHandle>()) {
			return std::nullopt;
		}

		return std::ref(bufferRetires[index]);
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

	void flushRetired() {
		rn::FenceStamp resolvedFenceStamp = context->resolvedFenceStamp(rn::QueueType::Transfer);

		bufferRetires.erase(std::remove_if(std::begin(bufferRetires), std::end(bufferRetires), [=] (const auto &entry) {
			return entry.fenceStamp < resolvedFenceStamp;
		}), std::end(bufferRetires));
	}

	void advance() {
		pendingFenceStamp = context->pendingFenceStamp(rn::QueueType::Transfer);
	}
};

template<class T>
class Resources {
public:
	// std::reference_wrapper<T> context{};
	T* context{nullptr};
	TextureResources<T> texture{};
	BufferResources<T> buffer{};

	rn::TransitionCommand acquireTransitionCommand{};
	std::vector<rn::TransferCommandVariant> transferCommands{};
	rn::TransitionCommand releaseTransitionCommand{};

	rn::FenceStamp pendingFenceStamp{0};

	Resources() = default;

	explicit Resources(T &context) :
		context{&context},
		texture{context},
		buffer{context}
	{}

	Resources(const Resources &other) = delete;
	Resources(Resources &&other) = default;

	Resources & operator=(const Resources &other) = delete;
	Resources & operator=(Resources &&other) = delete;

	[[nodiscard]] rn::FenceStamp enqueueTextureUpload(rn::TextureHandle handle, const rn::TextureDataAccessor &accessor) {
		const auto textureSlotO = texture.describe(handle);
		if ( ! textureSlotO) {
			ngn::log::error("rn::Resources::enqueueTextureUpload(<{:x}>) => invalid texture handle", handle.index);
			return rn::end<rn::FenceStamp>();
		}

		rn::TextureDescription description = textureSlotO->get().description;

		util::TrivialVector<rn::BufferCopyRange, 1> ranges{};
		ranges.reserve(description.levels * description.layers);

		util::TrivialVector<rn::CopyBufferToTextureCommand::Region, 1> regions{};
		regions.reserve(description.levels * description.layers);

		// size_t dataFormatSize = rn::sizeOf(accessor.format());
		size_t bufferSize = 0;

		// vulkan requires each buffer address of layer and level to be aligned to 4 bytes
		constexpr size_t alignment = 4;
		for (uint32_t layer = 0; layer < description.layers; layer++) {
			for (uint32_t level = 0; level < description.levels; level++) {
				uint32_t layerWidth = std::max(1u, description.dimensions.width >> level);
				uint32_t layerHeight = std::max(1u, description.dimensions.height >> level);
				uint32_t layerDepth = std::max(1u, description.dimensions.depth >> level);
				size_t levelSize = ((accessor.size(level) + (alignment - 1u)) / alignment) * alignment;

				ranges.push_back({
					/*.bufferOffset=*/ bufferSize,
					/*.data=*/ accessor.data(layer, level),
					/*.length=*/ accessor.size(level)
				});

				regions.push_back({
					/*.sourceOffset=*/ bufferSize,
					/*.sourceRowLength=*/ layerWidth,
					/*.sourceRowCount=*/ layerHeight * layerDepth,
					/*.mipLevel=*/ level,
					/*.baseArrayLayer=*/ layer,
					/*.layerCount=*/ 1,
					/*.destinationOffset=*/ {
						/*.x=*/ 0,
						/*.y=*/ 0,
						/*.z=*/ 0
					},
					/*.destinationExtent=*/ {
						/*.width=*/ layerWidth,
						/*.height=*/ layerHeight,
						/*.depth=*/ layerDepth
					},
				});

				bufferSize += levelSize;
			}
		}

		rn::BufferDescription stagingBufferDescription{
			/*.size=*/ bufferSize,
			/*.usage=*/ rn::BufferUsage::TransferSource | rn::BufferUsage::Staging,
			/*.paging=*/ rn::BufferPaging::Static,
		};

		std::string stagingBufferName = "TextureStagingBuffer:" + std::to_string(handle.index) + "#" + textureSlotO->get().name;
		rn::BufferHandle stagingBufferHandle = buffer.create(stagingBufferName, stagingBufferDescription);

		// upload texture to staging buffer
		context->uploadBufferSync(stagingBufferHandle, util::makeSpan(ranges));

		acquireTransitionCommand.textures.push_back(rn::TransitionCommand::Texture{
			/*.handle=*/ handle,
			/*.oldAccess=*/ rn::TextureAccess::None,
			/*.newAccess=*/ rn::TextureAccess::TransferWrite,
			/*.oldQueueType=*/ rn::QueueType::None,
			/*.newQueueType=*/ rn::QueueType::Transfer,
			/*.baseMipLevel=*/ 0,
			/*.mipCount=*/ description.levels,
			/*.baseArrayLayer=*/ 0,
			/*.layerCount=*/ description.layers,
		});

		// enqueue transfer from staging buffer to the texture
		transferCommands.push_back(rn::CopyBufferToTextureCommand{
			/*.source=*/ stagingBufferHandle,
			/*.destination=*/ handle,
			/*.regions=*/ std::move(regions)
		});

		releaseTransitionCommand.textures.push_back(rn::TransitionCommand::Texture{
			/*.handle=*/ handle,
			/*.oldAccess=*/ rn::TextureAccess::TransferWrite,
			/*.newAccess=*/ rn::TextureAccess::AnyShaderSampledRead,
			/*.oldQueueType=*/ rn::QueueType::Transfer,
			/*.newQueueType=*/ rn::QueueType::Graphic,
			/*.baseMipLevel=*/ 0,
			/*.mipCount=*/ description.levels,
			/*.baseArrayLayer=*/ 0,
			/*.layerCount=*/ description.layers,
		});

		buffer.retire(stagingBufferHandle);

		return pendingFenceStamp;
	}

	[[nodiscard]] rn::FenceStamp enqueueBufferUpload(rn::BufferHandle handle, const rn::BufferDataAccessor &accessor) {
		const auto bufferSlotO = buffer.describe(handle);
		if ( ! bufferSlotO) {
			ngn::log::error("rn::Resources::enqueueBufferUpload(<{:x}>) => invalid buffer handle", handle.index);
			return rn::end<rn::FenceStamp>();
		}

		rn::BufferDescription stagingBufferDescription{
			/*.size=*/ accessor.size(),
			/*.usage=*/ rn::BufferUsage::TransferSource | rn::BufferUsage::Staging,
			/*.paging=*/ rn::BufferPaging::Static,
		};

		std::string stagingBufferName = "BufferStagingBuffer:" + std::to_string(handle.index) + "#" + bufferSlotO->get().name;
		rn::BufferHandle stagingBufferHandle = buffer.create(stagingBufferName, stagingBufferDescription);

		// upload buffer to staging buffer
		util::TrivialVector<rn::BufferCopyRange, 1> ranges{};
		ranges.push_back({
			/*.bufferOffset=*/ 0,
			/*.data=*/ accessor.data(),
			/*.length=*/ accessor.size()
		});
		context->uploadBufferSync(stagingBufferHandle, util::makeSpan(ranges));

		acquireTransitionCommand.buffers.push_back(rn::TransitionCommand::Buffer{
			/*.handle=*/ handle,
			/*.oldAccess=*/ rn::BufferAccess::None,
			/*.newAccess=*/ rn::BufferAccess::TransferWrite,
			/*.oldQueueType=*/ rn::QueueType::None,
			/*.newQueueType=*/ rn::QueueType::Transfer,
			/*.offset=*/ 0,
			/*.size=*/ accessor.size(),
		});

		// enqueue transfer from staging buffer to the texture
		transferCommands.push_back(rn::CopyBufferToBufferCommand{
			/*.source=*/ stagingBufferHandle,
			/*.destination=*/ handle,
			/*.regions=*/ {
				{
					/*.sourceOffset=*/ 0,
					/*.destinationOffset=*/ 0,
					/*.size=*/ accessor.size(),
				}
			}
		});

		releaseTransitionCommand.buffers.push_back(rn::TransitionCommand::Buffer{
			/*.handle=*/ handle,
			/*.oldAccess=*/ rn::BufferAccess::TransferWrite,
			/*.newAccess=*/ rn::BufferAccess::VertexBufferRead,
			/*.oldQueueType=*/ rn::QueueType::Transfer,
			/*.newQueueType=*/ rn::QueueType::Graphic,
			/*.offset=*/ 0,
			/*.size=*/ accessor.size(),
		});

		buffer.retire(stagingBufferHandle);

		return pendingFenceStamp;
	}

	void advance() {
		buffer.flushRetired();
		texture.flushRetired();

		pendingFenceStamp = context->pendingFenceStamp(rn::QueueType::Transfer);

		buffer.advance();
		texture.advance();
	}

	void commit() {
		if ( ! transferCommands.empty()) {
			context->enqueueCommands(std::vector<rn::TransferCommandVariant>{ acquireTransitionCommand });
			context->enqueueCommands(std::move(transferCommands));
			context->enqueueCommands(std::vector<rn::TransferCommandVariant>{ releaseTransitionCommand });

			acquireTransitionCommand = {};
			transferCommands = {};
			releaseTransitionCommand = {};
		}
	}
};

} // rn
