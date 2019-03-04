#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace rn::vki {

template<typename T>
uintptr_t id(const T &handle);

template<typename T, typename D>
uintptr_t id(const vk::UniqueHandle<T, D> &handle) {
	return id(handle.get());
}

} // rn::vki
