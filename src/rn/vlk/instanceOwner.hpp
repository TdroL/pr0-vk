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
		: handle{instance} {
		instance = vk::Instance{};
		if (handle) {
			std::cout << "creating vk::Instance" << std::endl;
		}
	}

	InstanceOwner(InstanceOwner &&rhs)
		: handle{rhs.handle} {
		rhs.handle = vk::Instance{};
		if (handle) {
			std::cout << "creating vk::Instance" << std::endl;
		}
	}

	InstanceOwner & operator=(InstanceOwner &&rhs) {
		handle = rhs.handle;
		rhs.handle = vk::Instance{};

		return *this;
	}

	InstanceOwner(const InstanceOwner &rhs) = delete;
	InstanceOwner & operator=(const InstanceOwner &rhs) = delete;

	~InstanceOwner() {
		if (handle) {
			std::cout << "destroying vk::Instance" << std::endl;
			handle.destroy();
		}
	}
};

} // vlk

} // rn
