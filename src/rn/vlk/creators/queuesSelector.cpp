#include "queuesSelector.hpp"

#include <cassert>
#include <map>
#include <stdexcept>
#include <vector>

#include "queuesPlanner.hpp"

#include "../trace.hpp"

namespace rn::vlk {

std::tuple<Context::Queue, Context::Family> QueuesSelector::select(Context &context) {
	vk::Instance instance = context.instance;
	vk::SurfaceKHR surface = context.surface.handle;
	vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;
	vk::Device device = context.device;

	assert(surface);
	assert(instance);
	assert(physicalDevice);
	assert(device);

	QueuesPlanner planner{surface, physicalDevice};
	QueueIndices indices = planner.selectQueueIndices();

	std::map<uint32_t, uint32_t> usageCount = planner.countQueueFamilyUsage(indices);
	std::map<uint32_t, std::vector<vk::Queue>> queueMap{};

	for (const auto &kv : usageCount) {
		queueMap[kv.first] = std::vector<vk::Queue>(kv.second, vk::Queue{});

		for (uint32_t i = 0; i < kv.second; i++) {
			queueMap[kv.first][i] = RN_VLK_TRACE(device.getQueue(kv.first, i));
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

	return std::make_tuple(Context::Queue{
			presentation,
			graphic,
			compute,
			transfer
		},
		Context::Family{
			indices.presentation.family,
			indices.graphic.family,
			indices.compute.family,
			indices.transfer.family
		}
	);
}

} // rn::vlk
