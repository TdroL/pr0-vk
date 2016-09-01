#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class InstanceOwner {
public:
	vk::Instance handle{};

	InstanceOwner() = default;
	explicit InstanceOwner(vk::Instance &&instance)
		: handle{std::move(instance)} {
		instance = vk::Instance{};
		if (handle) {
			std::cout << "creating vk::Instance" << std::endl;
		}
	}

	InstanceOwner(InstanceOwner &&rhs)
		: handle{std::move(rhs.handle)} {
		rhs.handle = vk::Instance{};
		if (handle) {
			std::cout << "creating vk::Instance" << std::endl;
		}
	}

	InstanceOwner & operator=(InstanceOwner &&rhs) {
		destroy();

		handle = std::move(rhs.handle);
		rhs.handle = vk::Instance{};

		return *this;
	}

	void destroy() {
		if (handle) {
			std::cout << "destroying vk::Instance" << std::endl;
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
