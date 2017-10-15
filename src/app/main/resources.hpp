#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include <ngn/fs.hpp>
#include <ngn/log.hpp>
#include <ngn/prof.hpp>

#include <util/map.hpp>

#include <rn/window.hpp>
#include <rn/vlk/context.hpp>

namespace app::main {

struct Handles {
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

struct PerSwapchain {
	vk::UniqueRenderPass renderPass{};
	vk::UniquePipeline pipeline{};

	vk::UniqueBuffer modelBuffer{};

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

class RingPool {
public:
	rn::Window &window;
	rn::vlk::Context &context;

	uint32_t imageCount = 0;

	// PerInstance perInstance{};
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
		imageCount = context.surface.images.size();
		// perInstance = initPerInstance();
		perSwapchain = initPerSwapchain();
		perAquireList = initPerAquireList(imageCount);
		perSubmitList = initPerSubmitList(imageCount);
		perImageList = initPerImageList(imageCount);
	}

	void deinit() {
		if (context.device == vk::Device{} || imageCount == 0) {
			return;
		}

		// gather swapchain fences
		std::vector<vk::Fence> imageFences = util::map<vk::Fence>(perImageList, [] (auto &perImage) {
			return perImage.fence.get();
		});

		std::vector<vk::Fence> acquireFences = util::map<vk::Fence>(perAquireList, [] (auto &perAquire) {
			return perAquire.fence.get();
		});

		// merge both lists
		std::vector<vk::Fence> fences{imageFences.size() + acquireFences.size()};
		fences.insert(std::end(fences), std::begin(imageFences), std::end(imageFences));
		fences.insert(std::end(fences), std::begin(acquireFences), std::end(acquireFences));

		// remove not initialized fences
		std::remove_if(std::begin(fences), std::end(fences), [] (auto &fence) { return ! fence; });

		// wait for both device and presentation to finish all work
		context.device.waitIdle();
		context.device.waitForFences(fences, true, std::numeric_limits<uint64_t>::max());

		// perInstance = {};
		perSwapchain = {};
		perAquireList.clear();
		perImageList.clear();

		imageCount = 0;
	}

	Handles next() {
		ngn::prof::Scope("app::main::Resources::next()");

		if (context.device == vk::Device{}) {
			throw std::runtime_error("app::main::Resource not initialized");
		}

		auto &perAquire = nextAvailablePerAquire();
		{
			ngn::prof::Scope("reset perAquire fence");
			context.device.resetFences({ perAquire.fence.get() });
		}

		uint32_t idx;
		{
			ngn::prof::Scope("acquire image");

			const auto acquireResult = context.device.acquireNextImageKHR(context.swapchain, std::numeric_limits<uint64_t>::max(), perAquire.semaphore.get(), perAquire.fence.get());
			if (acquireResult.result != vk::Result::eSuccess) {
				ngn::log::warn("Acquire result: {}", vk::to_string(acquireResult.result));
			}

			idx = acquireResult.value;
		}

		auto &perSubmit = nextAvailablePerSubmit(idx, perAquire.fence.get());

		auto &perImage = nextAvailablePerImage(idx);
		{
			ngn::prof::Scope("reset perImage fence");
			context.device.resetFences({ perImage.fence.get() });
		}

		perAquire.wait = perImage.fence.get();

		// assign
		Handles handles{};
		// handles.device = context.device;
		// handles.queues = context.queues;
		// handles.families = context.families;
		// handles.swapchain = context.swapchain;

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
			if (context.device.getFenceStatus(fences[i]) != vk::Result::eSuccess) {
				return i;
			}
		}

		return std::numeric_limits<uint32_t>::max();
	}

	PerAquire & nextAvailablePerAquire() {
		ngn::prof::Scope("::nextAvailablePerAquire()");
		assert(perAquireList.size() > 0);

		std::vector<vk::Fence> fences{};
		fences.resize(perAquireList.size());

		for (uint32_t i = 0; i < perAquireList.size(); i++) {
			fences[i] = perAquireList[i].wait;
		}

		context.device.waitForFences(fences, false, std::numeric_limits<uint64_t>::max());

		const auto found = std::find_if(std::begin(perAquireList), std::end(perAquireList), [&] (const auto &perAquire) {
			const auto result = context.device.getFenceStatus(perAquire.wait);

			return result == vk::Result::eSuccess;
		});

		if (found != std::end(perAquireList)) {
			#if defined(DEBUG)
			{
				uint32_t fenceIdx = needsWait({ found->fence.get() });

				if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
					ngn::log::debug("nextAvailablePerAquire/found needsWait = {}", found - std::begin(perAquireList));
				}
			}
			#endif

			return *found;
		} else {
			// should never happen, but handle it anyway -- wait for the first entry
			ngn::log::warn("No PerAquire available even after the wait for all fences");

			context.device.waitIdle();
			context.device.waitForFences({ perAquireList[0].wait }, false, std::numeric_limits<uint64_t>::max());

			#if defined(DEBUG)
			{
				uint32_t fenceIdx = needsWait({ perAquireList[0].fence.get() });

				if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
					ngn::log::debug("nextAvailablePerAquire/fallback needsWait = {}", 0);
				}
			}
			#endif

			return perAquireList[0];
		}
	}

	PerSubmit & nextAvailablePerSubmit(uint32_t idx, const vk::Fence &acquireFence) {
		ngn::prof::Scope("::nextAvailablePerSubmit()");

		auto &perSubmit = perSubmitList[idx];

		#if defined(DEBUG)
		{
			uint32_t fenceIdx = needsWait({ perSubmit.wait });

			if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
				ngn::log::debug("nextAvailablePerSubmit needsWait = {}", idx);
			}
		}
		#endif

		context.device.waitForFences({ perSubmit.wait }, false, std::numeric_limits<uint64_t>::max());

		perSubmit.wait = acquireFence;

		return perSubmit;
	}

	PerImage & nextAvailablePerImage(uint32_t idx) {
		ngn::prof::Scope("::nextAvailablePerImage()");

		auto &perImage = perImageList[idx];

		#if defined(DEBUG)
		{
			uint32_t fenceIdx = needsWait({ perImage.fence.get() });

			if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
				ngn::log::debug("nextAvailablePerImage needsWait = {}", idx);
			}
		}
		#endif

		context.device.waitForFences({ perImage.fence.get() }, false, std::numeric_limits<uint64_t>::max());

		return perImage;
	}

	// PerInstance initPerInstance() {
	// 	PerInstance result{};

	// 	result.device = context.owners.device.get();

	// 	result.queues = {
	// 		context.handles.queues.presentation.handle,
	// 		context.handles.queues.graphic.handle,
	// 		context.handles.queues.compute.handle,
	// 		context.handles.queues.transfer.handle
	// 	};

	// 	result.families = {
	// 		context.handles.queues.presentation.family,
	// 		context.handles.queues.graphic.family,
	// 		context.handles.queues.compute.family,
	// 		context.handles.queues.transfer.family
	// 	};

	// 	result.swapchain = context.owners.swapchain.get();

	// 	return result;
	// }

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
		std::vector<vk::AttachmentDescription> attachments{
			{
				/*.flags=*/ vk::AttachmentDescriptionFlags{},
				/*.format=*/ context.surface.format.format,
				/*.samples=*/ vk::SampleCountFlagBits::e1,
				/*.loadOp=*/ vk::AttachmentLoadOp::eClear,
				/*.storeOp=*/ vk::AttachmentStoreOp::eStore,
				/*.stencilLoadOp=*/ vk::AttachmentLoadOp::eDontCare,
				/*.stencilStoreOp=*/ vk::AttachmentStoreOp::eDontCare,
				/*.initialLayout=*/ vk::ImageLayout::eUndefined,
				/*.finalLayout=*/ vk::ImageLayout::ePresentSrcKHR
			}
		};

		std::vector<vk::AttachmentReference> colorAttachmentRefs{
			{
				/*.attachment=*/ 0,
				/*.layout=*/ vk::ImageLayout::eColorAttachmentOptimal
			}
		};

		std::vector<vk::SubpassDescription> subpasses{
			{
				/*.flags=*/ vk::SubpassDescriptionFlags{},
				/*.pipelineBindPoint=*/ vk::PipelineBindPoint::eGraphics,
				/*.inputAttachmentCount=*/ 0,
				/*.pInputAttachments=*/ nullptr,
				/*.colorAttachmentCount=*/ static_cast<uint32_t>(colorAttachmentRefs.size()),
				/*.pColorAttachments=*/ colorAttachmentRefs.data(),
				/*.pResolveAttachments=*/ nullptr,
				/*.pDepthStencilAttachment=*/ nullptr,
				/*.preserveAttachmentCount=*/ 0,
				/*.pPreserveAttachments=*/ nullptr
			}
		};

		std::vector<vk::SubpassDependency> dependencies{
			{
				/*.srcSubpass=*/ VK_SUBPASS_EXTERNAL,
				/*.dstSubpass=*/ 0,
				/*.srcStageMask=*/ vk::PipelineStageFlagBits::eBottomOfPipe,
				/*.dstStageMask=*/ vk::PipelineStageFlagBits::eColorAttachmentOutput,
				/*.srcAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
				/*.dstAccessMask=*/ vk::AccessFlagBits::eColorAttachmentWrite,
				/*.dependencyFlags=*/ vk::DependencyFlagBits::eByRegion
			}, {
				/*.srcSubpass=*/ 0,
				/*.dstSubpass=*/ VK_SUBPASS_EXTERNAL,
				/*.srcStageMask=*/ vk::PipelineStageFlagBits::eColorAttachmentOutput,
				/*.dstStageMask=*/ vk::PipelineStageFlagBits::eBottomOfPipe,
				/*.srcAccessMask=*/ vk::AccessFlagBits::eColorAttachmentWrite,
				/*.dstAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
				/*.dependencyFlags=*/ vk::DependencyFlagBits::eByRegion
			}
		};

		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
		renderPassInfo.pSubpasses = subpasses.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		return context.device.createRenderPassUnique(renderPassInfo);
	}

	vk::UniqueShaderModule createShaderModule(const std::vector<std::byte> &code) {
		assert(code.size() > 0);
		assert(code.size() % sizeof(uint32_t) == 0);

		vk::ShaderModuleCreateInfo shaderModuleCreateInfo{};
		shaderModuleCreateInfo.codeSize = code.size();
		shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

		return context.device.createShaderModuleUnique(shaderModuleCreateInfo);
	}

	vk::UniquePipeline createGraphicsPipeline(const vk::RenderPass &renderPass) {
		// shader modules
		std::vector<std::byte> vertCode = ngn::fs::read("shaders/main.vert.spv-3", 4);
		std::vector<std::byte> fragCode = ngn::fs::read("shaders/main.frag.spv-3", 4);

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
		viewport.width = static_cast<float>(context.surface.extent.width);
		viewport.height = static_cast<float>(context.surface.extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// scissor
		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D{0, 0};
		scissor.extent = context.surface.extent;

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

		vk::UniquePipelineLayout pipelineLayout = context.device.createPipelineLayoutUnique(pipelineLayoutInfo);

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

		return context.device.createGraphicsPipelineUnique(vk::PipelineCache{}, pipelineInfo);
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
		framebufferInfo.width = context.surface.extent.width;
		framebufferInfo.height = context.surface.extent.height;
		framebufferInfo.layers = 1;

		return context.device.createFramebufferUnique(framebufferInfo);
	}

	vk::UniqueCommandPool createCommandPool() {
		vk::CommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.queueFamilyIndex = context.family.graphic;
		commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		return context.device.createCommandPoolUnique(commandPoolInfo);
	}

	vk::UniqueCommandBuffer createCommandBuffer(const vk::CommandPool &commandPool) {
		vk::CommandBufferAllocateInfo commandBufferAllocate{};
		commandBufferAllocate.commandPool = commandPool;
		commandBufferAllocate.level = vk::CommandBufferLevel::ePrimary;
		commandBufferAllocate.commandBufferCount = 1;

		std::vector<vk::UniqueCommandBuffer> buffers = context.device.allocateCommandBuffersUnique(commandBufferAllocate);

		return std::move(buffers[0]);
	}

	vk::UniqueFence createFence() {
		return context.device.createFenceUnique(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled});
	}

	vk::UniqueSemaphore createSemaphore() {
		return context.device.createSemaphoreUnique(vk::SemaphoreCreateInfo{});
	}
};

}
