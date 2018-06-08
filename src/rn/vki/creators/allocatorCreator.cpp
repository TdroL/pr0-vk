#include "allocatorCreator.hpp"

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../device.hpp"
#include "../memory/pool.hpp"

namespace rn::vki {

Context::Allocator AllocatorCreator::create(Context &context) {
	vk::PhysicalDeviceMemoryProperties &memoryProperties = context.physicalDevice.memoryProperties;
	Device &device = context.device;

	return {
		/*.buffer=*/  memory::Pool{device, memoryProperties, 8, 4 * 1024 * 1024},
		/*.texture=*/ memory::Pool{device, memoryProperties, 8, 256 * 1024 * 1024},
		/*.staging=*/ memory::Pool{device, memoryProperties, 8, 16 * 1024 * 1024},
	};
}

} // rn::vki

