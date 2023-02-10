#pragma once

#include "../../contextVK/contextVK.hpp"
#include "../../swapchainVK/swapchainVK.hpp"
#include "../../contextImgui/contextImgui.hpp"

namespace ware::rendererVK::passes::imgui {

struct FrameResources {
	vk::UniqueCommandPool renderingCommandPool;
	vk::CommandBuffer renderingCommandBuffer;
	ware::contextVK::UniqueBuffer vertexBuffer;
	ware::contextVK::UniqueBuffer indexBuffer;
	uint32_t vertexCount;
	uint32_t indexCount;
};

struct Description {
	bool changed;
};

struct State {
	ware::windowGLFW::State &window;
	ware::contextVK::State &context;
	ware::swapchainVK::State &swapchain;

	vk::UniqueDescriptorPool descriptorPool;
	std::vector<vk::UniqueDescriptorSetLayout> descriptorSetLayouts;
	vk::UniquePipelineLayout layout;
	vk::UniquePipeline pipeline;
	ware::contextVK::UniqueImage fontImage;
	vk::UniqueImageView fontImageView;
	ware::contextVK::UniqueBuffer fontStaginBuffer;
	vk::UniqueSampler fontSampler;
	bool fontImageUploaded;
	std::vector<vk::DescriptorSet> descriptorSets;

	std::vector<FrameResources> frameResources;

	Description description;

	~State();
};

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::contextImgui::State &imgui, ware::swapchainVK::State &swapchain);

void refresh(State &state);

vk::CommandBuffer process(State &state);

} // ware::rendererVK::passes::imgui
