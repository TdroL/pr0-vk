#pragma once

#include <vector>

#include "../../contextVK/contextVK.hpp"
#include "../../swapchainVK/swapchainVK.hpp"

namespace ware::rendererVK::passes::simple {

struct FrameResources {
	vk::UniqueCommandPool renderingCommandPool;
	vk::CommandBuffer renderingCommandBuffer;
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
	std::vector<vk::DescriptorSet> descriptorSets;
	ware::contextVK::UniqueBuffer vertexBuffer;
	ware::contextVK::UniqueBuffer stagingBuffer;
	bool vertexBufferUploaded;
	std::vector<FrameResources> frameResources;

	Description description;

	~State();
};

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::swapchainVK::State &swapchain);

void refresh(State &state);

vk::CommandBuffer process(State &state);

} // ware::rendererVK::passes::simple
