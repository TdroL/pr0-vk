#include "swapchainCreator.hpp"

#include <algorithm>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../device.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"

namespace rn::vki {

vk::UniqueSwapchainKHR SwapchainCreator::create(Context &context) {
	vk::SurfaceKHR surface = context.surface.handle;
	vk::PhysicalDevice physicalDevice = context.physicalDevice.handle;
	Device &device = context.device;
	vk::SurfaceFormatKHR surfaceFormat = context.surface.format;
	vk::Extent2D surfaceExtent = context.surface.extent;

	uint32_t presentationFamily = context.family.presentation;
	uint32_t graphicFamily = context.family.graphic;

	assert(surface);
	assert(physicalDevice);
	assert(device);

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = RN_VLK_TRACE(physicalDevice.getSurfaceCapabilitiesKHR(surface));
	std::vector<vk::PresentModeKHR> presentModes = RN_VLK_TRACE(physicalDevice.getSurfacePresentModesKHR(surface));

	uint32_t imageCount = chooseImageCount(surfaceCapabilities);
	vk::PresentModeKHR presentMode = choosePresentMode(presentModes);

	std::vector<uint32_t> queueFamilies{};
	if (presentationFamily != graphicFamily) {
		queueFamilies = { presentationFamily, graphicFamily };
	}

	vk::SwapchainCreateInfoKHR swapchainCreateInfo{
		/*.flags=*/ vk::SwapchainCreateFlagsKHR{},
		/*.surface=*/ surface,
		/*.minImageCount=*/ imageCount,
		/*.imageFormat=*/ surfaceFormat.format,
		/*.imageColorSpace=*/ surfaceFormat.colorSpace,
		/*.imageExtent=*/ surfaceExtent,
		/*.imageArrayLayers=*/ 1,
		/*.imageUsage=*/ vk::ImageUsageFlagBits::eColorAttachment, // eTransferDst
		/*.imageSharingMode=*/ queueFamilies.size() ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ static_cast<uint32_t>(queueFamilies.size()),
		/*.pQueueFamilyIndices=*/ queueFamilies.size() ? queueFamilies.data() : nullptr,
		/*.preTransform=*/ surfaceCapabilities.currentTransform,
		/*.compositeAlpha=*/ vk::CompositeAlphaFlagBitsKHR::eOpaque,
		/*.presentMode=*/ presentMode,
		/*.clipped=*/ VK_TRUE,
		/*.oldSwapchain=*/ vk::SwapchainKHR{}
	};

	vk::UniqueSwapchainKHR swapchainOwner{RN_VLK_TRACE(device.createSwapchainKHRUnique(swapchainCreateInfo))};

	// sync config
	ngn::config::core.window.width(surfaceExtent.width);
	ngn::config::core.window.height(surfaceExtent.height);
	ngn::config::core.window.imageBuffers(imageCount);
	ngn::config::core.window.vsync(toCore<rn::VSync>(presentMode));

	const rn::PixelFormat coreFormat = toCore<rn::PixelFormat>(surfaceFormat.format);
	if (coreFormat != rn::PixelFormat::Undefined) {
		ngn::config::core.window.surfaceFormat(coreFormat);
	} else {
		ngn::config::core.window.surfaceFormat(static_cast<uint32_t>(surfaceFormat.format));
	}

	const rn::ColorSpace coreColorSpace = toCore<rn::ColorSpace>(surfaceFormat.colorSpace);
	if (coreColorSpace != rn::ColorSpace::Undefined) {
		ngn::config::core.window.surfaceColorSpace(coreColorSpace);
	} else {
		ngn::config::core.window.surfaceColorSpace(static_cast<uint32_t>(surfaceFormat.colorSpace));
	}

	return swapchainOwner;
}

uint32_t SwapchainCreator::chooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities) {
	return std::max(capabilities.minImageCount, std::min(capabilities.maxImageCount, ngn::config::core.window.imageBuffers()));
}

vk::PresentModeKHR SwapchainCreator::choosePresentMode(const std::vector<vk::PresentModeKHR> &presentModes) {
	if (presentModes.empty()) {
		throw std::runtime_error{"Vulkan present modes not available"};
	}

	vk::PresentModeKHR desiredPresentMode = fromCore(ngn::config::core.window.vsync());

	for (const auto& presentMode : presentModes) {
		if (presentMode == desiredPresentMode) {
			return desiredPresentMode;
		}
	}

	return presentModes[0];
}

} // rn::vki
