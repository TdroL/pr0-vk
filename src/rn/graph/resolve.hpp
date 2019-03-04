#pragma once

// #include <string>
// #include <functional>
#include <string_view>
#include <utility>
#include <variant>
#include <variant>
#include <vector>

#include "../../ngn/log.hpp"
#include "../../util/contains.hpp"
#include "../context.hpp"
#include "../resources.hpp"
#include "compile.hpp"
#include "types.hpp"

namespace rn::graph {

template<class T, class U>
bool resolve([[maybe_unused]] rn::Context<T> &context, [[maybe_unused]] rn::Resources<U> &resources, util::FlatStorage<std::string, rn::graph::SetupResult> &setupResults) {
	util::FlatStorage<std::string_view, rn::BufferDescription> bufferDescriptions{};
	util::FlatStorage<std::string_view, rn::TextureDescription> textureDescriptions{};

	for (auto &&it : setupResults) {
		if (auto setupResult = std::get_if<rn::graph::GraphicSetupResult>(&it.second)) {
			for (auto &&bufferIt : setupResult->resourceDescriptors.buffer) {
				const auto &name = bufferIt.first;
				auto &descriptor = bufferIt.second;

				auto &&entry = bufferDescriptions.findOrAssign(std::string_view{name});
				auto &bufferDescription = entry.second.get().second;

				if (auto descriptorCreate = std::get_if<rn::graph::BufferCreate>(&descriptor)) {
					bufferDescription.size = descriptorCreate->size;
					bufferDescription.usage = bufferDescription.usage | descriptorCreate->usage;
					bufferDescription.paging = descriptorCreate->paging;
				}
			}
			// for (auto &&textureIt : setupResult->resourceDescriptors.texture) {
			auto &textureEntries = setupResult->resourceDescriptors.texture.storage.entries;
			for (size_t i = 0; i < textureEntries.size(); i++) {
				rn::graph::TextureResourceHandle handle{ static_cast<rn::graph::TextureResourceHandle::InternalType>(i) };
				const auto &name = textureEntries[i].first;
				const auto &descriptor = textureEntries[i].second;

				auto &&entry = textureDescriptions.findOrAssign(std::string_view{name});
				auto &textureDescription = entry.second.get().second;

				if (auto descriptorCreate = std::get_if<rn::graph::TextureCreate>(&descriptor)) {
					textureDescription.format = descriptorCreate->format;
					continue;
					textureDescription.dimensions = descriptorCreate->dimensions;
					textureDescription.type = descriptorCreate->type;
					textureDescription.levels = descriptorCreate->levels;
					textureDescription.layers = descriptorCreate->layers;
					textureDescription.usage = textureDescription.usage | descriptorCreate->usage;
				}
				continue;

				for (auto &&subpassIt : setupResult->subpasses) {
					const auto &subpassDescription = subpassIt.second.description;

					if (subpassDescription.depthStencil.texture == handle) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::DepthStencilAttachment;
					}

					if ((textureDescription.usage & rn::TextureUsage::InputAttachment) == rn::TextureUsage::None && util::contains(subpassDescription.inputs, handle)) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::InputAttachment;
					}

					if ((textureDescription.usage & rn::TextureUsage::ColorAttachment) == rn::TextureUsage::None && util::contains(subpassDescription.outputs, handle)) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::ColorAttachment;
					}

					if ((textureDescription.usage & rn::TextureUsage::Sampled) == rn::TextureUsage::None && util::contains(subpassDescription.textures, handle)) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::Sampled;
					}
				}
			}

		} else if (auto setupResult = std::get_if<rn::graph::ComputeSetupResult>(&it.second)) {
			// value
		} else if (auto setupResult = std::get_if<rn::graph::TransferSetupResult>(&it.second)) {
			// value
		} else {
			ngn::log::warn("rn::graph::resolve() => unknown setup result type for pass {}", it.first);
		}
	}

	// for (auto &resourcesUsage : resourcesUsageList) {
	// 	for (auto &buffer : resourcesUsage.resources.buffer) {
	// 		auto &bufferDescription = buffer.second;

	// 		// if (auto value = std::get_if<rn::graph::BufferCreate>(&bufferDescription)) {
	// 		// 	std::string_view name{buffer.first};
	// 		// 	rn::BufferDescription description{
	// 		// 			/*.size=*/ value->size,
	// 		// 			/*.usage=*/ value->usage,
	// 		// 			/*.paging=*/ value->paging,
	// 		// 	};

	// 		// 	rn::BufferHandle handle = resources.buffer.findOrCreate(name, description);
	// 		// 	bufferCreates.emplace_back(name, handle);
	// 		// }
	// 	}

	// 	for (auto &texture : resourcesUsage.resources.texture) {
	// 		auto &textureDescription = texture.second;

	// 		// if (auto value = std::get_if<rn::graph::TextureCreate>(&textureDescription)) {
	// 		// 	std::string_view name{texture.first};
	// 		// 	rn::TextureDescription description{
	// 		// 		/*.format=*/ value->format, // rn::PixelFormat;
	// 		// 		/*.dimensions=*/ value->dimensions, // rn::Extent3D;
	// 		// 		/*.type=*/ value->type, // rn::ImageType{rn::ImageType::Tex2D};
	// 		// 		/*.levels=*/ value->levels, // uint32_t{1};
	// 		// 		/*.layers=*/ value->layers, // uint32_t{1};
	// 		// 		/*.usage=*/ value->usage, // rn::TextureUsage{rn::TextureUsage::None};
	// 		// 	};

	// 		// 	rn::TextureHandle handle = resources.texture.findOrCreate(name, description);
	// 		// 	textureCreates.emplace_back(name, handle);
	// 		// }
	// 	}
	// }

	std::vector<std::pair<std::string_view, rn::BufferHandle>> bufferCreates{};
	std::vector<std::pair<std::string_view, rn::TextureHandle>> textureCreates{};

	for (auto &bufferCreate : bufferCreates) {
		ngn::log::debug("bufferCreate {}={}", bufferCreate.first, bufferCreate.second.index);
	}

	for (auto &textureCreate : textureCreates) {
		ngn::log::debug("textureCreate {}={}", textureCreate.first, textureCreate.second.index);
	}

	return false;
}

} // rn::graph
