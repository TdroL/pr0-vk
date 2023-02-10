#define VMA_IMPLEMENTATION

#include "vma.hpp"

static_assert(sizeof(vma::AllocationCreateInfo) == sizeof(VmaAllocationCreateInfo));
static_assert(offsetof(vma::AllocationCreateInfo, flags) == offsetof(VmaAllocationCreateInfo, flags));
static_assert(offsetof(vma::AllocationCreateInfo, usage) == offsetof(VmaAllocationCreateInfo, usage));
static_assert(offsetof(vma::AllocationCreateInfo, requiredFlags) == offsetof(VmaAllocationCreateInfo, requiredFlags));
static_assert(offsetof(vma::AllocationCreateInfo, preferredFlags) == offsetof(VmaAllocationCreateInfo, preferredFlags));
static_assert(offsetof(vma::AllocationCreateInfo, memoryTypeBits) == offsetof(VmaAllocationCreateInfo, memoryTypeBits));
static_assert(offsetof(vma::AllocationCreateInfo, pool) == offsetof(VmaAllocationCreateInfo, pool));
static_assert(offsetof(vma::AllocationCreateInfo, pUserData) == offsetof(VmaAllocationCreateInfo, pUserData));
static_assert(offsetof(vma::AllocationCreateInfo, priority) == offsetof(VmaAllocationCreateInfo, priority));
