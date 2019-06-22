#pragma once

#include <vector>

#include "../contextAbstract.hpp"
#include "../types.hpp"

namespace ngn::config {

struct Config;

}

namespace rn {

class Window;

}

namespace rn::nulli {

class Context : public rn::ContextAbstract {
public:
	static rn::nulli::Context factory(ngn::config::Config &config, rn::Window &window);

	std::vector<bool> imageSlots{};
	std::vector<bool> bufferSlots{};

	std::array<rn::FenceStamp, 4> resolvedFenceStamps{
		/* Graphic: */ 0,
		/* Compute: */ 0,
		/* Transfer: */ 0
	};

	std::array<rn::FenceStamp, 4> pendingFenceStamps{
		/* Graphic: */ 1,
		/* Compute: */ 1,
		/* Transfer: */ 1
	};

	Context() = default;

	Context(const Context &other) = delete;
	Context(Context &&other);

	Context & operator=(const Context &other) = delete;
	Context & operator=(Context &&other);

	~Context();

	void advance() override;
	void commit(util::ThreadPool &threadPool) override;

	rn::TextureHandle createTexture(const rn::TextureDescription &description) override;
	bool destroyTexture(rn::TextureHandle handle) override;

	rn::BufferHandle createBuffer(const rn::BufferDescription &description) override;
	bool uploadBufferSync(rn::BufferHandle handle, util::TrivialVector<rn::BufferCopyRange, 1> &&ranges) override;
	bool destroyBuffer(rn::BufferHandle handle) override;

	rn::FenceStamp pendingFenceStamp(rn::QueueType queueType) override;
	rn::FenceStamp resolvedFenceStamp(rn::QueueType queueType) override;
	rn::FenceStamp enqueueCommands(std::vector<rn::GraphicCommandVariant> &&commands) override;
	rn::FenceStamp enqueueCommands(std::vector<rn::ComputeCommandVariant> &&commands) override;
	rn::FenceStamp enqueueCommands(std::vector<rn::TransferCommandVariant> &&commands) override;
	void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::GraphicCommandVariant> &&commands) override;
	void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::ComputeCommandVariant> &&commands) override;
	void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::TransferCommandVariant> &&commands) override;

	void reset() override;
	void waitIdle() override;
};

} // rn::nulli
