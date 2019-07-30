#include "context.hpp"

#include <cstring>
#include <random>

#include "../../ngn/config.hpp"
#include "../../ngn/log.hpp"
#include "../../ngn/prof.hpp"
#include "../../util/map.hpp"
#include "../window.hpp"
#include "id.hpp"
#include "mapping.hpp"
#include "recorder.hpp"
#include "trace.hpp"

#include "context/instanceCreator.hpp"
#include "context/surfaceCreator.hpp"
#include "context/debugCreator.hpp"
#include "context/deviceCreator.hpp"
#include "context/swapchainCreator.hpp"
#include "context/surfaceImageViewsCreator.hpp"
#include "context/allocatorCreator.hpp"
#include "context/commandPoolGroupsCreator.hpp"
#include "context/synchronizationCreator.hpp"

#include "context/physicalDeviceSelector.hpp"
#include "context/queueFamilyIndexSelector.hpp"
#include "context/queueSelector.hpp"
#include "context/surfaceExtentSelector.hpp"
#include "context/surfaceFormatSelector.hpp"
#include "context/surfaceImagesSelector.hpp"

std::random_device rd;
std::uniform_int_distribution<uint8_t> dist(0, 255);

namespace rn::vki {

rn::vki::Context Context::factory(ngn::config::Config &config, rn::Window &window) {
	struct Creators {
		rn::vki::context::InstanceCreator instance{};
		rn::vki::context::DebugCreator debug{};
		rn::vki::context::SurfaceCreator surface{};
		rn::vki::context::DeviceCreator device{};
		rn::vki::context::SwapchainCreator swapchain{};
		rn::vki::context::SurfaceImageViewsCreator surfaceImageViews{};
		rn::vki::context::AllocatorCreator allocator{};
		rn::vki::context::CommandPoolGroupsCreator commandPoolGroups{};
		rn::vki::context::SynchronizationCreator synchronization{};
	} creators{};

	struct Selectors {
		rn::vki::context::PhysicalDeviceSelector physicalDevice{};
		rn::vki::context::QueueFamilyIndexSelector queueFamilyIndex{};
		rn::vki::context::QueueSelector queue{};
		rn::vki::context::SurfaceExtentSelector surfaceExtent{};
		rn::vki::context::SurfaceFormatSelector surfaceFormat{};
		rn::vki::context::SurfaceImagesSelector surfaceImages{};
	} selectors{};

	rn::vki::Context context{};

	context.instance = creators.instance.create(config);

	context.debug = creators.debug.create(context.instance.handle(), config);

	context.surface = creators.surface.create(context.instance.handle(), window);

	std::tie(context.physicalDevice, context.physicalDeviceDescription) = selectors.physicalDevice.select(context.surface.handle(), context.instance.handle(), config);

	context.queueFamilyIndex = selectors.queueFamilyIndex.select(context.physicalDevice.handle(), context.physicalDeviceDescription, context.surface.handle(), config);

	context.device = creators.device.create(context.physicalDevice.handle(), context.physicalDeviceDescription, context.queueFamilyIndex, context.instance.handle());

	context.queue = selectors.queue.select(context.device.handle(), context.queueFamilyIndex);

	context.surfaceDescription.extent = selectors.surfaceExtent.select(context.physicalDevice.handle(), context.surface.handle(), config);
	context.surfaceDescription.format = selectors.surfaceFormat.select(context.physicalDevice.handle(), context.surface.handle(), config);

	context.swapchain = creators.swapchain.create(context.surfaceDescription, context.device.handle(), context.physicalDevice.handle(), context.queueFamilyIndex, context.surface.handle(), config);

	context.surfaceDescription.images = selectors.surfaceImages.select(context.swapchain.handle(), context.device.handle());

	context.surfaceImageViews = creators.surfaceImageViews.create(context.surfaceDescription, context.device.handle(), config);
	context.surfaceDescription.imageViews = util::map(context.surfaceImageViews, [] (auto &imageView) {
		return imageView.get();
	});

	context.allocator = creators.allocator.create(context.device.handle(), context.physicalDeviceDescription);

	context.commandPoolGroups = creators.commandPoolGroups.create(context.surfaceDescription, config);

	context.synchronization = creators.synchronization.create(context.surfaceDescription, context.device.handle());

	return context;
}

Context::Context(Context &&other) {
	*this = std::move(other);
}

Context & Context::operator=(Context &&other) {
	reset();

	// move values in reverse order
	transferCommandLists = std::move(other.transferCommandLists);
	// computeCommandLists = std::move(other.computeCommandLists);
	// graphicCommandLists = std::move(other.graphicCommandLists);
	pendingFenceStamps = std::move(other.pendingFenceStamps);
	resolvedFenceStamps = std::move(other.resolvedFenceStamps);
	bufferSlots = std::move(other.bufferSlots);
	textureSlots = std::move(other.textureSlots);
	synchronization = std::move(other.synchronization);
	commandPoolGroups = std::move(other.commandPoolGroups);
	allocator = std::move(other.allocator);
	surfaceDescription = std::move(other.surfaceDescription);
	queue = std::move(other.queue);
	queueFamilyIndex = std::move(other.queueFamilyIndex);
	physicalDeviceDescription = std::move(other.physicalDeviceDescription);
	surfaceImageViews = std::move(other.surfaceImageViews);
	swapchain = std::move(other.swapchain);
	device = std::move(other.device);
	surface = std::move(other.surface);
	debug = std::move(other.debug);
	physicalDevice = std::move(other.physicalDevice);
	instance = std::move(other.instance);

	return *this;
}

Context::~Context() {
	reset();
}

util::Span<vk::CommandPool> Context::getCommandPools([[maybe_unused]] rn::QueueType queueType, [[maybe_unused]] uint32_t count) {
	rn::vki::context::CommandPoolGroups::Group *group = nullptr;
	uint32_t family = 0;

	switch (queueType) {
		case rn::QueueType::Transfer: {
			size_t transferIndex = synchronization.resolvedIndex;
			assert(transferIndex < commandPoolGroups.transferGroup.size());

			group = &(commandPoolGroups.transferGroup[transferIndex]);
			family = queueFamilyIndex.transfer.family;
			break;
		}
		case rn::QueueType::Compute: {
			size_t renderingIndex = synchronization.renderingIndex;
			assert(renderingIndex < commandPoolGroups.computeGroup.size());

			group = &(commandPoolGroups.computeGroup[renderingIndex]);
			family = queueFamilyIndex.compute.family;
			break;
		}
		case rn::QueueType::Graphic: [[fallthrough]];
		default: {
			size_t renderingIndex = synchronization.renderingIndex;
			assert(renderingIndex < commandPoolGroups.graphicGroup.size());

			group = &(commandPoolGroups.graphicGroup[renderingIndex]);
			family = queueFamilyIndex.graphic.family;
			break;
		}
	}

	assert(group != nullptr);

	uint32_t size = static_cast<uint32_t>(group->owners.size());
	if (size < count) {
		group->owners.resize(count);
		group->handles.resize(count);

		for (uint32_t i = size; i < count; i++) {
			group->owners[i] = {
				RN_VKI_TRACE(device->createCommandPoolUnique({
					/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
					/*.queueFamilyIndex=*/ family
				}, nullptr, device.table())),
				device.table()
			};

			group->handles[i] = group->owners[i].get();
		}
	}

	assert(count <= group->handles.size());

	return { group->handles.data(), count };
}

void Context::advance() {
	size_t renderingIndex = synchronization.renderingIndex;

	{
		NGN_PROF_SCOPE("context advance: resolve rendering (graphic, compute)");

		size_t renderingCount = synchronization.renderingResources.size();

		renderingIndex = (renderingIndex + 1) % renderingCount;

		std::array<vk::Fence, 2> renderingFences{
			synchronization.renderingResources[renderingIndex].graphicFence.get(),
			synchronization.renderingResources[renderingIndex].computeFence.get()
		};

		RN_VKI_TRACE(device->waitForFences(renderingFences, true, std::numeric_limits<uint64_t>::max(), device.table()));
		RN_VKI_TRACE(device->resetFences(renderingFences, device.table()));

		synchronization.renderingIndex = renderingIndex;
		synchronization.renderingCounter++;

		if (synchronization.renderingCounter > renderingCount) {
			resolvedFenceStamps[static_cast<uint32_t>(rn::QueueType::Graphic) - 1] = synchronization.renderingCounter - renderingCount;
			resolvedFenceStamps[static_cast<uint32_t>(rn::QueueType::Compute) - 1] = synchronization.renderingCounter - renderingCount;
		}
	}

	{
		NGN_PROF_SCOPE("context advance: resolve transfer");
		size_t transferIndex = synchronization.resolvedIndex;
		size_t transferCounter = synchronization.resolvedCounter;
		size_t transferCount = synchronization.transferFences.size();

		std::vector<vk::Fence> resetFences{};
		resetFences.reserve(synchronization.pendingCounter - synchronization.resolvedCounter);

		while (transferCounter < synchronization.pendingCounter && RN_VKI_TRACE(device->getFenceStatus(synchronization.transferFences[transferIndex].get(), device.table())) == vk::Result::eSuccess) {
			resetFences.push_back(synchronization.transferFences[transferIndex].get());
			transferIndex = (transferIndex + 1) % transferCount;
			transferCounter++;
		}

		if ( ! resetFences.empty()) {
			// ngn::log::debug("transfer: resetFences={} resolvedCounter={} transferCounter={}", resetFences.size(), synchronization.resolvedCounter, transferCounter);
			RN_VKI_TRACE(device->resetFences(resetFences, device.table()));

			synchronization.resolvedIndex = transferIndex;
			synchronization.resolvedCounter = transferCounter;
			resolvedFenceStamps[static_cast<uint32_t>(rn::QueueType::Transfer) - 1] = synchronization.resolvedCounter;
		} else {
			// ngn::log::debug("transfer: resetFences={}", resetFences.size());
		}
	}

	{
		NGN_PROF_SCOPE("context advance: acquire next image");

		auto imageIndexResult = RN_VKI_TRACE(device->acquireNextImage2KHR({
			/*.swapchain=*/ swapchain.get(),
			/*.timeout=*/ std::numeric_limits<uint64_t>::max(),
			/*.semaphore=*/ synchronization.renderingResources[renderingIndex].acquireSemaphore.get() ,
			/*.fence=*/ vk::Fence{},
			/*.deviceMask=*/ 0x0001 // A device mask value is valid if every bit that is set in the mask is at a bit position that is less than the number of physical devices in the logical device.
		}, device.table()));

		surfaceDescription.imageIndex = imageIndexResult.value;
	}
}

void Context::commit(util::ThreadPool &threadPool) {
	rn::vki::Recorder recorder{*this, threadPool};

	{
		NGN_PROF_SCOPE("context commit: submit transfer commands");

		if ( ! transferCommandLists.empty()) {
			rn::vki::HandleCommandBuffer transferCommandBuffer = recorder.record(std::move(transferCommandLists));
			transferCommandLists = {};

			size_t transferIndex = synchronization.pendingIndex;
			size_t transferCount = synchronization.transferFences.size();

			if (RN_VKI_TRACE(device->getFenceStatus(synchronization.transferFences[transferIndex].get(), device.table())) != vk::Result::eSuccess) {
				synchronization.transferFences.insert(std::next(std::begin(synchronization.transferFences), transferIndex), {
					RN_VKI_TRACE(device->createFenceUnique({ vk::FenceCreateFlagBits::eSignaled }, nullptr, device.table())),
					device.table()
				});

				transferIndex = (transferIndex + 1) % transferCount;
			}

			RN_VKI_TRACE(device->resetFences({
				synchronization.transferFences[transferIndex].get()
			}, device.table()));

			RN_VKI_TRACE(queue.transfer.submit({
				{
					/*.waitSemaphoreCount=*/ 0,
					/*.pWaitSemaphores=*/ nullptr,
					/*.pWaitDstStageMask=*/ nullptr,
					/*.commandBufferCount=*/ 1,
					/*.pCommandBuffers=*/ &transferCommandBuffer.get(),
					/*.signalSemaphoreCount=*/ 0,
					/*.pSignalSemaphores=*/ nullptr,
				}
			}, synchronization.transferFences[transferIndex].get(), device.table()));

			pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Transfer) - 1]++;

			synchronization.pendingIndex = transferIndex;
			synchronization.pendingCounter++;
		}
	}

	{
		NGN_PROF_SCOPE("context commit: submit compute commands");

		// vk::CommandBuffer computeCommandBuffer = recorder.record(std::move(computeCommandLists));
		// computeCommandLists = {};

		// DEV: do something so that validation is happy
		rn::vki::HandleCommandBuffer computeCommandBufferHandle{};
		{
			vk::CommandPool commandPool = getCommandPools(rn::QueueType::Compute, 1)[0];

			vk::CommandBufferAllocateInfo commandBufferAllocateInfo{
				/*.commandPool=*/ commandPool,
				/*.level=*/ vk::CommandBufferLevel::ePrimary,
				/*.commandBufferCount=*/ 1,
			};
			vk::CommandBuffer commandBuffer{};
			RN_VKI_TRACE(device->allocateCommandBuffers(&commandBufferAllocateInfo, &commandBuffer, device.table()));

			computeCommandBufferHandle = rn::vki::HandleCommandBuffer{commandBuffer, device.table()};

			RN_VKI_TRACE(computeCommandBufferHandle->begin({
				/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
				/*.pInheritanceInfo=*/ nullptr
			}, computeCommandBufferHandle.table()));
			RN_VKI_TRACE(computeCommandBufferHandle->end(computeCommandBufferHandle.table()));
		}

		size_t renderingIndex = synchronization.renderingIndex;

		RN_VKI_TRACE(queue.compute.submit({
			{
				/*.waitSemaphoreCount=*/ 0,
				/*.pWaitSemaphores=*/ nullptr,
				/*.pWaitDstStageMask=*/ nullptr,
				/*.commandBufferCount=*/ 1,
				/*.pCommandBuffers=*/ &computeCommandBufferHandle.get(),
				/*.signalSemaphoreCount=*/ 0,
				/*.pSignalSemaphores=*/ nullptr,
			}
		}, synchronization.renderingResources[renderingIndex].computeFence.get(), device.table()));

		pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Compute) - 1]++;
	}

	{
		NGN_PROF_SCOPE("context commit: submit graphic commands");

		// vk::CommandBuffer graphicCommandBuffer = recorder.record(std::move(graphicCommandLists));
		// graphicCommandLists = {};
		rn::vki::HandleCommandBuffer graphicCommandBufferHandle{};
		{
			vk::CommandPool commandPool = getCommandPools(rn::QueueType::Graphic, 1)[0];

			vk::CommandBufferAllocateInfo commandBufferAllocateInfo{
				/*.commandPool=*/ commandPool,
				/*.level=*/ vk::CommandBufferLevel::ePrimary,
				/*.commandBufferCount=*/ 1,
			};
			vk::CommandBuffer commandBuffer{};
			RN_VKI_TRACE(device->allocateCommandBuffers(&commandBufferAllocateInfo, &commandBuffer, device.table()));

			graphicCommandBufferHandle = rn::vki::HandleCommandBuffer{commandBuffer, device.table()};

			RN_VKI_TRACE(graphicCommandBufferHandle->begin({
				/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
				/*.pInheritanceInfo=*/ nullptr
			}, graphicCommandBufferHandle.table()));
			{
				vk::ImageMemoryBarrier imageMemoryBarrier{
					/*.srcAccessMask=*/ vk::AccessFlags{},
					/*.dstAccessMask=*/ vk::AccessFlagBits::eTransferWrite,
					/*.oldLayout=*/ vk::ImageLayout::eUndefined,
					/*.newLayout=*/ vk::ImageLayout::eTransferDstOptimal,
					/*.srcQueueFamilyIndex=*/ queueFamilyIndex.presentation.family,
					/*.dstQueueFamilyIndex=*/ queueFamilyIndex.presentation.family,
					/*.image=*/ surfaceDescription.images[surfaceDescription.imageIndex],
					/*.subresourceRange=*/ vk::ImageSubresourceRange{
						/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
						/*.baseMipLevel=*/ 0,
						/*.levelCount=*/ 1,
						/*.baseArrayLayer=*/ 0,
						/*.layerCount=*/ 1,
					},
				};
				RN_VKI_TRACE(graphicCommandBufferHandle->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags{}, {}, {}, { imageMemoryBarrier }, graphicCommandBufferHandle.table()));
			}
			{
				vk::ClearColorValue color{ std::array<float,4>{ dist(rd) / 256.f, dist(rd) / 256.f, dist(rd) / 256.f, 1.f } };
				vk::ImageSubresourceRange range{
					/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
					/*.baseMipLevel=*/ 0,
					/*.levelCount=*/ 1,
					/*.baseArrayLayer=*/ 0,
					/*.layerCount=*/ 1,
				};
				RN_VKI_TRACE(graphicCommandBufferHandle->clearColorImage(surfaceDescription.images[surfaceDescription.imageIndex], vk::ImageLayout::eTransferDstOptimal, color, { range }, graphicCommandBufferHandle.table()));
			}
			{
				vk::ImageMemoryBarrier imageMemoryBarrier{
					/*.srcAccessMask=*/ vk::AccessFlagBits::eTransferWrite,
					/*.dstAccessMask=*/ vk::AccessFlags{},
					/*.oldLayout=*/ vk::ImageLayout::eTransferDstOptimal,
					/*.newLayout=*/ vk::ImageLayout::ePresentSrcKHR,
					/*.srcQueueFamilyIndex=*/ queueFamilyIndex.presentation.family,
					/*.dstQueueFamilyIndex=*/ queueFamilyIndex.presentation.family,
					/*.image=*/ surfaceDescription.images[surfaceDescription.imageIndex],
					/*.subresourceRange=*/ vk::ImageSubresourceRange{
						/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
						/*.baseMipLevel=*/ 0,
						/*.levelCount=*/ 1,
						/*.baseArrayLayer=*/ 0,
						/*.layerCount=*/ 1,
					},
				};
				graphicCommandBufferHandle->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlags{}, {}, {}, { imageMemoryBarrier }, graphicCommandBufferHandle.table());
			}
			RN_VKI_TRACE(graphicCommandBufferHandle->end(graphicCommandBufferHandle.table()));
		}

		size_t renderingIndex = synchronization.renderingIndex;

		vk::Semaphore acquireSemaphore = synchronization.renderingResources[renderingIndex].acquireSemaphore.get();
		vk::Semaphore graphicSemaphore = synchronization.renderingResources[renderingIndex].graphicSemaphores[0].get();
		vk::PipelineStageFlags acquireStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		RN_VKI_TRACE(queue.graphic.submit({
			{
				/*.waitSemaphoreCount=*/ 1,
				/*.pWaitSemaphores=*/ &acquireSemaphore,
				/*.pWaitDstStageMask=*/ &acquireStageMask,
				/*.commandBufferCount=*/ 1,
				/*.pCommandBuffers=*/ &graphicCommandBufferHandle.get(),
				/*.signalSemaphoreCount=*/ 1,
				/*.pSignalSemaphores=*/ &graphicSemaphore,
			}
		}, synchronization.renderingResources[renderingIndex].graphicFence.get(), device.table()));
		pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Graphic) - 1]++;
	}

	{
		NGN_PROF_SCOPE("context commit: present");

		size_t renderingIndex = synchronization.renderingIndex;

		vk::Semaphore graphicSemaphore = synchronization.renderingResources[renderingIndex].graphicSemaphores[0].get();

		RN_VKI_TRACE(queue.presentation.presentKHR({
			/*.waitSemaphoreCount=*/ 1,
			/*.pWaitSemaphores=*/ &graphicSemaphore,
			/*.swapchainCount=*/ 1,
			/*.pSwapchains=*/ &swapchain.get(),
			/*.pImageIndices=*/ &surfaceDescription.imageIndex,
			/*.pResults=*/ nullptr
		}, device.table()));
	}

	{
		size_t renderingIndex = synchronization.renderingIndex;
		size_t renderingCount = synchronization.renderingResources.size();

		// I have no idea how to check if semaphore from graphic queue submit was reset by vkPresentKHR... so use double-buffer style semaphore swapping
		std::swap(synchronization.renderingResources[renderingIndex].graphicSemaphores[0], synchronization.renderingResources[renderingIndex].graphicSemaphores[1]);

		synchronization.renderingIndex = (renderingIndex + 1) % renderingCount;
		synchronization.renderingCounter++;
	}
}

rn::TextureHandle Context::createTexture(const rn::TextureDescription &description) {
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
	usage |= ((rn::TextureUsage::OutputAttachment & description.usage) != rn::TextureUsage::None)
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

	// ngn::log::debug("rn::vki::Context::createTexture():\n\t.flags={}\n\t.imageType={}\n\t.format={}\n\t.extent={}, {}, {}\n\t.mipLevels={}\n\t.arrayLayers={}\n\t.samples={}\n\t.tiling={}\n\t.usage={}\n\t.sharingMode={}\n\t.queueFamilyIndexCount={}\n\t.initialLayout={}",
	// 	vk::to_string(imageCreateInfo.flags),
	// 	vk::to_string(imageCreateInfo.imageType),
	// 	vk::to_string(imageCreateInfo.format),
	// 	imageCreateInfo.extent.width, imageCreateInfo.extent.height, imageCreateInfo.extent.depth,
	// 	imageCreateInfo.mipLevels,
	// 	imageCreateInfo.arrayLayers,
	// 	vk::to_string(imageCreateInfo.samples),
	// 	vk::to_string(imageCreateInfo.tiling),
	// 	vk::to_string(imageCreateInfo.usage),
	// 	vk::to_string(imageCreateInfo.sharingMode),
	// 	imageCreateInfo.queueFamilyIndexCount,
	// 	vk::to_string(imageCreateInfo.initialLayout)
	// );
	rn::vki::UniqueImage imageUnique{
		RN_VKI_TRACE(device->createImageUnique(imageCreateInfo, nullptr, device.table())),
		device.table()
	};

	rn::vki::memory::Handle memory = allocator.texture.alloc(rn::vki::memory::Usage::GPU, imageUnique.get());

	RN_VKI_TRACE(device->bindImageMemory(imageUnique.get(), memory.memory.get(), memory.offset, device.table()));

	for (size_t i = 0; i < textureSlots.size(); i++) {
		if ( ! textureSlots[i].image) {
			textureSlots[i] = { std::move(description), std::move(imageUnique), std::move(memory) };

			return rn::TextureHandle{ static_cast<rn::TextureHandle::InternalType>(i) };
		}
	}

	size_t index = textureSlots.size();
	textureSlots.push_back({ std::move(description), std::move(imageUnique), std::move(memory) });

	return rn::TextureHandle{ static_cast<rn::TextureHandle::InternalType>(index) };
}

// rn::FenceHandle Context::enqueueTextureUpload(rn::TextureHandle handle, rn::DataFormat dataFormat, std::byte *data)  {
// 	size_t index = handle.index;
// 	if (index >= textureSlots.size()) {
// 		return rn::end<rn::FenceHandle>();
// 	}

// 	if ( ! textureSlots[index].image) {
// 		return rn::end<rn::FenceHandle>();
// 	}

// 	if (data == nullptr) {
// 		return rn::end<rn::FenceHandle>();
// 	}

// 	auto dimensions = textureSlots[index].description.dimensions;
// 	size_t size = rn::sizeOf(dataFormat) * dimensions.width * dimensions.height * dimensions.depth * textureSlots[index].description.layers * textureSlots[index].description.levels;

// 	rn::BufferDescription descriptionStaging{
// 		/*.size=*/ size,
// 		/*.usage*/ rn::BufferUsage::TransferSource,
// 		/*.paging*/ rn::BufferPaging::Static,
// 	};

// 	// createBuffer(const rn::BufferDescription &description)
// 	return rn::end<rn::FenceHandle>();
// }

bool Context::destroyTexture(rn::TextureHandle handle) {
	if (handle == rn::end<rn::TextureHandle>() || handle == rn::swapchainHandle()) {
		return false;
	}

	size_t index = handle.index;
	if (index >= textureSlots.size()) {
		return false;
	}

	if ( ! textureSlots[index].image) {
		return false;
	}

	textureSlots[index].image.reset();
	textureSlots[index].memory.release();

	textureSlots[index] = {};

	return true;
}

rn::BufferHandle Context::createBuffer(const rn::BufferDescription &description) {
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
		/*.size=*/ description.size * (description.paging == rn::BufferPaging::Static ? 1u : surfaceImageViews.size()),
		/*.usage=*/ usage,
		/*.sharingMode=*/ vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ 0,
		/*.pQueueFamilyIndices=*/ nullptr,
	};
	// ngn::log::debug("rn::vki::Context::createBuffer():\n\t.flags={}\n\t.size={}\n\t.usage={}\n\t.sharingMode={}\n\t.queueFamilyIndexCount={}",
	// 	vk::to_string(bufferCreateInfo.flags),
	// 	bufferCreateInfo.size,
	// 	vk::to_string(bufferCreateInfo.usage),
	// 	vk::to_string(bufferCreateInfo.sharingMode),
	// 	bufferCreateInfo.queueFamilyIndexCount
	// );
	rn::vki::UniqueBuffer bufferUnique{
		RN_VKI_TRACE(device->createBufferUnique(bufferCreateInfo, nullptr, device.table())),
		device.table()
	};

	rn::vki::memory::Handle memory;
	if ((rn::BufferUsage::Staging & description.usage) == rn::BufferUsage::Staging) {
		memory = allocator.staging.alloc(rn::vki::memory::Usage::CPU, bufferUnique.get());
	} else {
		memory = allocator.buffer.alloc(rn::vki::memory::Usage::GPU, bufferUnique.get());
	}

	RN_VKI_TRACE(device->bindBufferMemory(bufferUnique.get(), memory.memory.get(), memory.offset, device.table()));

	for (size_t i = 0; i < bufferSlots.size(); i++) {
		if ( ! bufferSlots[i].buffer) {
			bufferSlots[i] = { std::move(description), std::move(bufferUnique), std::move(memory) };

			return rn::BufferHandle{ static_cast<rn::BufferHandle::InternalType>(i) };
		}
	}

	size_t index = bufferSlots.size();
	bufferSlots.push_back({ std::move(description), std::move(bufferUnique), std::move(memory) });

	return rn::BufferHandle{ static_cast<rn::BufferHandle::InternalType>(index) };
}

// rn::FenceHandle enqueueBufferUpload(rn::BufferHandle handle, rn::DataFormat dataFormat, std::byte *data) {
// 	return rn::end<rn::FenceHandle>();
// }

bool Context::uploadBufferSync(rn::BufferHandle handle, util::Span<rn::BufferCopyRange> &&ranges) {
	if (handle == rn::end<rn::BufferHandle>()) {
		ngn::log::error("rn::vki::Context::uploadBufferSync(< {:x} >, [ .length={} ]) => upload failed, invalid buffer handle", handle.index, ranges.size());
		return false;
	}

	size_t index = handle.index;
	if (index >= bufferSlots.size()) {
		ngn::log::error("rn::vki::Context::uploadBufferSync(< {:x} >, [ .length={} ]) => upload failed, buffer does not exist", handle.index, ranges.size());
		return false;
	}

	if ( ! bufferSlots[index].buffer) {
		ngn::log::error("rn::vki::Context::uploadBufferSync(< {:x} >, [ .length={} ]) => upload failed, buffer is invalid", handle.index, ranges.size());
		return false;
	}

	if (bufferSlots[index].memory.pointer == nullptr) {
		ngn::log::error("rn::vki::Context::uploadBufferSync(< {:x} >, [ .length={} ]) => upload failed, buffer memory is not mapped", handle.index, ranges.size());
		return false;
	}

	for (auto &&range : ranges) {
		if (range.data != nullptr) {
			std::memcpy(static_cast<std::byte *>(bufferSlots[index].memory.pointer) + range.bufferOffset, range.data, range.length);
		}
	}

	if (bufferSlots[index].memory.needsFlushing()) {
		std::vector<vk::MappedMemoryRange> mappedMemoryRanges = util::map(ranges, [&] (const auto &range) {
			return vk::MappedMemoryRange{
				/*.memory=*/ bufferSlots[index].memory.memory.get(),
				/*.offset=*/ bufferSlots[index].memory.offset + range.bufferOffset,
				/*.size=*/ range.length
			};
		});

		RN_VKI_TRACE(device->flushMappedMemoryRanges(mappedMemoryRanges, device.table()));
	}

	return true;
}

bool Context::destroyBuffer(rn::BufferHandle handle) {
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

rn::FenceStamp Context::pendingFenceStamp(rn::QueueType queueType) {
	if (queueType != rn::QueueType::None) {
		return pendingFenceStamps[static_cast<uint32_t>(queueType)];
	} else {
		return rn::end<rn::FenceStamp>();
	}
}

rn::FenceStamp Context::resolvedFenceStamp(rn::QueueType queueType) {
	if (queueType != rn::QueueType::None) {
		return resolvedFenceStamps[static_cast<uint32_t>(queueType)];
	} else {
		return rn::end<rn::FenceStamp>();
	}
}

rn::FenceStamp Context::enqueueCommands([[maybe_unused]] std::vector<rn::GraphicCommandVariant> &&commands) {
	// graphicCommandLists.push_back(std::move(commands));

	return pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Graphic) - 1];
}

rn::FenceStamp Context::enqueueCommands([[maybe_unused]] std::vector<rn::ComputeCommandVariant> &&commands) {
	// computeCommandLists.push_back(std::move(commands));

	return pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Compute) - 1];
}

rn::FenceStamp Context::enqueueCommands(std::vector<rn::TransferCommandVariant> &&commands) {
	transferCommandLists.push_back(std::move(commands));

	return pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Transfer) - 1];
}

// void Context::deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::GraphicCommandVariant> &&commands) {
// 	assert(fenceStamp < rn::end<rn::FenceStamp>());
// 	assert(fenceQueueType != rn::QueueType::None);

// 	deferredGraphicCommandLists.push_back({
// 		fenceStamp,
// 		fenceQueueType,
// 		std::move(commands)
// 	});
// }

// void Context::deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::ComputeCommandVariant> &&commands) {
// 	assert(fenceStamp < rn::end<rn::FenceStamp>());
// 	assert(fenceQueueType != rn::QueueType::None);

// 	deferredComputeCommandLists.push_back({
// 		fenceStamp,
// 		fenceQueueType,
// 		std::move(commands)
// 	});
// }

// void Context::deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::TransferCommandVariant> &&commands) {
// 	assert(fenceStamp < rn::end<rn::FenceStamp>());
// 	assert(fenceQueueType != rn::QueueType::None);

// 	deferredTransferCommandLists.push_back({
// 		fenceStamp,
// 		fenceQueueType,
// 		std::move(commands)
// 	});
// }

// void Context::submitEnqueuedCommands(rn::QueueType queueType, [[maybe_unused]] util::ThreadPool &threadPool) {
// 	rn::vki::Recorder recorder{*this, threadPool};

// 	switch (queueType) {
// 		case rn::QueueType::Graphic: {
// 			recorder.record(std::move(graphicCommandLists));
// 			graphicCommandLists = {};
// 			// const auto commandBuffer = buildCommandBuffer(graphicCommandLists);
// 			// const auto &fence = waitForAvailableFence(queueType);
// 			// RN_VKI_TRACE(queue.graphic.submit({}, fence, device.table()));

// 			pendingFenceStamps[static_cast<uint32_t>(queueType)]++;
// 			return;
// 		}
// 		case rn::QueueType::Compute: {
// 			recorder.record(std::move(computeCommandLists));
// 			computeCommandLists = {};
// 			// const auto commandBuffer = buildCommandBuffer(computeCommandLists);
// 			// const auto &fence = waitForAvailableFence(queueType);
// 			// RN_VKI_TRACE(queue.compute.submit({}, fence, device.table()));

// 			pendingFenceStamps[static_cast<uint32_t>(queueType)]++;
// 			return;
// 		}
// 		case rn::QueueType::Transfer: {
// 			recorder.record(std::move(transferCommandLists));
// 			transferCommandLists = {};
// 			// const auto commandBuffer = buildCommandBuffer(transferCommandLists);
// 			// const auto &fence = waitForAvailableFence(queueType);
// 			// RN_VKI_TRACE(queue.transfer.submit({}, fence, device.table()));

// 			pendingFenceStamps[static_cast<uint32_t>(queueType)]++;
// 			return;
// 		}
// 		default: {
// 			return;
// 		}
// 	}
// }

void Context::reset() {
	waitIdle();

	transferCommandLists.clear();
	// computeCommandLists.clear();
	// graphicCommandLists.clear();
	pendingFenceStamps = {};
	resolvedFenceStamps = {};
	bufferSlots.clear();
	textureSlots.clear();
	synchronization = {};
	commandPoolGroups = {};
	allocator = {};
	surfaceDescription = {};
	queue = {};
	queueFamilyIndex = {};
	physicalDeviceDescription = {};
	surfaceImageViews.clear();
	swapchain = {};
	device = {};
	surface = {};
	debug = {};
	physicalDevice = {};
	instance = {};
}

void Context::waitIdle() {
	if (device) {
		ngn::log::debug("wait for device");
		RN_VKI_TRACE(device->waitIdle(device.table()));

		if (queue.presentation) {
			ngn::log::debug("wait for queue.presentation");
			RN_VKI_TRACE(queue.presentation.waitIdle(device.table()));
		}
	}
}

} // rn::vki
