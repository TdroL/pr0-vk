#include "queueSelector.hpp"

#include <map>
#include <stdexcept>
// #include <vector>

// #include "../../../ngn/log.hpp"
#include "../dispatch.hpp"
#include "../trace.hpp"
// #include "queuesPlanner.hpp"

namespace rn::vki::context {

rn::vki::context::Queue QueueSelector::select(rn::vki::HandleDevice &&device, const rn::vki::context::QueueFamilyIndex &queueFamilyIndex) {
	std::map<std::pair<uint32_t, uint32_t>, vk::Queue> queueMap{};
	vk::Queue presentation{};
	vk::Queue graphic{};
	vk::Queue compute{};
	vk::Queue transfer{};

	{
		const auto key = std::make_pair(queueFamilyIndex.presentation.family, queueFamilyIndex.presentation.index);
		auto it = queueMap.find(key);
		if (it == std::end(queueMap)) {
			it = queueMap.insert_or_assign(key, RN_VKI_TRACE(device->getQueue(key.first, key.second, device.table()))).first;
		}

		presentation = it->second;
	}

	{
		const auto key = std::make_pair(queueFamilyIndex.graphic.family, queueFamilyIndex.graphic.index);
		auto it = queueMap.find(key);
		if (it == std::end(queueMap)) {
			it = queueMap.insert_or_assign(key, RN_VKI_TRACE(device->getQueue(key.first, key.second, device.table()))).first;
		}

		graphic = it->second;
	}

	{
		const auto key = std::make_pair(queueFamilyIndex.compute.family, queueFamilyIndex.compute.index);
		auto it = queueMap.find(key);
		if (it == std::end(queueMap)) {
			it = queueMap.insert_or_assign(key, RN_VKI_TRACE(device->getQueue(key.first, key.second, device.table()))).first;
		}

		compute = it->second;
	}

	{
		const auto key = std::make_pair(queueFamilyIndex.transfer.family, queueFamilyIndex.transfer.index);
		auto it = queueMap.find(key);
		if (it == std::end(queueMap)) {
			it = queueMap.insert_or_assign(key, RN_VKI_TRACE(device->getQueue(key.first, key.second, device.table()))).first;
		}

		transfer = it->second;
	}

	// {
	// 	auto familyIt = familyQueueCount.find(queueFamilyIndex.presentation.family);
	// 	uint32_t currentCount = 0;
	// 	if (familyIt != std::end(familyQueueCount)) {
	// 		currentCount = familyId->second;
	// 	}

	// 	if (currentCount < queueFamilyIndex.presentation.index + 1) {
	// 		familyQueueCount.insert_or_assign(queueFamilyIndex.presentation.family, queueFamilyIndex.presentation.index + 1);
	// 	}
	// }

	// {
	// 	auto familyIt = familyQueueCount.find(queueFamilyIndex.graphic.family);
	// 	uint32_t currentCount = 0;
	// 	if (familyIt != std::end(familyQueueCount)) {
	// 		currentCount = familyId->second;
	// 	}

	// 	if (currentCount < queueFamilyIndex.graphic.index + 1) {
	// 		familyQueueCount.insert_or_assign(queueFamilyIndex.graphic.family, queueFamilyIndex.graphic.index + 1);
	// 	}
	// }

	// {
	// 	auto familyIt = familyQueueCount.find(queueFamilyIndex.compute.family);
	// 	uint32_t currentCount = 0;
	// 	if (familyIt != std::end(familyQueueCount)) {
	// 		currentCount = familyId->second;
	// 	}

	// 	if (currentCount < queueFamilyIndex.compute.index + 1) {
	// 		familyQueueCount.insert_or_assign(queueFamilyIndex.compute.family, queueFamilyIndex.compute.index + 1);
	// 	}
	// }

	// {
	// 	auto familyIt = familyQueueCount.find(queueFamilyIndex.transfer.family);
	// 	uint32_t currentCount = 0;
	// 	if (familyIt != std::end(familyQueueCount)) {
	// 		currentCount = familyId->second;
	// 	}

	// 	if (currentCount < queueFamilyIndex.transfer.index + 1) {
	// 		familyQueueCount.insert_or_assign(queueFamilyIndex.transfer.family, queueFamilyIndex.transfer.index + 1);
	// 	}
	// }

	// for (const auto &kv : familyQueueCount) {
	// 	queueMap[kv.first] = std::vector<vk::Queue>(kv.second, vk::Queue{});

	// 	for (uint32_t i = 0; i < kv.second; i++) {
	// 		queueMap[kv.first][i] = RN_VKI_TRACE(device->getQueue(kv.first, i, device.table()));
	// 	}
	// }

	// ----------------
	// QueuesPlanner planner{surface.handle(), physicalDevice.handle(), RN_VKI_TRACE(physicalDevice->getQueueFamilyProperties2(physicalDevice.table()))};
	// QueueIndices indices = planner.selectQueueIndices();

	// std::map<uint32_t, uint32_t> usageCount = planner.countQueueFamilyUsage(indices);
	// std::map<uint32_t, std::vector<vk::Queue>> queueMap{};

	// for (const auto &kv : usageCount) {
	// 	queueMap[kv.first] = std::vector<vk::Queue>(kv.second, vk::Queue{});

	// 	for (uint32_t i = 0; i < kv.second; i++) {
	// 		queueMap[kv.first][i] = RN_VKI_TRACE(device->getQueue(kv.first, i, device.table()));
	// 	}
	// }

	// vk::Queue presentation = queueMap[indices.presentation.family][indices.presentation.index];
	// vk::Queue graphic = queueMap[indices.graphic.family][indices.graphic.index];
	// vk::Queue compute = queueMap[indices.compute.family][indices.compute.index];
	// vk::Queue transfer = queueMap[indices.transfer.family][indices.transfer.index];

	// if ( ! presentation) {
	// 	throw std::runtime_error{"Vulkan presentation queue could not be created"};
	// }

	// if ( ! graphic) {
	// 	throw std::runtime_error{"Vulkan graphic queue could not be created"};
	// }

	// if ( ! compute) {
	// 	throw std::runtime_error{"Vulkan compute queue could not be created"};
	// }

	// if ( ! transfer) {
	// 	throw std::runtime_error{"Vulkan transfer queue could not be created"};
	// }

	// ngn::log::debug("rn::vki::context::QueueSelector::select() => families: presentation={} graphic={} compute={} transfer={}", indices.presentation.family, indices.graphic.family, indices.compute.family, indices.transfer.family);

	return rn::vki::context::Queue{
		/*.presentation=*/ presentation,
		/*.graphic=*/ graphic,
		/*.compute=*/ compute,
		/*.transfer=*/ transfer
	};
}

} // rn::vki::context
