#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

#include "../../ngn/event.hpp"

namespace rn {

namespace vlk {

NGN_EVENT(EventInstanceDestroy) {
	const vk::Instance &instance;

	explicit EventInstanceDestroy(const vk::Instance &instance) : instance(instance) {}
};

class InstanceOwner {
public:
	vk::Instance handle{};

	InstanceOwner() = default;
	explicit InstanceOwner(vk::Instance &&instance)
		: handle{std::move(instance)} {
		instance = vk::Instance{};
	}

	InstanceOwner(InstanceOwner &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = vk::Instance{};
	}

	InstanceOwner & operator=(InstanceOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		rhs.handle = vk::Instance{};

		return *this;
	}

	void destroy() {
		if (handle) {
			ngn::event::emit(EventInstanceDestroy{handle});

			handle.destroy();
			handle = vk::Instance{};
		}
	}

	InstanceOwner(const InstanceOwner &rhs) = delete;
	InstanceOwner & operator=(const InstanceOwner &rhs) = delete;

	~InstanceOwner() {
		destroy();
	}
};

} // vlk

} // rn
