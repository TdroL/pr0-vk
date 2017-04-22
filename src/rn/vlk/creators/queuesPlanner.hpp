#pragma once

#include <map>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>

#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

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
	std::vector<vk::QueueFamilyProperties> familyProperties{};
	std::vector<uint32_t> reservedCount{};

	const vk::SurfaceKHR &surface;
	const vk::Instance &instance;
	const vk::PhysicalDevice &physicalDevice;

	QueuesPlanner(const vk::SurfaceKHR &surface, const vk::Instance &instance, const vk::PhysicalDevice &physicalDevice)
		: surface(surface), instance(instance), physicalDevice(physicalDevice) {
		familyProperties = physicalDevice.getQueueFamilyProperties();
		reservedCount.resize(familyProperties.size(), 0u);
	}

	QueueIndices selectQueueIndices() {
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

	std::map<uint32_t, uint32_t> countQueueFamilyUsage() {
		return countQueueFamilyUsage(selectQueueIndices());
	}

	std::map<uint32_t, uint32_t> countQueueFamilyUsage(const QueueIndices &indices) {
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

	QueueFamilyIndex reserveQueue(const std::vector<QueueTypeMask> types, const TransferGranularity granularity = {0, 0, 0}) {
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

		/*
		if (type == QueueType::Graphics) {
			for (uint32_t i = 0; i < familyProperties.size(); i++) {
				if (reservedCount[i] >= familyProperties[i].queueCount) {
					continue;
				}

				// check if it's graphic without compute
				if ((familyProperties[i].queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)) == vk::QueueFlagBits::eGraphics) {
					return QueueFamilyIndex{i, reservedCount[i]++};
				}
			}
		}

		if (type == QueueType::Compute) {
			for (uint32_t i = 0; i < familyProperties.size(); i++) {
				if (reservedCount[i] >= familyProperties[i].queueCount) {
					continue;
				}

				// check if it's compute without graphic
				if ((familyProperties[i].queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)) == vk::QueueFlagBits::eCompute) {
					return QueueFamilyIndex{i, reservedCount[i]++};
				}
			}
		}

		if (type == QueueType::Transfer) {
			for (uint32_t i = 0; i < familyProperties.size(); i++) {
				if (reservedCount[i] >= familyProperties[i].queueCount) {
					continue;
				}

				// check if it's compute without graphic
				if ((familyProperties[i].queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer)) == vk::QueueFlagBits::eTransfer) {
					if (testQueueProperties(type, granularity, i)) {
						return QueueFamilyIndex{i, reservedCount[i]++};
					}
				}

				if ((familyProperties[i].queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer)) == (vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer)) {
					if (testQueueProperties(type, granularity, i)) {
						return QueueFamilyIndex{i, reservedCount[i]++};
					}
				}
			}
		}

		for (uint32_t i = 0; i < familyProperties.size(); i++) {
			if (reservedCount[i] >= familyProperties[i].queueCount) {
				continue;
			}

			if (testQueueProperties(type, granularity, i)) {
				return QueueFamilyIndex{i, reservedCount[i]++};
			}
		}
		*/
		return QueueFamilyIndex{};
	}

	QueueFamilyIndex shareWith(const std::vector<QueueFamilyIndex> &queueIndices, const std::vector<QueueTypeMask> types, const TransferGranularity granularity = {0, 0, 0}) {
		for (QueueTypeMask type : types) {
			for (uint32_t i = 0; i < queueIndices.size(); i++) {
				if (testQueueProperties(type, granularity, queueIndices[i].family)) {
					return queueIndices[i];
				}
			}
		}
		/*
		for (uint32_t i = 0; i < queueIndices.size(); i++) {
			if (testQueueProperties(type, granularity, queueIndices[i].family)) {
				return queueIndices[i];
			}
		}
		*/
		return QueueFamilyIndex{};
	}

	bool testQueueProperties(const QueueTypeMask type, const TransferGranularity &granularity, uint32_t i) {
		if ((type & QueueType::Presentation)) {
			if ( ! physicalDevice.getSurfaceSupportKHR(i, surface)) {
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

		/*
		if ((type & QueueType::Presentation) && ! glfw.getFamilyQueuePresentationSupport(instance, physicalDevice, i)) {
			return false;
		}

		if ((type & QueueType::Graphics) && ! (familyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
			return false;
		}

		if ((type & QueueType::Compute) && ! (familyProperties[i].queueFlags & vk::QueueFlagBits::eCompute)) {
			return false;
		}

		if ((type & QueueType::Transfer)) {
			if ( ! (familyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer)) {
				return false;
			}

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

		if ((type & QueueType::SparseBinding) && ! (familyProperties[i].queueFlags & vk::QueueFlagBits::eSparseBinding)) {
			return false;
		}

		return true;
		*/
	}
};

} // vlk

} // rn