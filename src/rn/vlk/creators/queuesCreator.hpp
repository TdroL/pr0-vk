#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

#include <vulkan/vulkan.hpp>

#include "../queuesOwner.hpp"
#include "../surfaceOwner.hpp"
#include "../instanceOwner.hpp"
#include "../physicalDeviceOwner.hpp"
#include "queuesPlanner.hpp"
#include "../../../ngn/log.hpp"

namespace rn {

namespace vlk {

class QueuesCreator {
public:
	QueuesOwner create(SurfaceOwner &surfaceOwner, InstanceOwner &instanceOwner, PhysicalDeviceOwner &physicalDeviceOwner, DeviceOwner &deviceOwner) {
		vk::SurfaceKHR &surface = surfaceOwner.handle;
		vk::Instance &instance = instanceOwner.handle;
		vk::PhysicalDevice &physicalDevice = physicalDeviceOwner.handle;
		vk::Device &device = deviceOwner.handle;

		assert(surface);
		assert(instance);
		assert(physicalDevice);
		assert(device);

		QueuesPlanner planner{surface, instance, physicalDevice};
		QueueIndices indices = planner.selectQueueIndices();

		std::map<uint32_t, uint32_t> usageCount = planner.countQueueFamilyUsage(indices);
		std::map<uint32_t, std::vector<vk::Queue>> queueMap{};

		for (const auto &kv : usageCount) {
			queueMap[kv.first] = std::vector<vk::Queue>(kv.second, vk::Queue{});

			for (uint32_t i = 0; i < kv.second; i++) {
				queueMap[kv.first][i] = device.getQueue(kv.first, i);
			}
		}

		vk::Queue presentation = queueMap[indices.presentation.family][indices.presentation.index];
		vk::Queue graphic = queueMap[indices.graphic.family][indices.graphic.index];
		vk::Queue compute = queueMap[indices.compute.family][indices.compute.index];
		vk::Queue transfer = queueMap[indices.transfer.family][indices.transfer.index];

		QueuesOwner queuesOwner{
			std::move(presentation),
			std::move(graphic),
			std::move(compute),
			std::move(transfer),
			device
		};

		if ( ! queuesOwner.presentation) {
			throw std::runtime_error{"Vulkan presentation queue could not be created"};
		}

		if ( ! queuesOwner.graphic) {
			throw std::runtime_error{"Vulkan graphic queue could not be created"};
		}

		if ( ! queuesOwner.compute) {
			throw std::runtime_error{"Vulkan compute queue could not be created"};
		}

		if ( ! queuesOwner.transfer) {
			throw std::runtime_error{"Vulkan transfer queue could not be created"};
		}

		return queuesOwner;
	}
};

} // vlk

} // rn
