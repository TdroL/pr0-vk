#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class QueuesOwner {
public:
	vk::Queue presentation{};
	vk::Queue graphic{};
	vk::Queue compute{};
	vk::Queue transfer{};

	vk::Device device{};

	QueuesOwner() = default;
	QueuesOwner(
		vk::Queue &&presentation,
		vk::Queue &&graphic,
		vk::Queue &&compute,
		vk::Queue &&transfer,
		const vk::Device &device
	) :
		presentation{std::move(presentation)},
		graphic{std::move(graphic)},
		compute{std::move(compute)},
		transfer{std::move(transfer)},
		device{std::move(device)} {

		presentation = vk::Queue{};
		graphic = vk::Queue{};
		compute = vk::Queue{};
		transfer = vk::Queue{};
	}

	QueuesOwner(QueuesOwner &&rhs)
		:
		presentation{std::move(rhs.presentation)},
		graphic{std::move(rhs.graphic)},
		compute{std::move(rhs.compute)},
		transfer{std::move(rhs.transfer)},
		device{std::move(rhs.device)} {

		rhs.presentation = vk::Queue{};
		rhs.graphic = vk::Queue{};
		rhs.compute = vk::Queue{};
		rhs.transfer = vk::Queue{};

		rhs.device = vk::Device{};
	}

	QueuesOwner & operator=(QueuesOwner &&rhs) {
		destroy();

		presentation = std::move(rhs.presentation);
		graphic = std::move(rhs.graphic);
		compute = std::move(rhs.compute);
		transfer = std::move(rhs.transfer);

		device = std::move(rhs.device);

		rhs.presentation = vk::Queue{};
		rhs.graphic = vk::Queue{};
		rhs.compute = vk::Queue{};
		rhs.transfer = vk::Queue{};

		rhs.device = vk::Device{};

		return *this;
	}

	void destroy() {
		if (presentation) {
			presentation = vk::Queue{};
		}

		if (graphic) {
			graphic = vk::Queue{};
		}

		if (compute) {
			compute = vk::Queue{};
		}

		if (transfer) {
			transfer = vk::Queue{};
		}

		if (device) {
			device = vk::Device{};
		}
	}

	QueuesOwner(const QueuesOwner &rhs) = delete;
	QueuesOwner & operator=(const QueuesOwner &rhs) = delete;

	~QueuesOwner() {
		destroy();
	}
};

} // vlk

} // rn
