#include "synchronizationCreator.hpp"

#include "../trace.hpp"

namespace rn::vki::context {

rn::vki::context::Synchronization SynchronizationCreator::create(const rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device) {
	size_t renderingResourcesCount = surfaceDescription.images.size();
	std::vector<rn::vki::context::Synchronization::RenderingResources> renderingResources{};
	renderingResources.resize(renderingResourcesCount);

	for (size_t i = 0; i < renderingResourcesCount; i++) {
		renderingResources[i] = {
			/*.acquireSemaphore=*/ { RN_VKI_TRACE(device->createSemaphoreUnique({}, nullptr, device.table())), device.table() },
			/*.graphicSemaphores=*/ {
				rn::vki::UniqueSemaphore{ RN_VKI_TRACE(device->createSemaphoreUnique({}, nullptr, device.table())), device.table() },
				rn::vki::UniqueSemaphore{ RN_VKI_TRACE(device->createSemaphoreUnique({}, nullptr, device.table())), device.table() }
			},
			/*.graphicFence=*/ { RN_VKI_TRACE(device->createFenceUnique({ vk::FenceCreateFlagBits::eSignaled }, nullptr, device.table())), device.table() },
			/*.computeFence=*/ { RN_VKI_TRACE(device->createFenceUnique({ vk::FenceCreateFlagBits::eSignaled }, nullptr, device.table())), device.table() }
		};
	}

	constexpr size_t initialTransferFenceCount = 3;
	std::vector<rn::vki::UniqueFence> transferFences{};
	transferFences.resize(initialTransferFenceCount);

	for (size_t i = 0; i < initialTransferFenceCount; i++) {
		transferFences[i] = { RN_VKI_TRACE(device->createFenceUnique({ vk::FenceCreateFlagBits::eSignaled }, nullptr, device.table())), device.table() };
	}

	return {
		/*.renderingResources=*/ std::move(renderingResources),
		/*.renderingIndex=*/ 0,
		/*.renderingCounter=*/ 0,
		/*.transferFences=*/ std::move(transferFences),
		/*.resolvedIndex=*/ 0,
		/*.resolvedCounter=*/ 0,
		/*.pendingIndex=*/ 0,
		/*.pendingCounter=*/ 0,
	};
}

} // rn::vki::context
