#include "context.hpp"

#include <tuple>

#include "../../ngn/config.hpp"
#include "../../ngn/log.hpp"
#include "../../util/map.hpp"

#include "../window.hpp"

#include "trace.hpp"

#include "context/instanceCreator.hpp"
#include "context/surfaceCreator.hpp"
#include "context/debugCreator.hpp"
#include "context/deviceCreator.hpp"
#include "context/swapchainCreator.hpp"
#include "context/surfaceImageViewsCreator.hpp"
#include "context/allocatorCreator.hpp"

#include "context/physicalDeviceSelector.hpp"
#include "context/queuesSelector.hpp"
#include "context/muticesSelector.hpp"
#include "context/surfaceExtentSelector.hpp"
#include "context/surfaceFormatSelector.hpp"
#include "context/surfaceImagesSelector.hpp"

namespace rn::vki {

Context Context::factory(ngn::config::Config &config, rn::Window &window) {
	struct Creators {
		rn::vki::context::InstanceCreator instance{};
		rn::vki::context::DebugCreator debug{};
		rn::vki::context::SurfaceCreator surface{};
		rn::vki::context::DeviceCreator device{};
		rn::vki::context::SwapchainCreator swapchain{};
		rn::vki::context::SurfaceImageViewsCreator surfaceImageViews{};
		rn::vki::context::AllocatorCreator allocator{};
	} creators{};

	struct Selectors {
		rn::vki::context::PhysicalDeviceSelector physicalDevice{};
		rn::vki::context::QueuesSelector queues{};
		rn::vki::context::MuticesSelector mutices{};
		rn::vki::context::SurfaceExtentSelector surfaceExtent{};
		rn::vki::context::SurfaceFormatSelector surfaceFormat{};
		rn::vki::context::SurfaceImagesSelector surfaceImages{};
	} selectors{};

	Context context{};

	context.instance = creators.instance.create(config);

	context.debug = creators.debug.create(context.instance.handle(), config);

	context.surface = creators.surface.create(context.instance.handle(), window);

	std::tie(context.physicalDevice, context.physicalDeviceDescription) = selectors.physicalDevice.select(context.instance.handle(), context.surface.handle(), config);

	context.device = creators.device.create(context.instance.handle(), context.surface.handle(), context.physicalDevice.handle(), context.physicalDeviceDescription);

	std::tie(context.queue, context.family) = selectors.queues.select(context.surface.handle(), context.device.handle(), context.physicalDevice.handle());

	context.mutex = selectors.mutices.select(context.queue);

	context.surfaceDescription.extent = selectors.surfaceExtent.select(context.surface.handle(), context.physicalDevice.handle(), config);
	context.surfaceDescription.format = selectors.surfaceFormat.select(context.surface.handle(), context.physicalDevice.handle(), config);

	context.swapchain = creators.swapchain.create(context.surfaceDescription, context.family, context.surface.handle(), context.device.handle(), context.physicalDevice.handle(), config);

	context.surfaceDescription.images = selectors.surfaceImages.select(context.swapchain.handle(), context.device.handle());

	context.surfaceImageViews = creators.surfaceImageViews.create(context.surfaceDescription, context.device.handle(), config);
	context.surfaceDescription.imageViews = util::map(context.surfaceImageViews, [] (auto &imageView) {
		return imageView.get();
	});

	context.allocator = creators.allocator.create(context.device.handle(), context.physicalDeviceDescription);

	return context;
}

Context::Context(Context &&other) {
	*this = std::move(other);
}

Context & Context::operator=(Context &&other) {
	reset();

	allocator = std::move(other.allocator);
	surfaceDescription = std::move(other.surfaceDescription);
	mutex = std::move(other.mutex);

	surfaceImageViews = std::move(other.surfaceImageViews);
	swapchain = std::move(other.swapchain);
	device = std::move(other.device);
	surface = std::move(other.surface);

	debug = std::move(other.debug);

	physicalDevice = std::move(other.physicalDevice);
	instance = std::move(other.instance);

	return *this;
}

Context::~Context() {
	reset();
}

void Context::reset() {
	waitIdle();

	allocator = {};
	surfaceDescription = {};
	mutex = {};

	surfaceImageViews.clear();
	swapchain.reset();
	device.reset();
	surface.reset();

	debug = {};

	physicalDevice.reset();
	instance.reset();
}

void Context::waitIdle() {
	if (device) {
		ngn::log::debug("wait for device");
		RN_VKI_TRACE(device->waitIdle(device.table()));

		if (queue.presentation) {
			ngn::log::debug("wait for queue.presentation");
			RN_VKI_TRACE(queue.presentation.waitIdle(device.table()));
		}
	}
}

} // rn::vki
