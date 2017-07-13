#pragma once

#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class QueueFamily {
public:
	vk::Queue handle{};
	uint32_t family{std::numeric_limits<uint32_t>::max()};

	QueueFamily() = default;
	QueueFamily(
		vk::Queue &&handle,
		uint32_t family
	) :
		handle{std::move(handle)},
		family{family}
	{
		handle = vk::Queue{};
	}

	QueueFamily(QueueFamily &&rhs) :
		handle{std::move(rhs.handle)},
		family{std::move(rhs.family)}
	{

		rhs.handle = vk::Queue{};
		rhs.family = std::numeric_limits<uint32_t>::max();
	}

	QueueFamily & operator=(QueueFamily &&rhs) {
		handle = std::move(rhs.handle);
		family = std::move(rhs.family);

		rhs.handle = vk::Queue{};
		rhs.family = std::numeric_limits<uint32_t>::max();

		return *this;
	}

	QueueFamily(const QueueFamily &rhs) :
		handle{rhs.handle},
		family{rhs.family}
	{}

	QueueFamily & operator=(const QueueFamily &rhs) {
		handle = rhs.handle;
		family = rhs.family;

		return *this;
	}
};

class QueuesHandle {
public:
	QueueFamily presentation{};
	QueueFamily graphic{};
	QueueFamily compute{};
	QueueFamily transfer{};

	QueuesHandle() = default;
	QueuesHandle(
		QueueFamily &&presentation,
		QueueFamily &&graphic,
		QueueFamily &&compute,
		QueueFamily &&transfer
	) :
		presentation{std::move(presentation)},
		graphic{std::move(graphic)},
		compute{std::move(compute)},
		transfer{std::move(transfer)}
	{

		presentation = QueueFamily{};
		graphic = QueueFamily{};
		compute = QueueFamily{};
		transfer = QueueFamily{};
	}

	QueuesHandle(QueuesHandle &&rhs) :
		presentation{std::move(rhs.presentation)},
		graphic{std::move(rhs.graphic)},
		compute{std::move(rhs.compute)},
		transfer{std::move(rhs.transfer)}
	{

		rhs.presentation = QueueFamily{};
		rhs.graphic = QueueFamily{};
		rhs.compute = QueueFamily{};
		rhs.transfer = QueueFamily{};
	}

	QueuesHandle & operator=(QueuesHandle &&rhs) {
		presentation = std::move(rhs.presentation);
		graphic = std::move(rhs.graphic);
		compute = std::move(rhs.compute);
		transfer = std::move(rhs.transfer);

		rhs.presentation = QueueFamily{};
		rhs.graphic = QueueFamily{};
		rhs.compute = QueueFamily{};
		rhs.transfer = QueueFamily{};

		return *this;
	}

	QueuesHandle(const QueuesHandle &rhs) = default;
	QueuesHandle & operator=(const QueuesHandle &rhs) = default;
};

} // vlk

} // rn
