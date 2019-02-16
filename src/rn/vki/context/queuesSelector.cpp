#include "queuesSelector.hpp"

#include <map>
#include <stdexcept>
#include <vector>

#include "../dispatch.hpp"
#include "../trace.hpp"
#include "queuesPlanner.hpp"

namespace rn::vki::context {

std::tuple<rn::vki::context::Queue, rn::vki::context::Family> QueuesSelector::select(rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandleDevice &&device, rn::vki::HandlePhysicalDevice &&physicalDevice) {
	QueuesPlanner planner{surface.handle(), physicalDevice.handle(), RN_VKI_TRACE(physicalDevice->getQueueFamilyProperties2(physicalDevice.table()))};
	QueueIndices indices = planner.selectQueueIndices();

	std::map<uint32_t, uint32_t> usageCount = planner.countQueueFamilyUsage(indices);
	std::map<uint32_t, std::vector<vk::Queue>> queueMap{};

	for (const auto &kv : usageCount) {
		queueMap[kv.first] = std::vector<vk::Queue>(kv.second, vk::Queue{});

		for (uint32_t i = 0; i < kv.second; i++) {
			queueMap[kv.first][i] = RN_VKI_TRACE(device->getQueue(kv.first, i, device.table()));
		}
	}

	vk::Queue presentation = queueMap[indices.presentation.family][indices.presentation.index];
	vk::Queue graphic = queueMap[indices.graphic.family][indices.graphic.index];
	vk::Queue compute = queueMap[indices.compute.family][indices.compute.index];
	vk::Queue transfer = queueMap[indices.transfer.family][indices.transfer.index];

	if ( ! presentation) {
		throw std::runtime_error{"Vulkan presentation queue could not be created"};
	}

	if ( ! graphic) {
		throw std::runtime_error{"Vulkan graphic queue could not be created"};
	}

	if ( ! compute) {
		throw std::runtime_error{"Vulkan compute queue could not be created"};
	}

	if ( ! transfer) {
		throw std::runtime_error{"Vulkan transfer queue could not be created"};
	}

	return std::make_tuple(
		rn::vki::context::Queue{
			presentation,
			graphic,
			compute,
			transfer
		},
		rn::vki::context::Family{
			indices.presentation.family,
			indices.graphic.family,
			indices.compute.family,
			indices.transfer.family
		}
	);
}

} // rn::vki::context
