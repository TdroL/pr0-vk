#pragma once

#include <algorithm>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <ngn/fs.hpp>
#include <ngn/log.hpp>
#include <ngn/prof.hpp>

#include <util/map.hpp>

#include <rn/window.hpp>
#include <rn/vlk/trace.hpp>
#include <rn/vlk/context.hpp>

namespace app::main {

struct VertexData {
	float position[4];
	float color[4];
};

struct StaticData {
	std::vector<VertexData> modelData{
		{
			{ -0.7f, -0.7f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 0.0f }
		},
		{
			{ -0.7f, 0.7f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f }
		},
		{
			{ 0.7f, -0.7f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f }
		},
		{
			{ 0.7f, 0.7f, 0.0f, 1.0f },
			{ 0.3f, 0.3f, 0.3f, 0.0f }
		}
	};
};

struct Model {
	vk::UniqueBuffer buffer{};
	rn::vlk::memory::Handle memory{};
};

struct Handles {
	vk::RenderPass renderPass{};
	vk::Pipeline pipeline{};

	vk::Buffer modelBuffer{};

	uint32_t imageIndex{};

	vk::Fence acquireFence{};
	vk::Semaphore acquireSemaphore{};

	vk::Fence submitFence{};
	vk::Semaphore submitSemaphore{};

	vk::Image surfaceImage{};

	vk::Framebuffer framebuffer{};
	vk::CommandPool commandPool{};
	vk::CommandBuffer commandBuffer{};
};

struct PerSwapchain {
	vk::UniqueRenderPass renderPass{};
	vk::UniquePipeline pipeline{};

	Model model{};

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
struct NeedsWaitStats {
	uint32_t nextAvailablePerAquireFound = 0;
	uint32_t nextAvailablePerAquireFallback = 0;
	uint32_t nextAvailablePerSubmit = 0;
	uint32_t nextAvailablePerImage = 0;
};
#endif

class RingPool {
public:
	rn::Window &window;
	rn::vlk::Context &context;

	uint32_t imageCount = 0;

	StaticData staticData{};
	// PerInstance perInstance{};
	PerSwapchain perSwapchain{};
	std::vector<PerAquire> perAquireList{};
	std::vector<PerSubmit> perSubmitList{};
	std::vector<PerImage> perImageList{};

	#if defined(DEBUG)
	NeedsWaitStats needsWaitStats{};
	#endif

	RingPool(rn::Window &window, rn::vlk::Context &context) :
		window(window), context(context)
	{}

	~RingPool() {
		deinit();

		#if defined(DEBUG)
		ngn::log::info("needsWait stats: nextAvailablePerAquireFound = {}", needsWaitStats.nextAvailablePerAquireFound);
		ngn::log::info("needsWait stats: nextAvailablePerAquireFallback = {}", needsWaitStats.nextAvailablePerAquireFallback);
		ngn::log::info("needsWait stats: nextAvailablePerSubmit = {}", needsWaitStats.nextAvailablePerSubmit);
		ngn::log::info("needsWait stats: nextAvailablePerImage = {}", needsWaitStats.nextAvailablePerImage);
		#endif
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
		std::vector<vk::Fence> imageFences = util::map(perImageList, [] (auto &perImage) {
			return perImage.fence.get();
		});

		std::vector<vk::Fence> acquireFences = util::map(perAquireList, [] (auto &perAquire) {
			return perAquire.fence.get();
		});

		// merge both lists
		std::vector<vk::Fence> fences{imageFences.size() + acquireFences.size()};
		fences.insert(std::end(fences), std::begin(imageFences), std::end(imageFences));
		fences.insert(std::end(fences), std::begin(acquireFences), std::end(acquireFences));

		// remove not initialized fences
		fences.erase(std::remove_if(std::begin(fences), std::end(fences), [] (auto &fence) { return ! fence; }), std::end(fences));

		// wait for both device and presentation to finish all work
		RN_VLK_TRACE(context.device.waitIdle());
		RN_VLK_TRACE(context.device.waitForFences(fences, true, std::numeric_limits<uint64_t>::max()));

		// perInstance = {};
		perSwapchain = {};
		perAquireList.clear();
		perImageList.clear();

		imageCount = 0;
	}

	Handles next() {
		NGN_PROF_SCOPE("app::main::Resources::next()");

		if (context.device == vk::Device{}) {
			throw std::runtime_error("app::main::Resource not initialized");
		}

		auto &perAquire = nextAvailablePerAquire();
		{
			NGN_PROF_SCOPE("reset perAquire fence");
			RN_VLK_TRACE(context.device.resetFences({ perAquire.fence.get() }));
		}

		uint32_t idx;
		{
			NGN_PROF_SCOPE("acquire image");

			const auto acquireResult = RN_VLK_TRACE(context.device.acquireNextImageKHR(context.swapchain, std::numeric_limits<uint64_t>::max(), perAquire.semaphore.get(), perAquire.fence.get()));
			if (acquireResult.result != vk::Result::eSuccess) {
				ngn::log::warn("Acquire result: {}", vk::to_string(acquireResult.result));
			}

			idx = acquireResult.value;
		}

		auto &perSubmit = nextAvailablePerSubmit(idx, perAquire.fence.get());

		auto &perImage = nextAvailablePerImage(idx);
		{
			NGN_PROF_SCOPE("reset perImage fence");
			RN_VLK_TRACE(context.device.resetFences({ perImage.fence.get() }));
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

		handles.modelBuffer = perSwapchain.model.buffer.get();

		handles.imageIndex = idx;

		handles.acquireFence = perAquire.fence.get();
		handles.acquireSemaphore = perAquire.semaphore.get();

		handles.submitFence = perImage.fence.get();
		handles.submitSemaphore = perSubmit.semaphore.get();

		handles.surfaceImage = context.surface.images[idx];

		handles.framebuffer = perImage.framebuffer.get();
		handles.commandPool = perImage.commandPool.get();
		handles.commandBuffer = perImage.commandBuffer.get();

		return handles;
	}

	uint32_t needsWait(const std::vector<vk::Fence> & fences) {
		for (uint32_t i = 0; i < fences.size(); i++) {
			if (RN_VLK_TRACE(context.device.getFenceStatus(fences[i])) != vk::Result::eSuccess) {
				return i;
			}
		}

		return std::numeric_limits<uint32_t>::max();
	}

	PerAquire & nextAvailablePerAquire() {
		NGN_PROF_SCOPE("::nextAvailablePerAquire()");
		assert(perAquireList.size() > 0);

		std::vector<vk::Fence> fences = util::map(perAquireList, [] (auto &perAquire) { return perAquire.wait; });

		RN_VLK_TRACE(context.device.waitForFences(fences, false, std::numeric_limits<uint64_t>::max()));

		const auto found = std::find_if(std::begin(perAquireList), std::end(perAquireList), [&] (const auto &perAquire) {
			const auto result = RN_VLK_TRACE(context.device.getFenceStatus(perAquire.wait));

			return result == vk::Result::eSuccess;
		});

		if (found != std::end(perAquireList)) {
			#if defined(DEBUG)
			{
				uint32_t fenceIdx = needsWait({ found->fence.get() });

				if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
					needsWaitStats.nextAvailablePerAquireFound++;
				}
			}
			#endif

			return *found;
		} else {
			// should never happen, but handle it anyway -- wait for the first entry
			ngn::log::warn("No PerAquire available even after the wait for all fences");

			RN_VLK_TRACE(context.device.waitIdle());
			RN_VLK_TRACE(context.device.waitForFences({ perAquireList[0].wait }, false, std::numeric_limits<uint64_t>::max()));

			#if defined(DEBUG)
			{
				uint32_t fenceIdx = needsWait({ perAquireList[0].fence.get() });

				if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
					needsWaitStats.nextAvailablePerAquireFallback++;
				}
			}
			#endif

			return perAquireList[0];
		}
	}

	PerSubmit & nextAvailablePerSubmit(uint32_t idx, const vk::Fence &acquireFence) {
		NGN_PROF_SCOPE("::nextAvailablePerSubmit()");

		auto &perSubmit = perSubmitList[idx];

		#if defined(DEBUG)
		{
			uint32_t fenceIdx = needsWait({ perSubmit.wait });

			if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
				needsWaitStats.nextAvailablePerSubmit++;
			}
		}
		#endif

		RN_VLK_TRACE(context.device.waitForFences({ perSubmit.wait }, false, std::numeric_limits<uint64_t>::max()));

		perSubmit.wait = acquireFence;

		return perSubmit;
	}

	PerImage & nextAvailablePerImage(uint32_t idx) {
		NGN_PROF_SCOPE("::nextAvailablePerImage()");

		auto &perImage = perImageList[idx];

		#if defined(DEBUG)
		{
			uint32_t fenceIdx = needsWait({ perImage.fence.get() });

			if (fenceIdx != std::numeric_limits<uint32_t>::max()) {
				needsWaitStats.nextAvailablePerImage++;
			}
		}
		#endif

		RN_VLK_TRACE(context.device.waitForFences({ perImage.fence.get() }, false, std::numeric_limits<uint64_t>::max()));

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

		result.model = createModel();

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

		vk::RenderPassCreateInfo renderPassInfo{
			/*.flags=*/ vk::RenderPassCreateFlags{},
			/*.attachmentCount=*/ static_cast<uint32_t>(attachments.size()),
			/*.pAttachments=*/ attachments.data(),
			/*.subpassCount=*/ static_cast<uint32_t>(subpasses.size()),
			/*.pSubpasses=*/ subpasses.data(),
			/*.dependencyCount=*/ static_cast<uint32_t>(dependencies.size()),
			/*.pDependencies=*/ dependencies.data()
		};

		return RN_VLK_TRACE(context.device.createRenderPassUnique(renderPassInfo));
	}

	vk::UniqueShaderModule createShaderModule(const std::vector<std::byte> &code) {
		assert(code.size() > 0);
		assert(code.size() % sizeof(uint32_t) == 0);

		vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
			/*.flags=*/ vk::ShaderModuleCreateFlags{},
			/*.codeSize=*/ static_cast<uint32_t>(code.size()),
			/*.pCode=*/ reinterpret_cast<const uint32_t *>(code.data()),
		};

		return RN_VLK_TRACE(context.device.createShaderModuleUnique(shaderModuleCreateInfo));
	}

	vk::UniquePipeline createGraphicsPipeline(const vk::RenderPass &renderPass) {
		// shader modules
		std::vector<std::byte> vertCode = ngn::fs::read("shaders/main.vert.spv", 4);
		std::vector<std::byte> fragCode = ngn::fs::read("shaders/main.frag.spv", 4);

		vk::UniqueShaderModule vertShaderModule = createShaderModule(vertCode);
		vk::UniqueShaderModule fragShaderModule = createShaderModule(fragCode);

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{
			{
				/*.flags=*/ vk::PipelineShaderStageCreateFlags{},
				/*.stage=*/ vk::ShaderStageFlagBits::eVertex,
				/*.module=*/ vertShaderModule.get(),
				/*.pName=*/ "main",
				/*.pSpecializationInfo=*/ nullptr
			},
			{
				/*.flags=*/ vk::PipelineShaderStageCreateFlags{},
				/*.stage=*/ vk::ShaderStageFlagBits::eFragment,
				/*.module=*/ fragShaderModule.get(),
				/*.pName=*/ "main",
				/*.pSpecializationInfo=*/ nullptr
			}
		};

		// vertex input state
		std::vector<vk::VertexInputBindingDescription> vertexBindingDescriptions{
			{
				/*.binding=*/ 0,
				/*.stride=*/ sizeof(VertexData),
				/*.inputRate=*/ vk::VertexInputRate::eVertex
			}
		};

		std::vector<vk::VertexInputAttributeDescription> vertexAttributeDescriptions{
			{
				/*.location=*/ 0,
				/*.binding=*/ vertexBindingDescriptions[0].binding,
				/*.format=*/ vk::Format::eR32G32B32A32Sfloat,
				/*.offset=*/ offsetof(struct VertexData, position)
			},
			{
				/*.location=*/ 1,
				/*.binding=*/ vertexBindingDescriptions[0].binding,
				/*.format=*/ vk::Format::eR32G32B32A32Sfloat,
				/*.offset=*/ offsetof(struct VertexData, color)
			}
		};

		vk::PipelineVertexInputStateCreateInfo vertexInputState{
			/*.flags=*/ vk::PipelineVertexInputStateCreateFlags{},
			/*.vertexBindingDescriptionCount=*/ static_cast<uint32_t>(vertexBindingDescriptions.size()),
			/*.pVertexBindingDescriptions=*/ vertexBindingDescriptions.data(),
			/*.vertexAttributeDescriptionCount=*/ static_cast<uint32_t>(vertexAttributeDescriptions.size()),
			/*.pVertexAttributeDescriptions=*/ vertexAttributeDescriptions.data()
		};

		// input assembly state
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{
			/*.flags=*/ vk::PipelineInputAssemblyStateCreateFlags{},
			/*.topology=*/ vk::PrimitiveTopology::eTriangleStrip,
			/*.primitiveRestartEnable=*/ false
		};

		// viewport
		vk::Viewport viewport{
			/*.x=*/ 0.0f,
			/*.y=*/ 0.0f,
			/*.width=*/ static_cast<float>(context.surface.extent.width),
			/*.height=*/ static_cast<float>(context.surface.extent.height),
			/*.minDepth=*/ 0.0f,
			/*.maxDepth=*/ 1.0f
		};

		// scissor
		vk::Rect2D scissor{
			/*.offset=*/ vk::Offset2D{0, 0},
			/*.extent=*/ context.surface.extent
		};

		// viewport state
		vk::PipelineViewportStateCreateInfo viewportState{
			/*.flags=*/ vk::PipelineViewportStateCreateFlags{},
			/*.viewportCount=*/ 1,
			/*.pViewports=*/ &viewport,
			/*.scissorCount=*/ 1,
			/*.pScissors=*/ &scissor
		};

		// rasterization state
		vk::PipelineRasterizationStateCreateInfo rasterizerState{
			/*.flags=*/ vk::PipelineRasterizationStateCreateFlags{},
			/*.depthClampEnable=*/ false,
			/*.rasterizerDiscardEnable=*/ false,
			/*.polygonMode=*/ vk::PolygonMode::eFill,
			/*.cullMode=*/ vk::CullModeFlagBits::eBack,
			/*.frontFace=*/ vk::FrontFace::eCounterClockwise,
			/*.depthBiasEnable=*/ false,
			/*.depthBiasConstantFactor=*/ 0.0f,
			/*.depthBiasClamp=*/ 0.0f,
			/*.depthBiasSlopeFactor=*/ 0.0f,
			/*.lineWidth=*/ 1.0f
		};

		// multisample state
		vk::PipelineMultisampleStateCreateInfo multisampleState{
			/*.flags=*/ vk::PipelineMultisampleStateCreateFlags{},
			/*.rasterizationSamples=*/ vk::SampleCountFlagBits::e1,
			/*.sampleShadingEnable=*/ false,
			/*.minSampleShading=*/ 1.0f,
			/*.pSampleMask=*/ nullptr,
			/*.alphaToCoverageEnable=*/ false,
			/*.alphaToOneEnable=*/ false,
		};

		// color blend attachment state
		vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{
			/*.blendEnable=*/ false,
			/*.srcColorBlendFactor=*/ vk::BlendFactor::eSrcAlpha,
			/*.dstColorBlendFactor=*/ vk::BlendFactor::eOneMinusSrcAlpha,
			/*.colorBlendOp=*/ vk::BlendOp::eAdd,
			/*.srcAlphaBlendFactor=*/ vk::BlendFactor::eOne,
			/*.dstAlphaBlendFactor=*/ vk::BlendFactor::eZero,
			/*.alphaBlendOp=*/ vk::BlendOp::eAdd,
			/*.colorWriteMask=*/ vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
		};

		// color blend state
		vk::PipelineColorBlendStateCreateInfo colorBlendingState{
			/*.flags=*/ vk::PipelineColorBlendStateCreateFlags{},
			/*.logicOpEnable=*/ false,
			/*.logicOp=*/ vk::LogicOp::eCopy,
			/*.attachmentCount=*/ 1,
			/*.pAttachments=*/ &colorBlendAttachmentState,
			/*.blendConstants=*/ { { 0.0f, 0.0f, 0.0f, 0.0f } }
		};

		// list of dynamic states
		std::vector<vk::DynamicState> dynamicStates{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		// dynamic state
		vk::PipelineDynamicStateCreateInfo dynamicState{
			/*.flags=*/ vk::PipelineDynamicStateCreateFlags{},
			/*.dynamicStateCount=*/ static_cast<uint32_t>(dynamicStates.size()),
			/*.pDynamicStates=*/ dynamicStates.data()
		};

		// pipeline layout
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
			/*.flags=*/ vk::PipelineLayoutCreateFlags{},
			/*.setLayoutCount=*/ 0,
			/*.pSetLayouts=*/ nullptr,
			/*.pushConstantRangeCount=*/ 0,
			/*.pPushConstantRanges=*/ 0
		};

		vk::UniquePipelineLayout pipelineLayout = RN_VLK_TRACE(context.device.createPipelineLayoutUnique(pipelineLayoutInfo));

		// pipeline
		vk::GraphicsPipelineCreateInfo pipelineInfo{
			/*.flags=*/ vk::PipelineCreateFlags{},
			/*.stageCount=*/ static_cast<uint32_t>(shaderStages.size()),
			/*.pStages=*/ shaderStages.data(),
			/*.pVertexInputState=*/ &vertexInputState,
			/*.pInputAssemblyState=*/ &inputAssemblyState,
			/*.pTessellationState=*/ nullptr,
			/*.pViewportState=*/ &viewportState,
			/*.pRasterizationState=*/ &rasterizerState,
			/*.pMultisampleState=*/ &multisampleState,
			/*.pDepthStencilState=*/ nullptr,
			/*.pColorBlendState=*/ &colorBlendingState,
			/*.pDynamicState=*/ &dynamicState,
			/*.layout=*/ pipelineLayout.get(),
			/*.renderPass=*/ renderPass,
			/*.subpass=*/ 0,
			/*.basePipelineHandle=*/ vk::Pipeline{},
			/*.basePipelineIndex=*/ -1
		};

		return RN_VLK_TRACE(context.device.createGraphicsPipelineUnique(vk::PipelineCache{}, pipelineInfo));
	}

	Model createModel() {
		vk::BufferCreateInfo bufferCreateInfo{
			/*.flags=*/ vk::BufferCreateFlags{},
			/*.size=*/ staticData.modelData.size() * sizeof(staticData.modelData[0]),
			/*.usage=*/ vk::BufferUsageFlagBits::eVertexBuffer,
			/*.sharingMode=*/ vk::SharingMode::eExclusive,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/ nullptr
		};

		vk::UniqueBuffer buffer = RN_VLK_TRACE(context.device.createBufferUnique(bufferCreateInfo));
		vk::MemoryRequirements memoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(buffer.get()));
		rn::vlk::memory::Handle memory = context.allocator.mesh.alloc(rn::vlk::memory::Usage::CPU_TO_GPU, memoryRequirements);

		assert(memory.pointer);
		assert(memory.flags & vk::MemoryPropertyFlagBits::eHostVisible);

		RN_VLK_TRACE(context.device.bindBufferMemory(buffer.get(), memory.memory, memory.offset));
		std::copy(std::begin(staticData.modelData), std::end(staticData.modelData), reinterpret_cast<decltype(staticData.modelData.data())>(memory.pointer));
		if (memory.needsFlushing()) {
			RN_VLK_TRACE(context.device.flushMappedMemoryRanges({
				{
					/*.memory=*/ memory.memory,
					/*.offset=*/ memory.offset,
					/*.size=*/ memoryRequirements.size
				}
			}));
		}

		return Model{
			std::move(buffer),
			std::move(memory),
		};
	}

	// Per Image
	vk::UniqueFramebuffer createFramebuffer(uint32_t idx) {
		std::vector<vk::ImageView> attachments{
			context.surface.imageViews[idx]
		};

		vk::FramebufferCreateInfo framebufferInfo{
			/*.flags=*/ vk::FramebufferCreateFlags{},
			/*.renderPass=*/ perSwapchain.renderPass.get(),
			/*.attachmentCount=*/ static_cast<uint32_t>(attachments.size()),
			/*.pAttachments=*/ attachments.data(),
			/*.width=*/ context.surface.extent.width,
			/*.height=*/ context.surface.extent.height,
			/*.layers=*/ 1
		};

		return RN_VLK_TRACE(context.device.createFramebufferUnique(framebufferInfo));
	}

	vk::UniqueCommandPool createCommandPool() {
		vk::CommandPoolCreateInfo commandPoolInfo{
			/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			/*.queueFamilyIndex=*/ context.family.graphic,
		};

		return RN_VLK_TRACE(context.device.createCommandPoolUnique(commandPoolInfo));
	}

	vk::UniqueCommandBuffer createCommandBuffer(const vk::CommandPool &commandPool) {
		vk::CommandBufferAllocateInfo commandBufferAllocate{
			/*.commandPool=*/ commandPool,
			/*.level=*/ vk::CommandBufferLevel::ePrimary,
			/*.commandBufferCount=*/ 1
		};

		std::vector<vk::UniqueCommandBuffer> buffers = RN_VLK_TRACE(context.device.allocateCommandBuffersUnique(commandBufferAllocate));

		return std::move(buffers[0]);
	}

	vk::UniqueFence createFence() {
		return RN_VLK_TRACE(context.device.createFenceUnique(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled}));
	}

	vk::UniqueSemaphore createSemaphore() {
		return RN_VLK_TRACE(context.device.createSemaphoreUnique(vk::SemaphoreCreateInfo{}));
	}
};

}
