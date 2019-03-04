#include "types.hpp"

#include <tuple>

namespace rn {

bool operator==(const rn::Extent3D &a, const rn::Extent3D &b) {
	return std::tie(a.width, a.height, a.depth) == std::tie(b.width, b.height, b.depth);
}

bool operator!=(const rn::Extent3D &a, const rn::Extent3D &b) {
	return !(a == b);
}

bool operator==(const rn::Offset3D &a, const rn::Offset3D &b) {
	return std::tie(a.x, a.y, a.z) == std::tie(b.x, b.y, b.z);
}

bool operator!=(const rn::Offset3D &a, const rn::Offset3D &b) {
	return !(a == b);
}

bool operator==(rn::BufferHandle a, rn::BufferHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::BufferHandle a, rn::BufferHandle b) {
	return a.index != b.index;
}

rn::BufferUsage operator|(rn::BufferUsage left, rn::BufferUsage right) {
	return static_cast<rn::BufferUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

rn::BufferUsage operator&(rn::BufferUsage left, rn::BufferUsage right) {
	return static_cast<rn::BufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

bool operator==(const rn::BufferDescription &a, const rn::BufferDescription &b) {
	return std::tie(a.size, a.usage, a.paging) == std::tie(b.size, b.usage, b.paging);
}

bool operator!=(const rn::BufferDescription &a, const rn::BufferDescription &b) {
	return !(a == b);
}

bool operator==(const rn::TextureDescription &a, const rn::TextureDescription &b) {
	return std::tie(a.format, a.dimensions, a.type, a.levels, a.layers, a.usage) == std::tie(b.format, b.dimensions, b.type, b.levels, b.layers, b.usage);
}

bool operator!=(const rn::TextureDescription &a, const rn::TextureDescription &b) {
	return !(a == b);
}

bool operator==(rn::TextureHandle a, rn::TextureHandle b) {
	return a.index == b.index;
}

bool operator!=(rn::TextureHandle a, rn::TextureHandle b) {
	return a.index != b.index;
}

rn::TextureUsage operator|(rn::TextureUsage left, rn::TextureUsage right) {
	return static_cast<rn::TextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

rn::TextureUsage operator&(rn::TextureUsage left, rn::TextureUsage right) {
	return static_cast<rn::TextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

} // rn
