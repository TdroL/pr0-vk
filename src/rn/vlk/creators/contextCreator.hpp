#pragma once

#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../../../ngn/str.hpp"

#include "../../window.hpp"

#include "../context.hpp"

#include "instanceCreator.hpp"
#include "surfaceCreator.hpp"
#include "debugCallbackCreator.hpp"
#include "deviceCreator.hpp"
#include "swapchainCreator.hpp"
#include "surfaceImageViewsCreator.hpp"

#include "physicalDeviceSelector.hpp"
#include "queuesSelector.hpp"
#include "surfaceExtentSelector.hpp"
#include "surfaceFormatSelector.hpp"
#include "surfaceImagesSelector.hpp"

namespace rn {

namespace vlk {

class ContextCreator {
public:
	rn::Window &window;

	struct Creators {
		InstanceCreator instance{};
		DebugCallbackCreator debugCallback{};
		SurfaceCreator surface{};
		DeviceCreator device{};
		SwapchainCreator swapchain{};
		SurfaceImageViewsCreator surfaceImageViews{};
	} creators{};

	struct Selectors {
		PhysicalDeviceSelector physicalDevice{};
		QueuesSelector queues{};
		SurfaceExtentSelector surfaceExtent{};
		SurfaceFormatSelector surfaceFormat{};
		SurfaceImagesSelector surfaceImages{};
	} selectors;

	ContextCreator(rn::Window &window)
		: window(window) {}

	Context create() {
		Context context{};

		context.owners.instance = createInstance(context.owners, context.handles);
		context.owners.debugCallback = createDebugCallback(context.owners, context.handles);
		context.owners.surface = createSurface(context.owners, context.handles);

		context.handles.physicalDevice = selectPhysicalDevice(context.owners, context.handles);

		context.owners.device = createDevice(context.owners, context.handles);
		context.device = context.owners.device.get();

		context.handles.queues = selectQueues(context.owners, context.handles);
		context.queues.presentation = context.handles.queues.presentation.handle;
		context.queues.graphic = context.handles.queues.graphic.handle;
		context.queues.compute = context.handles.queues.compute.handle;
		context.queues.transfer = context.handles.queues.transfer.handle;

		context.handles.surfaceExtent = selectSurfaceExtent(context.owners, context.handles);
		context.handles.surfaceFormat = selectSurfaceFormat(context.owners, context.handles);
		context.owners.swapchain = createSwapchain(context.owners, context.handles);
		context.handles.surfaceImages = selectSurfaceImages(context.owners, context.handles);
		context.owners.surfaceImageViews = createSurfaceImageViews(context.owners, context.handles);
		context.swapchain = context.owners.swapchain.get();

		// if (ngn::config::core.dirty() && ! ngn::config::core.store()) {
		// 	ngn::log::error("Failed to store core config in \"{}\":\n{}", ngn::config::core.source(), ngn::config::core.dump());
		// }

		return context;
	}

	vk::UniqueInstance createInstance(Context::Owners &/*owners*/, Context::Handles &/*handles*/) {
		return creators.instance.create(creators.debugCallback, creators.surface);
	}

	vk::UniqueDebugReportCallbackEXT createDebugCallback(Context::Owners &owners, Context::Handles &/*handles*/) {
		return creators.debugCallback.create(owners.instance);
	}

	vk::UniqueSurfaceKHR createSurface(Context::Owners &owners, Context::Handles &/*handles*/) {
		return creators.surface.create(window, owners.instance);
	}

	PhysicalDeviceHandle selectPhysicalDevice(Context::Owners &owners, Context::Handles &/*handles*/) {
		return selectors.physicalDevice.select(owners.surface, owners.instance);
	}

	vk::UniqueDevice createDevice(Context::Owners &owners, Context::Handles &handles) {
		return creators.device.create(owners.surface, owners.instance, handles.physicalDevice);
	}

	QueuesHandle selectQueues(Context::Owners &owners, Context::Handles &handles) {
		return selectors.queues.select(owners.surface, owners.instance, handles.physicalDevice, owners.device);
	}

	vk::Extent2D selectSurfaceExtent(Context::Owners &owners, Context::Handles &handles) {
		return selectors.surfaceExtent.select(owners.surface, handles.physicalDevice);
	}

	vk::SurfaceFormatKHR selectSurfaceFormat(Context::Owners &owners, Context::Handles &handles) {
		return selectors.surfaceFormat.select(owners.surface, handles.physicalDevice);
	}

	vk::UniqueSwapchainKHR createSwapchain(Context::Owners &owners, Context::Handles &handles) {
		return creators.swapchain.create(owners.surface, handles.physicalDevice, owners.device, handles.queues, handles.surfaceExtent, handles.surfaceFormat);
	}

	std::vector<vk::Image> selectSurfaceImages(Context::Owners &owners, Context::Handles &/*handles*/) {
		return selectors.surfaceImages.select(owners.device, owners.swapchain);
	}

	std::vector<vk::UniqueImageView> createSurfaceImageViews(Context::Owners &owners, Context::Handles &handles) {
		return creators.surfaceImageViews.create(owners.device, handles.surfaceImages);
	}
};

} // vlk

} // rn
