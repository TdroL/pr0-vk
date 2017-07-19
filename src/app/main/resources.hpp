#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include <ngn/fs.hpp>
#include <ngn/log.hpp>

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

	uint32_t acquireIndex{};
	vk::Fence acquireFence{};
	vk::Semaphore acquireSemaphore{};

	vk::Framebuffer framebuffer{};
	vk::CommandPool commandPool{};
	vk::CommandBuffer commandBuffer{};
	vk::Fence submitFence{};
	vk::Semaphore submitSemaphore{};
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
};

struct PerAquire {
	vk::UniqueFence fence{};
	vk::UniqueSemaphore semaphore{};
};

struct PerImage {
	vk::UniqueFramebuffer framebuffer{};
	vk::UniqueCommandPool commandPool{};
	vk::UniqueCommandBuffer commandBuffer{};
	vk::UniqueFence fence{};
	vk::UniqueSemaphore semaphore{};
};

class RingPool {
public:
	rn::Window &window;
	rn::vlk::Context &context;

	uint32_t imageCount;

	PerInstance perInstance{};

	PerSwapchain perSwapchain{};
	std::vector<PerAquire> perAquireList{};
	std::vector<std::vector<PerImage>> perImageLists{};

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

		perAquireList.resize(imageCount);
		for (uint32_t i = 0; i < imageCount; i++) {
			perAquireList[i] = initPerAquire();
		}

		perImageLists.resize(imageCount);
		for (uint32_t i = 0; i < imageCount; i++) {
			perImageLists[i].push_back(initPerImage(i));
		}
	}

	void deinit() {
		if (perInstance.device == vk::Device{}) {
			return;
		}

		perInstance.device.waitIdle();

		// wait for all fences not associated with queues
		std::vector<vk::Fence> fences{};
		for (uint32_t i = 0; i < imageCount; i++) {
			fences.push_back(perAquireList[i].fence.get());
		}

		perInstance.device.waitForFences(fences, true, std::numeric_limits<uint64_t>::max());

		perInstance = {};
		perSwapchain = {};
		perAquireList.clear();
		perImageLists.clear();
	}

	Handles next() {
		const PerAquire &perAquire = nextAvailablePerAquire();

		perInstance.device.resetFences({
			perAquire.fence.get()
		});

		const auto result = perInstance.device.acquireNextImageKHR(perInstance.swapchain, std::numeric_limits<uint64_t>::max(), perAquire.semaphore.get(), perAquire.fence.get());

		if (result.result != vk::Result::eSuccess) {
			ngn::log::warn("acquireNextImageKHR: {}", vk::to_string(result.result));
		}

		uint32_t idx = result.value;

		const PerImage &perImage = nextAvailablePerImage(idx);

		perInstance.device.resetFences({
			perImage.fence.get()
		});

		Handles handles{};
		handles.device = perInstance.device;
		handles.queues = perInstance.queues;
		handles.families = perInstance.families;
		handles.swapchain = perInstance.swapchain;

		handles.renderPass = perSwapchain.renderPass.get();
		handles.pipeline = perSwapchain.pipeline.get();

		handles.acquireIndex = idx;
		handles.acquireFence = perAquire.fence.get();
		handles.acquireSemaphore = perAquire.semaphore.get();

		handles.framebuffer = perImage.framebuffer.get();
		handles.commandPool = perImage.commandPool.get();
		handles.commandBuffer = perImage.commandBuffer.get();
		handles.submitFence = perImage.fence.get();
		handles.submitSemaphore = perImage.semaphore.get();

		return handles;
	}

	PerAquire & nextAvailablePerAquire() {
		for (uint32_t i = 0; i < perAquireList.size(); i++) {
			const vk::Fence fence = perAquireList[i].fence.get();

			const auto result = perInstance.device.getFenceStatus(fence);
			if (result == vk::Result::eSuccess) {
				return perAquireList[i];
			}
		}

		perAquireList.push_back(initPerAquire());
		uint32_t last = perAquireList.size() - 1;

		return perAquireList[last];
	}

	PerImage & nextAvailablePerImage(uint32_t idx) {
		assert(idx < perImageLists.size());

		auto &perImageList = perImageLists[idx];

		for (uint32_t i = 0; i < perImageList.size(); i++) {
			const vk::Fence fence = perImageList[i].fence.get();

			const auto result = perInstance.device.getFenceStatus(fence);
			if (result == vk::Result::eSuccess) {
				return perImageList[i];
			}
		}

		perImageList.push_back(initPerImage(idx));
		uint32_t last = perImageList.size() - 1;

		return perImageList[last];
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

		return result;
	}

	PerAquire initPerAquire() {
		PerAquire result{};

		result.fence = createFence();
		result.semaphore = createSemaphore();

		return result;
	}

	PerImage initPerImage(uint32_t idx) {
		PerImage result{};

		result.framebuffer = createFramebuffer(idx);
		result.commandPool = createCommandPool();
		result.commandBuffer = createCommandBuffer(result.commandPool.get());
		result.fence = createFence();
		result.semaphore = createSemaphore();

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
