#include "queueFamilyIndexSelector.hpp"

// #include <map>
// #include <stdexcept>
#include <optional>
#include <vector>

#include "../../../util/map.hpp"
#include "../../../ngn/log.hpp"
#include "../../types.hpp"
#include "../dispatch.hpp"
#include "../trace.hpp"

namespace rn::vki::context {

enum QueueFlags : uint32_t {
	None = 0x00000000,
	Presentation = 0x00000001,
	Graphic = 0x00000002,
	Compute = 0x00000004,
	Transfer = 0x00000008,
	SparseBinding = 0x00000010,
	AnyOther = 0x00000100,
};

bool testQueueProperties(rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandlePhysicalDevice &&physicalDevice, const std::vector<vk::QueueFamilyProperties2> &familyProperties, uint32_t flags, uint32_t family, const rn::Extent3D &granularity = {1, 1, 1}) {
	if ((flags & QueueFlags::Presentation)) {
		if ( ! RN_VKI_TRACE(physicalDevice->getSurfaceSupportKHR(family, surface.get(), physicalDevice.table()))) {
			return false;
		}
	}

	if ((flags & QueueFlags::Transfer)) {
		if (familyProperties[family].queueFamilyProperties.minImageTransferGranularity.width > granularity.width) {
			return false;
		}

		if (familyProperties[family].queueFamilyProperties.minImageTransferGranularity.height > granularity.height) {
			return false;
		}

		if (familyProperties[family].queueFamilyProperties.minImageTransferGranularity.depth > granularity.depth) {
			return false;
		}
	}

	vk::QueueFlags queueFlags{};
	if ((flags & QueueFlags::Graphic)) {
		queueFlags |= vk::QueueFlagBits::eGraphics;
	}

	if ((flags & QueueFlags::Compute)) {
		queueFlags |= vk::QueueFlagBits::eCompute;
	}

	if ((flags & QueueFlags::Transfer)) {
		queueFlags |= vk::QueueFlagBits::eTransfer;
	}

	if ((flags & QueueFlags::SparseBinding)) {
		queueFlags |= vk::QueueFlagBits::eSparseBinding;
	}

	if (queueFlags != vk::QueueFlags{}) {
		if ((flags & QueueFlags::AnyOther)) {
			if ((familyProperties[family].queueFamilyProperties.queueFlags & queueFlags) != queueFlags) {
				return false;
			}
		} else {
			if (familyProperties[family].queueFamilyProperties.queueFlags != queueFlags) {
				return false;
			}
		}
	}

	return true;
}

std::optional<rn::vki::context::QueueFamilyIndex::FamilyIndex> findFamilyIndex(rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandlePhysicalDevice &&physicalDevice, const std::vector<vk::QueueFamilyProperties2> familyProperties, uint32_t flags, const std::vector<std::vector<bool>> &reservations, bool allowIndexShading) {
	for (uint32_t i = 0; i < familyProperties.size(); i++) {
		if (reservations[i].empty()) {
			continue;
		}

		const auto availableIndexIt = std::find(std::begin(reservations[i]), std::end(reservations[i]), false);
		uint32_t index;
		if (availableIndexIt == std::end(reservations[i])) {
			if ( ! allowIndexShading) {
				// no indices available
				continue;
			} else {
				index = 0;
			}
		} else {
			index = static_cast<uint32_t>(std::distance(std::begin(reservations[i]), availableIndexIt));
		}

		if (testQueueProperties(surface.handle(), physicalDevice.handle(), familyProperties, flags, i)) {
			return rn::vki::context::QueueFamilyIndex::FamilyIndex{
				/*.family=*/ i,
				/*.index=*/ index,
			};
		}
	}

	return std::nullopt;
}

rn::vki::context::QueueFamilyIndex QueueFamilyIndexSelector::select(rn::vki::HandlePhysicalDevice &&physicalDevice, const rn::vki::context::PhysicalDeviceDescription &physicalDeviceDescription, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config) {
	std::vector<std::vector<bool>> reservations = util::map(physicalDeviceDescription.familyProperties, [] (const vk::QueueFamilyProperties2 &familyProperties) {
		std::vector<bool> flags{};
		flags.resize(familyProperties.queueFamilyProperties.queueCount, false);

		return flags;
	});

	const auto find = [&] (uint32_t flags, bool allowIndexShading = false) {
		return findFamilyIndex(surface.handle(), physicalDevice.handle(), physicalDeviceDescription.familyProperties, flags, reservations, allowIndexShading);
	};

	const auto test = [&] (uint32_t flags, uint32_t family) {
		return testQueueProperties(surface.handle(), physicalDevice.handle(), physicalDeviceDescription.familyProperties, flags, family);
	};

	if (config.core.context.vki.forcePresentQueue.family >= 0) {
		int32_t family = config.core.context.vki.forcePresentQueue.family;
		int32_t index = config.core.context.vki.forcePresentQueue.index;
		if (family >= static_cast<int32_t>(reservations.size())) {
			config.core.context.vki.forcePresentQueue.family = -1;
			config.core.context.vki.forcePresentQueue.index = -1;
			config.core.dirty = true;
		} else if (index >= static_cast<int32_t>(reservations[family].size())) {
			config.core.context.vki.forcePresentQueue.index = -1;
			config.core.dirty = true;
		}
	}

	if (config.core.context.vki.forceGraphicQueue.family >= 0) {
		int32_t family = config.core.context.vki.forceGraphicQueue.family;
		int32_t index = config.core.context.vki.forceGraphicQueue.index;
		if (family >= static_cast<int32_t>(reservations.size())) {
			config.core.context.vki.forceGraphicQueue.family = -1;
			config.core.context.vki.forceGraphicQueue.index = -1;
			config.core.dirty = true;
		} else if (index >= static_cast<int32_t>(reservations[family].size())) {
			config.core.context.vki.forceGraphicQueue.index = -1;
			config.core.dirty = true;
		}
	}

	if (config.core.context.vki.forceComputeQueue.family >= 0) {
		int32_t family = config.core.context.vki.forceComputeQueue.family;
		int32_t index = config.core.context.vki.forceComputeQueue.index;
		if (family >= static_cast<int32_t>(reservations.size())) {
			config.core.context.vki.forceComputeQueue.family = -1;
			config.core.context.vki.forceComputeQueue.index = -1;
			config.core.dirty = true;
		} else if (index >= static_cast<int32_t>(reservations[family].size())) {
			config.core.context.vki.forceComputeQueue.index = -1;
			config.core.dirty = true;
		}
	}

	if (config.core.context.vki.forceTransferQueue.family >= 0) {
		int32_t family = config.core.context.vki.forceTransferQueue.family;
		int32_t index = config.core.context.vki.forceTransferQueue.index;
		if (family >= static_cast<int32_t>(reservations.size())) {
			config.core.context.vki.forceTransferQueue.family = -1;
			config.core.context.vki.forceTransferQueue.index = -1;
			config.core.dirty = true;
		} else if (index >= static_cast<int32_t>(reservations[family].size())) {
			config.core.context.vki.forceTransferQueue.index = -1;
			config.core.dirty = true;
		}
	}

	rn::vki::context::QueueFamilyIndex::FamilyIndex presentation{};
	rn::vki::context::QueueFamilyIndex::FamilyIndex graphic{};
	rn::vki::context::QueueFamilyIndex::FamilyIndex compute{};
	rn::vki::context::QueueFamilyIndex::FamilyIndex transfer{};

	{
		int32_t family = config.core.context.vki.forcePresentQueue.family;
		int32_t index = config.core.context.vki.forcePresentQueue.index;

		if (family >= 0) {
			presentation.family = static_cast<uint32_t>(family);

			if (index >= 0) {
				reservations[static_cast<uint32_t>(family)][static_cast<uint32_t>(index)] = true;
				presentation.index = static_cast<uint32_t>(index);
			} else {
				// find first available
				const auto it = std::find(std::begin(reservations[static_cast<uint32_t>(family)]), std::end(reservations[static_cast<uint32_t>(family)]), false);
				if (it != std::end(reservations[static_cast<uint32_t>(family)])) {
					index = static_cast<int32_t>(std::distance(std::begin(reservations[static_cast<uint32_t>(family)]), it));
					presentation.index = static_cast<uint32_t>(index);
				} else {
					presentation.index = 0;
				}
			}
		} else {
			if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic)) {
				presentation = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::Compute)) {
				presentation = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::AnyOther)) {
				presentation = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::AnyOther, true)) {
				presentation = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation)) {
				presentation = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::AnyOther)) {
				presentation = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::AnyOther, true)) {
				presentation = *familyIndexO;
			} else {
				throw std::runtime_error{"No valid presentation queue family found"};
			}
		}

		reservations[static_cast<uint32_t>(presentation.family)][static_cast<uint32_t>(presentation.index)] = true;
	}

	{
		int32_t family = config.core.context.vki.forceGraphicQueue.family;
		int32_t index = config.core.context.vki.forceGraphicQueue.index;

		if (family >= 0) {
			graphic.family = static_cast<uint32_t>(family);

			if (index >= 0) {
				reservations[static_cast<uint32_t>(family)][static_cast<uint32_t>(index)] = true;
				graphic.index = static_cast<uint32_t>(index);
			} else {
				// find first available
				const auto it = std::find(std::begin(reservations[static_cast<uint32_t>(family)]), std::end(reservations[static_cast<uint32_t>(family)]), false);
				if (it != std::end(reservations[static_cast<uint32_t>(family)])) {
					index = static_cast<int32_t>(std::distance(std::begin(reservations[static_cast<uint32_t>(family)]), it));
					graphic.index = static_cast<uint32_t>(index);
				} else {
					graphic.index = 0;
				}
			}
		} else {
			if (test(QueueFlags::Presentation | QueueFlags::Graphic, presentation.family)) {
				graphic = presentation;
			} else if (test(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::AnyOther, presentation.family)) {
				graphic = presentation;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::Compute)) {
				graphic = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::AnyOther)) {
				graphic = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Presentation | QueueFlags::Graphic | QueueFlags::AnyOther, true)) {
				graphic = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic)) {
				graphic = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::Compute)) {
				graphic = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::AnyOther)) {
				graphic = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::AnyOther, true)) {
				graphic = *familyIndexO;
			} else {
				throw std::runtime_error{"No valid graphic queue family found"};
			}
		}

		reservations[static_cast<uint32_t>(presentation.family)][static_cast<uint32_t>(presentation.index)] = true;
	}

	{
		int32_t family = config.core.context.vki.forceComputeQueue.family;
		int32_t index = config.core.context.vki.forceComputeQueue.index;

		if (family >= 0) {
			compute.family = static_cast<uint32_t>(family);

			if (index >= 0) {
				reservations[static_cast<uint32_t>(family)][static_cast<uint32_t>(index)] = true;
				compute.index = static_cast<uint32_t>(index);
			} else {
				// find first available
				const auto it = std::find(std::begin(reservations[static_cast<uint32_t>(family)]), std::end(reservations[static_cast<uint32_t>(family)]), false);
				if (it != std::end(reservations[static_cast<uint32_t>(family)])) {
					index = static_cast<int32_t>(std::distance(std::begin(reservations[static_cast<uint32_t>(family)]), it));
					compute.index = static_cast<uint32_t>(index);
				} else {
					compute.index = 0;
				}
			}
		} else {
			if (auto familyIndexO = find(QueueFlags::Compute)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Compute | QueueFlags::Transfer)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Compute | QueueFlags::Graphic)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Compute | QueueFlags::AnyOther)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Compute | QueueFlags::AnyOther, true)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::AnyOther)) {
				compute = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::AnyOther, true)) {
				compute = *familyIndexO;
			} else {
				throw std::runtime_error{"No valid compute queue family found"};
			}
		}

		reservations[static_cast<uint32_t>(presentation.family)][static_cast<uint32_t>(presentation.index)] = true;
	}

	{
		int32_t family = config.core.context.vki.forceTransferQueue.family;
		int32_t index = config.core.context.vki.forceTransferQueue.index;

		if (family >= 0) {
			transfer.family = static_cast<uint32_t>(family);

			if (index >= 0) {
				reservations[static_cast<uint32_t>(family)][static_cast<uint32_t>(index)] = true;
				transfer.index = static_cast<uint32_t>(index);
			} else {
				// find first available
				const auto it = std::find(std::begin(reservations[static_cast<uint32_t>(family)]), std::end(reservations[static_cast<uint32_t>(family)]), false);
				if (it != std::end(reservations[static_cast<uint32_t>(family)])) {
					index = static_cast<int32_t>(std::distance(std::begin(reservations[static_cast<uint32_t>(family)]), it));
					reservations[static_cast<uint32_t>(family)][static_cast<uint32_t>(index)] = true;
					transfer.index = static_cast<uint32_t>(index);
				} else {
					reservations[static_cast<uint32_t>(family)][0] = true;
					transfer.index = 0;
				}
			}
		} else {
			if (auto familyIndexO = find(QueueFlags::Transfer)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Transfer | QueueFlags::Compute)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Transfer | QueueFlags::Graphic)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Transfer | QueueFlags::AnyOther)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Transfer | QueueFlags::AnyOther, true)) {
				transfer = *familyIndexO;
			// graphic and compute queues can act as a transfer queue
			} else if (auto familyIndexO = find(QueueFlags::Compute)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Compute | QueueFlags::AnyOther)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Compute | QueueFlags::AnyOther, true)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::AnyOther)) {
				transfer = *familyIndexO;
			} else if (auto familyIndexO = find(QueueFlags::Graphic | QueueFlags::AnyOther, true)) {
				transfer = *familyIndexO;
			} else {
				throw std::runtime_error{"No valid transfer queue family found"};
			}
		}

		reservations[static_cast<uint32_t>(presentation.family)][static_cast<uint32_t>(presentation.index)] = true;
	}
#ifdef RN_VKI_REINDEX_QUEUE_FAMILIES
	std::vector<std::vector<uint32_t>> indexMapping = util::map(reservations, [] (const auto &familyReservation) {
		uint32_t cursor = 0;

		return util::map(familyReservation, [&] (const auto &isReserved) {
			if (isReserved) {
				return cursor++;
			} else {
				return 0u;
			}
		});
	});

	ngn::log::debug("rn::vki::context::QueueFamilyIndexSelector::select() => presentation=[{}, {}] graphic=[{}, {}] compute=[{}, {}] transfer=[{}, {}]", presentation.family, indexMapping[presentation.family][presentation.index], graphic.family, indexMapping[graphic.family][graphic.index], compute.family, indexMapping[compute.family][compute.index], transfer.family, indexMapping[transfer.family][transfer.index]);

	return rn::vki::context::QueueFamilyIndex{
		/*.presentation=*/ {
			/*.family=*/ presentation.family,
			/*.index=*/ indexMapping[presentation.family][presentation.index]
		},
		/*.graphic=*/ {
			/*.family=*/ graphic.family,
			/*.index=*/ indexMapping[graphic.family][graphic.index]
		},
		/*.compute=*/ {
			/*.family=*/ compute.family,
			/*.index=*/ indexMapping[compute.family][compute.index]
		},
		/*.transfer=*/ {
			/*.family=*/ transfer.family,
			/*.index=*/ indexMapping[transfer.family][transfer.index]
		}
	};
#else
	ngn::log::debug("rn::vki::context::QueueFamilyIndexSelector::select() => presentation=[{}, {}] graphic=[{}, {}] compute=[{}, {}] transfer=[{}, {}]", presentation.family, presentation.index, graphic.family, graphic.index, compute.family, compute.index, transfer.family, transfer.index);

	return rn::vki::context::QueueFamilyIndex{
		/*.presentation=*/ std::move(presentation),
		/*.graphic=*/ std::move(graphic),
		/*.compute=*/ std::move(compute),
		/*.transfer=*/ std::move(transfer)
	};
#endif
}


// std::tuple<rn::vki::context::Queue, rn::vki::context::Family> QueueSelector::select(rn::vki::HandleSurfaceKHR &&surface, rn::vki::HandleDevice &&device, rn::vki::QueueFamilyIndex &&queueFamilyIndex) {





// 	QueuesPlanner planner{surface.handle(), physicalDevice.handle(), RN_VKI_TRACE(physicalDevice->getQueueFamilyProperties2(physicalDevice.table()))};
// 	QueueIndices indices = planner.selectQueueIndices();

// 	std::map<uint32_t, uint32_t> usageCount = planner.countQueueFamilyUsage(indices);
// 	std::map<uint32_t, std::vector<vk::Queue>> queueMap{};

// 	for (const auto &kv : usageCount) {
// 		queueMap[kv.first] = std::vector<vk::Queue>(kv.second, vk::Queue{});

// 		for (uint32_t i = 0; i < kv.second; i++) {
// 			queueMap[kv.first][i] = RN_VKI_TRACE(device->getQueue(kv.first, i, device.table()));
// 		}
// 	}

// 	vk::Queue presentation = queueMap[indices.presentation.family][indices.presentation.index];
// 	vk::Queue graphic = queueMap[indices.graphic.family][indices.graphic.index];
// 	vk::Queue compute = queueMap[indices.compute.family][indices.compute.index];
// 	vk::Queue transfer = queueMap[indices.transfer.family][indices.transfer.index];

// 	if ( ! presentation) {
// 		throw std::runtime_error{"Vulkan presentation queue could not be created"};
// 	}

// 	if ( ! graphic) {
// 		throw std::runtime_error{"Vulkan graphic queue could not be created"};
// 	}

// 	if ( ! compute) {
// 		throw std::runtime_error{"Vulkan compute queue could not be created"};
// 	}

// 	if ( ! transfer) {
// 		throw std::runtime_error{"Vulkan transfer queue could not be created"};
// 	}

// 	ngn::log::debug("rn::vki::context::QueueSelector::select() => families: presentation={} graphic={} compute={} transfer={}", indices.presentation.family, indices.graphic.family, indices.compute.family, indices.transfer.family);

// 	return std::make_tuple(
// 		rn::vki::context::Queue{
// 			/*.presentation=*/ presentation,
// 			/*.graphic=*/ graphic,
// 			/*.compute=*/ compute,
// 			/*.transfer=*/ transfer
// 		},
// 		rn::vki::context::Family{
// 			/*.presentation=*/ indices.presentation.family,
// 			/*.graphic=*/ indices.graphic.family,
// 			/*.compute=*/ indices.compute.family,
// 			/*.transfer=*/ indices.transfer.family
// 		}
// 	);
// }

} // rn::vki::context
