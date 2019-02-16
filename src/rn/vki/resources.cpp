#include "resources.hpp"

#include "context.hpp"
#include "mapping.hpp"
#include "trace.hpp"

namespace rn::vki {

Resources::Resources(rn::vki::Context &context) :
	context(context)
{}

Resources::Resources(Resources &&other) :
	context(other.context)
{}

rn::TextureHandle Resources::createTexture(const rn::TextureDescription &description) {
	vk::ImageCreateFlags flags{};
	if (description.type == rn::ImageType::Tex2D && description.layers > 1) {
		flags |= vk::ImageCreateFlagBits::e2DArrayCompatible;

		if (description.layers == 6) {
			flags |= vk::ImageCreateFlagBits::eCubeCompatible;
		}
	}

	vk::ImageType imageType = rn::vki::fromCommon(description.type);
	vk::Format format = rn::vki::fromCommon(description.format);

	vk::ImageUsageFlags usage{};
	usage |= ((rn::TextureUsage::TransferSource & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eTransferSrc
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::TransferDestination & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eTransferDst
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::Sampled & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eSampled
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::Storage & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eStorage
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::ColorAttachment & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eColorAttachment
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::DepthStencilAttachment & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eDepthStencilAttachment
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::TransientAttachment & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eTransientAttachment
		: vk::ImageUsageFlags{};
	usage |= ((rn::TextureUsage::InputAttachment & description.usage) != rn::TextureUsage::None)
		? vk::ImageUsageFlagBits::eInputAttachment
		: vk::ImageUsageFlags{};

	vk::ImageCreateInfo imageCreateInfo{
			/*.flags=*/ flags,
			/*.imageType=*/ imageType,
			/*.format=*/ format,
			/*.extent=*/ { description.dimensions.width, description.dimensions.height, description.dimensions.depth },
			/*.mipLevels=*/ description.levels,
			/*.arrayLayers=*/ description.layers,
			/*.samples=*/ vk::SampleCountFlagBits::e1,
			/*.tiling=*/ vk::ImageTiling::eOptimal,
			/*.usage=*/ usage,
			/*.sharingMode=*/ vk::SharingMode::eExclusive,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/ nullptr,
			/*.initialLayout=*/ vk::ImageLayout::eUndefined,
	};
	rn::vki::UniqueImage imageUnique{
		RN_VKI_TRACE(context.device->createImageUnique(imageCreateInfo, nullptr, context.device.table())),
		context.device.table()
	};

	rn::vki::memory::Handle memory = context.allocator.texture.alloc(rn::vki::memory::Usage::GPU, imageUnique.get());

	for (size_t i = 0; i < imageSlots.size(); i++) {
		if ( ! imageSlots[i].image) {
			imageSlots[i] = { std::move(imageUnique), std::move(memory) };

			return rn::TextureHandle{ static_cast<rn::TextureHandle::InternalType>(i) };
		}
	}

	size_t index = imageSlots.size();
	imageSlots.push_back({ std::move(imageUnique), std::move(memory) });

	return rn::TextureHandle{ static_cast<rn::TextureHandle::InternalType>(index) };
}

bool Resources::destroyTexture(rn::TextureHandle handle) {
	if (handle == rn::end<rn::TextureHandle>() || handle == rn::swapchainHandle()) {
		return false;
	}

	size_t index = handle.index;
	if (index >= imageSlots.size()) {
		return false;
	}

	if ( ! imageSlots[index].image) {
		return false;
	}

	imageSlots[index].image.reset();
	imageSlots[index].memory.release();

	imageSlots[index] = {};

	return true;
}

rn::BufferHandle Resources::createBuffer(const rn::BufferDescription &description) {
	vk::BufferUsageFlags usage{};
	usage |= ((rn::BufferUsage::TransferSource & description.usage) != rn::BufferUsage::None)
		? vk::BufferUsageFlagBits::eTransferSrc
		: vk::BufferUsageFlags{};
	usage |= ((rn::BufferUsage::TransferDestination & description.usage) != rn::BufferUsage::None)
		? vk::BufferUsageFlagBits::eTransferDst
		: vk::BufferUsageFlags{};
	usage |= ((rn::BufferUsage::Index & description.usage) != rn::BufferUsage::None)
		? vk::BufferUsageFlagBits::eIndexBuffer
		: vk::BufferUsageFlags{};
	usage |= ((rn::BufferUsage::Vertex & description.usage) != rn::BufferUsage::None)
		? vk::BufferUsageFlagBits::eVertexBuffer
		: vk::BufferUsageFlags{};
	usage |= ((rn::BufferUsage::Uniform & description.usage) != rn::BufferUsage::None)
		? vk::BufferUsageFlagBits::eUniformBuffer
		: vk::BufferUsageFlags{};
	usage |= ((rn::BufferUsage::Storage & description.usage) != rn::BufferUsage::None)
		? vk::BufferUsageFlagBits::eStorageBuffer
		: vk::BufferUsageFlags{};

	vk::BufferCreateInfo bufferCreateInfo{
		/*.flags=*/ vk::BufferCreateFlags{},
		/*.size=*/ description.size * (description.paging == rn::BufferPaging::Single ? 1u : context.surfaceImageViews.size()),
		/*.usage=*/ usage,
		/*.sharingMode=*/ vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ 0,
		/*.pQueueFamilyIndices=*/ nullptr,
	};

	rn::vki::UniqueBuffer bufferUnique{
		RN_VKI_TRACE(context.device->createBufferUnique(bufferCreateInfo, nullptr, context.device.table())),
		context.device.table()
	};

	rn::vki::memory::Handle memory = context.allocator.buffer.alloc(rn::vki::memory::Usage::GPU, bufferUnique.get());

	for (size_t i = 0; i < bufferSlots.size(); i++) {
		if ( ! bufferSlots[i].buffer) {
			bufferSlots[i] = { std::move(bufferUnique), std::move(memory) };

			return rn::BufferHandle{ static_cast<rn::BufferHandle::InternalType>(i) };
		}
	}

	size_t index = bufferSlots.size();
	bufferSlots.push_back({ std::move(bufferUnique), std::move(memory) });

	return rn::BufferHandle{ static_cast<rn::BufferHandle::InternalType>(index) };
}

bool Resources::destroyBuffer(rn::BufferHandle handle) {
	if (handle == rn::end<rn::BufferHandle>()) {
		return false;
	}

	size_t index = handle.index;
	if (index >= bufferSlots.size()) {
		return false;
	}

	if ( ! bufferSlots[index].buffer) {
		return false;
	}

	bufferSlots[index].buffer.reset();
	bufferSlots[index].memory.release();

	bufferSlots[index] = {};

	return true;
}

Resources Resources::create(rn::Context<rn::vki::Context> &context) {
	return Resources{context.internal};
}

} // rn::vki