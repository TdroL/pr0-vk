#pragma once

#include <cassert>
#include <vector>

#include <vulkan/vulkan.h>

#include "interface.hpp"

namespace rn {

namespace vlk {

class Instance {
public:
	VkInstance handle;

	Instance(VkInstance handle = VK_NULL_HANDLE)
		: handle{handle}
	{}

	Instance(Instance &&instance)
		: handle{instance.handle}
	{
		instance.handle = VK_NULL_HANDLE;
	}

	operator =(Instance &&instance) {
		handle = instance.handle;
		instance.handle = VK_NULL_HANDLE;

		return *this;
	}

	Instance(const Instance &instance) = delete;
	operator =(const Instance &instance) = delete;

	operator VkInstance() const {
		return handle;
	}

	operator bool() const {
		return handle != VK_NULL_HANDLE;
	}

	void destroy() {
		if (handle != VK_NULL_HANDLE) {
			vkDestroyInstance(handle, nullptr);
			handle = VK_NULL_HANDLE;
		}
	}

	~Instance() {
		destroy();
	}
};

} // vlk

} // rn
