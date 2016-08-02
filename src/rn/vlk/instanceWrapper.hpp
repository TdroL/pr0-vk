#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace rn {

namespace vlk {

class InstanceWrapper {
public:
	vk::Instance handle{};

	InstanceWrapper() = default;
	explicit InstanceWrapper(vk::Instance &&instance)
		: handle{instance} {
		instance = vk::Instance{};
	}

	InstanceWrapper(InstanceWrapper &&rhs)
		: handle{rhs.handle} {
		rhs.handle = vk::Instance{};
	}

	InstanceWrapper & operator=(InstanceWrapper &&rhs) {
		handle = rhs.handle;
		rhs.handle = vk::Instance{};

		return *this;
	}

	InstanceWrapper(const InstanceWrapper &rhs) = delete;
	InstanceWrapper & operator=(const InstanceWrapper &rhs) = delete;

	~InstanceWrapper() {
		if (handle) {
			std::cout << "destroying vk::Instance" << std::endl;
			handle.destroy();
		}
	}
};

} // vlk

} // rn
