#pragma once

#include <mutex>
#include <vector>

#include "../../util/span.hpp"
#include "../../util/threadPool.hpp"
#include "../../util/trivialVector.hpp"
#include "../contextAbstract.hpp"
#include "../commands.hpp"
#include "../types.hpp"
#include "dispatch.hpp"
#include "context/types.hpp"

namespace ngn::config {

struct Config;

}

namespace rn {

class Window;

}

namespace rn::vki {

class Context : public rn::ContextAbstract {
public:
	static rn::vki::Context factory(ngn::config::Config &config, rn::Window &window);

	rn::vki::UniqueTableInstance instance{};
	rn::vki::HandlePhysicalDevice physicalDevice{};
	rn::vki::context::Debug debug{};
	rn::vki::UniqueSurfaceKHR surface{};
	rn::vki::UniqueTableDevice device{};
	rn::vki::UniqueSwapchainKHR swapchain{};
	std::vector<rn::vki::UniqueImageView> surfaceImageViews{};
	rn::vki::context::PhysicalDeviceDescription physicalDeviceDescription{};
	rn::vki::context::QueueFamilyIndex queueFamilyIndex{};
	rn::vki::context::Queue queue{};
	rn::vki::context::SurfaceDescription surfaceDescription{};
	rn::vki::context::Allocator allocator{};
	rn::vki::context::CommandPoolGroups commandPoolGroups{};
	rn::vki::context::Synchronization synchronization{};

	struct TextureSlot {
		rn::TextureDescription description{};
		rn::vki::UniqueImage image{};
		rn::vki::memory::Handle memory{};
	};

	struct BufferSlot {
		rn::BufferDescription description{};
		rn::vki::UniqueBuffer buffer{};
		rn::vki::memory::Handle memory{};
	};

	std::vector<TextureSlot> textureSlots{};
	std::vector<BufferSlot> bufferSlots{};

	std::array<rn::FenceStamp, 3> resolvedFenceStamps{
		/* Graphic: */ 0,
		/* Compute: */ 0,
		/* Transfer: */ 0
	};

	std::array<rn::FenceStamp, 3> pendingFenceStamps{
		/* Graphic: */ 1,
		/* Compute: */ 1,
		/* Transfer: */ 1
	};

	struct CommandPoolList {
		rn::FenceStamp fenceStamp{rn::end<rn::FenceStamp>()};
		std::vector<rn::vki::UniqueCommandPool> pool{};
	};

	std::array<std::vector<CommandPoolList>, 3> commandPoolLists{
		/* Graphic: */ std::vector<CommandPoolList>{},
		/* Compute: */ std::vector<CommandPoolList>{},
		/* Transfer: */ std::vector<CommandPoolList>{}
	};
	// // std::vector<std::vector<rn::vki::UniqueCommandPool>> computeCommandPoolLists{};
	// // std::vector<std::vector<rn::vki::UniqueCommandPool>> transferCommandPoolLists{};

	// std::vector<std::vector<rn::GraphicCommandVariant>> graphicCommandLists{};
	// std::vector<std::vector<rn::ComputeCommandVariant>> computeCommandLists{};
	std::vector<std::vector<rn::TransferCommandVariant>> transferCommandLists{};

	// template<typename T>
	// struct DeferredCommands {
	// 	rn::FenceStamp fenceStamp{rn::end<rn::FenceStamp>()};
	// 	rn::QueueType fenceQueueType{rn::QueueType::None};
	// 	std::vector<T> commandList{};
	// };

	// std::vector<DeferredCommands<rn::GraphicCommandVariant>> deferredGraphicCommandLists{};
	// std::vector<DeferredCommands<rn::ComputeCommandVariant>> deferredComputeCommandLists{};
	// std::vector<DeferredCommands<rn::TransferCommandVariant>> deferredTransferCommandLists{};

	Context() = default;

	Context(const Context &other) = delete;
	Context(Context &&other);

	Context & operator=(const Context &other) = delete;
	Context & operator=(Context &&other);

	~Context();

	util::Span<vk::CommandPool> getCommandPools(rn::QueueType queueType, uint32_t count);
	// void submitEnqueuedCommands(rn::QueueType queueType, util::ThreadPool &threadPool);

	void advance() override;
	void commit(util::ThreadPool &threadPool) override;

	rn::TextureHandle createTexture(const rn::TextureDescription &description) override;
	bool destroyTexture(rn::TextureHandle handle) override;

	rn::BufferHandle createBuffer(const rn::BufferDescription &description) override;
	bool uploadBufferSync(rn::BufferHandle handle, util::Span<rn::BufferCopyRange> &&ranges) override;
	bool destroyBuffer(rn::BufferHandle handle) override;

	rn::FenceStamp pendingFenceStamp(rn::QueueType queueType) override;
	rn::FenceStamp resolvedFenceStamp(rn::QueueType queueType) override;
	rn::FenceStamp enqueueCommands(std::vector<rn::GraphicCommandVariant> &&commands) override;
	rn::FenceStamp enqueueCommands(std::vector<rn::ComputeCommandVariant> &&commands) override;
	rn::FenceStamp enqueueCommands(std::vector<rn::TransferCommandVariant> &&commands) override;
	// void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::GraphicCommandVariant> &&commands) override;
	// void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::ComputeCommandVariant> &&commands) override;
	// void deferCommands(rn::FenceStamp fenceStamp, rn::QueueType fenceQueueType, std::vector<rn::TransferCommandVariant> &&commands) override;

	void reset() override;
	void waitIdle() override;
};

} // rn::vki
