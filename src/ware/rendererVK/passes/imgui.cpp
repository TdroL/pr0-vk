#include "imgui.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <vector>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#include <util/fs.hpp>
#include <util/map.hpp>

namespace ware::rendererVK::passes::imgui {

struct PushConstant {
	float scale[2];
	float translate[2];
};

const uint32_t sampledImageMaxCount = 1;
const uint32_t samplerMaxCount = 1;

enum DescriptorIndex : uint32_t {
	Images = 0,
	Samplers = 1,
};

[[nodiscard]] vk::UniqueDescriptorPool createDescriptorPool(ware::contextVK::State &context) {
	std::array poolSizes{
		 vk::DescriptorPoolSize{
			.type = vk::DescriptorType::eSampledImage,
			.descriptorCount = sampledImageMaxCount,
		},
		 vk::DescriptorPoolSize{
			.type = vk::DescriptorType::eSampler,
			.descriptorCount = samplerMaxCount,
		},
	};

	return context.device->createDescriptorPoolUnique({
		.flags = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
		.maxSets = sampledImageMaxCount + samplerMaxCount,
		.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
		.pPoolSizes = poolSizes.data(),
	});
}

[[nodiscard]] std::vector<vk::UniqueDescriptorSetLayout> createDescriptorSetLayouts(ware::contextVK::State &context) {
	std::array sampledImagesSetLayoutBindings{
		vk::DescriptorSetLayoutBinding{
			.binding = 0,
			.descriptorType = vk::DescriptorType::eSampledImage,
			.descriptorCount = sampledImageMaxCount,
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

	std::array<vk::DescriptorBindingFlags, sampledImagesSetLayoutBindings.size()> sampledImagesSetLayoutBindingFlags{};
	std::array<vk::DescriptorBindingFlags, samplersSetLayoutBindings.size()> samplersSetLayoutBindingFlags{};

	auto bindlessBindingFlags = vk::DescriptorBindingFlagBits::ePartiallyBound | vk::DescriptorBindingFlagBits::eVariableDescriptorCount | vk::DescriptorBindingFlagBits::eUpdateAfterBind;
	sampledImagesSetLayoutBindingFlags.fill(bindlessBindingFlags);
	samplersSetLayoutBindingFlags.fill(bindlessBindingFlags);

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

	auto sampledImagesSetLayout = context.device->createDescriptorSetLayoutUnique(sampledImagesSetLayoutCreateInfo.get());
	auto samplersSetLayout = context.device->createDescriptorSetLayoutUnique(samplersSetLayoutCreateInfo.get());

	std::vector<vk::UniqueDescriptorSetLayout> setLayouts{};
	setLayouts.reserve(2);
	setLayouts.push_back(std::move(sampledImagesSetLayout));
	setLayouts.push_back(std::move(samplersSetLayout));

	return setLayouts;
}

[[nodiscard]] vk::UniquePipelineLayout createPipelineLayout(ware::contextVK::State &context, const std::vector<vk::UniqueDescriptorSetLayout> &descriptorSetLayouts) {
	std::vector setLayouts = util::map(descriptorSetLayouts, [] (const auto &setLayout) { return setLayout.get(); });

	std::array pushConstantRanges{
		vk::PushConstantRange{
			.stageFlags = vk::ShaderStageFlagBits::eAll,
			.offset = 0,
			.size = sizeof(PushConstant),
		},
	};

	return context.device->createPipelineLayoutUnique({
		.setLayoutCount = static_cast<uint32_t>(setLayouts.size()),
		.pSetLayouts = setLayouts.data(),
		.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size()),
		.pPushConstantRanges = pushConstantRanges.data(),
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
	auto vertexShaderModule = createShaderModule(context, "shaders/imgui.vert.spv");
	auto fragmentShaderModule = createShaderModule(context, "shaders/imgui.frag.spv");

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

	std::array vertexInputBindingDescriptions{
		vk::VertexInputBindingDescription{
			.binding = 0,
			.stride = sizeof(ImDrawVert),
			.inputRate = vk::VertexInputRate::eVertex,
		},
	};
	std::array vertexInputAttributeDescriptions{
		vk::VertexInputAttributeDescription{
			.location = 0,
			.binding = 0,
			.format = vk::Format::eR32G32Sfloat,
			.offset = offsetof(ImDrawVert, pos),
		},
		vk::VertexInputAttributeDescription{
			.location = 1,
			.binding = 0,
			.format = vk::Format::eR32G32Sfloat,
			.offset = offsetof(ImDrawVert, uv),
		},
		vk::VertexInputAttributeDescription{
			.location = 2,
			.binding = 0,
			.format = vk::Format::eR8G8B8A8Unorm,
			.offset = offsetof(ImDrawVert, col),
		},
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputState{
		.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindingDescriptions.size()),
		.pVertexBindingDescriptions = vertexInputBindingDescriptions.data(),
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributeDescriptions.size()),
		.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data(),
	};

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
			.blendEnable = true,
			.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
			.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
			.colorBlendOp = vk::BlendOp::eAdd,
			.srcAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
			.dstAlphaBlendFactor = vk::BlendFactor::eZero,
			.alphaBlendOp = vk::BlendOp::eAdd,
			.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
		},
	};

	vk::PipelineColorBlendStateCreateInfo colorBlendState{
		.attachmentCount = static_cast<uint32_t>(blendAttachmentState.size()),
		.pAttachments = blendAttachmentState.data(),
	};

	vk::PipelineDepthStencilStateCreateInfo depthStencilState{
		.depthTestEnable = false,
		.depthWriteEnable = false,
	};

	std::array dynamicStates{
		vk::DynamicState::eScissorWithCount,
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
			.pDepthStencilState = &depthStencilState,
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
		sampledImageMaxCount,
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

std::tuple<ware::contextVK::UniqueImage, vk::UniqueImageView, ware::contextVK::UniqueBuffer, vk::UniqueSampler> createFontResources(ware::contextVK::State &context) {
	auto &io = ImGui::GetIO();

	unsigned char* texData;
	int texWidth, texHeight, bytesPerPixel;
	io.Fonts->GetTexDataAsRGBA32(&texData, &texWidth, &texHeight, &bytesPerPixel);
	vk::DeviceSize uploadSize = texWidth * texHeight * bytesPerPixel;

	vk::ImageCreateInfo imageCreateInfo{
		.imageType = vk::ImageType::e2D,
		.format = vk::Format::eR8G8B8A8Unorm,
		.extent = { static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1 },
		.mipLevels = 1,
		.arrayLayers = 1,
		.usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
		.initialLayout = vk::ImageLayout::eUndefined,
	};

	vma::AllocationCreateInfo allocationCreateInfo{
		.usage = vma::MemoryUsage::eAutoPreferDevice,
	};

	auto image = ware::contextVK::createImage(context, imageCreateInfo, allocationCreateInfo);

	auto imageView = context.device->createImageViewUnique({
		.image = image->image,
		.viewType = vk::ImageViewType::e2D,
		.format = vk::Format::eR8G8B8A8Unorm,
		.subresourceRange = {
			.aspectMask = vk::ImageAspectFlagBits::eColor,
			.levelCount = 1,
			.layerCount = 1,
		},
	});

	vk::BufferCreateInfo stagingBufferCreateInfo{
		.size = uploadSize,
		.usage = vk::BufferUsageFlagBits::eTransferSrc,
	};
	vma::AllocationCreateInfo stagingAllocationCreateInfo{
		.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped,
		.usage = vma::MemoryUsage::eAutoPreferHost,
		.requiredFlags = vk::MemoryPropertyFlagBits::eHostVisible,
	};

	auto stagingBuffer = ware::contextVK::createBuffer(context, stagingBufferCreateInfo, stagingAllocationCreateInfo);

	std::copy_n(texData, uploadSize, reinterpret_cast<unsigned char *>(stagingBuffer->mappedData));

	ware::contextVK::flushMappedData(stagingBuffer);

	auto sampler = context.device->createSamplerUnique({
		.magFilter = vk::Filter::eLinear,
		.minFilter = vk::Filter::eLinear,
		.mipmapMode = vk::SamplerMipmapMode::eLinear,
		.addressModeU = vk::SamplerAddressMode::eClampToEdge,
		.addressModeV = vk::SamplerAddressMode::eClampToEdge,
		.addressModeW = vk::SamplerAddressMode::eClampToEdge,
		.borderColor = vk::BorderColor::eFloatOpaqueWhite,
	});

	return { std::move(image), std::move(imageView), std::move(stagingBuffer), std::move(sampler) };
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
			.vertexBuffer = {},
			.indexBuffer = {},
			.vertexCount = 0,
			.indexCount = 0,
		};
	});
}

void recreateFrameResources(State &state) {
	auto &context = state.context;
	auto &swapchain = state.swapchain;

	if (swapchain.description.swapchainResized && state.frameResources.size() != swapchain.frameResources.size()) {
		ware::contextVK::requestWaitIdle(context);

		state.frameResources.clear();

		state.frameResources = createFrameResources(context, swapchain);
	}
}

void recordNewFrame([[maybe_unused]] State &state) {
	ZoneScopedN("ware::rendererVK::passes::imgui::refresh()#record new frame");

	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::EndFrame();

	ImGui::Render();
}

void resizeBuffers(State &state) {
	ZoneScopedN("ware::rendererVK::passes::imgui::refresh()#resize buffers");

	auto &context = state.context;
	const auto &swapchain = state.swapchain;
	auto &frameResources = state.frameResources[swapchain.frameIndex];

	ImDrawData *drawData = ImGui::GetDrawData();
	const vk::DeviceSize vertexBufferSize = std::max(drawData ? drawData->TotalVtxCount : 0, 1024) * sizeof(ImDrawVert);
	const vk::DeviceSize indexBufferSize = std::max(drawData ? drawData->TotalIdxCount : 0, 1024) * sizeof(ImDrawIdx);

	if ( ! frameResources.vertexBuffer || frameResources.vertexBuffer->size < vertexBufferSize) {
		vk::BufferCreateInfo vertexBufferCreateInfo{
			.size = vertexBufferSize * 2,
			.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		};
		vma::AllocationCreateInfo vertexAllocationCreateInfo{
			.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped,
			.usage = vma::MemoryUsage::eAutoPreferHost,
			.requiredFlags = vk::MemoryPropertyFlagBits::eHostVisible,
		};

		frameResources.vertexBuffer = ware::contextVK::createBuffer(context, vertexBufferCreateInfo, vertexAllocationCreateInfo);
	}

	if ( ! frameResources.indexBuffer || frameResources.indexBuffer->size < indexBufferSize) {
		vk::BufferCreateInfo indexBufferCreateInfo{
			.size = indexBufferSize * 2,
			.usage = vk::BufferUsageFlagBits::eIndexBuffer,
		};
		vma::AllocationCreateInfo indexAllocationCreateInfo{
			.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped,
			.usage = vma::MemoryUsage::eAutoPreferHost,
			.requiredFlags = vk::MemoryPropertyFlagBits::eHostVisible,
		};

		frameResources.indexBuffer = ware::contextVK::createBuffer(context, indexBufferCreateInfo, indexAllocationCreateInfo);
	}
}

void uploadBuffers(State &state) {
	ZoneScopedN("ware::rendererVK::passes::imgui::refresh()#upload buffers");

	ImDrawData *drawData = ImGui::GetDrawData();
	if ( ! drawData || drawData->CmdListsCount <= 0) {
		return;
	}

	const vk::DeviceSize vertexBufferSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
	const vk::DeviceSize indexBufferSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);

	const auto &swapchain = state.swapchain;
	auto &frameResources = state.frameResources[swapchain.frameIndex];

	auto *vertexMappedData = reinterpret_cast<ImDrawVert *>(frameResources.vertexBuffer->mappedData);
	auto *indexMappedData = reinterpret_cast<ImDrawIdx *>(frameResources.indexBuffer->mappedData);

	for (const auto &cmdList : std::span{drawData->CmdLists, static_cast<uint32_t>(drawData->CmdListsCount)}) {
		vertexMappedData = std::copy_n(cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size, vertexMappedData);
		indexMappedData = std::copy_n(cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size, indexMappedData);
	}

	ware::contextVK::flushMappedData(frameResources.vertexBuffer, 0, vertexBufferSize);
	ware::contextVK::flushMappedData(frameResources.indexBuffer, 0, indexBufferSize);

	frameResources.vertexCount = drawData->TotalVtxCount;
	frameResources.indexCount = drawData->TotalIdxCount;
}

vk::CommandBuffer render(State &state) {
	const auto &context = state.context;
	const auto &swapchain = state.swapchain;
	const auto &swapchainImageResources = swapchain.imageResources[swapchain.imageIndex];
	auto &frameResources = state.frameResources[swapchain.frameIndex];

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

	if ( ! state.fontImageUploaded) {
		std::array preImageMemoryBerries{
			vk::ImageMemoryBarrier2{
				.srcStageMask = vk::PipelineStageFlagBits2::eHost,
				.srcAccessMask = vk::AccessFlagBits2::eNone,
				.dstStageMask = vk::PipelineStageFlagBits2::eTransfer,
				.dstAccessMask = vk::AccessFlagBits2::eTransferWrite,
				.oldLayout = vk::ImageLayout::eUndefined,
				.newLayout = vk::ImageLayout::eTransferDstOptimal,
				.srcQueueFamilyIndex = context.graphicQueueFamily,
				.dstQueueFamilyIndex = context.graphicQueueFamily,
				.image = state.fontImage->image,
				.subresourceRange = {
					.aspectMask = vk::ImageAspectFlagBits::eColor,
					.levelCount = 1,
					.layerCount = 1,
				},
			},
		};
		cmd.pipelineBarrier2({
			.imageMemoryBarrierCount = static_cast<uint32_t>(preImageMemoryBerries.size()),
			.pImageMemoryBarriers = preImageMemoryBerries.data(),
		});

		std::array regions{
			vk::BufferImageCopy2{
				.bufferOffset = 0,
				.bufferRowLength = 0, // tightly packed
				.bufferImageHeight = 0, // tightly packed
				.imageSubresource = {
					.aspectMask = vk::ImageAspectFlagBits::eColor,
					.mipLevel = 0,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
				.imageOffset = {},
				.imageExtent = state.fontImage->extent,
			},
		};

		cmd.copyBufferToImage2({
			.srcBuffer = state.fontStaginBuffer->buffer,
			.dstImage = state.fontImage->image,
			.dstImageLayout = vk::ImageLayout::eTransferDstOptimal,
			.regionCount = static_cast<uint32_t>(regions.size()),
			.pRegions = regions.data(),
		});

		std::array postImageMemoryBerries{
			vk::ImageMemoryBarrier2{
				.srcStageMask = vk::PipelineStageFlagBits2::eTransfer,
				.srcAccessMask = vk::AccessFlagBits2::eTransferWrite,
				.dstStageMask = vk::PipelineStageFlagBits2::eFragmentShader,
				.dstAccessMask = vk::AccessFlagBits2::eShaderRead,
				.oldLayout = vk::ImageLayout::eTransferDstOptimal,
				.newLayout = vk::ImageLayout::eReadOnlyOptimal,
				.srcQueueFamilyIndex = context.graphicQueueFamily,
				.dstQueueFamilyIndex = context.graphicQueueFamily,
				.image = state.fontImage->image,
				.subresourceRange = {
					.aspectMask = vk::ImageAspectFlagBits::eColor,
					.levelCount = 1,
					.layerCount = 1,
				},
			},
		};
		cmd.pipelineBarrier2({
			.imageMemoryBarrierCount = static_cast<uint32_t>(postImageMemoryBerries.size()),
			.pImageMemoryBarriers = postImageMemoryBerries.data(),
		});

		vk::DescriptorImageInfo fontImageInfo{
			.imageView = state.fontImageView.get(),
			.imageLayout = vk::ImageLayout::eReadOnlyOptimal,
		};
		vk::DescriptorImageInfo fontSamplerInfo{
			.sampler = state.fontSampler.get(),
		};

		std::array writeDescriptorSets{
			vk::WriteDescriptorSet{
				.dstSet = state.descriptorSets[DescriptorIndex::Images],
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = vk::DescriptorType::eSampledImage,
				.pImageInfo = &fontImageInfo,
			},
			vk::WriteDescriptorSet{
				.dstSet = state.descriptorSets[DescriptorIndex::Samplers],
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = vk::DescriptorType::eSampler,
				.pImageInfo = &fontSamplerInfo,
			},
		};

		context.device->updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

		state.fontImageUploaded = true;
	}

	ImDrawData *drawData = ImGui::GetDrawData();
	if (drawData && drawData->CmdListsCount > 0) {
		auto &io = ImGui::GetIO();
		const auto width = static_cast<uint32_t>(io.DisplaySize.x);
		const auto height = static_cast<uint32_t>(io.DisplaySize.y);

		std::array colorAttachments{
			vk::RenderingAttachmentInfo{
				.imageView = swapchainImageResources.imageView.get(),
				.imageLayout = vk::ImageLayout::eAttachmentOptimal,
				.loadOp = vk::AttachmentLoadOp::eLoad,
				.storeOp = vk::AttachmentStoreOp::eStore,
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

		{
			PushConstant pushConstant{
				.scale = { 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y },
				.translate = { -1.0f, -1.0f },
			};
			cmd.pushConstants(state.layout.get(), vk::ShaderStageFlagBits::eAll, 0, sizeof(PushConstant), &pushConstant);
		}

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

		cmd.bindIndexBuffer(frameResources.indexBuffer->buffer, 0, sizeof(ImDrawIdx) == sizeof(uint16_t) ? vk::IndexType::eUint16 : vk::IndexType::eUint32);

		{
			std::array buffers{
				frameResources.vertexBuffer->buffer,
			};
			std::array offsets{
				vk::DeviceSize{0u},
			};
			cmd.bindVertexBuffers2(0, 1, buffers.data(), offsets.data(), nullptr, nullptr);
		}

		int32_t vertexOffset = 0;
		int32_t indexOffset = 0;
		for (int32_t i = 0; i < drawData->CmdListsCount; i++) {
			const ImDrawList *drawList = drawData->CmdLists[i];

			for (int32_t j = 0; j < drawList->CmdBuffer.Size; j++) {
				const ImDrawCmd *drawCmd = &drawList->CmdBuffer[j];

				cmd.setScissorWithCount({
					vk::Rect2D{
						static_cast<int32_t>(drawCmd->ClipRect.x),
						static_cast<int32_t>(drawCmd->ClipRect.y),
						static_cast<uint32_t>(drawCmd->ClipRect.z - drawCmd->ClipRect.x),
						static_cast<uint32_t>(drawCmd->ClipRect.w - drawCmd->ClipRect.y),
					},
				});

				cmd.drawIndexed(drawCmd->ElemCount, 1, indexOffset, vertexOffset, 0);

				indexOffset += drawCmd->ElemCount;
			}

			vertexOffset += drawList->VtxBuffer.Size;
		}

		cmd.endRendering();
	}

	{
		std::array imageMemoryBerries{
			vk::ImageMemoryBarrier2{
				.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
				.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentRead | vk::AccessFlagBits2::eColorAttachmentWrite,
				.dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
				.dstAccessMask = vk::AccessFlagBits2::eMemoryRead,
				.oldLayout = vk::ImageLayout::eAttachmentOptimal,
				.newLayout = vk::ImageLayout::ePresentSrcKHR,
				.srcQueueFamilyIndex = context.graphicQueueFamily,
				.dstQueueFamilyIndex = context.presentationQueueFamily,
				.image = swapchainImageResources.image,
				.subresourceRange = swapchainSubresourceRange,
			},
		};

		cmd.pipelineBarrier2({
			.imageMemoryBarrierCount = static_cast<uint32_t>(imageMemoryBerries.size()),
			.pImageMemoryBarriers = imageMemoryBerries.data(),
		});
	}

	cmd.end();

	return cmd;
}

State::~State() {
	ware::contextVK::requestWaitIdle(context);
}

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, [[maybe_unused]] ware::contextImgui::State &imgui, ware::swapchainVK::State &swapchain) {
	auto descriptorPool = createDescriptorPool(context);

	auto descriptorSetLayouts = createDescriptorSetLayouts(context);

	auto layout = createPipelineLayout(context, descriptorSetLayouts);

	auto pipeline = createPipeline(window, context, swapchain, layout.get());

	auto [fontImage, fontImageView, fontStaginBuffer, fontSampler] = createFontResources(context);

	auto descriptorSets = allocateDescriptorSets(context, descriptorPool.get(), descriptorSetLayouts);

	auto frameResources = createFrameResources(context, swapchain);

	return State{
		.window = window,
		.context = context,
		.swapchain = swapchain,
		.descriptorPool = std::move(descriptorPool),
		.descriptorSetLayouts = std::move(descriptorSetLayouts),
		.layout = std::move(layout),
		.pipeline = std::move(pipeline),
		.fontImage = std::move(fontImage),
		.fontImageView = std::move(fontImageView),
		.fontStaginBuffer = std::move(fontStaginBuffer),
		.fontSampler = std::move(fontSampler),
		.fontImageUploaded = false,
		.descriptorSets = std::move(descriptorSets),
		.frameResources = std::move(frameResources),
		.description = {
			.changed = false,
		},
	};
}

void refresh(State &state) {
	ZoneScopedN("ware::rendererVK::passes::imgui::refresh()");

	if (state.swapchain.description.changed) {
		recreateFrameResources(state);
	}

	recordNewFrame(state);

	resizeBuffers(state);

	uploadBuffers(state);
}

vk::CommandBuffer process(State &state) {
	ZoneScopedN("ware::rendererVK::passes::imgui::process()");

	return render(state);
}

} // ware::rendererVK::passes::imgui
