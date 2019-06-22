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
// #include "../context.hpp"
#include "../resources.hpp"
#include "compile.hpp"
#include "types.hpp"

namespace rn::graph {

template<class T, class U>
bool resolve([[maybe_unused]] T &context, [[maybe_unused]] rn::Resources<U> &resources, util::FlatStorage<std::string, rn::graph::SetupResult> &setupResults) {
	util::FlatStorage<std::string_view, rn::BufferDescription> bufferDescriptions{};
	util::FlatStorage<std::string_view, rn::TextureDescription> textureDescriptions{};

	for (auto &&it : setupResults) {
		if (auto setupResult = std::get_if<rn::graph::GraphicSetupResult>(&it.second)) {
			auto &bufferEntries = setupResult->resourceDescriptors.buffer.storage.entries;
			for (size_t i = 0; i < bufferEntries.size(); i++) {
				rn::graph::BufferResourceHandle handle{ static_cast<rn::graph::BufferResourceHandle::InternalType>(i) };
				const auto &name = bufferEntries[i].first;
				const auto &descriptor = bufferEntries[i].second;

				auto &bufferDescription = bufferDescriptions.findOrAssign(std::string_view{name}).value;

				if (const auto descriptorCreate = std::get_if<rn::graph::BufferCreate>(&descriptor)) {
					bufferDescription.size = descriptorCreate->size;
					bufferDescription.usage = bufferDescription.usage | descriptorCreate->usage;
					bufferDescription.paging = descriptorCreate->paging;
				}

				for (auto &&subpassIt : setupResult->subpasses) {
					const auto &subpassDescription = subpassIt.second.description;

					if ((bufferDescription.usage & rn::BufferUsage::Uniform) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::GraphicBufferUsage::Uniform })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Uniform;
					}

					if ((bufferDescription.usage & rn::BufferUsage::Storage) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::GraphicBufferUsage::Storage })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Storage;
					}

					if ((bufferDescription.usage & rn::BufferUsage::Indirect) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::GraphicBufferUsage::Indirect })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Indirect;
					}

					if ((bufferDescription.usage & rn::BufferUsage::Index) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::GraphicBufferUsage::Index })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Index;
					}

					if ((bufferDescription.usage & rn::BufferUsage::Vertex) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::GraphicBufferUsage::Vertex })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Vertex;
					}
				}
			}

			const auto &textureEntries = setupResult->resourceDescriptors.texture.storage.entries;
			for (size_t i = 0; i < textureEntries.size(); i++) {
				rn::graph::TextureResourceHandle handle{ static_cast<rn::graph::TextureResourceHandle::InternalType>(i) };
				const auto &name = textureEntries[i].first;
				const auto &descriptor = textureEntries[i].second;

				auto &textureDescription = textureDescriptions.findOrAssign(std::string_view{name}).value;

				if (const auto descriptorCreate = std::get_if<rn::graph::TextureCreate>(&descriptor)) {
					textureDescription.format = descriptorCreate->format;
					textureDescription.dimensions = descriptorCreate->dimensions;
					textureDescription.type = descriptorCreate->type;
					textureDescription.levels = descriptorCreate->levels;
					textureDescription.layers = descriptorCreate->layers;
					textureDescription.usage = textureDescription.usage | descriptorCreate->usage;
				}

				for (auto &&subpassIt : setupResult->subpasses) {
					const auto &subpassDescription = subpassIt.second.description;

					if (subpassDescription.depthStencil.texture == handle) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::DepthStencilAttachment;
					}

					if ((textureDescription.usage & rn::TextureUsage::InputAttachment) == rn::TextureUsage::None && util::contains(subpassDescription.inputs, handle)) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::InputAttachment;
					}

					if ((textureDescription.usage & rn::TextureUsage::OutputAttachment) == rn::TextureUsage::None && util::contains(subpassDescription.outputs, handle)) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::OutputAttachment;
					}

					if ((textureDescription.usage & rn::TextureUsage::Sampled) == rn::TextureUsage::None && util::contains(subpassDescription.textures, { handle, rn::graph::GraphicTextureUsage::Sampled })) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::Sampled;
					}

					if ((textureDescription.usage & rn::TextureUsage::Storage) == rn::TextureUsage::None && util::contains(subpassDescription.textures, { handle, rn::graph::GraphicTextureUsage::Storage })) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::Storage;
					}
				}
			}
		} else if (auto setupResult = std::get_if<rn::graph::ComputeSetupResult>(&it.second)) {
			auto &bufferEntries = setupResult->resourceDescriptors.buffer.storage.entries;
			for (size_t i = 0; i < bufferEntries.size(); i++) {
				rn::graph::BufferResourceHandle handle{ static_cast<rn::graph::BufferResourceHandle::InternalType>(i) };
				const auto &name = bufferEntries[i].first;
				const auto &descriptor = bufferEntries[i].second;

				auto &bufferDescription = bufferDescriptions.findOrAssign(std::string_view{name}).value;

				if (const auto descriptorCreate = std::get_if<rn::graph::BufferCreate>(&descriptor)) {
					bufferDescription.size = descriptorCreate->size;
					bufferDescription.usage = bufferDescription.usage | descriptorCreate->usage;
					bufferDescription.paging = descriptorCreate->paging;
				}

				for (auto &&subpassIt : setupResult->subpasses) {
					const auto &subpassDescription = subpassIt.second.description;

					if ((bufferDescription.usage & rn::BufferUsage::Uniform) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::ComputeBufferUsage::Uniform })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Uniform;
					}

					if ((bufferDescription.usage & rn::BufferUsage::Storage) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::ComputeBufferUsage::Storage })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Storage;
					}

					if ((bufferDescription.usage & rn::BufferUsage::Indirect) == rn::BufferUsage::None && util::contains(subpassDescription.buffers, { handle, rn::graph::ComputeBufferUsage::Indirect })) {
						bufferDescription.usage = bufferDescription.usage | rn::BufferUsage::Indirect;
					}
				}
			}

			const auto &textureEntries = setupResult->resourceDescriptors.texture.storage.entries;
			for (size_t i = 0; i < textureEntries.size(); i++) {
				rn::graph::TextureResourceHandle handle{ static_cast<rn::graph::TextureResourceHandle::InternalType>(i) };
				const auto &name = textureEntries[i].first;
				const auto &descriptor = textureEntries[i].second;

				auto &textureDescription = textureDescriptions.findOrAssign(std::string_view{name}).value;

				if (const auto descriptorCreate = std::get_if<rn::graph::TextureCreate>(&descriptor)) {
					textureDescription.format = descriptorCreate->format;
					textureDescription.dimensions = descriptorCreate->dimensions;
					textureDescription.type = descriptorCreate->type;
					textureDescription.levels = descriptorCreate->levels;
					textureDescription.layers = descriptorCreate->layers;
					textureDescription.usage = textureDescription.usage | descriptorCreate->usage;
				}

				for (auto &&subpassIt : setupResult->subpasses) {
					const auto &subpassDescription = subpassIt.second.description;

					if ((textureDescription.usage & rn::TextureUsage::Sampled) == rn::TextureUsage::None && util::contains(subpassDescription.textures, { handle, rn::graph::ComputeTextureUsage::Sampled })) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::Sampled;
					}

					if ((textureDescription.usage & rn::TextureUsage::Storage) == rn::TextureUsage::None && util::contains(subpassDescription.textures, { handle, rn::graph::ComputeTextureUsage::Storage })) {
						textureDescription.usage = textureDescription.usage | rn::TextureUsage::Storage;
					}
				}
			}
		} else if (auto setupResult = std::get_if<rn::graph::TransferSetupResult>(&it.second)) {
			auto &bufferEntries = setupResult->resourceDescriptors.buffer.storage.entries;
			for (size_t i = 0; i < bufferEntries.size(); i++) {
				// rn::graph::BufferResourceHandle handle{ static_cast<rn::graph::BufferResourceHandle::InternalType>(i) };
				const auto &name = bufferEntries[i].first;
				const auto &descriptor = bufferEntries[i].second;

				auto &bufferDescription = bufferDescriptions.findOrAssign(std::string_view{name}).value;

				if (const auto descriptorCreate = std::get_if<rn::graph::BufferCreate>(&descriptor)) {
					bufferDescription.size = descriptorCreate->size;
					bufferDescription.usage = bufferDescription.usage | descriptorCreate->usage;
					bufferDescription.paging = descriptorCreate->paging;
				}
			}

			const auto &textureEntries = setupResult->resourceDescriptors.texture.storage.entries;
			for (size_t i = 0; i < textureEntries.size(); i++) {
				// rn::graph::TextureResourceHandle handle{ static_cast<rn::graph::TextureResourceHandle::InternalType>(i) };
				const auto &name = textureEntries[i].first;
				const auto &descriptor = textureEntries[i].second;

				auto &textureDescription = textureDescriptions.findOrAssign(std::string_view{name}).value;

				if (const auto descriptorCreate = std::get_if<rn::graph::TextureCreate>(&descriptor)) {
					textureDescription.format = descriptorCreate->format;
					textureDescription.dimensions = descriptorCreate->dimensions;
					textureDescription.type = descriptorCreate->type;
					textureDescription.levels = descriptorCreate->levels;
					textureDescription.layers = descriptorCreate->layers;
					textureDescription.usage = textureDescription.usage | descriptorCreate->usage;
				}
			}
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

	ngn::log::debug("bufferDescriptions={}", bufferDescriptions.entries.size());
	ngn::log::debug("textureDescriptions={}", textureDescriptions.entries.size());

	for (auto &&it : bufferDescriptions) {
		std::string usage = "";
		if ((it.second.usage & rn::BufferUsage::Index) != rn::BufferUsage::None) {
			usage +=  "Index | ";
		}

		if ((it.second.usage & rn::BufferUsage::Storage) != rn::BufferUsage::None) {
			usage +=  "Storage | ";
		}

		if ((it.second.usage & rn::BufferUsage::TransferDestination) != rn::BufferUsage::None) {
			usage +=  "TransferDestination | ";
		}

		if ((it.second.usage & rn::BufferUsage::TransferSource) != rn::BufferUsage::None) {
			usage +=  "TransferSource | ";
		}

		if ((it.second.usage & rn::BufferUsage::Uniform) != rn::BufferUsage::None) {
			usage +=  "Uniform | ";
		}

		if ((it.second.usage & rn::BufferUsage::Vertex) != rn::BufferUsage::None) {
			usage +=  "Vertex | ";
		}

		ngn::log::debug("buffer: {} [{}; {}] {}", it.first, it.second.size, it.second.paging == rn::BufferPaging::Static ? std::string_view{"static"} : std::string_view{"swapchain-relative"}, usage.empty() ? std::string_view{usage} : std::string_view{usage}.substr(0, usage.size() - 3));
	}

	for (auto &&it : textureDescriptions) {
		std::string usage = "";

		if ((it.second.usage & rn::TextureUsage::TransferSource) != rn::TextureUsage::None) {
			usage +=  "TransferSource | ";
		}

		if ((it.second.usage & rn::TextureUsage::TransferDestination) != rn::TextureUsage::None) {
			usage +=  "TransferDestination | ";
		}

		if ((it.second.usage & rn::TextureUsage::Sampled) != rn::TextureUsage::None) {
			usage +=  "Sampled | ";
		}

		if ((it.second.usage & rn::TextureUsage::Storage) != rn::TextureUsage::None) {
			usage +=  "Storage | ";
		}

		if ((it.second.usage & rn::TextureUsage::OutputAttachment) != rn::TextureUsage::None) {
			usage +=  "OutputAttachment | ";
		}

		if ((it.second.usage & rn::TextureUsage::DepthStencilAttachment) != rn::TextureUsage::None) {
			usage +=  "DepthStencilAttachment | ";
		}

		if ((it.second.usage & rn::TextureUsage::TransientAttachment) != rn::TextureUsage::None) {
			usage +=  "TransientAttachment | ";
		}

		if ((it.second.usage & rn::TextureUsage::InputAttachment) != rn::TextureUsage::None) {
			usage +=  "InputAttachment | ";
		}

		ngn::log::debug("texture: {} [{}, {}, {}] {}", it.first, it.second.dimensions.width, it.second.dimensions.height, it.second.dimensions.depth, usage.empty() ? std::string_view{usage} : std::string_view{usage}.substr(0, usage.size() - 3));
	}


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
