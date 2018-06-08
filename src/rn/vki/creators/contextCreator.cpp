#include "contextCreator.hpp"

#include <tuple>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"

#include "../../../util/map.hpp"

#include "instanceCreator.hpp"
#include "extensionsStaticLoader.hpp"
#include "surfaceCreator.hpp"
#include "debugCallbackCreator.hpp"
#include "deviceCreator.hpp"
#include "swapchainCreator.hpp"
#include "surfaceImageViewsCreator.hpp"
#include "allocatorCreator.hpp"

#include "physicalDeviceSelector.hpp"
#include "queuesSelector.hpp"
#include "muticesSelector.hpp"
#include "surfaceExtentSelector.hpp"
#include "surfaceFormatSelector.hpp"
#include "surfaceImagesSelector.hpp"

namespace rn::vki {

struct Creators {
	InstanceCreator instance{};
	DebugCallbackCreator debugCallback{};
	SurfaceCreator surface{};
	DeviceCreator device{};
	SwapchainCreator swapchain{};
	SurfaceImageViewsCreator surfaceImageViews{};
	AllocatorCreator allocator{};
};

struct Loaders {
	ExtensionsStaticLoader extensionsStatic{};
};

struct Selectors {
	PhysicalDeviceSelector physicalDevice{};
	QueuesSelector queues{};
	MuticesSelector mutices{};
	SurfaceExtentSelector surfaceExtent{};
	SurfaceFormatSelector surfaceFormat{};
	SurfaceImagesSelector surfaceImages{};
};

Context ContextCreator::create() {
	Creators creators{};
	Loaders loaders{};
	Selectors selectors{};

	Context context{};

	context.owners.instance = creators.instance.create(context, creators.debugCallback, creators.surface);
	context.instance = context.owners.instance.get();

	loaders.extensionsStatic.loadInstance(context);

	context.owners.debugCallback = creators.debugCallback.create(context);

	context.owners.surface = creators.surface.create(context, window);
	context.surface.handle = context.owners.surface.get();

	context.physicalDevice = selectors.physicalDevice.select(context);

	context.owners.device = creators.device.create(context);
	context.device = Device{context.owners.device.get(), context.owners.instance.get()};

	loaders.extensionsStatic.loadDevice(context);

	std::tie(context.queue, context.family) = selectors.queues.select(context);

	context.mutex = selectors.mutices.select(context);

	context.surface.extent = selectors.surfaceExtent.select(context);
	context.surface.format = selectors.surfaceFormat.select(context);

	context.owners.swapchain = creators.swapchain.create(context);
	context.swapchain = context.owners.swapchain.get();

	context.surface.images = selectors.surfaceImages.select(context);

	context.owners.surfaceImageViews = creators.surfaceImageViews.create(context);
	context.surface.imageViews = util::map(context.owners.surfaceImageViews, [] (auto &imageView) {
		return imageView.get();
	});

	context.allocator = creators.allocator.create(context);

	// if (ngn::config::core.dirty() && ! ngn::config::core.store()) {
	// 	ngn::log::error("Failed to store core config in \"{}\":\n{}", ngn::config::core.source(), ngn::config::core.dump());
	// }

	return context;
}

} // rn::vki
