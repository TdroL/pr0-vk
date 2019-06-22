#pragma once

#include <cstdint>
#include <variant>

#include "../util/trivialVector.hpp"
#include "types.hpp"

namespace rn {

struct TransitionCommand {
	struct Texture {
		rn::TextureHandle handle;

		rn::TextureAccess oldAccess;
		rn::TextureAccess newAccess;
		rn::QueueType oldQueueType;
		rn::QueueType newQueueType;

		uint32_t baseMipLevel;
		uint32_t mipCount;
		uint32_t baseArrayLayer;
		uint32_t layerCount;
	};

	struct Buffer {

	};

	util::TrivialVector<Texture, 1> textures;
	util::TrivialVector<Buffer, 1> buffers;
};

struct CopyBufferToBufferCommand {
	struct Region {
		uint64_t sourceOffset;
		uint64_t destinationOffset;
		uint64_t size;
	};

	rn::BufferHandle source;
	rn::BufferHandle destination;
	util::TrivialVector<Region, 1> regions;
};

struct CopyBufferToTextureCommand {
	struct Region {
		uint64_t sourceOffset;
		uint32_t sourceRowLength;
		uint32_t sourceRowCount;

		uint32_t mipLevel;
		uint32_t baseArrayLayer;
		uint32_t layerCount;

		rn::Offset3D destinationOffset;
		rn::Extent3D destinationExtent;
	};

	rn::BufferHandle source;
	rn::TextureHandle destination;
	util::TrivialVector<Region, 1> regions;
};

struct CopyTextureToTextureCommand {
	struct Region {
		rn::Offset3D sourceOffset;
		rn::Offset3D destinationOffset;
		rn::Extent3D extent;
	};

	rn::TextureHandle source;
	rn::TextureHandle destination;
	util::TrivialVector<Region, 1> regions;
};

struct CopyTextureToBufferCommand {
	struct Region {
		rn::Offset3D sourceOffset;
		rn::Extent3D sourceExtent;

		uint64_t destinationOffset;
		uint32_t destinationRowLength;
		uint32_t destinationRowCount;
	};

	rn::TextureHandle source;
	rn::BufferHandle destination;
	util::TrivialVector<Region, 1> regions;
};

using TransferCommandVariant = std::variant<
	// transition
	rn::TransitionCommand,
	// transfer
	rn::CopyBufferToBufferCommand,
	rn::CopyBufferToTextureCommand,
	rn::CopyTextureToTextureCommand
>;

struct DispatchCommand {
	uint32_t groupCountX;
	uint32_t groupCountY;
	uint32_t groupCountZ;
};

struct DispatchIndirectCommand {
	rn::BufferHandle buffer;
	uint64_t offset;
};

using ComputeCommandVariant = std::variant<
	// transition
	rn::TransitionCommand,
	// transfer
	rn::CopyBufferToBufferCommand,
	rn::CopyBufferToTextureCommand,
	rn::CopyTextureToTextureCommand,
	// compute
	rn::DispatchCommand,
	rn::DispatchIndirectCommand
>;

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
	rn::BufferHandle buffer;
	uint64_t offset{0};
	uint32_t drawCount;
	uint32_t stride;
};

struct DrawIndirectCommand {
	rn::BufferHandle buffer;
	uint64_t offset{0};
	uint32_t drawCount;
	uint32_t stride;
};

struct BindDescriptorSetsCommand {
	uint32_t firstSet{0};
	uint32_t descriptorSetCount;
};

struct BindIndexBufferCommand {
	rn::BufferHandle buffer;
	uint64_t offset{0};
};

// struct BindPipelineCommand {
// 	rn::PipelineHandle pipeline;
// };

struct BindVertexBuffersCommand {
	rn::BufferHandle buffer;
	uint64_t offset;
};

using GraphicCommandVariant = std::variant<
	// transition
	rn::TransitionCommand,
	// transfer
	rn::CopyBufferToBufferCommand,
	rn::CopyBufferToTextureCommand,
	rn::CopyTextureToTextureCommand,
	// compute
	rn::DispatchCommand,
	rn::DispatchIndirectCommand,
	// graphic
	rn::DrawCommand,
	rn::DrawIndexedCommand,
	rn::DrawIndexedIndirectCommand,
	rn::DrawIndirectCommand,
	rn::BindDescriptorSetsCommand,rn:: BindIndexBufferCommand,
	// rn::BindPipelineCommand,
	rn::BindVertexBuffersCommand
>;

} // rn
