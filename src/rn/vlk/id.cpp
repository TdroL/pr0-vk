#include "id.hpp"

namespace rn::vlk {

#define ID(type) template<> \
void * id(const vk::type &handle) { \
	return reinterpret_cast<void *>(static_cast<Vk ## type>(handle)); \
}

ID(Buffer)
ID(BufferView)
ID(CommandBuffer)
ID(CommandPool)
ID(DebugReportCallbackEXT)
ID(DescriptorPool)
ID(DescriptorSet)
ID(DescriptorSetLayout)
ID(DescriptorUpdateTemplateKHR)
ID(Device)
ID(DeviceMemory)
ID(Event)
ID(Fence)
ID(Framebuffer)
ID(Image)
ID(ImageView)
ID(IndirectCommandsLayoutNVX)
ID(Instance)
ID(ObjectTableNVX)
ID(Pipeline)
ID(PipelineCache)
ID(PipelineLayout)
ID(QueryPool)
ID(RenderPass)
ID(Sampler)
ID(SamplerYcbcrConversionKHR)
ID(Semaphore)
ID(ShaderModule)

} // rn::vlk
