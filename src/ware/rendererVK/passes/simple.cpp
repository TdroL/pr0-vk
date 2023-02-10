#include "simple.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <vector>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#include <util/fs.hpp>
#include <util/map.hpp>

namespace ware::rendererVK::passes::simple {

const uint32_t uniformBufferMaxCount = 1024;
const uint32_t sampledImageMaxCount = 1024;
const uint32_t storageImageMaxCount = 1024;
const uint32_t samplerMaxCount = 32;

[[nodiscard]] vk::UniqueDescriptorPool createDescriptorPool(ware::contextVK::State &context) {
	std::array poolSizes{
		 vk::DescriptorPoolSize{
			.type = vk::DescriptorType::eUniformBuffer,
			.descriptorCount = uniformBufferMaxCount,
		},
		 vk::DescriptorPoolSize{
			.type = vk::DescriptorType::eSampledImage,
			.descriptorCount = sampledImageMaxCount,
		},
		 vk::DescriptorPoolSize{
			.type = vk::DescriptorType::eStorageImage,
			.descriptorCount = storageImageMaxCount,
		},
		 vk::DescriptorPoolSize{
			.type = vk::DescriptorType::eSampler,
			.descriptorCount = samplerMaxCount,
		},
	};

	return context.device->createDescriptorPoolUnique({
		.flags = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
		.maxSets = uniformBufferMaxCount + sampledImageMaxCount + storageImageMaxCount + samplerMaxCount,
		.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
		.pPoolSizes = poolSizes.data(),
	});
}

[[nodiscard]] std::vector<vk::UniqueDescriptorSetLayout> createDescriptorSetLayouts(ware::contextVK::State &context) {
	std::array buffersSetLayoutBindings{
		vk::DescriptorSetLayoutBinding{
			.binding = 0,
			.descriptorType = vk::DescriptorType::eUniformBuffer,
			.descriptorCount = uniformBufferMaxCount,
			.stageFlags = vk::ShaderStageFlagBits::eAll,
			.pImmutableSamplers = nullptr,
		},
	};

	std::array sampledImagesSetLayoutBindings{
		vk::DescriptorSetLayoutBinding{
			.binding = 0,
			.descriptorType = vk::DescriptorType::eSampledImage,
			.descriptorCount = sampledImageMaxCount,
			.stageFlags = vk::ShaderStageFlagBits::eAll,
			.pImmutableSamplers = nullptr,
		},
	};

	std::array storageImagesSetLayoutBindings{
		vk::DescriptorSetLayoutBinding{
			.binding = 0,
			.descriptorType = vk::DescriptorType::eStorageImage,
			.descriptorCount = storageImageMaxCount,
			.stageFlags = vk::ShaderStageFlagBits::eAll,
			.pImmutableSamplers = nullptr,
		},
	};

	std::array samplersSetLayoutBindings{
		vk::DescriptorSetLayoutBinding{
			.binding = 0,
			.descriptorType = vk::DescriptorType::eSampler,
			.descriptorCount = samplerMaxCount,
			.stageFlags = vk::ShaderStageFlagBits::eAll,
			.pImmutableSamplers = nullptr,
		},
	};

	std::array<vk::DescriptorBindingFlags, buffersSetLayoutBindings.size()> buffersSetLayoutBindingFlags{};
	std::array<vk::DescriptorBindingFlags, sampledImagesSetLayoutBindings.size()> sampledImagesSetLayoutBindingFlags{};
	std::array<vk::DescriptorBindingFlags, storageImagesSetLayoutBindings.size()> storageImagesSetLayoutBindingFlags{};
	std::array<vk::DescriptorBindingFlags, samplersSetLayoutBindings.size()> samplersSetLayoutBindingFlags{};

	auto bindlessBindingFlags = vk::DescriptorBindingFlagBits::ePartiallyBound | vk::DescriptorBindingFlagBits::eVariableDescriptorCount | vk::DescriptorBindingFlagBits::eUpdateAfterBind;
	buffersSetLayoutBindingFlags.fill(bindlessBindingFlags);
	sampledImagesSetLayoutBindingFlags.fill(bindlessBindingFlags);
	storageImagesSetLayoutBindingFlags.fill(bindlessBindingFlags);
	samplersSetLayoutBindingFlags.fill(bindlessBindingFlags);

	vk::StructureChain buffersSetLayoutCreateInfo{
		vk::DescriptorSetLayoutCreateInfo{
			.flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
			.bindingCount = static_cast<uint32_t>(buffersSetLayoutBindings.size()),
			.pBindings = buffersSetLayoutBindings.data(),
		},
		vk::DescriptorSetLayoutBindingFlagsCreateInfo{
			.bindingCount = static_cast<uint32_t>(buffersSetLayoutBindingFlags.size()),
			.pBindingFlags = buffersSetLayoutBindingFlags.data(),
		},
	};
	vk::StructureChain sampledImagesSetLayoutCreateInfo{
		vk::DescriptorSetLayoutCreateInfo{
			.flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
			.bindingCount = static_cast<uint32_t>(sampledImagesSetLayoutBindings.size()),
			.pBindings = sampledImagesSetLayoutBindings.data(),
		},
		vk::DescriptorSetLayoutBindingFlagsCreateInfo{
			.bindingCount = static_cast<uint32_t>(sampledImagesSetLayoutBindingFlags.size()),
			.pBindingFlags = sampledImagesSetLayoutBindingFlags.data(),
		},
	};
	vk::StructureChain storageImagesSetLayoutCreateInfo{
		vk::DescriptorSetLayoutCreateInfo{
			.flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
			.bindingCount = static_cast<uint32_t>(storageImagesSetLayoutBindings.size()),
			.pBindings = storageImagesSetLayoutBindings.data(),
		},
		vk::DescriptorSetLayoutBindingFlagsCreateInfo{
			.bindingCount = static_cast<uint32_t>(storageImagesSetLayoutBindingFlags.size()),
			.pBindingFlags = storageImagesSetLayoutBindingFlags.data(),
		},
	};
	vk::StructureChain samplersSetLayoutCreateInfo{
		vk::DescriptorSetLayoutCreateInfo{
			.flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
			.bindingCount = static_cast<uint32_t>(samplersSetLayoutBindings.size()),
			.pBindings = samplersSetLayoutBindings.data(),
		},
		vk::DescriptorSetLayoutBindingFlagsCreateInfo{
			.bindingCount = static_cast<uint32_t>(samplersSetLayoutBindingFlags.size()),
			.pBindingFlags = samplersSetLayoutBindingFlags.data(),
		},
	};

	auto buffersSetLayout = context.device->createDescriptorSetLayoutUnique(buffersSetLayoutCreateInfo.get());
	auto sampledImagesSetLayout = context.device->createDescriptorSetLayoutUnique(sampledImagesSetLayoutCreateInfo.get());
	auto storageImagesSetLayout = context.device->createDescriptorSetLayoutUnique(storageImagesSetLayoutCreateInfo.get());
	auto samplersSetLayout = context.device->createDescriptorSetLayoutUnique(samplersSetLayoutCreateInfo.get());

	std::vector<vk::UniqueDescriptorSetLayout> setLayouts{};
	setLayouts.reserve(4);
	setLayouts.push_back(std::move(buffersSetLayout));
	setLayouts.push_back(std::move(sampledImagesSetLayout));
	setLayouts.push_back(std::move(storageImagesSetLayout));
	setLayouts.push_back(std::move(samplersSetLayout));

	return setLayouts;
}

[[nodiscard]] vk::UniquePipelineLayout createPipelineLayout(ware::contextVK::State &context, const std::vector<vk::UniqueDescriptorSetLayout> &descriptorSetLayouts) {
	std::vector setLayouts = util::map(descriptorSetLayouts, [] (const auto &setLayout) { return setLayout.get(); });

	return context.device->createPipelineLayoutUnique({
		.setLayoutCount = static_cast<uint32_t>(setLayouts.size()),
		.pSetLayouts = setLayouts.data(),
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr,
	});
}

[[nodiscard]] vk::UniqueShaderModule createShaderModule(ware::contextVK::State &context, const std::filesystem::path &filePath) {
	auto contentsO = util::fsReadBytes(filePath);

	if ( ! contentsO) {
		throw std::runtime_error{fmt::format("Unable to read shader file \"{}\"", filePath.string())};
	}

	if (contentsO->size() == 0) {
		throw std::runtime_error{fmt::format("Shader file \"{}\" is empty", filePath.string())};
	}

	if (contentsO->size() % sizeof(uint32_t) != 0) {
		throw std::runtime_error{fmt::format("Shader file \"{}\" is corrupted (size: {})", filePath.string(), contentsO->size())};
	}

	vk::ShaderModuleCreateInfo PFN_vkGetShaderModuleCreateInfoIdentifierEXT;

	return context.device->createShaderModuleUnique({
		.codeSize = static_cast<uint32_t>(contentsO->size()),
		.pCode = reinterpret_cast<uint32_t *>(contentsO->data()),
	});
}

[[nodiscard]] vk::UniquePipeline createPipeline([[maybe_unused]] ware::windowGLFW::State &window, ware::contextVK::State &context, ware::swapchainVK::State &swapchain, vk::PipelineLayout layout) {
	auto vertexShaderModule = createShaderModule(context, "shaders/simple.vert.spv");
	auto fragmentShaderModule = createShaderModule(context, "shaders/simple.frag.spv");

	std::array stages{
		vk::PipelineShaderStageCreateInfo{
			.stage = vk::ShaderStageFlagBits::eVertex,
			.module = vertexShaderModule.get(),
			.pName = "main",
		},
		vk::PipelineShaderStageCreateInfo{
			.stage = vk::ShaderStageFlagBits::eFragment,
			.module = fragmentShaderModule.get(),
			.pName = "main",
		},
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputState{};
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{
		.topology = vk::PrimitiveTopology::eTriangleList,
	};
	vk::PipelineViewportStateCreateInfo viewportState{};
	vk::PipelineRasterizationStateCreateInfo rasterizationState{
		.depthClampEnable = false,
		.rasterizerDiscardEnable = false,
		.polygonMode = vk::PolygonMode::eFill,
		.cullMode = vk::CullModeFlagBits::eNone,
		.frontFace = vk::FrontFace::eCounterClockwise,
		.depthBiasEnable = false,
		.lineWidth = 1.0f,
	};

	std::array blendAttachmentState{
		vk::PipelineColorBlendAttachmentState{
			.blendEnable = false,
			.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
		},
	};
	vk::PipelineColorBlendStateCreateInfo colorBlendState{
		.attachmentCount = static_cast<uint32_t>(blendAttachmentState.size()),
		.pAttachments = blendAttachmentState.data(),
	};

	std::array dynamicStates{
		vk::DynamicState::eBlendConstants,
		vk::DynamicState::eCullMode,
		vk::DynamicState::eDepthBias,
		vk::DynamicState::eDepthBiasEnable,
		vk::DynamicState::eDepthBounds,
		vk::DynamicState::eDepthBoundsTestEnable,
		vk::DynamicState::eDepthCompareOp,
		vk::DynamicState::eDepthTestEnable,
		vk::DynamicState::eDepthWriteEnable,
		vk::DynamicState::eFrontFace,
		vk::DynamicState::eLineWidth,
		// vk::DynamicState::ePrimitiveRestartEnable,
		vk::DynamicState::ePrimitiveTopology,
		vk::DynamicState::eRasterizerDiscardEnable,
		// vk::DynamicState::eScissor, // use ScissorWithCount instead
		vk::DynamicState::eScissorWithCount,
		vk::DynamicState::eStencilCompareMask,
		vk::DynamicState::eStencilOp,
		vk::DynamicState::eStencilReference,
		vk::DynamicState::eStencilTestEnable,
		vk::DynamicState::eStencilWriteMask,
		// vk::DynamicState::eVertexInputBindingStride,
		// vk::DynamicState::eViewport, // use ViewportWithCount instead
		vk::DynamicState::eViewportWithCount,
	};
	vk::PipelineDynamicStateCreateInfo dynamicState{
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	std::array colorAttachmentFormats{
		swapchain.surfaceFormat.format,
	};

	vk::StructureChain craphicsPipelineCreateInfo{
		vk::GraphicsPipelineCreateInfo{
			.stageCount = static_cast<uint32_t>(stages.size()),
			.pStages = stages.data(),
			.pVertexInputState = &vertexInputState,
			.pInputAssemblyState = &inputAssemblyState,
			.pViewportState = &viewportState,
			.pRasterizationState = &rasterizationState,
			.pColorBlendState = &colorBlendState,
			.pDynamicState = &dynamicState,
			.layout = layout,
		},
		vk::PipelineRenderingCreateInfo{
			.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentFormats.size()),
			.pColorAttachmentFormats = colorAttachmentFormats.data(),
		},
	};

	auto resultValue = context.device->createGraphicsPipelineUnique(context.pipelineCache.get(), craphicsPipelineCreateInfo.get());

	if (resultValue.result != vk::Result::eSuccess && resultValue.result != vk::Result::ePipelineCompileRequired) {
		throw std::runtime_error{fmt::format("Unable to create pipeline (error: {})", vk::to_string(resultValue.result))};
	}

	return std::move(resultValue.value);
}

[[nodiscard]] std::vector<vk::DescriptorSet> allocateDescriptorSets(ware::contextVK::State &context, vk::DescriptorPool descriptorPool, const std::vector<vk::UniqueDescriptorSetLayout> &descriptorSetLayouts) {
	std::vector setLayouts = util::map(descriptorSetLayouts, [] (const auto &setLayout) { return setLayout.get(); });

	std::array descriptorCounts{
		uniformBufferMaxCount,
		sampledImageMaxCount,
		storageImageMaxCount,
		samplerMaxCount,
	};

	vk::StructureChain descriptorSetsAllocateInfo{
		vk::DescriptorSetAllocateInfo{
			.descriptorPool = descriptorPool,
			.descriptorSetCount = static_cast<uint32_t>(setLayouts.size()),
			.pSetLayouts = setLayouts.data(),
		},
		vk::DescriptorSetVariableDescriptorCountAllocateInfo{
			.descriptorSetCount = static_cast<uint32_t>(descriptorCounts.size()),
			.pDescriptorCounts = descriptorCounts.data(),
		},
	};

	return context.device->allocateDescriptorSets(descriptorSetsAllocateInfo.get());
}

std::tuple<ware::contextVK::UniqueBuffer, ware::contextVK::UniqueBuffer> createBuffers(ware::contextVK::State &context) {
	vk::BufferCreateInfo vertexBufferCreateInfo{
		.size = 3 * 2 * sizeof(float),
		.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
	};
	vma::AllocationCreateInfo vertexAllocationCreateInfo{
		.usage = vma::MemoryUsage::eAutoPreferDevice,
	};

	auto vertexBuffer = ware::contextVK::createBuffer(context, vertexBufferCreateInfo, vertexAllocationCreateInfo);

	vk::BufferCreateInfo stagingBufferCreateInfo{
		.size = 3 * 2 * sizeof(float),
		.usage = vk::BufferUsageFlagBits::eTransferSrc,
	};
	vma::AllocationCreateInfo stagingAllocationCreateInfo{
		.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped,
		.usage = vma::MemoryUsage::eAutoPreferHost,
		.requiredFlags = vk::MemoryPropertyFlagBits::eHostVisible,
	};

	auto stagingBuffer = ware::contextVK::createBuffer(context, stagingBufferCreateInfo, stagingAllocationCreateInfo);

	// upload data to staging buffer
	std::array vertices{
		 0.0f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};
	std::copy_n(vertices.data(), vertices.size(), reinterpret_cast<float *>(stagingBuffer->mappedData));

	ware::contextVK::flushMappedData(stagingBuffer);

	return { std::move(vertexBuffer), std::move(stagingBuffer) };
}

std::vector<FrameResources> createFrameResources(ware::contextVK::State &context, ware::swapchainVK::State &swapchain) {
	return util::mapRange(swapchain.frameResources.size(), [&] ([[maybe_unused]] const auto &index) {
		auto renderingCommandPool = context.device->createCommandPoolUnique({
			.flags = vk::CommandPoolCreateFlagBits::eTransient,
			.queueFamilyIndex = context.graphicQueueFamily,
		});

		std::vector<vk::CommandBuffer> renderingCommandBuffers = context.device->allocateCommandBuffers({
			.commandPool = renderingCommandPool.get(),
			.level = vk::CommandBufferLevel::eSecondary,
			.commandBufferCount = 1,
		});

		return FrameResources{
			.renderingCommandPool = std::move(renderingCommandPool),
			.renderingCommandBuffer = renderingCommandBuffers[0],
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

vk::CommandBuffer render(State &state) {
	const auto &context = state.context;
	const auto &swapchain = state.swapchain;
	const auto &swapchainImageResources = swapchain.imageResources[swapchain.imageIndex];
	// const auto &swapchainFrameResources = swapchain.frameResources[swapchain.frameIndex];
	auto &frameResources = state.frameResources[swapchain.frameIndex];

	const auto width = static_cast<uint32_t>(state.window.description->width);
	const auto height = static_cast<uint32_t>(state.window.description->height);

	context.device->resetCommandPool(frameResources.renderingCommandPool.get());

	auto &cmd = frameResources.renderingCommandBuffer;

	vk::ImageSubresourceRange  swapchainSubresourceRange{
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	vk::CommandBufferInheritanceInfo inheritanceInfo{};
	cmd.begin({
		.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
		.pInheritanceInfo = &inheritanceInfo,
	});

	if ( ! state.vertexBufferUploaded) {
		std::array regions{
			vk::BufferCopy2{
				.srcOffset = 0,
				.dstOffset = 0,
				.size = state.vertexBuffer.get().size,
			},
		};

		cmd.copyBuffer2({
			.srcBuffer = state.stagingBuffer.get().buffer,
			.dstBuffer = state.vertexBuffer.get().buffer,
			.regionCount = static_cast<uint32_t>(regions.size()),
			.pRegions = regions.data(),
		});

		std::array bufferMemoryBerries{
			vk::BufferMemoryBarrier2{
				.srcStageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
				.srcAccessMask = vk::AccessFlagBits2::eNone,
				.dstStageMask = vk::PipelineStageFlagBits2::eVertexInput,
				.dstAccessMask = vk::AccessFlagBits2::eVertexAttributeRead | vk::AccessFlagBits2::eIndexRead,
				.srcQueueFamilyIndex = context.graphicQueueFamily,
				.dstQueueFamilyIndex = context.graphicQueueFamily,
				.buffer = state.vertexBuffer.get().buffer,
				.offset = 0,
				.size = state.vertexBuffer.get().size,
			},
		};

		cmd.pipelineBarrier2({
			.bufferMemoryBarrierCount = static_cast<uint32_t>(bufferMemoryBerries.size()),
			.pBufferMemoryBarriers = bufferMemoryBerries.data(),
		});

		state.vertexBufferUploaded = true;
	}

	{
		{
			std::array imageMemoryBerries{
				vk::ImageMemoryBarrier2{
					.srcStageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
					.srcAccessMask = vk::AccessFlagBits2::eMemoryRead,
					.dstStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
					.dstAccessMask = vk::AccessFlagBits2::eColorAttachmentRead | vk::AccessFlagBits2::eColorAttachmentWrite,
					.oldLayout = vk::ImageLayout::eUndefined,
					.newLayout = vk::ImageLayout::eAttachmentOptimal,
					.srcQueueFamilyIndex = context.presentationQueueFamily,
					.dstQueueFamilyIndex = context.graphicQueueFamily,
					.image = swapchainImageResources.image,
					.subresourceRange = swapchainSubresourceRange,
				},
			};

			cmd.pipelineBarrier2({
				.imageMemoryBarrierCount = static_cast<uint32_t>(imageMemoryBerries.size()),
				.pImageMemoryBarriers = imageMemoryBerries.data(),
			});
		}

		std::array colorAttachments{
			vk::RenderingAttachmentInfo{
				.imageView = swapchainImageResources.imageView.get(),
				.imageLayout = vk::ImageLayout::eAttachmentOptimal,
				// .loadOp = vk::AttachmentLoadOp::eClear,
				.loadOp = vk::AttachmentLoadOp::eDontCare,
				.storeOp = vk::AttachmentStoreOp::eStore,
				// .clearValue = vk::ClearColorValue{std::array<float, 4>{ 1.0f, 1.0f, 1.0f, 0.0f }},
			},
		};

		cmd.beginRendering(vk::RenderingInfo{
			.renderArea = vk::Rect2D{ 0, 0, width, height },
			.layerCount = 1,
			.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size()),
			.pColorAttachments = colorAttachments.data(),
		});

		cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, state.layout.get(), 0, static_cast<uint32_t>(state.descriptorSets.size()), state.descriptorSets.data(), 0, nullptr);

		cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, state.pipeline.get());
		// set vk::DynamicState::eBlendConstants
		const float blendConstants[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
		cmd.setBlendConstants(blendConstants);
		// set vk::DynamicState::eCullMode
		cmd.setCullMode(vk::CullModeFlagBits::eNone);
		// set vk::DynamicState::eDepthBias
		cmd.setDepthBias(0.0f, 0.0f, 0.0f);
		// set vk::DynamicState::eDepthBiasEnable
		cmd.setDepthBiasEnable(false);
		// set vk::DynamicState::eDepthBounds
		cmd.setDepthBounds(0.0f, 1.0f);
		// set vk::DynamicState::eDepthBoundsTestEnable
		cmd.setDepthBoundsTestEnable(false);
		// set vk::DynamicState::eDepthCompareOp
		cmd.setDepthCompareOp(vk::CompareOp::eNever);
		// set vk::DynamicState::eDepthTestEnable
		cmd.setDepthTestEnable(false);
		// set vk::DynamicState::eDepthWriteEnable
		cmd.setDepthWriteEnable(false);
		// set vk::DynamicState::eFrontFace
		cmd.setFrontFace(vk::FrontFace::eCounterClockwise);
		// set vk::DynamicState::eLineWidth
		cmd.setLineWidth(1.0);
		// set vk::DynamicState::ePrimitiveTopology
		cmd.setPrimitiveTopology(vk::PrimitiveTopology::eTriangleStrip);
		// set vk::DynamicState::eRasterizerDiscardEnable
		cmd.setRasterizerDiscardEnable(false);
		// set vk::DynamicState::eScissorWithCount
		cmd.setScissorWithCount({
			vk::Rect2D{ 0, 0, width, height },
		});
		// set vk::DynamicState::eStencilCompareMask
		cmd.setStencilCompareMask(vk::StencilFaceFlagBits::eFrontAndBack, 0);
		// set vk::DynamicState::eStencilOp
		cmd.setStencilOp(vk::StencilFaceFlagBits::eFrontAndBack, vk::StencilOp::eZero, vk::StencilOp::eZero, vk::StencilOp::eZero, vk::CompareOp::eNever);
		// set vk::DynamicState::eStencilReference
		cmd.setStencilReference(vk::StencilFaceFlagBits::eFrontAndBack, 0);
		// set vk::DynamicState::eStencilTestEnable
		cmd.setStencilTestEnable(false);
		// set vk::DynamicState::eStencilWriteMask
		cmd.setStencilWriteMask(vk::StencilFaceFlagBits::eFrontAndBack, 0);
		// set vk::DynamicState::eViewportWithCount
		cmd.setViewportWithCount({
			vk::Viewport{
				.x = 0.0f,
				.y = 0.0f,
				.width = static_cast<float>(width),
				.height = static_cast<float>(height),
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			},
		});

		cmd.draw(3, 1, 0, 0);

		cmd.endRendering();

		// {
		// 	std::array imageMemoryBerries{
		// 		vk::ImageMemoryBarrier2{
		// 			.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		// 			.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentRead | vk::AccessFlagBits2::eColorAttachmentWrite,
		// 			.dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
		// 			.dstAccessMask = vk::AccessFlagBits2::eMemoryRead,
		// 			.oldLayout = vk::ImageLayout::eAttachmentOptimal,
		// 			.newLayout = vk::ImageLayout::ePresentSrcKHR,
		// 			.srcQueueFamilyIndex = context.graphicQueueFamily,
		// 			.dstQueueFamilyIndex = context.presentationQueueFamily,
		// 			.image = swapchainImageResources.image,
		// 			.subresourceRange = swapchainSubresourceRange,
		// 		},
		// 	};

		// 	cmd.pipelineBarrier2({
		// 		.imageMemoryBarrierCount = static_cast<uint32_t>(imageMemoryBerries.size()),
		// 		.pImageMemoryBarriers = imageMemoryBerries.data(),
		// 	});
		// }
	}

	cmd.end();

	// vk::SemaphoreSubmitInfo waitSemaphoreInfo{
	// 	.semaphore = swapchainFrameResources.acquireSemaphore.get(),
	// 	.stageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
	// 	.deviceIndex = 0,
	// };
	// vk::CommandBufferSubmitInfo commandBufferInfo{
	// 	.commandBuffer = cmd,
	// 	.deviceMask = 0x0001,
	// };
	// vk::SemaphoreSubmitInfo signalSemaphoreInfo{
	// 	.semaphore = swapchainImageResources.presentSemaphore.get(),
	// 	.stageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
	// 	.deviceIndex = 0,
	// };
	// context.graphicQueue.submit2({
	// 	{
	// 		.waitSemaphoreInfoCount = 1,
	// 		.pWaitSemaphoreInfos = &waitSemaphoreInfo,
	// 		.commandBufferInfoCount = 1,
	// 		.pCommandBufferInfos = &commandBufferInfo,
	// 		.signalSemaphoreInfoCount = 1,
	// 		.pSignalSemaphoreInfos = &signalSemaphoreInfo,
	// 	}
	// }, swapchainFrameResources.renderingFence.get());

	return cmd;
}

State::~State() {
	ware::contextVK::requestWaitIdle(context);
}

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::swapchainVK::State &swapchain) {
	auto descriptorPool = createDescriptorPool(context);

	auto descriptorSetLayouts = createDescriptorSetLayouts(context);

	auto layout = createPipelineLayout(context, descriptorSetLayouts);

	auto pipeline = createPipeline(window, context, swapchain, layout.get());

	auto descriptorSets = allocateDescriptorSets(context, descriptorPool.get(), descriptorSetLayouts);

	auto [vertexBuffer, stagingBuffer] = createBuffers(context);

	auto frameResources = createFrameResources(context, swapchain);

	return State{
		.window = window,
		.context = context,
		.swapchain = swapchain,
		.descriptorPool = std::move(descriptorPool),
		.descriptorSetLayouts = std::move(descriptorSetLayouts),
		.layout = std::move(layout),
		.pipeline = std::move(pipeline),
		.descriptorSets = std::move(descriptorSets),
		.vertexBuffer = std::move(vertexBuffer),
		.stagingBuffer = std::move(stagingBuffer),
		.vertexBufferUploaded = false,
		.frameResources = std::move(frameResources),
		.description = {
			.changed = false,
		},
	};
}

void refresh(State &state) {
	ZoneScopedN("ware::rendererVK::passes::simple::refresh()");

	if (state.swapchain.description.changed) {
		recreateFrameResources(state);
	}
}

vk::CommandBuffer process(State &state) {
	ZoneScopedN("ware::rendererVK::passes::simple::process()");

	return render(state);
}

} // ware::rendererVK::passes::simple
