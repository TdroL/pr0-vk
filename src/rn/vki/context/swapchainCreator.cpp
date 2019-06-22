#include "swapchainCreator.hpp"

#include <algorithm>

#include "../../../ngn/config.hpp"
#include "../../../ngn/log.hpp"
#include "../mapping.hpp"
#include "../trace.hpp"
#include "types.hpp"

namespace rn::vki::context {

uint32_t chooseSwapchainSize(const vk::SurfaceCapabilitiesKHR& capabilities, uint32_t desiredSwapchainSize) {
	return std::max(capabilities.minImageCount, std::min(capabilities.maxImageCount, desiredSwapchainSize));
}

vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &presentModes, vk::PresentModeKHR desiredPresentMode) {
	if (presentModes.empty()) {
		return vk::PresentModeKHR::eFifo;
	}

	const auto foundPresentMode = std::find_if(std::begin(presentModes), std::end(presentModes), [=] (const auto &presentMode) {
		return presentMode == desiredPresentMode;
	});
	if (foundPresentMode != std::end(presentModes)) {
		return *foundPresentMode;
	} else {
		return presentModes[0];
	}
}

rn::vki::UniqueSwapchainKHR SwapchainCreator::create(rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device, rn::vki::HandlePhysicalDevice &&physicalDevice, rn::vki::context::QueueFamilyIndex &queueFamilyIndex, rn::vki::HandleSurfaceKHR &&surface, ngn::config::Config &config) {
	vk::SurfaceFormatKHR surfaceFormat = surfaceDescription.format;
	vk::Extent2D surfaceExtent = surfaceDescription.extent;

	uint32_t presentationFamily = queueFamilyIndex.presentation.family;
	uint32_t graphicFamily = queueFamilyIndex.graphic.family;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = RN_VKI_TRACE(physicalDevice->getSurfaceCapabilitiesKHR(surface.get(), physicalDevice.table()));
	std::vector<vk::PresentModeKHR> presentModes = RN_VKI_TRACE(physicalDevice->getSurfacePresentModesKHR(surface.get(), physicalDevice.table()));

	uint32_t imageCount = chooseSwapchainSize(surfaceCapabilities, config.core.window.swapchainSize);
	vk::PresentModeKHR presentMode = choosePresentMode(presentModes, fromCommon(config.core.window.vsync));

	std::vector<uint32_t> queueFamilies{};
	if (presentationFamily != graphicFamily) {
		queueFamilies = { presentationFamily, graphicFamily };
	}

	vk::SwapchainCreateInfoKHR swapchainCreateInfo{
		/*.flags=*/ vk::SwapchainCreateFlagsKHR{},
		/*.surface=*/ surface.get(),
		/*.minImageCount=*/ imageCount,
		/*.imageFormat=*/ surfaceFormat.format,
		/*.imageColorSpace=*/ surfaceFormat.colorSpace,
		/*.imageExtent=*/ surfaceExtent,
		/*.imageArrayLayers=*/ 1,
		/*.imageUsage=*/ vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
		/*.imageSharingMode=*/ queueFamilies.size() ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ static_cast<uint32_t>(queueFamilies.size()),
		/*.pQueueFamilyIndices=*/ queueFamilies.size() ? queueFamilies.data() : nullptr,
		/*.preTransform=*/ surfaceCapabilities.currentTransform,
		/*.compositeAlpha=*/ vk::CompositeAlphaFlagBitsKHR::eOpaque,
		/*.presentMode=*/ presentMode,
		/*.clipped=*/ VK_TRUE,
		/*.oldSwapchain=*/ vk::SwapchainKHR{}
	};

	rn::vki::UniqueSwapchainKHR swapchainOwner = {
		RN_VKI_TRACE(device->createSwapchainKHRUnique(swapchainCreateInfo, nullptr, device.table())),
		device.table()
	};

	auto windowCopy = config.core.window;
	windowCopy.width = surfaceExtent.width;
	windowCopy.height = surfaceExtent.height;
	windowCopy.swapchainSize = imageCount;
	windowCopy.vsync = toCommon<rn::VSync>(presentMode);

	const rn::PixelFormat coreFormat = toCommon<rn::PixelFormat>(surfaceFormat.format);
	if (coreFormat != rn::PixelFormat::Undefined) {
		windowCopy.surfaceFormat = coreFormat;
	} else {
		windowCopy.surfaceFormat = static_cast<uint32_t>(surfaceFormat.format);
	}

	const rn::ColorSpace coreColorSpace = toCommon<rn::ColorSpace>(surfaceFormat.colorSpace);
	if (coreColorSpace != rn::ColorSpace::Undefined) {
		windowCopy.surfaceColorSpace = coreColorSpace;
	} else {
		windowCopy.surfaceColorSpace = static_cast<uint32_t>(surfaceFormat.colorSpace);
	}

	// sync config
	if (config.core.window != windowCopy) {
		config.core.window = windowCopy;
		config.core.dirty = true;
	}

	return swapchainOwner;
}

} // rn::vki::context
