#pragma once

#include "../config/config.hpp"
#include "../contextVK/contextVK.hpp"

namespace ware::swapchainVK {

struct ImageResources {
	vk::Image image;
	vk::UniqueImageView imageView;
	vk::UniqueSemaphore presentSemaphore;
};

struct FrameResources {
	vk::UniqueFence renderingFence;
	vk::UniqueSemaphore acquireSemaphore;
	// vk::UniqueCommandPool renderingCommandPool;
	// vk::CommandBuffer renderingCommandBuffer;
};

struct Properties {
	bool swapchainResized;
};

struct Description {
	Properties current;
	Properties next;
	bool changed;
};

struct State {
	ware::config::State &config;
	ware::windowGLFW::State &window;
	ware::contextVK::State &context;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR presentMode;
	vk::UniqueSwapchainKHR swapchain;
	std::vector<ImageResources> imageResources;
	uint32_t imageIndex;
	std::vector<FrameResources> frameResources;
	uint32_t frameIndex;
	Description description;

	~State();
};

State setup(ware::config::State &config, ware::windowGLFW::State &window, ware::contextVK::State &context);

void refresh(State &state);

void process(State &state);

} // ware::swapchainVK
