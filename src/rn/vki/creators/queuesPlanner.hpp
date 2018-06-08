#pragma once

#include <map>
#include <vector>
#include <cassert>
#include <limits>

#include <vulkan/vulkan.hpp>

namespace rn::vki {

struct TransferGranularity {
	uint32_t width;
	uint32_t height;
	uint32_t depth;
};

struct QueueFamilyIndex {
	static constexpr uint32_t invalidValue = std::numeric_limits<uint32_t>::max();

	uint32_t family = invalidValue;
	uint32_t index = invalidValue;

	bool isValid() const {
		return (
			family != invalidValue &&
			index != invalidValue
		);
	}

	operator bool() const {
		return isValid();
	}

	bool operator !() const {
		return ! isValid();
	}
};

struct QueueIndices {
	QueueFamilyIndex presentation{};
	QueueFamilyIndex graphic{};
	QueueFamilyIndex compute{};
	QueueFamilyIndex transfer{};

	bool areValid() const {
		return (
			presentation &&
			graphic &&
			compute &&
			transfer
		);
	}

	operator bool() const {
		return areValid();
	}

	bool operator !() const {
		return ! areValid();
	}
};

enum QueueType {
	None = 0,
	Presentation = 1,
	Graphics = 2,
	Compute = 4,
	Transfer = 8,
	SparseBinding = 16,
	AnyOther = 0x40000000,
	Max_Enum = 0x7FFFFFFF
};

typedef uint32_t QueueTypeMask;

static_assert(sizeof(QueueType) <= sizeof(QueueTypeMask), "Size of QueueType must be smaller or equal to size of QueueTypeMask");

struct QueuesPlanner {
	const vk::SurfaceKHR &surface;
	const vk::PhysicalDevice &physicalDevice;

	std::vector<vk::QueueFamilyProperties> familyProperties{};
	std::vector<uint32_t> reservedCount{};

	QueuesPlanner(const vk::SurfaceKHR &surface, const vk::PhysicalDevice &physicalDevice);

	QueueIndices selectQueueIndices();

	std::map<uint32_t, uint32_t> countQueueFamilyUsage();

	std::map<uint32_t, uint32_t> countQueueFamilyUsage(const QueueIndices &indices);

	QueueFamilyIndex reserveQueue(const std::vector<QueueTypeMask> types, const TransferGranularity granularity = {0, 0, 0});

	QueueFamilyIndex shareWith(const std::vector<QueueFamilyIndex> &queueIndices, const std::vector<QueueTypeMask> types, const TransferGranularity granularity = {0, 0, 0});

	bool testQueueProperties(const QueueTypeMask type, const TransferGranularity &granularity, uint32_t i);
};

} // rn::vki
