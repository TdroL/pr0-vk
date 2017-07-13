#include <vector>
#include <limits>
#include <iterator>

#include <vulkan/vulkan.hpp>

#include <ngn/fs.hpp>
#include <ngn/log.hpp>

namespace app {

struct RootResources {
	rn::Window &window;
	rn::vlk::Context &context;

	RootResources(rn::Window &window, rn::vlk::Context &context) :
		window(window), context(context)
	{}
};

struct PersistentResources {
	vk::UniqueRenderPass renderPass{};
	vk::UniquePipeline pipeline{};
	// std::vector<vk::UniqueFramebuffer> framebuffers{};

	RootResources &root;

	PersistentResources(RootResources &root) :
		root(root)
	{}

	void init() {
		renderPass = createRenderPass();
		pipeline = createGraphicsPipeline(renderPass.get());
		// framebuffers = createFramebuffers(renderPass.get());
	}

	vk::UniqueRenderPass createRenderPass() {
		vk::AttachmentDescription colorAttachment{};
		colorAttachment.format = root.context.handles.surfaceFormat.format;
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

		return root.context.device.createRenderPassUnique(renderPassInfo);
	}

	vk::UniqueShaderModule createShaderModule(const std::vector<uint32_t> &code) {
		assert(code.size() > 0);

		vk::ShaderModuleCreateInfo shaderModuleCreateInfo{};
		shaderModuleCreateInfo.codeSize = code.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = code.data();

		return root.context.device.createShaderModuleUnique(shaderModuleCreateInfo);
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
		viewport.width = static_cast<float>(root.context.handles.surfaceExtent.width);
		viewport.height = static_cast<float>(root.context.handles.surfaceExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// scissor
		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D{0, 0};
		scissor.extent = root.context.handles.surfaceExtent;

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

		vk::UniquePipelineLayout pipelineLayout = root.context.device.createPipelineLayoutUnique(pipelineLayoutInfo);

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

		return root.context.device.createGraphicsPipelineUnique(vk::PipelineCache{}, pipelineInfo);
	}

	// std::vector<vk::UniqueFramebuffer> createFramebuffers(const vk::RenderPass &renderPass) {
	// 	std::vector<vk::UniqueFramebuffer> framebuffers{};
	// 	framebuffers.resize(root.context.handles.surfaceImages.size());

	// 	for (size_t i = 0; i < framebuffers.size(); ++i) {
	// 		vk::ImageView attachments[] {
	// 			root.context.owners.surfaceImageViews[i].get()
	// 		};

	// 		vk::FramebufferCreateInfo framebufferInfo{};
	// 		framebufferInfo.renderPass = renderPass;
	// 		framebufferInfo.attachmentCount = 1;
	// 		framebufferInfo.pAttachments = attachments;
	// 		framebufferInfo.width = root.context.handles.surfaceExtent.width;
	// 		framebufferInfo.height = root.context.handles.surfaceExtent.height;
	// 		framebufferInfo.layers = 1;

	// 		framebuffers[i] = root.context.device.createFramebufferUnique(framebufferInfo);
	// 	}

	// 	return framebuffers;
	// }
};

struct TransientResources {
	vk::UniqueCommandPool commandPool{};
	std::vector<vk::UniqueCommandBuffer> commandBuffers{};

	vk::UniqueSemaphore imageAvailableSemaphore{};
	vk::UniqueSemaphore renderFinishedSemaphore{};

	RootResources &root;
	PersistentResources &runtime;

	explicit TransientResources(RootResources &root, PersistentResources &runtime) :
		root(root), runtime(runtime)
	{}

	// TransientResources() = delete;

	// TransientResources(const TransientResources &lhs) = delete;
	// TransientResources & operator=(const TransientResources &lhs) = delete;

	// TransientResources(TransientResources &&rhs) = default;
	// TransientResources & operator=(TransientResources &&rhs) = default;

	void init() {
		commandPool = createCommandPool();
		commandBuffers = createCommandBuffers(runtime.renderPass.get(), runtime.pipeline.get(), runtime.framebuffers, commandPool.get());

		imageAvailableSemaphore = createSemaphore();
		renderFinishedSemaphore = createSemaphore();
	}

	bool isAvailable() {
		return false;
	}

	vk::UniqueCommandPool createCommandPool() {
		vk::CommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.queueFamilyIndex = root.context.handles.queues.graphic.family;
		commandPoolInfo.flags = vk::CommandPoolCreateFlags{};

		return root.context.device.createCommandPoolUnique(commandPoolInfo);
	}

	std::vector<vk::UniqueCommandBuffer> createCommandBuffers(const vk::RenderPass &renderPass, const vk::Pipeline &pipeline, const std::vector<vk::UniqueFramebuffer> &framebuffers, const vk::CommandPool &commandPool) {
		vk::CommandBufferAllocateInfo commandBufferAllocate{};
		commandBufferAllocate.commandPool = commandPool;
		commandBufferAllocate.level = vk::CommandBufferLevel::ePrimary;
		commandBufferAllocate.commandBufferCount = static_cast<uint32_t>(framebuffers.size());

		std::vector<vk::UniqueCommandBuffer> commandBuffers = root.context.device.allocateCommandBuffersUnique(commandBufferAllocate);

		vk::ClearValue clearColor{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			vk::CommandBufferBeginInfo commandBufferBegin{};
			commandBufferBegin.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
			commandBufferBegin.pInheritanceInfo = nullptr;

			vk::RenderPassBeginInfo renderPassBegin{};
			renderPassBegin.renderPass = renderPass;
			renderPassBegin.framebuffer = framebuffers[i].get();
			renderPassBegin.renderArea.offset = vk::Offset2D{0, 0};
			renderPassBegin.renderArea.extent = root.context.handles.surfaceExtent;
			renderPassBegin.clearValueCount = 1;
			renderPassBegin.pClearValues = &clearColor;

			vk::CommandBuffer commandBuffer = commandBuffers[i].get();

			commandBuffer.begin(commandBufferBegin);
				commandBuffer.beginRenderPass(renderPassBegin, vk::SubpassContents::eInline);

					commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
					commandBuffer.draw(3, 1, 0, 0);

				commandBuffer.endRenderPass();
			commandBuffer.end();
		}

		return commandBuffers;
	}

	vk::UniqueSemaphore createSemaphore() {
		return root.context.device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});
	}
};

struct FrameResources {
	vk::Device device{};

	struct Queues {
		vk::Queue presentation{};
		vk::Queue graphic{};
		vk::Queue compute{};
		vk::Queue transfer{};
	} queues{};

	struct Families {
		uint32_t presentation{};
		uint32_t graphic{};
		uint32_t compute{};
		uint32_t transfer{};
	} families{};

	vk::SwapchainKHR swapchain{};

	vk::RenderPass renderPass{};
	vk::Pipeline pipeline{};
	vk::Framebuffer framebuffer{};

	vk::CommandPool commandPool{};
	vk::CommandBuffer commandBuffer{};

	vk::Semaphore imageAvailableSemaphore{};
	vk::Semaphore renderFinishedSemaphore{};
};

class ResourcePool {
public:
	RootResources root;

	PersistentResources runtime{root};

	std::vector<FrameResources> frameStack{};
	uint32_t current = 0;

	ResourcePool(rn::Window &window, rn::vlk::Context &context) :
		root(window, context)
	{}

	FrameResources & frame() {
		current++;

		for (uint32_t i = 0; i < frameStack.size(); i++) {
			uint32_t idx = (current + i) % frameStack.size();

			if (frameStack[idx].isAvailable()) {
				current = idx;
				return frameStack[current];
			}
		}

		current = frameStack.size();

		frameStack.emplace_back(root, runtime);
		frameStack[current].init();

		ngn::log::debug("Increase pool size to {} [{}]", frameStack.size());

		return frameStack[current];
	}
};

class MainState {
public:
	ResourcePool resourcePool;

	rn::Window &window;
	rn::vlk::Context &context;

	MainState(rn::Window &window, rn::vlk::Context &context) :
		resourcePool(window, context), window(window), context(context)
	{}

	void init() {

	}

	void refresh() {
		//
	}

	void render() {
		FrameResources &resources = resourcePool.frame();

		// context.device.waitIdle();

		auto result = context.device.acquireNextImageKHR(context.swapchain, std::numeric_limits<uint64_t>::max(), frameResources.imageAvailableSemaphore.get(), vk::Fence{});

		uint32_t imageIndex = result.value;
		vk::CommandBuffer commandBuffer = frameResources.commandBuffers[imageIndex].get();

		vk::Semaphore waitSemaphores[] { frameResources.imageAvailableSemaphore.get() };
		vk::PipelineStageFlags waitStages[] { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		vk::Semaphore signalSemaphores[] { frameResources.renderFinishedSemaphore.get() };

		static_assert(std::size(waitSemaphores) == std::size(waitStages));

		vk::SubmitInfo submitInfo{};
		submitInfo.waitSemaphoreCount = std::size(waitSemaphores);
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.signalSemaphoreCount = std::size(signalSemaphores);
		submitInfo.pSignalSemaphores = signalSemaphores;

		context.queues.graphic.submit({submitInfo}, vk::Fence{});

		vk::SwapchainKHR swapchain = context.owners.swapchain.get();

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = std::size(signalSemaphores);
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		context.queues.presentation.presentKHR(presentInfo);
	}
};

} // app