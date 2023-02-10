#pragma once

#include "passes/imgui.hpp"
#include "passes/simple.hpp"

namespace ware::rendererVK {

struct FrameResources {
	vk::UniqueCommandPool renderingCommandPool;
	vk::CommandBuffer renderingCommandBuffer;
};

struct State {
	ware::windowGLFW::State &window;
	ware::contextVK::State &context;
	ware::swapchainVK::State &swapchain;

	std::vector<FrameResources> frameResources;

	passes::imgui::State stateImgui;
	passes::simple::State stateSimple;
};

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::contextImgui::State &imgui, ware::swapchainVK::State &swapchain);

void refresh(State &state);

void process(State &state);

} // ware::rendererVK
