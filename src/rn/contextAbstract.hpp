#pragma once

#include <vector>

#include "../util/span.hpp"
#include "../util/threadPool.hpp"
#include "../util/trivialVector.hpp"
#include "commands.hpp"
#include "types.hpp"

namespace rn {

class ContextAbstract {
public:
	virtual void advance() = 0;
	virtual void commit(util::ThreadPool &threadPool) = 0;

	virtual rn::TextureHandle createTexture(const rn::TextureDescription &description) = 0;
	virtual bool destroyTexture(rn::TextureHandle handle) = 0;

	virtual rn::BufferHandle createBuffer(const rn::BufferDescription &description) = 0;
	virtual bool uploadBufferSync(rn::BufferHandle handle, util::TrivialVector<rn::BufferCopyRange, 1> &&ranges) = 0;
	virtual bool destroyBuffer(rn::BufferHandle handle) = 0;

	virtual rn::FenceStamp pendingFenceStamp(rn::QueueType queueType) = 0;
	virtual rn::FenceStamp resolvedFenceStamp(rn::QueueType queueType) = 0;
	virtual rn::FenceStamp enqueueCommands(std::vector<rn::GraphicCommandVariant> &&commands) = 0;
	virtual rn::FenceStamp enqueueCommands(std::vector<rn::ComputeCommandVariant> &&commands) = 0;
	virtual rn::FenceStamp enqueueCommands(std::vector<rn::TransferCommandVariant> &&commands) = 0;
	virtual void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::GraphicCommandVariant> &&commands) = 0;
	virtual void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::ComputeCommandVariant> &&commands) = 0;
	virtual void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::TransferCommandVariant> &&commands) = 0;

	virtual void reset() = 0;
	virtual void waitIdle() = 0;
};

} // rn
