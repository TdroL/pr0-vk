#include "recorder.hpp"

#include <numeric>
#include <tuple>

#include "../../ngn/log.hpp"
#include "../../ngn/prof.hpp"
#include "id.hpp"
#include "trace.hpp"

namespace rn::vki {

template<class T>
std::pair<uint32_t, uint32_t> findCommandListGroupIndex(const std::vector<std::vector<T>> &commandLists, uint32_t offset) {
	for (uint32_t i = 0, l = static_cast<uint32_t>(commandLists.size()); i < l; i++) {
		uint32_t commandListSize = static_cast<uint32_t>(commandLists[i].size());

		if (commandListSize <= offset) {
			offset -= commandListSize;
		} else {
			return std::make_pair(i, offset);
		}
	}

	return std::make_pair(static_cast<uint32_t>(commandLists.size()), 0);
}

template<class T>
rn::vki::HandleCommandBuffer processCommandLists(rn::vki::Recorder &recorder, rn::QueueType queueType, std::vector<std::vector<T>> &&commandLists) {
	rn::vki::Context &context = recorder.context;
	util::ThreadPool &threadPool = recorder.threadPool;
	rn::vki::Executor &executor = recorder.executor;

	const uint32_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (uint32_t acc, const auto &commandList) {
		return acc + static_cast<uint32_t>(commandList.size());
	});

	constexpr uint32_t minimalCommandCountPerThread = 25; // 100;

	uint32_t approxCommandsPerThread = std::max(static_cast<uint32_t>(threadPool.size()) * minimalCommandCountPerThread, commandCount) / static_cast<uint32_t>(threadPool.size());
	uint32_t threadCount = static_cast<uint32_t>(std::max(1u, commandCount / approxCommandsPerThread));

	util::Span<vk::CommandPool> commandPools = context.getCommandPools(queueType, threadCount);
	assert(commandPools.size() >= threadCount);

	rn::vki::HandleCommandBuffer primaryCommandBufferHandle{};
	{
		// create primary command buffer
		vk::CommandBufferAllocateInfo primaryCommandBufferAllocateInfo{
			/*.commandPool=*/ commandPools[0],
			/*.level=*/ vk::CommandBufferLevel::ePrimary,
			/*.commandBufferCount=*/ 1,
		};
		vk::CommandBuffer primaryCommandBuffer{};
		RN_VKI_TRACE(context.device->allocateCommandBuffers(&primaryCommandBufferAllocateInfo, &primaryCommandBuffer, context.device.table()));

		primaryCommandBufferHandle = rn::vki::HandleCommandBuffer{primaryCommandBuffer, context.device.table()};
	}

	if (commandCount == 0) {
		RN_VKI_TRACE(primaryCommandBufferHandle->begin({
			/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
			/*.pInheritanceInfo=*/ nullptr
		}, primaryCommandBufferHandle.table()));
		RN_VKI_TRACE(primaryCommandBufferHandle->end(primaryCommandBufferHandle.table()));

		return primaryCommandBufferHandle;
	}

	// create secondary command buffers - one for each thread
	std::vector<vk::CommandBuffer> secondaryCommandBuffers{};
	secondaryCommandBuffers.resize(threadCount);
	for (uint32_t i = 0; i < threadCount; i++) {
		vk::CommandBufferAllocateInfo secondaryCommandBufferAllocateInfo{
			/*.commandPool=*/ commandPools[i],
			/*.level=*/ vk::CommandBufferLevel::eSecondary,
			/*.commandBufferCount=*/ 1,
		};
		RN_VKI_TRACE(context.device->allocateCommandBuffers(&secondaryCommandBufferAllocateInfo, &secondaryCommandBuffers[i], context.device.table()));
	}

	for (uint32_t i = 0; i < threadCount; i++) {
		uint32_t threadIndex = i;
		uint32_t begin = approxCommandsPerThread * i;
		uint32_t end = (i + 1 >= threadCount) ? commandCount : approxCommandsPerThread * (i + 1);

		auto [commandListsGroup, commandListsIndex] = findCommandListGroupIndex(commandLists, begin);

		rn::vki::HandleCommandBuffer commandBufferHandle{secondaryCommandBuffers[threadIndex], context.device.table()};

		vk::CommandBufferInheritanceInfo commandBufferInheritanceInfo{};
		RN_VKI_TRACE(commandBufferHandle->begin({
			/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
			/*.pInheritanceInfo=*/ &commandBufferInheritanceInfo,
		}, commandBufferHandle.table()));

		while (begin++ < end) {
			executor.executeCommand(commandBufferHandle.handle(), commandLists[commandListsGroup][commandListsIndex]);

			commandListsIndex++;
			if (commandListsIndex >= commandLists[commandListsGroup].size()) {
				commandListsIndex = 0;
				commandListsGroup++;
			}
		}

		RN_VKI_TRACE(commandBufferHandle->end(commandBufferHandle.table()));
	}

	RN_VKI_TRACE(primaryCommandBufferHandle->begin({
		/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
		/*.pInheritanceInfo=*/ nullptr
	}, primaryCommandBufferHandle.table()));
	{
		RN_VKI_TRACE(primaryCommandBufferHandle->executeCommands(secondaryCommandBuffers, primaryCommandBufferHandle.table()));
	}
	RN_VKI_TRACE(primaryCommandBufferHandle->end(primaryCommandBufferHandle.table()));

	return primaryCommandBufferHandle;
}

rn::vki::HandleCommandBuffer Recorder::record(std::vector<std::vector<rn::GraphicCommandVariant>> &&commandLists) {
	NGN_PROF_SCOPE("rn::vki::Recorder::record<Graphic>");

	return processCommandLists(*this, rn::QueueType::Transfer, std::move(commandLists));
}

rn::vki::HandleCommandBuffer Recorder::record(std::vector<std::vector<rn::ComputeCommandVariant>> &&commandLists) {
	NGN_PROF_SCOPE("rn::vki::Recorder::record<Compute>");

	return processCommandLists(*this, rn::QueueType::Transfer, std::move(commandLists));
}

rn::vki::HandleCommandBuffer Recorder::record(std::vector<std::vector<rn::TransferCommandVariant>> &&commandLists) {
	NGN_PROF_SCOPE("rn::vki::Recorder::record<Transfer>");

	return processCommandLists(*this, rn::QueueType::Transfer, std::move(commandLists));
}

} // rn::vki
