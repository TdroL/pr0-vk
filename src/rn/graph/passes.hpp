#pragma once

#include <cassert>
#include <functional>
#include <string>
#include <vector>
#include <variant>

#include "../../util/flatStorage.hpp"
#include "../pipeline.hpp"
#include "commands.hpp"
#include "types.hpp"

namespace rn::graph {

template<class Handle, class CreateDescriptor, class ModifyDescriptor>
class Descritors {
public:
	util::FlatStorage<std::string, std::variant<std::monostate, CreateDescriptor, ModifyDescriptor>> storage{};

	Handle read(std::string &&name) {
		auto index = storage.assign(std::move(name), std::monostate{});
		assert(index < (1ull << (sizeof(typename Handle::InternalType) * 8)));
		return Handle{ static_cast<typename Handle::InternalType>(index) };
	}

	Handle create(std::string &&name, CreateDescriptor &&descriptor) {
		auto index = storage.assign(std::move(name), std::move(descriptor));
		assert(index < (1ull << (sizeof(typename Handle::InternalType) * 8)));
		return Handle{ static_cast<typename Handle::InternalType>(index) };
	}

	Handle modify(std::string &&name, ModifyDescriptor &&descriptor) {
		auto index = storage.assign(std::move(name), std::move(descriptor));
		assert(index < (1ull << (sizeof(typename Handle::InternalType) * 8)));
		return Handle{ static_cast<typename Handle::InternalType>(index) };
	}

	typename decltype(storage)::Iterator begin() {
		return storage.begin();
	}

	typename decltype(storage)::ConstIterator begin() const {
		return storage.begin();
	}

	typename decltype(storage)::Iterator end() {
		return storage.end();
	}

	typename decltype(storage)::ConstIterator end() const {
		return storage.end();
	}
};

using BufferDescritors = rn::graph::Descritors<rn::graph::BufferResourceHandle, rn::graph::BufferCreate, rn::graph::BufferModify>;
using TextureDescritors = rn::graph::Descritors<rn::graph::TextureResourceHandle, rn::graph::TextureCreate, rn::graph::TextureModify>;

struct ResourceDescriptors {
	rn::graph::BufferDescritors buffer{};
	rn::graph::TextureDescritors texture{};

	rn::graph::TextureResourceHandle swapchain() const {
		return rn::graph::swapchainResourceHandle();
	}
};

template<typename T>
class Pipelines {
public:
	util::FlatStorage<std::string, T> storage{};

	void reserve(size_t size) {
		storage.reserve(size);
	}

	rn::graph::PipelineHandle add(std::string &&name, T &&state) {
		auto index = storage.assign(std::move(name), std::move(state));
		return rn::graph::PipelineHandle{ static_cast<rn::graph::PipelineHandle::InternalType>(index) };
	}

	typename decltype(storage)::Iterator begin() {
		return storage.begin();
	}

	typename decltype(storage)::ConstIterator begin() const {
		return storage.begin();
	}

	typename decltype(storage)::Iterator end() {
		return storage.end();
	}

	typename decltype(storage)::ConstIterator end() const {
		return storage.end();
	}
};

using GraphicPipelines = rn::graph::Pipelines<rn::GraphicPipelineState>;
// using ComputePipelines = rn::graph::Pipelines<rn::ComputePipelineState>;

template<class T>
class Subpasses {
public:
	struct Entry {
		std::vector<rn::graph::SubpassResourceHandle> dependencies{};
		T description{};
	};

	util::FlatStorage<std::string, Entry> storage{};

	void reserve(size_t size) {
		storage.reserve(size);
	}

	rn::graph::SubpassResourceHandle add(std::string &&name, std::vector<rn::graph::SubpassResourceHandle> &&dependencies, T &&description) {
		auto index = storage.assign(std::move(name), Entry{ std::move(dependencies), std::move(description) });
		assert(index < (1ull << (sizeof(rn::graph::SubpassResourceHandle::InternalType) * 8)));
		return rn::graph::SubpassResourceHandle{ static_cast<rn::graph::SubpassResourceHandle::InternalType>(index) };
	}

	typename decltype(storage)::Iterator begin() {
		return storage.begin();
	}

	typename decltype(storage)::ConstIterator begin() const {
		return storage.begin();
	}

	typename decltype(storage)::Iterator end() {
		return storage.end();
	}

	typename decltype(storage)::ConstIterator end() const {
		return storage.end();
	}
};

struct GraphicSubpassDesc {
	rn::graph::TextureResourceHandleWithUsage<rn::graph::DepthStencilTextureUsage> depthStencil{};
	std::vector<rn::graph::TextureResourceHandle> inputs{};
	std::vector<rn::graph::TextureResourceHandle> outputs{};
	std::vector<rn::graph::TextureResourceHandleWithUsageWithStages<rn::graph::GraphicTextureUsage, rn::graph::GraphicStage>> textures{};
	std::vector<rn::graph::BufferResourceHandleWithUsageWithStages<rn::graph::GraphicBufferUsage, rn::graph::GraphicStage>> buffers{};
};

struct ComputeSubpassDesc {
	std::vector<rn::graph::TextureResourceHandleWithUsage<rn::graph::ComputeTextureUsage>> textures{};
	std::vector<rn::graph::BufferResourceHandleWithUsage<rn::graph::ComputeBufferUsage>> buffers{};
};

using GraphicSubpasses = rn::graph::Subpasses<rn::graph::GraphicSubpassDesc>;
using ComputeSubpasses = rn::graph::Subpasses<rn::graph::ComputeSubpassDesc>;

using GraphicCommandList = std::vector<rn::graph::GraphicCommandVariant>;
using ComputeCommandList = std::vector<rn::graph::ComputeCommandVariant>;
using TransferCommandList = std::vector<rn::graph::TransferCommandVariant>;

using GraphicCommandListRecorder = std::function<rn::graph::GraphicCommandList ()>;
using ComputeCommandListRecorder = std::function<rn::graph::ComputeCommandList ()>;
using TransferCommandListRecorder = std::function<rn::graph::TransferCommandList ()>;

using GraphicSubpassRecorders = std::vector<std::tuple<rn::graph::SubpassResourceHandle, rn::graph::GraphicCommandListRecorder>>;
using ComputeSubpassRecorders = std::vector<std::tuple<rn::graph::SubpassResourceHandle, rn::graph::ComputeCommandListRecorder>>;

struct GraphicSetupResult {
	rn::graph::ResourceDescriptors resourceDescriptors;
	rn::graph::GraphicSubpasses subpasses;
	rn::graph::GraphicPipelines pipelines;
	rn::graph::GraphicSubpassRecorders recorder;
};

struct ComputeSetupResult {
	rn::graph::ResourceDescriptors resourceDescriptors;
	rn::graph::ComputeSubpasses subpasses;
	// rn::graph::ComputePipelines pipelines;
	rn::graph::ComputeSubpassRecorders recorder;
};

struct TransferSetupResult {
	rn::graph::ResourceDescriptors resourceDescriptors;
	rn::graph::TransferCommandListRecorder recorder;
};

struct GraphicPass {
	std::string name;
	rn::Extent3D dimensions;
	std::function<rn::graph::GraphicSetupResult ()> setup;
};

struct ComputePass {
	std::string name;
	std::function<rn::graph::ComputeSetupResult ()> setup;
};

struct TransferPass {
	std::string name;
	std::function<rn::graph::TransferSetupResult ()> setup;
};

using Passes = std::vector<std::variant<rn::graph::GraphicPass, rn::graph::ComputePass, rn::graph::TransferPass>>;

} // rn::graph
