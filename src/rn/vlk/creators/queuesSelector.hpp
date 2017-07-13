#pragma once

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/log.hpp"

#include "../physicalDeviceHandle.hpp"
#include "queuesPlanner.hpp"

namespace rn {

namespace vlk {

class QueuesSelector {
public:
	QueuesHandle select(vk::UniqueSurfaceKHR &surfaceOwner, vk::UniqueInstance &instanceOwner, PhysicalDeviceHandle &physicalDeviceOwner, vk::UniqueDevice &deviceOwner) {
		vk::SurfaceKHR surface = surfaceOwner.get();
		vk::Instance instance = instanceOwner.get();
		vk::PhysicalDevice physicalDevice = physicalDeviceOwner.handle;
		vk::Device device = deviceOwner.get();

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

		return QueuesHandle{
			{ std::move(presentation), indices.presentation.family },
			{ std::move(graphic), indices.graphic.family },
			{ std::move(compute), indices.compute.family },
			{ std::move(transfer), indices.transfer.family }
		};
	}
};

} // vlk

} // rn
