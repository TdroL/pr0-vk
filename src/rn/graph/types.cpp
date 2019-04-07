#include "types.hpp"

namespace rn::graph {

constexpr rn::graph::DepthStencilTextureUsage operator|(rn::graph::DepthStencilTextureUsage left, rn::graph::DepthStencilTextureUsage right) {
	return static_cast<rn::graph::DepthStencilTextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr rn::graph::DepthStencilTextureUsage operator&(rn::graph::DepthStencilTextureUsage left, rn::graph::DepthStencilTextureUsage right) {
	return static_cast<rn::graph::DepthStencilTextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::GraphicTextureUsage operator|(rn::graph::GraphicTextureUsage left, rn::graph::GraphicTextureUsage right) {
	return static_cast<rn::graph::GraphicTextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr rn::graph::GraphicTextureUsage operator&(rn::graph::GraphicTextureUsage left, rn::graph::GraphicTextureUsage right) {
	return static_cast<rn::graph::GraphicTextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::ComputeTextureUsage operator|(rn::graph::ComputeTextureUsage left, rn::graph::ComputeTextureUsage right) {
	return static_cast<rn::graph::ComputeTextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr rn::graph::ComputeTextureUsage operator&(rn::graph::ComputeTextureUsage left, rn::graph::ComputeTextureUsage right) {
	return static_cast<rn::graph::ComputeTextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::TransferTextureUsage operator|(rn::graph::TransferTextureUsage left, rn::graph::TransferTextureUsage right) {
	return static_cast<rn::graph::TransferTextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr rn::graph::TransferTextureUsage operator&(rn::graph::TransferTextureUsage left, rn::graph::TransferTextureUsage right) {
	return static_cast<rn::graph::TransferTextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::GraphicBufferUsage operator|(rn::graph::GraphicBufferUsage left, rn::graph::GraphicBufferUsage right) {
	return static_cast<rn::graph::GraphicBufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::GraphicBufferUsage operator&(rn::graph::GraphicBufferUsage left, rn::graph::GraphicBufferUsage right) {
	return static_cast<rn::graph::GraphicBufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::GraphicStage operator|(rn::graph::GraphicStage left, rn::graph::GraphicStage right) {
	return static_cast<rn::graph::GraphicStage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::GraphicStage operator&(rn::graph::GraphicStage left, rn::graph::GraphicStage right) {
	return static_cast<rn::graph::GraphicStage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::ComputeBufferUsage operator|(rn::graph::ComputeBufferUsage left, rn::graph::ComputeBufferUsage right) {
	return static_cast<rn::graph::ComputeBufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::ComputeBufferUsage operator&(rn::graph::ComputeBufferUsage left, rn::graph::ComputeBufferUsage right) {
	return static_cast<rn::graph::ComputeBufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::TransferBufferUsage operator|(rn::graph::TransferBufferUsage left, rn::graph::TransferBufferUsage right) {
	return static_cast<rn::graph::TransferBufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

constexpr rn::graph::TransferBufferUsage operator&(rn::graph::TransferBufferUsage left, rn::graph::TransferBufferUsage right) {
	return static_cast<rn::graph::TransferBufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

bool operator==(rn::graph::BufferResourceHandle a, rn::graph::BufferResourceHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::graph::BufferResourceHandle a, rn::graph::BufferResourceHandle b) {
	return a.index != b.index;
}

bool operator==(rn::graph::TextureResourceHandle a, rn::graph::TextureResourceHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::graph::TextureResourceHandle a, rn::graph::TextureResourceHandle b) {
	return a.index != b.index;
}

bool operator==(rn::graph::DescriptorSetResourceHandle a, rn::graph::DescriptorSetResourceHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::graph::DescriptorSetResourceHandle a, rn::graph::DescriptorSetResourceHandle b) {
	return a.index != b.index;
}

bool operator==(rn::graph::SubpassResourceHandle a, rn::graph::SubpassResourceHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::graph::SubpassResourceHandle a, rn::graph::SubpassResourceHandle b) {
	return a.index != b.index;
}

bool operator==(rn::graph::PipelineResourceHandle a, rn::graph::PipelineResourceHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::graph::PipelineResourceHandle a, rn::graph::PipelineResourceHandle b) {
	return a.index != b.index;
}

} // rn::graph
