#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include <ngn/fs.hpp>
#include <ngn/log.hpp>
#include <ngn/prof.hpp>

#include <rn/window.hpp>
#include <rn/vlk/context.hpp>

namespace app::main {

struct Queues {
	vk::Queue presentation{};
	vk::Queue graphic{};
	vk::Queue compute{};
	vk::Queue transfer{};
};

struct Families {
	uint32_t presentation{};
	uint32_t graphic{};
	uint32_t compute{};
	uint32_t transfer{};
};

struct Handles {
	vk::Device device{};
	Queues queues{};
	Families families{};
	vk::SwapchainKHR swapchain{};

	vk::RenderPass renderPass{};
	vk::Pipeline pipeline{};

	uint32_t imageIndex{};

	vk::Fence acquireFence{};
	vk::Semaphore acquireSemaphore{};

	vk::Fence submitFence{};
	vk::Semaphore submitSemaphore{};

	vk::Framebuffer framebuffer{};
	vk::CommandPool commandPool{};
	vk::CommandBuffer commandBuffer{};
};

struct PerInstance {
	vk::Device device{};
	Queues queues{};
	Families families{};
	vk::SwapchainKHR swapchain{};
};

struct PerSwapchain {
	vk::UniqueRenderPass renderPass{};
	vk::UniquePipeline pipeline{};

	vk::UniqueFence dummyFence{};
};

struct PerAquire {
	vk::Fence wait{};
	vk::UniqueFence fence{};
	vk::UniqueSemaphore semaphore{};
};

struct PerSubmit {
	vk::Fence wait{};
	vk::UniqueSemaphore semaphore{};
};

struct PerImage {
	vk::UniqueFence fence{};

	vk::UniqueFramebuffer framebuffer{};
	vk::UniqueCommandPool commandPool{};
	vk::UniqueCommandBuffer commandBuffer{};
};

#if defined(DEBUG)
void * getKey(vk::UniqueFence &fence) {
	return reinterpret_cast<void *>(static_cast<VkFence>(fence.get()));
}

void * getKey(vk::Fence &fence) {
	return reinterpret_cast<void *>(static_cast<VkFence>(fence));
}

void * getKey(vk::UniqueSemaphore &semaphore) {
	return reinterpret_cast<void *>(static_cast<VkSemaphore>(semaphore.get()));
}

void * getKey(vk::Semaphore &semaphore) {
	return reinterpret_cast<void *>(static_cast<VkSemaphore>(semaphore));
}
#endif

class RingPool {
public:
	rn::Window &window;
	rn::vlk::Context &context;

	uint32_t imageCount;

	PerInstance perInstance{};
	PerSwapchain perSwapchain{};
	std::vector<PerAquire> perAquireList{};
	std::vector<PerSubmit> perSubmitList{};
	std::vector<PerImage> perImageList{};

	RingPool(rn::Window &window, rn::vlk::Context &context) :
		window(window), context(context)
	{}

	~RingPool() {
		deinit();
	}

	void init() {
		imageCount = context.handles.surfaceImages.size();
		perInstance = initPerInstance();
		perSwapchain = initPerSwapchain();
		perAquireList = initPerAquireList(imageCount);
		perSubmitList = initPerSubmitList(imageCount);
		perImageList = initPerImageList(imageCount);
	}

	void deinit() {
		if (perInstance.device == vk::Device{}) {
			return;
		}

		perInstance.device.waitIdle();

		perInstance = {};
		perSwapchain = {};
		perAquireList.clear();
		perImageList.clear();
	}

	Handles next() {
		ngn::prof::Scope profScope{"app::main::Resources::next()"};

		if (perInstance.device == vk::Device{}) {
			throw std::runtime_error("app::main::Resource not initialized");
		}

		auto &perAquire = nextAvailablePerAquire();
		{
			ngn::prof::Scope profScope{"reset perAquire fence"};
			perInstance.device.resetFences({ perAquire.fence.get() });
		}

		uint32_t idx;
		{
			ngn::prof::Scope profScope{"acquire image"};

			const auto acquireResult = perInstance.device.acquireNextImageKHR(perInstance.swapchain, std::numeric_limits<uint64_t>::max(), perAquire.semaphore.get(), perAquire.fence.get());
			if (acquireResult.result != vk::Result::eSuccess) {
				ngn::log::warn("Acquire result: {}", vk::to_string(acquireResult.result));
			}

			idx = acquireResult.value;
		}

		auto &perSubmit = nextAvailablePerSubmit(idx, perAquire.fence.get());

		auto &perImage = nextAvailablePerImage(idx);
		{
			ngn::prof::Scope profScope{"reset perImage fence"};
			perInstance.device.resetFences({ perImage.fence.get() });
		}

		perAquire.wait = perImage.fence.get();

		// assign
		Handles handles{};
		handles.device = perInstance.device;
		handles.queues = perInstance.queues;
		handles.families = perInstance.families;
		handles.swapchain = perInstance.swapchain;

		handles.renderPass = perSwapchain.renderPass.get();
		handles.pipeline = perSwapchain.pipeline.get();

		handles.imageIndex = idx;

		handles.acquireFence = perAquire.fence.get();
		handles.acquireSemaphore = perAquire.semaphore.get();

		handles.submitFence = perImage.fence.get();
		handles.submitSemaphore = perSubmit.semaphore.get();

		handles.framebuffer = perImage.framebuffer.get();
		handles.commandPool = perImage.commandPool.get();
		handles.commandBuffer = perImage.commandBuffer.get();

		return handles;
	}

	uint32_t needsWait(const std::vector<vk::Fence> & fences) {
		for (uint32_t i = 0; i < fences.size(); i++) {
			if (perInstance.device.getFenceStatus(fences[i]) != vk::Result::eSuccess) {
				return i;
			}
		}

		return std::numeric_limits<uint32_t>::max();
	}

	PerAquire & nextAvailablePerAquire() {
		ngn::prof::Scope profScope{"::nextAvailablePerAquire()"};
		assert(perAquireList.size() > 0);

		std::vector<vk::Fence> fences{};
		fences.resize(perAquireList.size());

		for (uint32_t i = 0; i < perAquireList.size(); i++) {
			fences[i] = perAquireList[i].wait;
		}

		perInstance.device.waitForFences(fences, false, std::numeric_limits<uint64_t>::max());

		const auto found = std::find_if(std::begin(perAquireList), std::end(perAquireList), [&] (const auto &perAquire) {
			const auto result = perInstance.device.getFenceStatus(perAquire.wait);

			return result == vk::Result::eSuccess;
		});

		if (found != std::end(perAquireList)) {
			#if defined(DEBUG)
			{
				uint32_t idx = needsWait({ found->fence.get() });

				if (idx != std::numeric_limits<uint32_t>::max()) {
					ngn::log::debug("nextAvailablePerAquire/found needsWait = {}", idx);
				}
			}
			#endif

			return *found;
		} else {
			// should never happen, but handle it anyway -- wait for the first entry
			ngn::log::warn("No PerAquire available even after the wait for all fences");

			perInstance.device.waitIdle();
			perInstance.device.waitForFences({ perAquireList[0].wait }, false, std::numeric_limits<uint64_t>::max());

			#if defined(DEBUG)
			{
				uint32_t idx = needsWait({ perAquireList[0].fence.get() });

				if (idx != std::numeric_limits<uint32_t>::max()) {
					ngn::log::debug("nextAvailablePerAquire/backup needsWait = {}", idx);
				}
			}
			#endif

			return perAquireList[0];
		}
	}

	PerSubmit & nextAvailablePerSubmit(uint32_t idx, const vk::Fence &acquireFence) {
		ngn::prof::Scope profScope{"::nextAvailablePerSubmit()"};

		auto &perSubmit = perSubmitList[idx];

		#if defined(DEBUG)
		{
			uint32_t idx = needsWait({ perSubmit.wait });

			if (idx != std::numeric_limits<uint32_t>::max()) {
				ngn::log::debug("nextAvailablePerSubmit needsWait = {}", idx);
			}
		}
		#endif

		perInstance.device.waitForFences({ perSubmit.wait }, false, std::numeric_limits<uint64_t>::max());

		perSubmit.wait = acquireFence;

		return perSubmit;
	}

	PerImage & nextAvailablePerImage(uint32_t idx) {
		ngn::prof::Scope profScope{"::nextAvailablePerImage()"};

		auto &perImage = perImageList[idx];

		#if defined(DEBUG)
		{
			uint32_t idx = needsWait({ perImage.fence.get() });

			if (idx != std::numeric_limits<uint32_t>::max()) {
				ngn::log::debug("nextAvailablePerImage needsWait = {}", idx);
			}
		}
		#endif

		perInstance.device.waitForFences({ perImage.fence.get() }, false, std::numeric_limits<uint64_t>::max());

		return perImage;
	}

	PerInstance initPerInstance() {
		PerInstance result{};

		result.device = context.owners.device.get();

		result.queues = {
			context.handles.queues.presentation.handle,
			context.handles.queues.graphic.handle,
			context.handles.queues.compute.handle,
			context.handles.queues.transfer.handle
		};

		result.families = {
			context.handles.queues.presentation.family,
			context.handles.queues.graphic.family,
			context.handles.queues.compute.family,
			context.handles.queues.transfer.family
		};

		result.swapchain = context.owners.swapchain.get();

		return result;
	}

	PerSwapchain initPerSwapchain() {
		PerSwapchain result{};

		result.renderPass = createRenderPass();
		result.pipeline = createGraphicsPipeline(result.renderPass.get());
		result.dummyFence = createFence();

		return result;
	}

	std::vector<PerAquire> initPerAquireList(uint32_t imageCount) {
		std::vector<PerAquire> result(imageCount * 2);

		for (uint32_t i = 0; i < result.size(); i++) {
			result[i].wait = perSwapchain.dummyFence.get();
			result[i].fence = createFence();
			result[i].semaphore = createSemaphore();
		}

		return result;
	}

	std::vector<PerSubmit> initPerSubmitList(uint32_t imageCount) {
		std::vector<PerSubmit> result(imageCount);

		for (uint32_t i = 0; i < result.size(); i++) {
			result[i].wait = perSwapchain.dummyFence.get();
			result[i].semaphore = createSemaphore();
		}

		return result;
	}

	std::vector<PerImage> initPerImageList(uint32_t imageCount) {
		std::vector<PerImage> result(imageCount);

		for (uint32_t i = 0; i < result.size(); i++) {
			result[i].fence = createFence();

			result[i].framebuffer = createFramebuffer(i);
			result[i].commandPool = createCommandPool();
			result[i].commandBuffer = createCommandBuffer(result[i].commandPool.get());
		}

		return result;
	}

	// Per Swapchain
	vk::UniqueRenderPass createRenderPass() {
		vk::AttachmentDescription colorAttachment{};
		colorAttachment.format = context.handles.surfaceFormat.format;
		colorAttachment.samples = vk::SampleCountFlagBits::e1;
		colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
		colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
		colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

		vk::AttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

		vk::SubpassDescription subpass{};
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pResolveAttachments = nullptr;
		subpass.pDepthStencilAttachment = nullptr;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		vk::SubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.srcAccessMask = vk::AccessFlags{};
		dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		return perInstance.device.createRenderPassUnique(renderPassInfo);
	}

	vk::UniqueShaderModule createShaderModule(const std::vector<uint32_t> &code) {
		assert(code.size() > 0);

		vk::ShaderModuleCreateInfo shaderModuleCreateInfo{};
		shaderModuleCreateInfo.codeSize = code.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = code.data();

		return perInstance.device.createShaderModuleUnique(shaderModuleCreateInfo);
	}

	vk::UniquePipeline createGraphicsPipeline(const vk::RenderPass &renderPass) {
		// shader modules
		std::vector<uint32_t> vertCode = ngn::fs::contents<std::vector<uint32_t>>("shaders/main.vert.spv-3");
		std::vector<uint32_t> fragCode = ngn::fs::contents<std::vector<uint32_t>>("shaders/main.frag.spv-3");

		vk::UniqueShaderModule vertShaderModule = createShaderModule(vertCode);
		vk::UniqueShaderModule fragShaderModule = createShaderModule(fragCode);

		enum ShaderStage {
			Vert = 0,
			Frag = 1,
			RangeSize = (Frag - Vert + 1)
		};
		vk::PipelineShaderStageCreateInfo shaderStages[ShaderStage::RangeSize]{};

		shaderStages[ShaderStage::Vert].stage = vk::ShaderStageFlagBits::eVertex;
		shaderStages[ShaderStage::Vert].module = vertShaderModule.get();
		shaderStages[ShaderStage::Vert].pName = "main";

		shaderStages[ShaderStage::Frag].stage = vk::ShaderStageFlagBits::eFragment;
		shaderStages[ShaderStage::Frag].module = fragShaderModule.get();
		shaderStages[ShaderStage::Frag].pName = "main";

		// vertex input state
		vk::PipelineVertexInputStateCreateInfo vertexInputState{};
		vertexInputState.vertexBindingDescriptionCount = 0;
		vertexInputState.pVertexBindingDescriptions = nullptr;
		vertexInputState.vertexAttributeDescriptionCount = 0;
		vertexInputState.pVertexAttributeDescriptions = nullptr;

		// input assembly state
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{};
		inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;

		// viewport
		vk::Viewport viewport{};
		viewport.width = static_cast<float>(context.handles.surfaceExtent.width);
		viewport.height = static_cast<float>(context.handles.surfaceExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// scissor
		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D{0, 0};
		scissor.extent = context.handles.surfaceExtent;

		// viewport state
		vk::PipelineViewportStateCreateInfo viewportState{};
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// rasterization state
		vk::PipelineRasterizationStateCreateInfo rasterizerState{};
		rasterizerState.polygonMode = vk::PolygonMode::eFill;
		rasterizerState.cullMode = vk::CullModeFlagBits::eBack;
		rasterizerState.frontFace = vk::FrontFace::eClockwise;
		rasterizerState.lineWidth = 1.0f;

		// multisample state
		vk::PipelineMultisampleStateCreateInfo multisampleState{};
		multisampleState.minSampleShading = 1.0f;

		// color blend attachment state
		vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{};
		colorBlendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		colorBlendAttachmentState.blendEnable = false;
		colorBlendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
		colorBlendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		colorBlendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;
		colorBlendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eOne;
		colorBlendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eZero;
		colorBlendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;

		// color blend state
		vk::PipelineColorBlendStateCreateInfo colorBlendingState{};
		colorBlendingState.logicOpEnable = false;
		colorBlendingState.logicOp = vk::LogicOp::eCopy;
		colorBlendingState.attachmentCount = 1;
		colorBlendingState.pAttachments = &colorBlendAttachmentState;
		colorBlendingState.blendConstants[0] = 0.0f;
		colorBlendingState.blendConstants[1] = 0.0f;
		colorBlendingState.blendConstants[2] = 0.0f;
		colorBlendingState.blendConstants[3] = 0.0f;

		// list of dynamic states
		std::vector<vk::DynamicState> dynamicStates{};

		// dynamic state
		vk::PipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.dynamicStateCount = dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		// pipeline layout
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = 0;

		vk::UniquePipelineLayout pipelineLayout = perInstance.device.createPipelineLayoutUnique(pipelineLayoutInfo);

		// pipeline
		vk::GraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputState;
		pipelineInfo.pInputAssemblyState = &inputAssemblyState;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizerState;
		pipelineInfo.pMultisampleState = &multisampleState;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlendingState;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = pipelineLayout.get();
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = vk::Pipeline{};
		pipelineInfo.basePipelineIndex = -1;

		return perInstance.device.createGraphicsPipelineUnique(vk::PipelineCache{}, pipelineInfo);
	}

	// Per Image
	vk::UniqueFramebuffer createFramebuffer(uint32_t idx) {
		vk::ImageView attachments[] {
			context.owners.surfaceImageViews[idx].get()
		};

		vk::FramebufferCreateInfo framebufferInfo{};
		framebufferInfo.renderPass = perSwapchain.renderPass.get();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = context.handles.surfaceExtent.width;
		framebufferInfo.height = context.handles.surfaceExtent.height;
		framebufferInfo.layers = 1;

		return perInstance.device.createFramebufferUnique(framebufferInfo);
	}

	vk::UniqueCommandPool createCommandPool() {
		vk::CommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.queueFamilyIndex = perInstance.families.graphic;
		commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		return perInstance.device.createCommandPoolUnique(commandPoolInfo);
	}

	vk::UniqueCommandBuffer createCommandBuffer(const vk::CommandPool &commandPool) {
		vk::CommandBufferAllocateInfo commandBufferAllocate{};
		commandBufferAllocate.commandPool = commandPool;
		commandBufferAllocate.level = vk::CommandBufferLevel::ePrimary;
		commandBufferAllocate.commandBufferCount = 1;

		std::vector<vk::UniqueCommandBuffer> buffers = perInstance.device.allocateCommandBuffersUnique(commandBufferAllocate);

		return std::move(buffers[0]);
	}

	vk::UniqueFence createFence() {
		return perInstance.device.createFenceUnique(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled});
	}

	vk::UniqueSemaphore createSemaphore() {
		return perInstance.device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});
	}
};

}
