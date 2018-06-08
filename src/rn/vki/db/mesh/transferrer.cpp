#include "reader.hpp"

#include <algorithm>

#include "../../../../ngn/log.hpp"
#include "../../../../ngn/prof.hpp"
#include "../../../../util/implode.hpp"
#include "../../../../util/map.hpp"
#include "../../context.hpp"
#include "../../id.hpp"
#include "../../mapping.hpp"
#include "../../trace.hpp"
#include "../mesh.hpp"

namespace rn::vki::db::mesh {

struct JobData {
	Payload payload;
	std::string_view source;

	vk::UniqueBuffer vertexBuffer{};
	memory::Handle vertexMemory{};

	vk::UniqueBuffer indexBuffer{};
	memory::Handle indexMemory{};

	JobData(Payload &&payload, std::string_view &&source) noexcept :
		payload{std::move(payload)},
		source{std::move(source)}
	{}
};

Transferrer::Transferrer(rn::vki::Context &context, rn::vki::db::Mesh &db) :
	context(context),
	db(db)
{
	vk::CommandPoolCreateInfo commandPoolCreateInfo{
		/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
		/*.queueFamilyIndex=*/ context.family.transfer,
	};
	commandPool = context.device.createCommandPoolUnique(commandPoolCreateInfo);
}

void Transferrer::run(std::vector<Payload> &&payloads) {
	std::vector<JobData> jobs{};
	std::string jobNames{"?"};

	try {
		NGN_PROF_SCOPE("Mesh::Transferrer");

		context.device.resetCommandPool(commandPool.get(), vk::CommandPoolResetFlagBits::eReleaseResources);

		std::vector<JobData> jobs = db.map([&] () {
			std::vector<JobData> jobs{};
			jobs.reserve(payloads.size());

			for (auto &payload : payloads) {
				uint32_t index = payload.index;

				jobs.emplace_back(
					std::move(payload),
					std::string_view{db.entries[index].source}
				);

				db.entries[index].status = MeshEntryStatus::TRANSFERING;
			}

			return jobs;
		});

		jobNames = util::implode(util::map(jobs, [] (const JobData &job) {
			return "{" + std::to_string(job.payload.index) + "} \"" + std::string{job.source} + "\"";
		}));
		ngn::log::debug("rn::vki::db::mesh::Transferrer::run([{}]) => transferring {} mesh(es)", jobNames, jobs.size());

		struct BufferHandles {
			vk::UniqueBuffer buffer{};
			memory::Handle bufferMemory{};
		};

		for (auto &job : jobs) {
			if (job.payload.vertexBufferSize > 0) {
				vk::BufferCreateInfo vertexBufferCreateInfo{
					/*.flags=*/ vk::BufferCreateFlags{},
					/*.size=*/ job.payload.vertexBufferSize,
					/*.usage=*/ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
					/*.sharingMode=*/ vk::SharingMode::eExclusive,
					/*.queueFamilyIndexCount=*/ 0,
					/*.pQueueFamilyIndices=*/ nullptr
				};
				job.vertexBuffer = RN_VLK_TRACE(context.device.createBufferUnique(vertexBufferCreateInfo));
				ngn::log::debug("Create device vertexBuffer {}", rn::vki::id(job.vertexBuffer));

				vk::MemoryRequirements vertexMemoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(job.vertexBuffer.get()));

				job.vertexMemory = context.allocator.buffer.alloc(memory::Usage::GPU, vertexMemoryRequirements);

				RN_VLK_TRACE(context.device.bindBufferMemory(job.vertexBuffer.get(), job.vertexMemory.memory, job.vertexMemory.offset));
			}

			if (job.payload.indexBufferSize > 0) {
				vk::BufferCreateInfo indexBufferCreateInfo{
					/*.flags=*/ vk::BufferCreateFlags{},
					/*.size=*/ job.payload.indexBufferSize,
					/*.usage=*/ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
					/*.sharingMode=*/ vk::SharingMode::eExclusive,
					/*.queueFamilyIndexCount=*/ 0,
					/*.pQueueFamilyIndices=*/ nullptr
				};
				job.indexBuffer = RN_VLK_TRACE(context.device.createBufferUnique(indexBufferCreateInfo));
				ngn::log::debug("Create device indexBuffer {}", rn::vki::id(job.indexBuffer));

				vk::MemoryRequirements indexMemoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(job.indexBuffer.get()));

				job.indexMemory = context.allocator.buffer.alloc(memory::Usage::GPU, indexMemoryRequirements);

				RN_VLK_TRACE(context.device.bindBufferMemory(job.indexBuffer.get(), job.indexMemory.memory, job.indexMemory.offset));
			}
		}

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

			for (auto &job : jobs) {
				if (job.payload.vertexBufferSize > 0) {
					std::array<vk::BufferCopy, 1> regions{vk::BufferCopy{
						/*.srcOffset=*/ 0,
						/*.dstOffset=*/ 0,
						/*.size=*/ job.payload.vertexBufferSize,
					}};

					RN_VLK_TRACE(commandBuffer.copyBuffer(job.payload.vertexBuffer.get(), job.vertexBuffer.get(), regions));
				}

				if (job.payload.indexBufferSize > 0) {
					std::array<vk::BufferCopy, 1> regions{vk::BufferCopy{
						/*.srcOffset=*/ 0,
						/*.dstOffset=*/ 0,
						/*.size=*/ job.payload.indexBufferSize,
					}};

					RN_VLK_TRACE(commandBuffer.copyBuffer(job.payload.indexBuffer.get(), job.indexBuffer.get(), regions));
				}
			}

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

		ngn::log::debug("rn::vki::db::mesh::Transferrer::run([{}]) => done", jobNames);

		db.map([&] () {
			for (auto &job : jobs) {
				db.entries[job.payload.index].status = MeshEntryStatus::READY;
				db.entries[job.payload.index].vertexBuffer = std::move(job.vertexBuffer);
				db.entries[job.payload.index].vertexMemory = std::move(job.vertexMemory);
				db.entries[job.payload.index].indexBuffer = std::move(job.indexBuffer);
				db.entries[job.payload.index].indexMemory = std::move(job.indexMemory);
			}
		});

		for (auto &job : jobs) {
			// resolve promises
			job.payload.promise.set_value(true);
		}
	} catch (std::runtime_error const &e) {
		ngn::log::debug("rn::vki::db::mesh::Transferrer::run([{}]) => error {}", jobNames, e.what());

		db.map([&] () {
			for (auto &payload : payloads) {
				db.entries[payload.index].status = MeshEntryStatus::TRANSFERRER_FAILED;
			}

			for (auto &job : jobs) {
				db.entries[job.payload.index].status = MeshEntryStatus::TRANSFERRER_FAILED;
			}
		});

		for (auto &payload : payloads) {
			payload.promise.set_value(false);
		}

		for (auto &job : jobs) {
			job.payload.promise.set_value(false);
		}
	} catch (...) {
		ngn::log::debug("rn::vki::db::mesh::Transferrer::run([{}]) => unknown error", jobNames);

		db.map([&] () {
			for (auto &payload : payloads) {
				db.entries[payload.index].status = MeshEntryStatus::TRANSFERRER_FAILED;
			}

			for (auto &job : jobs) {
				db.entries[job.payload.index].status = MeshEntryStatus::TRANSFERRER_FAILED;
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

} // rn::vki::db::mesh
