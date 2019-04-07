#include "dispatch.hpp"

namespace rn::vki {

vk::DispatchLoaderDynamic staticDispatchTable{};

template<>
void loadExtensions(vk::UniqueInstance &instance) {
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddrTemp = reinterpret_cast<PFN_vkGetInstanceProcAddr>(instance->getProcAddr("vkGetInstanceProcAddr"));

	staticDispatchTable.init(*instance, vkGetInstanceProcAddrTemp);
}

} // rn::vki
