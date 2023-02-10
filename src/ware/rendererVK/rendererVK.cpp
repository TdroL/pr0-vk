#include "rendererVK.hpp"

#include <tracy/Tracy.hpp>

#include <util/map.hpp>

namespace ware::rendererVK {

std::vector<FrameResources> createFrameResources(ware::contextVK::State &context, ware::swapchainVK::State &swapchain) {
	return util::mapRange(swapchain.frameResources.size(), [&] ([[maybe_unused]] const auto &index) {
		auto renderingCommandPool = context.device->createCommandPoolUnique({
			.flags = vk::CommandPoolCreateFlagBits::eTransient,
			.queueFamilyIndex = context.graphicQueueFamily,
		});

		std::vector<vk::CommandBuffer> commandBuffers = context.device->allocateCommandBuffers({
			.commandPool = renderingCommandPool.get(),
			.level = vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = 1,
		});

		return FrameResources{
			.renderingCommandPool = std::move(renderingCommandPool),
			.renderingCommandBuffer = commandBuffers[0],
		};
	});
}

void recreateFrameResources(State &state) {
	auto &context = state.context;
	auto &swapchain = state.swapchain;

	if (state.swapchain.description.swapchainResized && state.frameResources.size() != swapchain.frameResources.size()) {
		ware::contextVK::requestWaitIdle(context);

		state.frameResources.clear();

		state.frameResources = createFrameResources(context, swapchain);
	}
}

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::contextImgui::State &imgui, ware::swapchainVK::State &swapchain) {
	auto frameResources = createFrameResources(context, swapchain);

	return State{
		.window = window,
		.context = context,
		.swapchain = swapchain,
		.frameResources = std::move(frameResources),
		.stateImgui = passes::imgui::setup(window, context, imgui, swapchain),
		.stateSimple = passes::simple::setup(window, context, swapchain),
	};
}

void refresh([[maybe_unused]] State &state) {
	ZoneScopedN("ware::rendererVK::refresh()");

	if (state.swapchain.description.changed) {
		recreateFrameResources(state);
	}

	passes::imgui::refresh(state.stateImgui);
	passes::simple::refresh(state.stateSimple);
}

void process([[maybe_unused]] State &state) {
	ZoneScopedN("ware::rendererVK::process()");

	const auto &context = state.context;
	const auto &swapchain = state.swapchain;
	const auto &swapchainImageResources = swapchain.imageResources[swapchain.imageIndex];
	const auto &swapchainFrameResources = swapchain.frameResources[swapchain.frameIndex];
	const auto &frameResources = state.frameResources[swapchain.frameIndex];

	auto &cmd = frameResources.renderingCommandBuffer;

	context.device->resetCommandPool(frameResources.renderingCommandPool.get());

	auto cmdSimple = passes::simple::process(state.stateSimple);
	auto cmdImgui = passes::imgui::process(state.stateImgui);

	{
		ZoneScopedN("ware::rendererVK::process()#record");

		cmd.begin({
			.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
		});

		cmd.executeCommands({ cmdSimple, cmdImgui });

		cmd.end();
	}

	{
		ZoneScopedN("ware::rendererVK::process()#submit");

		vk::SemaphoreSubmitInfo waitSemaphoreInfo{
			.semaphore = swapchainFrameResources.acquireSemaphore.get(),
			.stageMask = vk::PipelineStageFlagBits2::eFragmentShader,
		};
		vk::CommandBufferSubmitInfo commandBufferInfo{
			.commandBuffer = cmd,
		};
		vk::SemaphoreSubmitInfo signalSemaphoreInfo{
			.semaphore = swapchainImageResources.presentSemaphore.get(),
			.stageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		};
		context.graphicQueue.submit2({
			{
				.waitSemaphoreInfoCount = 1,
				.pWaitSemaphoreInfos = &waitSemaphoreInfo,
				.commandBufferInfoCount = 1,
				.pCommandBufferInfos = &commandBufferInfo,
				.signalSemaphoreInfoCount = 1,
				.pSignalSemaphoreInfos = &signalSemaphoreInfo,
			}
		}, swapchainFrameResources.renderingFence.get());
	}
}

} // ware::rendererVK
