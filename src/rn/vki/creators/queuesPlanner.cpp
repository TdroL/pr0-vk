#include "queuesPlanner.hpp"

#include <map>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>

#include <vulkan/vulkan.hpp>

#include "../trace.hpp"

namespace rn::vki {

QueuesPlanner::QueuesPlanner(const vk::SurfaceKHR &surface, const vk::PhysicalDevice &physicalDevice) :
	surface(surface),
	physicalDevice(physicalDevice),
	familyProperties(RN_VLK_TRACE(physicalDevice.getQueueFamilyProperties())),
	reservedCount(familyProperties.size(), 0u)
{}

QueueIndices QueuesPlanner::selectQueueIndices() {
	reservedCount.clear();
	reservedCount.resize(familyProperties.size(), 0u);

	QueueIndices indices{};

	indices.graphic = reserveQueue({
		QueueType::Presentation | QueueType::Graphics,
		QueueType::Presentation | QueueType::Graphics | QueueType::Compute,
		QueueType::Presentation | QueueType::Graphics | QueueType::AnyOther
	});

	if (indices.graphic) {
		indices.presentation = indices.graphic;
	} else {
		indices.graphic = reserveQueue({
			QueueType::Graphics,
			QueueType::Graphics | QueueType::Compute,
			QueueType::Graphics | QueueType::AnyOther
		});

		indices.presentation = reserveQueue({
			QueueType::Presentation,
			QueueType::Presentation | QueueType::AnyOther
		});
	}

	assert(indices.graphic);
	assert(indices.presentation);

	indices.compute = reserveQueue({
		QueueType::Compute,
		QueueType::Compute | QueueType::Transfer,
		QueueType::Compute | QueueType::Graphics,
		QueueType::Compute | QueueType::AnyOther
	});

	if ( ! indices.compute) {
		indices.compute = shareWith({
			indices.graphic,
			indices.presentation
		}, {
			QueueType::Compute | QueueType::Graphics,
			QueueType::Compute | QueueType::Presentation,
			QueueType::Compute | QueueType::AnyOther
		});
	}

	assert(indices.compute);

	indices.transfer = reserveQueue({
		QueueType::Transfer,
		QueueType::Transfer | QueueType::Compute,
		QueueType::Transfer | QueueType::Graphics,
		QueueType::Transfer | QueueType::AnyOther,
		// graphic and compute queues can act as a transfer queue
		QueueType::Compute | QueueType::AnyOther,
		QueueType::Graphics | QueueType::AnyOther
	}, {1, 1, 1});

	if ( ! indices.transfer) {
		indices.transfer = shareWith({
			indices.graphic,
			indices.presentation,
			indices.compute
		}, {
			QueueType::Transfer | QueueType::AnyOther,
			QueueType::Compute | QueueType::AnyOther,
			QueueType::Graphics | QueueType::AnyOther
		}, {1, 1, 1});
	}

	assert(indices.transfer);

	return indices;
}

std::map<uint32_t, uint32_t> QueuesPlanner::countQueueFamilyUsage() {
	return countQueueFamilyUsage(selectQueueIndices());
}

std::map<uint32_t, uint32_t> QueuesPlanner::countQueueFamilyUsage(const QueueIndices &indices) {
	std::map<uint32_t, uint32_t> usageCount{
		{ indices.presentation.family, 0 },
		{ indices.graphic.family, 0 },
		{ indices.compute.family, 0 },
		{ indices.transfer.family, 0 }
	};

	usageCount[indices.presentation.family] = std::max(usageCount[indices.presentation.family], indices.presentation.index + 1);
	usageCount[indices.graphic.family]      = std::max(usageCount[indices.graphic.family], indices.graphic.index + 1);
	usageCount[indices.compute.family]      = std::max(usageCount[indices.compute.family], indices.compute.index + 1);
	usageCount[indices.transfer.family]     = std::max(usageCount[indices.transfer.family], indices.transfer.index + 1);

	return usageCount;
}

QueueFamilyIndex QueuesPlanner::reserveQueue(const std::vector<QueueTypeMask> types, const TransferGranularity granularity) {
	int x = 1;
	for (QueueTypeMask type : types) {
		for (uint32_t i = 0; i < familyProperties.size(); i++) {
			if (reservedCount[i] >= familyProperties[i].queueCount) {
				continue;
			}

			if (testQueueProperties(type, granularity, i)) {
				return QueueFamilyIndex{i, reservedCount[i]++};
			}
		}
		x++;
	}

	return QueueFamilyIndex{};
}

QueueFamilyIndex QueuesPlanner::shareWith(const std::vector<QueueFamilyIndex> &queueIndices, const std::vector<QueueTypeMask> types, const TransferGranularity granularity) {
	for (QueueTypeMask type : types) {
		for (uint32_t i = 0; i < queueIndices.size(); i++) {
			if (testQueueProperties(type, granularity, queueIndices[i].family)) {
				return queueIndices[i];
			}
		}
	}

	return QueueFamilyIndex{};
}

bool QueuesPlanner::testQueueProperties(const QueueTypeMask type, const TransferGranularity &granularity, uint32_t i) {
	if ((type & QueueType::Presentation)) {
		if ( ! RN_VLK_TRACE(physicalDevice.getSurfaceSupportKHR(i, surface))) {
			return false;
		}
	}

	if ((type & QueueType::Transfer)) {
		if (familyProperties[i].minImageTransferGranularity.width > granularity.width) {
			return false;
		}

		if (familyProperties[i].minImageTransferGranularity.height > granularity.height) {
			return false;
		}

		if (familyProperties[i].minImageTransferGranularity.depth > granularity.depth) {
			return false;
		}
	}

	vk::QueueFlags flags{};
	if ((type & QueueType::Graphics)) {
		flags |= vk::QueueFlagBits::eGraphics;
	}

	if ((type & QueueType::Compute)) {
		flags |= vk::QueueFlagBits::eCompute;
	}

	if ((type & QueueType::Transfer)) {
		flags |= vk::QueueFlagBits::eTransfer;
	}

	if ((type & QueueType::SparseBinding)) {
		flags |= vk::QueueFlagBits::eSparseBinding;
	}

	if (flags) {
		if ((type & QueueType::AnyOther)) {
			if ((familyProperties[i].queueFlags & flags) != flags) {
				return false;
			}
		} else {
			if (familyProperties[i].queueFlags != flags) {
				return false;
			}
		}
	}

	return true;
}

} // rn::vki
