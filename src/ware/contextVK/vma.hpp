#pragma once

#include <utility>

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

namespace vma {

enum AllocationCreateFlagBits : std::underlying_type<VmaAllocationCreateFlagBits>::type {
	eDedicatedMemory = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
	eNeverAllocate = VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT,
	eMapped = VMA_ALLOCATION_CREATE_MAPPED_BIT,
	eUserDataCopyString = VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT,
	eUpperAddress = VMA_ALLOCATION_CREATE_UPPER_ADDRESS_BIT,
	eDontBind = VMA_ALLOCATION_CREATE_DONT_BIND_BIT,
	eWithinBudget = VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT,
	eCanAlias = VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT,
	eHostAccessSequentialWrite = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
	eHostAccessRandom = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
	eHostAccessAllowTransferInstead = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT,
	eStrategyMinMemory = VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT,
	eStrategyMinTime = VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT,
	eStrategyMinOffset = VMA_ALLOCATION_CREATE_STRATEGY_MIN_OFFSET_BIT,
};

using AllocationCreateFlags = VmaAllocationCreateFlags;

enum class MemoryUsage : std::underlying_type<VmaMemoryUsage>::type {
	eUnknown = VMA_MEMORY_USAGE_UNKNOWN,
	eGpuLazilyAllocated = VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED,
	eAuto = VMA_MEMORY_USAGE_AUTO,
	eAutoPreferDevice = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
	eAutoPreferHost = VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
};

struct AllocationCreateInfo {
	AllocationCreateFlags flags = {};
	MemoryUsage usage = MemoryUsage::eAuto;
	vk::MemoryPropertyFlags requiredFlags = {};
	vk::MemoryPropertyFlags preferredFlags = {};
	uint32_t memoryTypeBits = {};
	VmaPool pool = {};
	void* pUserData = {};
	float priority = {};
};

} // vma
