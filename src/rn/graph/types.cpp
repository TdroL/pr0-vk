#include "types.hpp"

namespace rn::graph {

constexpr rn::graph::Access operator|(rn::graph::Access left, rn::graph::Access right) {
	return static_cast<rn::graph::Access>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr rn::graph::Access operator&(rn::graph::Access left, rn::graph::Access right) {
	return static_cast<rn::graph::Access>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
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
