#include "resources.hpp"

#include <algorithm>

#include "../../ngn/log.hpp"
#include "context.hpp"
#include "mapping.hpp"
#include "trace.hpp"

namespace rn::vki {

Resources::Resources(rn::vki::Context &implementation) :
	context(implementation)
{}

Resources::Resources(Resources &&other) :
	context{std::move(other.context)},
	imageSlots{std::move(other.imageSlots)},
	bufferSlots{std::move(other.bufferSlots)}
{}

Resources & Resources::operator=(Resources &&other) {
	context = std::move(other.context);
	imageSlots = std::move(other.imageSlots);
	bufferSlots = std::move(other.bufferSlots);

	return *this;
}

rn::TextureHandle Resources::createTexture(const rn::TextureDescription &description) {
	vk::ImageCreateFlags flags{};
	vk::Extent3D extent{};

	switch (description.type) {
		case rn::ImageType::Tex1D: {
			extent.width = std::max(1u, description.dimensions.width);
			extent.height = 1u;
			extent.depth = 1u;
			break;
		}
		case rn::ImageType::Tex2D: {
			extent.width = std::max(1u, description.dimensions.width);
			extent.height = std::max(1u, description.dimensions.height);
			extent.depth = 1u;

			if (description.layers > 1u) {
				flags |= vk::ImageCreateFlagBits::e2DArrayCompatible;

				if (description.layers == 6u) {
					flags |= vk::ImageCreateFlagBits::eCubeCompatible;
				}
			}

			break;
		}
		case rn::ImageType::Tex3D: {
			extent.width = std::max(1u, description.dimensions.width);
			extent.height = std::max(1u, description.dimensions.height);
			extent.depth = std::max(1u, description.dimensions.depth);

			break;
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
			/*.extent=*/ extent,
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
	ngn::log::debug("rn::vki::Resources::createTexture():\n" \
			"\t.flags={}\n" \
			"\t.imageType={}\n" \
			"\t.format={}\n" \
			"\t.extent={}, {}, {}\n" \
			"\t.mipLevels={}\n" \
			"\t.arrayLayers={}\n" \
			"\t.samples={}\n" \
			"\t.tiling={}\n" \
			"\t.usage={}\n" \
			"\t.sharingMode={}\n" \
			"\t.queueFamilyIndexCount={}\n" \
			"\t.initialLayout={}",
		vk::to_string(imageCreateInfo.flags),
		vk::to_string(imageCreateInfo.imageType),
		vk::to_string(imageCreateInfo.format),
		imageCreateInfo.extent.width, imageCreateInfo.extent.height, imageCreateInfo.extent.depth,
		imageCreateInfo.mipLevels,
		imageCreateInfo.arrayLayers,
		vk::to_string(imageCreateInfo.samples),
		vk::to_string(imageCreateInfo.tiling),
		vk::to_string(imageCreateInfo.usage),
		vk::to_string(imageCreateInfo.sharingMode),
		imageCreateInfo.queueFamilyIndexCount,
		vk::to_string(imageCreateInfo.initialLayout)
	);
	rn::vki::UniqueImage imageUnique{
		RN_VKI_TRACE(context->device->createImageUnique(imageCreateInfo, nullptr, context->device.table())),
		context->device.table()
	};

	rn::vki::memory::Handle memory = context->allocator.texture.alloc(rn::vki::memory::Usage::GPU, imageUnique.get());

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
		/*.size=*/ description.size * (description.paging == rn::BufferPaging::Single ? 1u : context->surfaceImageViews.size()),
		/*.usage=*/ usage,
		/*.sharingMode=*/ vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ 0,
		/*.pQueueFamilyIndices=*/ nullptr,
	};
	ngn::log::debug("rn::vki::Resources::createBuffer():\n" \
			"\t.flags={}\n" \
			"\t.size={}\n" \
			"\t.usage={}\n" \
			"\t.sharingMode={}\n" \
			"\t.queueFamilyIndexCount={}",
		vk::to_string(bufferCreateInfo.flags),
		bufferCreateInfo.size,
		vk::to_string(bufferCreateInfo.usage),
		vk::to_string(bufferCreateInfo.sharingMode),
		bufferCreateInfo.queueFamilyIndexCount
	);
	rn::vki::UniqueBuffer bufferUnique{
		RN_VKI_TRACE(context->device->createBufferUnique(bufferCreateInfo, nullptr, context->device.table())),
		context->device.table()
	};

	rn::vki::memory::Handle memory = context->allocator.buffer.alloc(rn::vki::memory::Usage::GPU, bufferUnique.get());

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

rn::vki::Resources Resources::create(rn::Context<rn::vki::Context> &context) {
	return rn::vki::Resources{context.internal};
}

} // rn::vki
