// #include "texture.hpp"

// #include <tuple>

// namespace rn::db::texture {

// bool operator==(const rn::db::texture::Description &a, const rn::db::texture::Description &b) {
// 	return std::tie(a.format, a.dimensions, a.type, a.levels, a.layers, a.usage) == std::tie(b.format, b.dimensions, b.type, b.levels, b.layers, b.usage);
// }

// bool operator!=(const rn::db::texture::Description &a, const rn::db::texture::Description &b) {
// 	return !(a == b);
// }

// bool operator==(const rn::db::texture::Key &a, const rn::db::texture::Key &b) {
// 	return std::tie(a.name, a.description) == std::tie(b.name, b.description);
// }

// bool operator!=(const rn::db::texture::Key &a, const rn::db::texture::Key &b) {
// 	return !(a == b);
// }

// } // rn::db::texture