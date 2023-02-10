#include "swapchainVK.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <tuple>
#include <vector>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#include <util/contains.hpp>
#include <util/map.hpp>

namespace ware::swapchainVK {

[[nodiscard]] std::tuple<vk::SurfaceFormatKHR, vk::PresentModeKHR, vk::UniqueSwapchainKHR, std::vector<ImageResources>, std::vector<FrameResources>> createSwapchain(const ware::config::State &config, const ware::windowGLFW::State &window, const ware::contextVK::State &context) {
	const auto surfaceFormats = context.physicalDevice.getSurfaceFormatsKHR(context.surface.get());
	if (surfaceFormats.empty()) {
		throw std::runtime_error{"Vulkan surface formats not available"};
	}

	vk::SurfaceFormatKHR surfaceFormat = surfaceFormats[0];

	vk::SurfaceFormatKHR desiredSurfaceFormat{
		.format = config.vk.surfaceFormat == vk::Format::eUndefined ? vk::Format::eB8G8R8A8Unorm : config.vk.surfaceFormat,
		.colorSpace = config.vk.surfaceColorSpace,
	};
	bool foundMatch = util::containsIf(surfaceFormats, [=] (const auto &surfaceFormat) {
		// if undefined format exists, then any format is accepted
		return surfaceFormat.format == vk::Format::eUndefined || surfaceFormat == desiredSurfaceFormat;
	});
	if (foundMatch) {
		surfaceFormat = desiredSurfaceFormat;
	}

	vk::Extent2D imageExtent{
		.width = static_cast<uint32_t>(std::max(1, window.description->width)),
		.height = static_cast<uint32_t>(std::max(1, window.description->height)),
	};

	const auto presentModes = context.physicalDevice.getSurfacePresentModesKHR(context.surface.get());

	// FIFO is required to be always present and valid
	vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

	std::array desiredPresentModes{
		config.vk.swapchainPresentMode,
		vk::PresentModeKHR::eMailbox,
		vk::PresentModeKHR::eFifoRelaxed,
		vk::PresentModeKHR::eImmediate,
	};
	for (const auto desiredPresentMode : desiredPresentModes) {
		if (util::contains(presentModes, desiredPresentMode)) {
			presentMode = desiredPresentMode;
			break;
		}
	}

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(context.surface.get());
	const uint32_t desiredImageCount = config.vk.swapchainImageCount > 0
		? static_cast<uint32_t>(config.vk.swapchainImageCount)
		: presentMode == vk::PresentModeKHR::eMailbox
			? 3
			: 2;
	const uint32_t imageCount = std::clamp(surfaceCapabilities.minImageCount, desiredImageCount, surfaceCapabilities.maxImageCount);

	std::vector<uint32_t> queueFamilies{};
	if (context.presentationQueueFamily != context.graphicQueueFamily) {
		queueFamilies = { context.presentationQueueFamily, context.graphicQueueFamily };
	}

	auto swapchain = context.device->createSwapchainKHRUnique({
		.flags = vk::SwapchainCreateFlagsKHR{},
		.surface = context.surface.get(),
		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,
		.imageExtent = imageExtent,
		.imageArrayLayers = 1,
		.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
		.imageSharingMode = queueFamilies.size() > 1 ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
		.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilies.size()),
		.pQueueFamilyIndices = queueFamilies.size() > 1 ? queueFamilies.data() : nullptr,
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
		.presentMode = presentMode,
		.clipped = VK_TRUE,
		.oldSwapchain = vk::SwapchainKHR{}
	});

	auto images = context.device->getSwapchainImagesKHR(swapchain.get());

	spdlog::debug("ware::swapchainVK::createSwapchain() => created swapchain (image count: {}, format: {}, color space: {}, present mode: {})", imageCount, vk::to_string(surfaceFormat.format), vk::to_string(surfaceFormat.colorSpace), vk::to_string(presentMode));

	std::vector<ImageResources> imageResources = util::map(images, [&] (vk::Image image) {
		vk::UniqueImageView imageView = context.device->createImageViewUnique({
			.image = image,
			.viewType = vk::ImageViewType::e2D,
			.format = surfaceFormat.format,
			.subresourceRange = {
				.aspectMask = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		});

		return ImageResources{
			.image = image,
			.imageView = std::move(imageView),
			.presentSemaphore = context.device->createSemaphoreUnique({}),
		};
	});

	std::vector<FrameResources> frameResources = util::mapRange(images.size(), [&] ([[maybe_unused]] size_t index) {
		// auto renderingCommandPool = context.device->createCommandPoolUnique({
		// 	.flags = vk::CommandPoolCreateFlagBits::eTransient,
		// 	.queueFamilyIndex = context.graphicQueueFamily,
		// });

		// std::vector<vk::CommandBuffer> commandBuffers = context.device->allocateCommandBuffers({
		// 	.commandPool = renderingCommandPool.get(),
		// 	.level = vk::CommandBufferLevel::ePrimary,
		// 	.commandBufferCount = 1,
		// });

		return FrameResources{
			.renderingFence = context.device->createFenceUnique({
				.flags = vk::FenceCreateFlagBits::eSignaled,
			}),
			.acquireSemaphore = context.device->createSemaphoreUnique({}),
			// .renderingCommandPool = std::move(renderingCommandPool),
			// .renderingCommandBuffer = commandBuffers[0],
		};
	});

	return { surfaceFormat, presentMode, std::move(swapchain), std::move(imageResources), std::move(frameResources) };
}

void recreateSwapchain(State &state) {
	const auto &config = state.config;
	const auto &window = state.window;
	auto &context = state.context;

	ware::contextVK::requestWaitIdle(context);

	state.frameResources.clear();
	state.imageResources.clear();
	state.swapchain.reset();

	auto [surfaceFormat, presentMode, swapchain, imageResources, frameResources] = createSwapchain(config, window, context);

	state.surfaceFormat = surfaceFormat;
	state.presentMode = presentMode;
	state.swapchain = std::move(swapchain);
	state.imageResources = std::move(imageResources);
	state.imageIndex = 0;
	state.frameResources = std::move(frameResources);
	state.frameIndex = 0;
}

void acquireNextImage(State &state) {
	const auto &context = state.context;
	const auto &swapchain = state.swapchain.get();
	const auto &frameResources = state.frameResources[state.frameIndex];

	vk::ResultValue<uint32_t> acquireResult{vk::Result::eTimeout, 0};
	do {
		acquireResult = context.device->acquireNextImage2KHR({
			.swapchain = swapchain,
			.timeout = 10000U,
			.semaphore = frameResources.acquireSemaphore.get(),
			.fence = vk::Fence{},
			.deviceMask = 0x0001, // A device mask value is valid if every bit that is set in the mask is at a bit position that is less than the number of physical devices in the logical device.
		});
	} while (acquireResult.result == vk::Result::eTimeout);

	state.imageIndex = acquireResult.value;
}

// void render([[maybe_unused]]State &state) {
// 	const auto &context = state.context;
// 	const auto &imageResources = state.imageResources[state.imageIndex];
// 	const auto &frameResources = state.frameResources[state.frameIndex];

// 	context.device->resetCommandPool(frameResources.renderingCommandPool.get());

// 	frameResources.renderingCommandBuffer.begin({
// 		.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
// 	});

// 	// clear swapchain image
// 	vk::ImageSubresourceRange  swapchainSubresourceRange{
// 		.aspectMask = vk::ImageAspectFlagBits::eColor,
// 		.baseMipLevel = 0,
// 		.levelCount = 1,
// 		.baseArrayLayer = 0,
// 		.layerCount = 1,
// 	};

// 	frameResources.renderingCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eBottomOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, {}, {}, vk::ImageMemoryBarrier{
// 		.srcAccessMask = vk::AccessFlagBits::eMemoryRead,
// 		.dstAccessMask = vk::AccessFlagBits::eTransferWrite,
// 		.oldLayout = vk::ImageLayout::eUndefined,
// 		.newLayout = vk::ImageLayout::eTransferDstOptimal,
// 		.srcQueueFamilyIndex = context.presentationQueueFamily,
// 		.dstQueueFamilyIndex = context.graphicQueueFamily,
// 		.image = imageResources.image,
// 		.subresourceRange = swapchainSubresourceRange,
// 	});

// 	vk::ClearColorValue clearColorValue{std::array<float, 4>{ 1.0f, 0.0f, 1.0f, 0.0f }};
// 	frameResources.renderingCommandBuffer.clearColorImage(imageResources.image, vk::ImageLayout::eTransferDstOptimal, clearColorValue, swapchainSubresourceRange);

// 	frameResources.renderingCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, {}, {}, {}, vk::ImageMemoryBarrier{
// 		.srcAccessMask = vk::AccessFlagBits::eTransferWrite,
// 		.dstAccessMask = vk::AccessFlagBits::eMemoryRead,
// 		.oldLayout = vk::ImageLayout::eTransferDstOptimal,
// 		.newLayout = vk::ImageLayout::ePresentSrcKHR,
// 		.srcQueueFamilyIndex = context.graphicQueueFamily,
// 		.dstQueueFamilyIndex = context.presentationQueueFamily,
// 		.image = imageResources.image,
// 		.subresourceRange = swapchainSubresourceRange,
// 	});

// 	frameResources.renderingCommandBuffer.end();

// 	vk::SemaphoreSubmitInfo waitSemaphoreInfo{
// 		.semaphore = frameResources.acquireSemaphore.get(),
// 		.stageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
// 		.deviceIndex = 0,
// 	};
// 	vk::CommandBufferSubmitInfo commandBufferInfo{
// 		.commandBuffer = frameResources.renderingCommandBuffer,
// 		.deviceMask = 0x0001,
// 	};
// 	vk::SemaphoreSubmitInfo signalSemaphoreInfo{
// 		.semaphore = imageResources.presentSemaphore.get(),
// 		.stageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
// 		.deviceIndex = 0,
// 	};
// 	context.graphicQueue.submit2({
// 		{
// 			.waitSemaphoreInfoCount = 1,
// 			.pWaitSemaphoreInfos = &waitSemaphoreInfo,
// 			.commandBufferInfoCount = 1,
// 			.pCommandBufferInfos = &commandBufferInfo,
// 			.signalSemaphoreInfoCount = 1,
// 			.pSignalSemaphoreInfos = &signalSemaphoreInfo,
// 		}
// 	}, frameResources.renderingFence.get());
// }

void presentImage(State &state) {
	const auto &context = state.context;
	const auto &swapchain = state.swapchain.get();
	const auto &imageResources = state.imageResources[state.imageIndex];

	vk::PresentInfoKHR presentInfo{
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &imageResources.presentSemaphore.get(),
		.swapchainCount = 1,
		.pSwapchains = &swapchain,
		.pImageIndices = &state.imageIndex,
		.pResults = nullptr
	};
	vk::Result result = context.presentationQueue.presentKHR(&presentInfo);

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
		spdlog::debug("ware::swapchainVK::recreateSwapchain() => recreate swapchain (result: {})", vk::to_string(result));

		recreateSwapchain(state);
	} else if (result != vk::Result::eSuccess) {
		throw std::runtime_error{fmt::format("Unable to present swapchain image (error: {})", vk::to_string(result))};
	}
}

State::~State() {
	ware::contextVK::requestWaitIdle(context);
}

State setup(ware::config::State &config, ware::windowGLFW::State &window, ware::contextVK::State &context) {
	auto [surfaceFormat, presentMode, swapchain, imageResources, frameResources] = createSwapchain(config, window, context);

	return State{
		.config = config,
		.window = window,
		.context = context,
		.surfaceFormat = surfaceFormat,
		.presentMode = presentMode,
		.swapchain = std::move(swapchain),
		.imageResources = std::move(imageResources),
		.imageIndex = 0,
		.frameResources = std::move(frameResources),
		.frameIndex = 0,
		.description = {
			.width = window.description->width,
			.height = window.description->height,
			.mode = window.description->mode,
			.swapchainResized = false,
			.changed = false,
		},
	};
}

void refresh(State &state) {
	ZoneScopedN("ware::swapchainVK::refresh()");

	const auto &window = state.window;
	const auto &context = state.context;

	if (window.description->changed && (state.description.width != window.description->width || state.description.height != window.description->height || state.description.mode != window.description->mode)) {
		recreateSwapchain(state);
		state.description.width = window.description->width;
		state.description.height = window.description->height;
		state.description.mode = window.description->mode;
		state.description.swapchainResized = true;
		state.description.changed = true;
	} else if (state.description.swapchainResized) {
		state.description.swapchainResized = false;
		state.description.changed = true;
	}

	{
		const auto &frameResources = state.frameResources[state.frameIndex];

		vk::Result waitResult;
		while ((waitResult = context.device->waitForFences({ frameResources.renderingFence.get() }, true, std::numeric_limits<uint64_t>::max())) != vk::Result::eSuccess) {
			spdlog::debug("ware::swapchainVK::refresh() => retrying wait for rendering fence (result: {}, frame index: {})", vk::to_string(waitResult), state.frameIndex);
		}

		context.device->resetFences({ frameResources.renderingFence.get() });
	}

	acquireNextImage(state);
}

void process(State &state) {
	ZoneScopedN("ware::swapchainVK::process()");

	// render(state);

	presentImage(state);

	state.frameIndex = static_cast<uint32_t>((state.frameIndex + 1) % state.frameResources.size());

	if (state.description.changed) {
		state.description.changed = false;
	}
}

} // ware::swapchainVK
