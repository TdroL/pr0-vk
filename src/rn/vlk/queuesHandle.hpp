#pragma once

#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class QueuesHandle {
public:
	vk::Queue presentation{};
	vk::Queue graphic{};
	vk::Queue compute{};
	vk::Queue transfer{};

	QueuesHandle() = default;
	QueuesHandle(
		vk::Queue &&presentation,
		vk::Queue &&graphic,
		vk::Queue &&compute,
		vk::Queue &&transfer
	) :
		presentation{std::move(presentation)},
		graphic{std::move(graphic)},
		compute{std::move(compute)},
		transfer{std::move(transfer)} {

		presentation = vk::Queue{};
		graphic = vk::Queue{};
		compute = vk::Queue{};
		transfer = vk::Queue{};
	}

	QueuesHandle(QueuesHandle &&rhs)
		:
		presentation{std::move(rhs.presentation)},
		graphic{std::move(rhs.graphic)},
		compute{std::move(rhs.compute)},
		transfer{std::move(rhs.transfer)} {

		rhs.presentation = vk::Queue{};
		rhs.graphic = vk::Queue{};
		rhs.compute = vk::Queue{};
		rhs.transfer = vk::Queue{};
	}

	QueuesHandle & operator=(QueuesHandle &&rhs) {
		presentation = std::move(rhs.presentation);
		graphic = std::move(rhs.graphic);
		compute = std::move(rhs.compute);
		transfer = std::move(rhs.transfer);

		rhs.presentation = vk::Queue{};
		rhs.graphic = vk::Queue{};
		rhs.compute = vk::Queue{};
		rhs.transfer = vk::Queue{};

		return *this;
	}

	QueuesHandle(const QueuesHandle &rhs) = delete;
	QueuesHandle & operator=(const QueuesHandle &rhs) = delete;
};

} // vlk

} // rn
