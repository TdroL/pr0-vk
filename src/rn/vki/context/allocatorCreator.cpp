#include "allocatorCreator.hpp"

#include "../memory/pool.hpp"
#include "../dispatch.hpp"

namespace rn::vki::context {

rn::vki::context::Allocator AllocatorCreator::create(rn::vki::HandleDevice &&device, rn::vki::context::PhysicalDeviceDescription &physicalDeviceDescription) {
	return {
		/*.buffer=*/  rn::vki::memory::Pool{device.handle(), physicalDeviceDescription.memoryProperties, 10, 64 * 1024 * 1024},
		/*.texture=*/ rn::vki::memory::Pool{device.handle(), physicalDeviceDescription.memoryProperties, 8, 256 * 1024 * 1024},
		/*.staging=*/ rn::vki::memory::Pool{device.handle(), physicalDeviceDescription.memoryProperties, 10, 64 * 1024 * 1024},
	};
}

} // rn::vki::context

