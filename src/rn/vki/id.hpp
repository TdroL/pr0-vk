#pragma once

#include <vulkan/vulkan.hpp>

namespace rn::vki {

template<typename T>
void * id(const T &handle);

template<typename T, typename D>
void * id(const vk::UniqueHandle<T, D> &handle) {
	return id(handle.get());
}

} // rn::vki
