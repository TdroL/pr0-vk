#include "id.hpp"

#include <cstring>

namespace rn::vki {

// TODO: replace with <bit> std::bit_cast()
#define X(type) template<> \
uintptr_t id(const vk::type &handle) { \
	static_assert(sizeof(uintptr_t) == sizeof(Vk ## type)); \
	uintptr_t result = 0; \
	Vk ## type vkHandle = static_cast<Vk ## type>(handle); \
	std::memcpy(&result, &vkHandle, sizeof(vkHandle)); \
	return result; \
}

X(Buffer)
X(BufferView)
X(CommandBuffer)
X(CommandPool)
X(DebugReportCallbackEXT)
X(DescriptorPool)
X(DescriptorSet)
X(DescriptorSetLayout)
X(DescriptorUpdateTemplateKHR)
X(Device)
X(DeviceMemory)
X(Event)
X(Fence)
X(Framebuffer)
X(Image)
X(ImageView)
X(IndirectCommandsLayoutNVX)
X(Instance)
X(ObjectTableNVX)
X(PhysicalDevice)
X(Pipeline)
X(PipelineCache)
X(PipelineLayout)
X(QueryPool)
X(RenderPass)
X(Sampler)
X(SamplerYcbcrConversionKHR)
X(Semaphore)
X(ShaderModule)
X(SurfaceKHR)

#undef X

} // rn::vki
