#pragma once

#include <cstdint>
#include <vector>
#include <variant>

#include "types.hpp"

namespace rn::graph {

struct DrawCommand {
	uint32_t vertexCount;
	uint32_t instanceCount;
	uint32_t firstVertex{0};
	uint32_t firstInstance{0};
};

struct DrawIndexedCommand {
	uint32_t indexCount;
	uint32_t instanceCount;
	uint32_t firstIndex{0};
	int32_t vertexOffset{0};
	uint32_t firstInstance{0};
};

struct DrawIndexedIndirectCommand {
	rn::graph::BufferResourceHandle buffer;
	uint64_t offset{0};
	uint32_t drawCount;
	uint32_t stride;
};

struct DrawIndirectCommand {
	rn::graph::BufferResourceHandle buffer;
	uint64_t offset{0};
	uint32_t drawCount;
	uint32_t stride;
};

struct BindDescriptorSetsCommand {
	uint32_t firstSet{0};
	uint32_t descriptorSetCount;
};

struct BindIndexBufferCommand {
	rn::graph::BufferResourceHandle buffer;
	uint64_t offset{0};
};

struct BindPipelineCommand {
	rn::graph::PipelineResourceHandle pipeline;
};

struct BindVertexBuffersCommand {
	rn::graph::BufferResourceHandle buffer;
	uint64_t offset;
};

using GraphicCommandVariant = std::variant<rn::graph::DrawCommand, rn::graph::DrawIndexedCommand, rn::graph::DrawIndexedIndirectCommand, rn::graph::DrawIndirectCommand, rn::graph::BindDescriptorSetsCommand,rn::graph:: BindIndexBufferCommand, rn::graph::BindPipelineCommand, rn::graph::BindVertexBuffersCommand>;

struct DispatchCommand {
	uint32_t groupCountX;
	uint32_t groupCountY;
	uint32_t groupCountZ;
};

struct DispatchIndirectCommand {
	rn::graph::BufferResourceHandle buffer;
  uint64_t offset;
};

using ComputeCommandVariant = std::variant<rn::graph::DispatchCommand, rn::graph::DispatchIndirectCommand>;

struct CopyBufferToBufferCommand {
	struct Region {
		uint64_t sourceOffset;
		uint64_t destinationOffset;
		uint64_t size;
	};

	rn::graph::BufferResourceHandle source;
	rn::graph::BufferResourceHandle destination;
	std::vector<Region> regions;
};

struct CopyBufferToTextureCommand {
	struct Region {
		uint64_t sourceOffset;
		uint32_t sourceRowLength;
		uint32_t sourceRowCount;

		rn::Offset3D destinationOffset;
		rn::Extent3D destinationExtent;
	};

	rn::graph::BufferResourceHandle source;
	rn::graph::TextureResourceHandle destination;
	std::vector<Region> regions;
};

struct CopyTextureToTextureCommand {
	struct Region {
    rn::Offset3D sourceOffset;
    rn::Offset3D destinationOffset;
    rn::Extent3D extent;
	};

	rn::graph::TextureResourceHandle source;
	rn::graph::TextureResourceHandle destination;
	std::vector<Region> regions;
};

struct CopyTextureToBufferCommand {
	struct Region {
		rn::Offset3D sourceOffset;
		rn::Extent3D sourceExtent;

		uint64_t destinationOffset;
		uint32_t destinationRowLength;
		uint32_t destinationRowCount;
	};

	rn::graph::TextureResourceHandle source;
	rn::graph::BufferResourceHandle destination;
	std::vector<Region> regions;
};

using TransferCommandVariant = std::variant<rn::graph::CopyBufferToBufferCommand, rn::graph::CopyBufferToTextureCommand, rn::graph::CopyTextureToTextureCommand>;

} // rn::graph
