#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../memory/pool.hpp"
#include "../context.hpp"

namespace rn::vlk {

class AllocatorCreator {
public:
	Context::Allocator create(Context &context) {
		vk::PhysicalDeviceMemoryProperties &memoryProperties = context.physicalDevice.memoryProperties;
		vk::Device device = context.device;
		std::vector<vk::Image> &images = context.surface.images;

		assert(device);
		assert(images.size() > 0);

		// find leaf size for frame-sized buffers (frame buffers)
		vk::MemoryRequirements frameReq = device.getImageMemoryRequirements(images[0]);
		vk::DeviceSize frameLeafSize = ((frameReq.size + frameReq.alignment - 1) / frameReq.alignment) * frameReq.alignment;

		// find leaf size for square-sized buffers (shadow maps)
		vk::UniqueImage squareImage = device.createImageUnique({
				/*.flags=*/                 vk::ImageCreateFlags{},
				/*.imageType=*/             vk::ImageType::e2D,
				/*.format=*/                vk::Format::eR8G8B8A8Unorm,
				/*.extent=*/                vk::Extent3D{1024, 1024, 1},
				/*.mipLevels=*/             1,
				/*.arrayLayers=*/           1,
				/*.samples=*/               vk::SampleCountFlagBits::e1,
				/*.tiling=*/                vk::ImageTiling::eOptimal,
				/*.usage=*/                 vk::ImageUsageFlagBits::eColorAttachment,
				/*.sharingMode=*/           vk::SharingMode::eExclusive,
				/*.queueFamilyIndexCount=*/ 0,
				/*.pQueueFamilyIndices=*/   nullptr,
				/*.initialLayout=*/         vk::ImageLayout::eUndefined
		});

		vk::MemoryRequirements squareReq = device.getImageMemoryRequirements(squareImage.get());
		vk::DeviceSize squareLeafSize = ((squareReq.size + squareReq.alignment - 1) / squareReq.alignment) * squareReq.alignment;

		return {
			/*.frameRT=*/  memory::Pool{device, memoryProperties, 4, frameLeafSize * 16},
			/*.squareRT=*/ memory::Pool{device, memoryProperties, 4, squareLeafSize * 16},
			/*.small=*/    memory::Pool{device, memoryProperties, 8, 64 * 1024 * 1024},
			/*.huge=*/     memory::Pool{device, memoryProperties, 8, 256 * 1024 * 1024},
		};
	}
};

} // rn::vlk
