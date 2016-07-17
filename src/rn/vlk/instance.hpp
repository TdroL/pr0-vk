#pragma once

#include <cassert>
#include <vector>

#include <vulkan/vulkan.h>

#include "interface.hpp"

namespace rn {

namespace vlk {

class Instance {
public:
	VkInstance handler;

	Instance(VkInstance handler)
		: handler{handler}
	{}

	Instance(Instance &&instance)
		: handler{instance.handler}
	{
		instance.handler = VK_NULL_HANDLE;
	}

	Instance(const Instance &instance) = delete;
	operator =(Instance &&instance) = delete;
	operator =(const Instance &instance) = delete;

	operator VkInstance() const {
		return handler;
	}

	operator bool() const {
		return handler != VK_NULL_HANDLE;
	}

	void destroy() {
		if (handler != VK_NULL_HANDLE) {
			vkDestroyInstance(instance, nullptr);
			handler = VK_NULL_HANDLE;
		}
	}

	~Instance() {
		destroy();
	}
};

} // vlk

} // rn
