#pragma once

#include <vector>
// #include <string>
#include <algorithm>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"

#include "../configMapping.hpp"
#include "../physicalDeviceHandle.hpp"
#include "../queuesHandle.hpp"

// #include "../swapchainOwner.hpp"

namespace rn {

namespace vlk {

class Context;

class SwapchainCreator {
public:
	vk::UniqueSwapchainKHR create(vk::UniqueSurfaceKHR &surfaceOwner, PhysicalDeviceHandle &physicalDeviceOwner, vk::UniqueDevice &deviceOwner, QueuesHandle &queuesOwner, vk::Extent2D &extent, vk::SurfaceFormatKHR &surfaceFormat) {
		vk::SurfaceKHR surface = surfaceOwner.get();
		vk::PhysicalDevice physicalDevice = physicalDeviceOwner.handle;
		vk::Device device = deviceOwner.get();
		QueueFamily presentation = queuesOwner.presentation;
		QueueFamily graphic = queuesOwner.graphic;

		assert(surface);
		assert(physicalDevice);
		assert(device);

		vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

		uint32_t imageCount = chooseImageCount(surfaceCapabilities);
		vk::PresentModeKHR presentMode = choosePresentMode(presentModes);

		std::vector<uint32_t> queueFamilies{};
		if (presentation.family != graphic.family) {
			queueFamilies = { presentation.family, graphic.family };
		}

		vk::SwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
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
		ngn::config::core.window.width(extent.width);
		ngn::config::core.window.height(extent.height);
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

} // vlk

} // rn
