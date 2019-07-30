#include "context.hpp"

#include "../../ngn/config.hpp"
#include "../../ngn/log.hpp"
#include "../../util/map.hpp"

#include "../window.hpp"

namespace rn::nulli {

rn::nulli::Context Context::factory([[maybe_unused]] ngn::config::Config &config, [[maybe_unused]] rn::Window &window) {
	rn::nulli::Context context{};

	return context;
}

Context::Context(Context &&other) :
	imageSlots{std::move(other.imageSlots)},
	bufferSlots{std::move(other.bufferSlots)}
{}

Context & Context::operator=([[maybe_unused]] Context &&other) {
	reset();

	imageSlots = std::move(other.imageSlots);
	bufferSlots = std::move(other.bufferSlots);

	return *this;
}

Context::~Context() {
	reset();
}



void Context::advance() {

}

void Context::commit([[maybe_unused]] util::ThreadPool &threadPool) {
	pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Graphic) - 1]++;
	pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Compute) - 1]++;
	pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Transfer) - 1]++;

	resolvedFenceStamps[static_cast<uint32_t>(rn::QueueType::Graphic) - 1]++;
	resolvedFenceStamps[static_cast<uint32_t>(rn::QueueType::Compute) - 1]++;
	resolvedFenceStamps[static_cast<uint32_t>(rn::QueueType::Transfer) - 1]++;
}

rn::TextureHandle Context::createTexture([[maybe_unused]] const rn::TextureDescription &description) {
	for (size_t i = 0; i < imageSlots.size(); i++) {
		if ( ! imageSlots[i]) {
			imageSlots[i] = true;

			return rn::TextureHandle{ static_cast<rn::TextureHandle::InternalType>(i) };
		}
	}

	size_t index = imageSlots.size();
	imageSlots.push_back(true);

	return rn::TextureHandle{ static_cast<rn::TextureHandle::InternalType>(index) };
}

bool Context::destroyTexture(rn::TextureHandle handle) {
	if (handle == rn::end<rn::TextureHandle>() || handle == rn::swapchainHandle()) {
		return false;
	}

	size_t index = handle.index;
	if (index >= imageSlots.size()) {
		return false;
	}

	if ( ! imageSlots[index]) {
		return false;
	}

	imageSlots[index] = false;

	return true;
}

rn::BufferHandle Context::createBuffer([[maybe_unused]] const rn::BufferDescription &description) {
	for (size_t i = 0; i < bufferSlots.size(); i++) {
		if ( ! bufferSlots[i]) {
			bufferSlots[i] = true;

			return rn::BufferHandle{ static_cast<rn::BufferHandle::InternalType>(i) };
		}
	}

	size_t index = bufferSlots.size();
	bufferSlots.push_back(true);

	return rn::BufferHandle{ static_cast<rn::BufferHandle::InternalType>(index) };
}

bool Context::uploadBufferSync([[maybe_unused]] rn::BufferHandle handle, [[maybe_unused]] util::Span<rn::BufferCopyRange> &&ranges) {
	if (handle == rn::end<rn::BufferHandle>()) {
		return false;
	}

	size_t index = handle.index;
	if (index >= bufferSlots.size()) {
		return false;
	}

	if ( ! bufferSlots[index]) {
		return false;
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

	if ( ! bufferSlots[index]) {
		return false;
	}

	bufferSlots[index] = false;

	return true;
}

rn::FenceStamp Context::pendingFenceStamp(rn::QueueType queueType) {
	if (queueType != rn::QueueType::None) {
		return pendingFenceStamps[static_cast<uint32_t>(queueType) - 1];
	} else {
		return rn::end<rn::FenceStamp>();
	}
}

rn::FenceStamp Context::resolvedFenceStamp(rn::QueueType queueType) {
	if (queueType != rn::QueueType::None) {
		return resolvedFenceStamps[static_cast<uint32_t>(queueType) - 1];
	} else {
		return rn::end<rn::FenceStamp>();
	}
}

rn::FenceStamp Context::enqueueCommands([[maybe_unused]] std::vector<rn::GraphicCommandVariant> &&commands) {
	return pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Graphic) - 1];
}

rn::FenceStamp Context::enqueueCommands([[maybe_unused]] std::vector<rn::ComputeCommandVariant> &&commands) {
	return pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Compute) - 1];
}

rn::FenceStamp Context::enqueueCommands([[maybe_unused]] std::vector<rn::TransferCommandVariant> &&commands) {
	return pendingFenceStamps[static_cast<uint32_t>(rn::QueueType::Transfer) - 1];
}

// void Context::deferCommands([[maybe_unused]] rn::FenceStamp fenceStamp, [[maybe_unused]] rn::QueueType fenceQueueType, [[maybe_unused]] std::vector<rn::GraphicCommandVariant> &&commands) {
// 	return;
// }

// void Context::deferCommands([[maybe_unused]] rn::FenceStamp fenceStamp, [[maybe_unused]] rn::QueueType fenceQueueType, [[maybe_unused]] std::vector<rn::ComputeCommandVariant> &&commands) {
// 	return;
// }

// void Context::deferCommands([[maybe_unused]] rn::FenceStamp fenceStamp, [[maybe_unused]] rn::QueueType fenceQueueType, [[maybe_unused]] std::vector<rn::TransferCommandVariant> &&commands) {
// 	return;
// }

void Context::reset() {
	waitIdle();
}

void Context::waitIdle() {
	// do nothing
}

} // rn::vki
