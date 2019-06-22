#include "recorder.hpp"

#include <numeric>
#include <tuple>
#include <thread>

#include "../../ngn/log.hpp"
#include "../../ngn/prof.hpp"
#include "trace.hpp"

namespace rn::vki {

template<class T>
std::pair<uint32_t, uint32_t> findCommandListGroupIndex(const std::vector<std::vector<T>> &commandLists, size_t offset) {
	size_t currentCount = 0;
	for (size_t i = 0; i < commandLists.size(); i++) {
		if (currentCount + commandLists[i].size() >= offset) {
			return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>((currentCount + commandLists[i].size()) - offset));
		}

		currentCount += commandLists[i].size();
	}

	return std::make_pair(static_cast<uint32_t>(commandLists.size()), 0);
}

vk::CommandBuffer Recorder::record([[maybe_unused]] std::vector<std::vector<rn::GraphicCommandVariant>> &&commandLists) {
	const size_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (size_t acc, const auto &list) {
		return acc + list.size();
	});

	ngn::log::debug("GraphicCommandVariant commandCount={}", commandCount);

	// std::vector<vk::CommandPool> commandPools = context.getAvailableCommandPools(rn::QueueType::Graphic, threadPool.size());
	// if (commandPools.empty()) {
	// 	throw std::runtime_error{"Unable to build command buffers - no command pools available"};
	// }

	// vk::CommandBuffer primary = commandPools[0];

	// const size_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (size_t acc, const auto &list) {
	// 	return acc + list.size();
	// });

	// const size_t threadCount = threadPool.size()

	return vk::CommandBuffer{};
}

vk::CommandBuffer Recorder::record([[maybe_unused]] std::vector<std::vector<rn::ComputeCommandVariant>> &&commandLists) {
	const size_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (size_t acc, const auto &list) {
		return acc + list.size();
	});

	ngn::log::debug("ComputeCommandVariant commandCount={}", commandCount);

	// std::vector<vk::CommandPool> commandPools = context.getAvailableCommandPools(rn::QueueType::Compute, threadPool.size());
	// if (commandPools.empty()) {
	// 	throw std::runtime_error{"Unable to build command buffers - no command pools available"};
	// }

	// vk::CommandBuffer primary = commandPools[0];

	// const size_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (size_t acc, const auto &list) {
	// 	return acc + list.size();
	// });

	// const size_t threadCount = threadPool.size()

	return vk::CommandBuffer{};
}

vk::CommandBuffer Recorder::record([[maybe_unused]] std::vector<std::vector<rn::TransferCommandVariant>> &&commandLists) {
	const size_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (size_t acc, const auto &list) {
		return acc + list.size();
	});

	ngn::log::debug("TransferCommandVariant commandCount={}", commandCount);

	if (commandCount == 0) {
		return vk::CommandBuffer{};
	}

	auto start = ngn::prof::now();

	constexpr size_t minimalCommandCountPerThread = 25; // 100;

	size_t approxCommandsPerThread = std::max(threadPool.size() * minimalCommandCountPerThread, commandCount) / threadPool.size();
	uint32_t threadCount = static_cast<uint32_t>(std::max(1ull, commandCount / approxCommandsPerThread));
	std::vector<std::future<vk::CommandBuffer>> futures{};
	futures.reserve(threadCount);

	util::Span<vk::CommandPool> commandPools = context.commandPools(rn::QueueType::Transfer, threadCount);
	assert(commandPools.size() == threadCount);

	// create primary command buffer
	vk::CommandBufferAllocateInfo primaryCommandBufferAllocateInfo{
		/*.commandPool=*/ commandPools[0],
		/*.level=*/ vk::CommandBufferLevel::ePrimary,
		/*.commandBufferCount=*/ 1,
	};
	vk::CommandBuffer primaryCommandBuffer{};
	RN_VKI_TRACE(context.device->allocateCommandBuffers(&primaryCommandBufferAllocateInfo, &primaryCommandBuffer, context.device.table()));

	// create secondary command buffers - one for each work thread
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
		size_t begin = approxCommandsPerThread * i;
		size_t end = (i + 1 >= threadCount) ? commandCount : approxCommandsPerThread * (i + 1);

		// futures.emplace_back(threadPool.enqueue<vk::CommandBuffer>([&] ([[maybe_unused]] size_t threadIndex) {
		// 	return vk::CommandBuffer{};
		// }));

		auto &dispatchTable = context.device.table();

		futures.emplace_back(threadPool.enqueue([begin, end, &commandLists, &secondaryCommandBuffers, &dispatchTable] ([[maybe_unused]] uint32_t threadIndex) {
			auto &commandBuffer = secondaryCommandBuffers[threadIndex];

			vk::CommandBufferInheritanceInfo commandBufferInheritanceInfo{};
			RN_VKI_TRACE(commandBuffer.begin({
				/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
				/*.pInheritanceInfo=*/ &commandBufferInheritanceInfo,
			}, dispatchTable));

			uint32_t commandListsGroup;
			uint32_t commandListsIndex;
			std::tie(commandListsGroup, commandListsIndex) = findCommandListGroupIndex(commandLists, begin);

			ngn::log::debug("thread={} begin={} end={} count={} commandListsGroup={} commandListsIndex={}", threadIndex, begin, end, end - begin, commandListsGroup, commandListsIndex);

			for (size_t j = begin; j < end; j++) {
				// switch ()
			}

			RN_VKI_TRACE(commandBuffer.end(dispatchTable));

			return secondaryCommandBuffers[threadIndex];
		}));
	}

	for (auto &&future : futures) {
		future.get();
	}

	auto stop = ngn::prof::now();

	RN_VKI_TRACE(primaryCommandBuffer.begin({
		/*.flags=*/ vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
		/*.pInheritanceInfo=*/ nullptr
	}, context.device.table()));
	RN_VKI_TRACE(primaryCommandBuffer.executeCommands(secondaryCommandBuffers, context.device.table()));
	RN_VKI_TRACE(primaryCommandBuffer.end(context.device.table()));

	ngn::log::debug("ComputeCommandVariant commandCount={} threadCount={} time={}", commandCount, threadCount, (stop - start) * 1000.0);

	// std::vector<vk::CommandPool> commandPools = context.getAvailableCommandPools(rn::QueueType::Transfer, threadPool.size());
	// if (commandPools.empty()) {
	// 	return vk::CommandBuffer{};
	// }

	// vk::CommandBuffer primary = commandPools[0];

	// const size_t commandCount = std::accumulate(std::begin(commandLists), std::end(commandLists), 0, [] (size_t acc, const auto &list) {
	// 	return acc + list.size();
	// });

	// const size_t threadCount = std::thread::hardware_concurrency

	return vk::CommandBuffer{};
}

} // rn::vki
