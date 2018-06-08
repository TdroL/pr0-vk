#include "reader.hpp"

#include <algorithm>

#include "../../../../ngn/config.hpp"
#include "../../../../ngn/log.hpp"
#include "../../../../ngn/prof.hpp"
#include "../../../../util/implode.hpp"
#include "../../../../util/map.hpp"
#include "../../context.hpp"
#include "../../mapping.hpp"
#include "../../trace.hpp"
#include "../texture.hpp"

namespace rn::vki::db::texture {

struct JobData {
	Payload payload;
	std::string_view source;

	vk::UniqueImage image{};
	memory::Handle imageMemory{};
	vk::UniqueImageView imageView{};

	JobData(Payload &&payload, std::string_view &&source) noexcept :
		payload{std::move(payload)},
		source{std::move(source)}
	{}
};

Transferrer::Transferrer(rn::vki::Context &context, rn::vki::db::Texture &db) :
	context(context),
	db(db)
{
	vk::CommandPoolCreateInfo commandPoolCreateInfo{
		/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
		/*.queueFamilyIndex=*/ context.family.transfer,
	};
	commandPool = RN_VLK_TRACE(context.device.createCommandPoolUnique(commandPoolCreateInfo));
}

void Transferrer::run(std::vector<Payload> &&payloads) {
	std::vector<JobData> jobs{};
	std::string jobNames{"?"};

	try {
		NGN_PROF_SCOPE("Texture::Transferrer");

		RN_VLK_TRACE(context.device.resetCommandPool(commandPool.get(), vk::CommandPoolResetFlags{}));

		std::vector<JobData> jobs = db.map([&] () {
			std::vector<JobData> jobs{};
			jobs.reserve(payloads.size());

			for (auto &payload : payloads) {
				uint32_t index = payload.index;

				jobs.emplace_back(
					std::move(payload),
					std::string_view{db.entries[index].source}
				);

				db.entries[index].status = TextureEntryStatus::TRANSFERING;
			}

			return jobs;
		});

		jobNames = util::implode(util::map(jobs, [] (const JobData &job) {
			return "{" + std::to_string(job.payload.index) + "} \"" + std::string{job.source} + "\"";
		}));
		ngn::log::debug("rn::vki::db::texture::Transferrer::run([{}]) => transferring {} texture(s)", jobNames, jobs.size());

		for (auto &job : jobs) {
			vk::ImageCreateInfo imageCreateInfo{
				/*.flags=*/ job.payload.flags,
				/*.imageType=*/ job.payload.imageType,
				/*.format=*/ job.payload.format,
				/*.extent=*/ job.payload.extents[0],
				/*.mipLevels=*/ job.payload.levels,
				/*.arrayLayers=*/ job.payload.layers,
				/*.samples=*/ vk::SampleCountFlagBits::e1,
				/*.tiling=*/ vk::ImageTiling::eOptimal,
				/*.usage=*/ vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				/*.sharingMode=*/ vk::SharingMode::eExclusive,
				/*.queueFamilyIndexCount=*/ 0,
				/*.pQueueFamilyIndices=*/ nullptr,
				/*.initialLayout=*/ vk::ImageLayout::eUndefined
			};
			job.image = RN_VLK_TRACE(context.device.createImageUnique(imageCreateInfo));

			vk::MemoryRequirements memoryRequirements = RN_VLK_TRACE(context.device.getImageMemoryRequirements(job.image.get()));
			job.imageMemory = context.allocator.texture.alloc(memory::Usage::GPU, memoryRequirements);

			RN_VLK_TRACE(context.device.bindImageMemory(job.image.get(), job.imageMemory.memory, job.imageMemory.offset));
		}

		std::vector<ImageData> imageDatas = util::map(jobs, [] (auto &job) {
			return ImageData{
				job.image.get(),
				job.payload.levels,
				job.payload.layers
			};
		});

		// transfer
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo{
			/*.commandPool=*/ commandPool.get(),
			/*.level=*/ vk::CommandBufferLevel::ePrimary,
			/*.commandBufferCount=*/ 1
		};
		std::vector<vk::UniqueCommandBuffer> commandBuffers = RN_VLK_TRACE(context.device.allocateCommandBuffersUnique(commandBufferAllocateInfo));
		vk::CommandBuffer commandBuffer = commandBuffers[0].get();

		vk::CommandBufferBeginInfo commandBufferBeginInfo{
			/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
			/*.pInheritanceInfo=*/ nullptr,
		};
		RN_VLK_TRACE(commandBuffer.begin(commandBufferBeginInfo));

			transitionImages(TransitionType::UNDEFINED_TO_TRANSFERABLE, commandBuffer, imageDatas);

			for (auto &job : jobs) {
				std::vector<vk::BufferImageCopy> bufferImageCopies{};
				bufferImageCopies.reserve(job.payload.faces * job.payload.levels);
				vk::DeviceSize bufferOffset = 0;

				for (uint32_t face = 0; face < job.payload.faces; face++) {
					for (uint32_t level = 0; level < job.payload.levels; level++) {
						bufferImageCopies.push_back(vk::BufferImageCopy{
							/*.bufferOffset=*/ bufferOffset,
							/*.bufferRowLength=*/ 0,
							/*.bufferImageHeight=*/ 0,
							/*.imageSubresource=*/ {
								/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
								/*.mipLevel=*/ level,
								/*.baseArrayLayer=*/ face,
								/*.layerCount=*/ 1,
							},
							/*.imageOffset=*/ vk::Offset3D{0, 0, 0},
							/*.imageExtent=*/ job.payload.extents[level]
						});

						bufferOffset += job.payload.sizes[level];
					}
				}

				RN_VLK_TRACE(commandBuffer.copyBufferToImage(job.payload.buffer.get(), job.image.get(), vk::ImageLayout::eTransferDstOptimal, bufferImageCopies));
			}

			transitionImages(TransitionType::TRANSFERABLE_TO_READABLE, commandBuffer, imageDatas);

		RN_VLK_TRACE(commandBuffer.end());

		vk::UniqueFence fence = RN_VLK_TRACE(context.device.createFenceUnique({}));
		{
			std::lock_guard lock{context.mutex.transfer()};

			RN_VLK_TRACE(context.queue.transfer.submit({
				{
					/*.waitSemaphoreCount=*/ 0,
					/*.pWaitSemaphores=*/ nullptr,
					/*.pWaitDstStageMask=*/ 0,
					/*.commandBufferCount=*/ 1,
					/*.pCommandBuffers=*/ &commandBuffer,
					/*.signalSemaphoreCount=*/ 0,
					/*.pSignalSemaphores=*/ nullptr
				}
			}, fence.get()));
		}
		RN_VLK_TRACE(context.device.waitForFences({ fence.get() }, true, std::numeric_limits<uint64_t>::max()));

		ngn::log::debug("rn::vki::db::texture::Transferrer::run([{}]) => done", jobNames);

		db.map([&] () {
			for (auto &job : jobs) {
				db.entries[job.payload.index].status = TextureEntryStatus::READY;
				db.entries[job.payload.index].image = std::move(job.image);
				db.entries[job.payload.index].imageMemory = std::move(job.imageMemory);
			}
		});

		for (auto &job : jobs) {
			// resolve promises
			job.payload.promise.set_value(true);
		}
	} catch (std::runtime_error const &e) {
		ngn::log::debug("rn::vki::db::texture::Transferrer::run([{}]) => error {}", jobNames, e.what());

		db.map([&] () {
			for (auto &payload : payloads) {
				db.entries[payload.index].status = TextureEntryStatus::TRANSFERRER_FAILED;
			}

			for (auto &job : jobs) {
				db.entries[job.payload.index].status = TextureEntryStatus::TRANSFERRER_FAILED;
			}
		});

		for (auto &payload : payloads) {
			payload.promise.set_value(false);
		}

		for (auto &job : jobs) {
			job.payload.promise.set_value(false);
		}
	} catch (...) {
		ngn::log::debug("rn::vki::db::texture::Transferrer::run([{}]) => unknown error", jobNames);

		db.map([&] () {
			for (auto &payload : payloads) {
				db.entries[payload.index].status = TextureEntryStatus::TRANSFERRER_FAILED;
			}

			for (auto &job : jobs) {
				db.entries[job.payload.index].status = TextureEntryStatus::TRANSFERRER_FAILED;
			}
		});

		for (auto &payload : payloads) {
			payload.promise.set_value(false);
		}

		for (auto &job : jobs) {
			job.payload.promise.set_value(false);
		}
	}
}

void Transferrer::transitionImages(TransitionType type, vk::CommandBuffer commandBuffer, const std::vector<ImageData> &imageDatas) {
	if (type == TransitionType::UNDEFINED_TO_TRANSFERABLE) {
		std::vector<vk::ImageMemoryBarrier> barriers{};
		barriers.reserve(imageDatas.size());

		for (auto &imageData : imageDatas) {
			barriers.push_back(vk::ImageMemoryBarrier{
				/*.srcAccessMask=*/ vk::AccessFlags{},
				/*.dstAccessMask=*/ vk::AccessFlagBits::eTransferWrite,
				/*.oldLayout=*/ vk::ImageLayout::eUndefined,
				/*.newLayout=*/ vk::ImageLayout::eTransferDstOptimal,
				/*.srcQueueFamilyIndex=*/ VK_QUEUE_FAMILY_IGNORED,
				/*.dstQueueFamilyIndex=*/ VK_QUEUE_FAMILY_IGNORED,
				/*.image=*/ imageData.image,
				/*.subresourceRange=*/ {
					/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
					/*.baseMipLevel=*/ 0,
					/*.levelCount=*/ imageData.levels,
					/*.baseArrayLayer=*/ 0,
					/*.layerCount=*/ imageData.layers
				}
			});
		}

		RN_VLK_TRACE(commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlagBits::eByRegion, {}, {}, barriers));
	} else {
		std::vector<vk::ImageMemoryBarrier> barriers{};
		barriers.reserve(imageDatas.size());

		for (auto &imageData : imageDatas) {
			barriers.push_back(vk::ImageMemoryBarrier{
				/*.srcAccessMask=*/ vk::AccessFlagBits::eTransferWrite,
				/*.dstAccessMask=*/ vk::AccessFlagBits::eMemoryRead,
				/*.oldLayout=*/ vk::ImageLayout::eTransferDstOptimal,
				/*.newLayout=*/ vk::ImageLayout::eShaderReadOnlyOptimal,
				/*.srcQueueFamilyIndex=*/ VK_QUEUE_FAMILY_IGNORED,
				/*.dstQueueFamilyIndex=*/ VK_QUEUE_FAMILY_IGNORED,
				/*.image=*/ imageData.image,
				/*.subresourceRange=*/ {
					/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor,
					/*.baseMipLevel=*/ 0,
					/*.levelCount=*/ imageData.levels,
					/*.baseArrayLayer=*/ 0,
					/*.layerCount=*/ imageData.layers
				}
			});
		}

		RN_VLK_TRACE(commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits::eByRegion, {}, {}, barriers));
	}
}

} // rn::vki::db::texture
