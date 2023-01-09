#pragma once

#include <vulkan/vulkan.hpp>

#if defined(_MSVC_LANG)
	#pragma warning(push, 4)
	#pragma warning(disable: 4127) // conditional expression is constant
	#pragma warning(disable: 4100) // unreferenced formal parameter
	#pragma warning(disable: 4189) // local variable is initialized but not referenced
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier

	#include <vk_mem_alloc.h>

	#pragma warning(pop)
#elif defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wconversion"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	#pragma GCC diagnostic ignored "-Wparentheses"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

	#include <vk_mem_alloc.h>

	#pragma GCC diagnostic pop
#elif defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wtautological-compare" // comparison of unsigned expression < 0 is always false
	#pragma clang diagnostic ignored "-Wunused-private-field"
	#pragma clang diagnostic ignored "-Wunused-parameter"
	#pragma clang diagnostic ignored "-Wmissing-field-initializers"
	#pragma clang diagnostic ignored "-Wnullability-completeness"

	#include <vk_mem_alloc.h>

	#pragma clang diagnostic pop
#endif
