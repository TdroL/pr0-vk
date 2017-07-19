#pragma once

#include <vector>
#include <cassert>

#include <vulkan/vulkan.hpp>

#include "../../../ngn/config.hpp"

#include "../physicalDeviceHandle.hpp"

namespace rn {

namespace vlk {

class SurfaceFormatSelector {
public:
	vk::SurfaceFormatKHR select(vk::UniqueSurfaceKHR &surfaceOwner, PhysicalDeviceHandle &physicalDeviceOwner) {
		vk::SurfaceKHR surface = surfaceOwner.get();
		vk::PhysicalDevice physicalDevice = physicalDeviceOwner.handle;

		assert(surface);
		assert(physicalDevice);

		std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);

		if (surfaceFormats.empty()) {
			throw std::runtime_error{"Vulkan surface formats not available"};
		}

		vk::SurfaceFormatKHR desiredSurfaceFormat{
			fromConfig(ngn::config::core.window.surfaceFormat()),
			fromConfig(ngn::config::core.window.surfaceColorSpace())
		};

		if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined) {
			return desiredSurfaceFormat;
		}

		for (const auto& surfaceFormat : surfaceFormats) {
			if (surfaceFormat.format == desiredSurfaceFormat.format && surfaceFormat.colorSpace == desiredSurfaceFormat.colorSpace) {
				return desiredSurfaceFormat;
			}
		}

		return surfaceFormats[0];
	}
};

} // vlk

} // rn
