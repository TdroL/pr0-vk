#pragma once

// #include <vector>
// #include <limits>
// #include <iterator>

// #include <vulkan/vulkan.hpp>

#include <ngn/prof.hpp>

#include <rn/window.hpp>
#include <rn/vlk/context.hpp>
#include <rn/vlk/trace.hpp>

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
		NGN_PROF_SCOPE("::recordCommandBuffer()");

		vk::CommandBufferBeginInfo commandBufferBegin{
			/*.flags=*/ vk::CommandBufferUsageFlags{},
			/*.pInheritanceInfo=*/ nullptr
		};

		vk::ClearValue clearColor{ std::array<float, 4>{ 1.0f, 0.8f, 0.4f, 0.0f } };
		vk::RenderPassBeginInfo renderPassBegin{
			/*.renderPass=*/ handles.renderPass,
			/*.framebuffer=*/ handles.framebuffer,
			/*.renderArea=*/ {
				/*.offset=*/ vk::Offset2D{0, 0},
				/*.extent=*/ context.surface.extent
			},
			/*.clearValueCount=*/ 1,
			/*.pClearValues=*/ &clearColor
		};

		RN_VLK_TRACE(handles.commandBuffer.begin(commandBufferBegin));
			vk::ImageSubresourceRange imageSubresourceRange{
				/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
				/*.baseMipLevel=*/ 0,
				/*.levelCount=*/ 1,
				/*.baseArrayLayer=*/ 0,
				/*.layerCount=*/ 1
			};

			if (context.queue.presentation != context.queue.graphic) {
				vk::ImageMemoryBarrier barrierFromPresentToDraw{
					/*.srcAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
					/*.dstAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
					/*.oldLayout=*/ vk::ImageLayout::ePresentSrcKHR,
					/*.newLayout=*/ vk::ImageLayout::ePresentSrcKHR,
					/*.srcQueueFamilyIndex=*/ context.family.presentation,
					/*.dstQueueFamilyIndex=*/ context.family.graphic,
					/*.image=*/ handles.surfaceImage,
					/*.subresourceRange=*/ imageSubresourceRange
				};

				RN_VLK_TRACE(handles.commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlags{}, {}, {}, { barrierFromPresentToDraw }));
			}

			RN_VLK_TRACE(handles.commandBuffer.beginRenderPass(renderPassBegin, vk::SubpassContents::eInline));

				RN_VLK_TRACE(handles.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, handles.pipeline));

				vk::Viewport viewport{
					/*.x=*/ 0.0f,
					/*.y=*/ 0.0f,
					/*.width=*/ static_cast<float>(context.surface.extent.width),
					/*.height=*/ static_cast<float>(context.surface.extent.height),
					/*.minDepth=*/ 0.0f,
					/*.maxDepth=*/ 1.0f
				};
				RN_VLK_TRACE(handles.commandBuffer.setViewport(0, { viewport }));

				vk::Rect2D scissor{
					/*.offset=*/ vk::Offset2D{0, 0},
					/*.extent=*/ context.surface.extent
				};
				RN_VLK_TRACE(handles.commandBuffer.setScissor(0, { scissor }));

				RN_VLK_TRACE(handles.commandBuffer.bindVertexBuffers(0, { handles.modelBuffer }, { 0 }));
				RN_VLK_TRACE(handles.commandBuffer.draw(4, 1, 0, 0));

			RN_VLK_TRACE(handles.commandBuffer.endRenderPass());

			if (context.queue.presentation != context.queue.graphic) {
				vk::ImageMemoryBarrier barrierFromDrawToPresent{
					/*.srcAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
					/*.dstAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
					/*.oldLayout=*/ vk::ImageLayout::ePresentSrcKHR,
					/*.newLayout=*/ vk::ImageLayout::ePresentSrcKHR,
					/*.srcQueueFamilyIndex=*/ context.family.presentation,
					/*.dstQueueFamilyIndex=*/ context.family.graphic,
					/*.image=*/ handles.surfaceImage,
					/*.subresourceRange=*/ imageSubresourceRange
				};

				RN_VLK_TRACE(handles.commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlags{}, {}, {}, { barrierFromDrawToPresent }));
			}
		RN_VLK_TRACE(handles.commandBuffer.end());
	}

	void submit(Handles &handles) {
		NGN_PROF_SCOPE("::submit()");

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

		vk::SubmitInfo submitInfo{
			/*.waitSemaphoreCount=*/ std::size(submitWaitSemaphores),
			/*.pWaitSemaphores=*/ submitWaitSemaphores,
			/*.pWaitDstStageMask=*/ submitWaitStages,
			/*.commandBufferCount=*/ 1,
			/*.pCommandBuffers=*/ &handles.commandBuffer,
			/*.signalSemaphoreCount=*/ std::size(submitSignalSemaphores),
			/*.pSignalSemaphores=*/ submitSignalSemaphores
		};
		RN_VLK_TRACE(context.queue.graphic.submit({submitInfo}, handles.submitFence));
	}

	void present(Handles &handles) {
		NGN_PROF_SCOPE("::present()");

		vk::Semaphore presentWaitSemaphores[] {
			handles.submitSemaphore
		};

		// present
		vk::PresentInfoKHR presentInfo{
			/*.waitSemaphoreCount=*/ std::size(presentWaitSemaphores),
			/*.pWaitSemaphores=*/ presentWaitSemaphores,
			/*.swapchainCount=*/ 1,
			/*.pSwapchains=*/ &context.swapchain,
			/*.pImageIndices=*/ &handles.imageIndex,
			/*.pResults=*/ nullptr
		};
		RN_VLK_TRACE(context.queue.presentation.presentKHR(presentInfo));
	}

	void render() {
		NGN_PROF_SCOPE("app::main::State::render()");

		Handles handles = resources.next();

		recordCommandBuffer(handles);

		submit(handles);

		present(handles);
	}
};

} // app::main
