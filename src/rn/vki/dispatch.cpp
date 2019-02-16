#include "dispatch.hpp"

namespace rn::vki {

vk::DispatchLoaderDynamic staticDispatchTable{};

template<>
void loadExtensions(vk::UniqueInstance &instance) {
	staticDispatchTable.init(*instance);
}

} // rn::vki
