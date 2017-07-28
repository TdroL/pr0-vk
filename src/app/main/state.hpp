#pragma once

// #include <vector>
// #include <limits>
// #include <iterator>

// #include <vulkan/vulkan.hpp>

#include <ngn/prof.hpp>

#include <rn/window.hpp>
#include <rn/vlk/context.hpp>

#include "resources.hpp"

namespace app::main {

class State {
public:
	RingPool resources;

	rn::Window &window;
	rn::vlk::Context &context;

	State(rn::Window &window, rn::vlk::Context &context) :
		resources(window, context), window(window), context(context)
	{}

	void init() {
		resources.init();
	}

	void deinit() {
		resources.deinit();
	}

	void refresh() {
		//
	}

	void recordCommandBuffer(Handles &handles) {
		ngn::prof::Scope profScope{"::recordCommandBuffer()"};

		vk::ClearValue clearColor{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};

		vk::CommandBufferBeginInfo commandBufferBegin{};
		commandBufferBegin.flags = vk::CommandBufferUsageFlags{};
		commandBufferBegin.pInheritanceInfo = nullptr;

		vk::RenderPassBeginInfo renderPassBegin{};
		renderPassBegin.renderPass = handles.renderPass;
		renderPassBegin.framebuffer = handles.framebuffer;
		renderPassBegin.renderArea.offset = vk::Offset2D{0, 0};
		renderPassBegin.renderArea.extent = context.handles.surfaceExtent;
		renderPassBegin.clearValueCount = 1;
		renderPassBegin.pClearValues = &clearColor;

		handles.commandBuffer.begin(commandBufferBegin);
			handles.commandBuffer.beginRenderPass(renderPassBegin, vk::SubpassContents::eInline);

				handles.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, handles.pipeline);
				handles.commandBuffer.draw(3, 1, 0, 0);

			handles.commandBuffer.endRenderPass();
		handles.commandBuffer.end();
	}

	void submit(Handles &handles) {
		ngn::prof::Scope profScope{"::submit()"};

		// submit command buffer to queue
		vk::Semaphore submitWaitSemaphores[] {
			handles.acquireSemaphore
		};

		vk::PipelineStageFlags submitWaitStages[] {
			vk::PipelineStageFlagBits::eColorAttachmentOutput
		};

		static_assert(std::size(submitWaitSemaphores) == std::size(submitWaitStages));

		vk::Semaphore submitSignalSemaphores[] {
			handles.submitSemaphore
		};

		vk::SubmitInfo submitInfo{};
		submitInfo.waitSemaphoreCount = std::size(submitWaitSemaphores);
		submitInfo.pWaitSemaphores = submitWaitSemaphores;
		submitInfo.pWaitDstStageMask = submitWaitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &handles.commandBuffer;
		submitInfo.signalSemaphoreCount = std::size(submitSignalSemaphores);
		submitInfo.pSignalSemaphores = submitSignalSemaphores;

		handles.queues.graphic.submit({submitInfo}, handles.submitFence);
	}

	void present(Handles &handles) {
		ngn::prof::Scope profScope{"::present()"};

		vk::Semaphore presentWaitSemaphores[] {
			handles.submitSemaphore
		};

		// present
		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = std::size(presentWaitSemaphores);
		presentInfo.pWaitSemaphores = presentWaitSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &handles.swapchain;
		presentInfo.pImageIndices = &handles.imageIndex;
		presentInfo.pResults = nullptr;
		handles.queues.presentation.presentKHR(presentInfo);
	}

	void render() {
		ngn::prof::Scope profScope{"app::main::State::render()"};

		Handles handles = resources.next();

		recordCommandBuffer(handles);

		submit(handles);

		present(handles);
	}
};

} // app::main
