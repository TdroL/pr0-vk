#include <vector>

#include "../../types.hpp"

namespace rn::db::loader {

template<class T>
struct LoadModelResults {
	std::vector<rn::db::Texture<T>> textures{};
	std::vector<rn::db::Buffer<T>> buffers{};
};

} // rn::db::loader