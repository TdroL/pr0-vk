#pragma once

#include <map>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

#include "queuesOwner.hpp"
#include "queuesPlanner.hpp"
#include "../../ngn/log.hpp"

namespace rn {

namespace vlk {

class QueuesCreator {
public:
	QueuesOwner create(GLFWOwner &glfwOwner, InstanceOwner &instanceOwner, PhysicalDeviceOwner &physicalDeviceOwner, DeviceOwner &deviceOwner) {
		GLFW &glfw = glfwOwner.handle;
		vk::Instance &instance = instanceOwner.handle;
		vk::PhysicalDevice &physicalDevice = physicalDeviceOwner.handle;
		vk::Device &device = deviceOwner.handle;

		QueuesPlanner planner{glfw, instance, physicalDevice};
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

		return QueuesOwner{
			std::move(presentation),
			std::move(graphic),
			std::move(compute),
			std::move(transfer),
			device
		};
	}
};

} // vlk

} // rn
