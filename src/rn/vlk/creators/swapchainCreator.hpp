#pragma once

#include <algorithm>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../configMapping.hpp"
#include "../context.hpp"
#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"

namespace rn::vlk {

class SwapchainCreator {
public:
	vk::UniqueSwapchainKHR create(Context &context) {
		vk::SurfaceKHR surface = context.surface.handle;
		vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;
		vk::Device device = context.device;
		vk::SurfaceFormatKHR surfaceFormat = context.surface.format;
		vk::Extent2D surfaceExtent = context.surface.extent;

		uint32_t presentationFamily = context.family.presentation;
		uint32_t graphicFamily = context.family.graphic;

		assert(surface);
		assert(physicalDevice);
		assert(device);

		vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

		uint32_t imageCount = chooseImageCount(surfaceCapabilities);
		vk::PresentModeKHR presentMode = choosePresentMode(presentModes);

		std::vector<uint32_t> queueFamilies{};
		if (presentationFamily != graphicFamily) {
			queueFamilies = { presentationFamily, graphicFamily };
		}

		vk::SwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = surfaceExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment; // eTransferDst
		swapchainCreateInfo.imageSharingMode = queueFamilies.size() ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;
		swapchainCreateInfo.queueFamilyIndexCount = queueFamilies.size();
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilies.size() ? queueFamilies.data() : nullptr;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = vk::SwapchainKHR{};

		vk::UniqueSwapchainKHR swapchainOwner{device.createSwapchainKHRUnique(swapchainCreateInfo)};

		// sync config
		ngn::config::core.window.width(surfaceExtent.width);
		ngn::config::core.window.height(surfaceExtent.height);
		ngn::config::core.window.imageBuffers(imageCount);
		ngn::config::core.window.vsync(toConfig(presentMode));
		ngn::config::core.window.surfaceFormat(toConfig(surfaceFormat.format));
		ngn::config::core.window.surfaceColorSpace(toConfig(surfaceFormat.colorSpace));

		return swapchainOwner;
	}

	uint32_t chooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities) {
		return std::max(capabilities.minImageCount, std::min(capabilities.maxImageCount, ngn::config::core.window.imageBuffers()));
	}

	vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &presentModes) {
		if (presentModes.empty()) {
			throw std::runtime_error{"Vulkan present modes not available"};
		}

		vk::PresentModeKHR desiredPresentMode = fromConfig(ngn::config::core.window.vsync());

		for (const auto& presentMode : presentModes) {
			if (presentMode == desiredPresentMode) {
				return desiredPresentMode;
			}
		}

		return presentModes[0];
	}
};

} // rn::vlk
