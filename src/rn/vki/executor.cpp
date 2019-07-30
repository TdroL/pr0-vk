#include "executor.hpp"

#include <vulkan/vulkan.hpp>

#include "../../ngn/log.hpp"
#include "../../ngn/prof.hpp"
#include "../../util/trivialVector.hpp"
#include "../types.hpp"
#include "trace.hpp"

namespace rn::vki {

struct AccessInfo {
	vk::PipelineStageFlags stageMask{};
	vk::AccessFlags accessMask{};
	vk::ImageLayout imageLayout{};
};

AccessInfo getAccessInfo(rn::BufferAccess access) {
	switch (access) {
		default: [[fallthrough]];
		case rn::BufferAccess::None: {
			return {};
		}
		// read
		case rn::BufferAccess::IndirectBufferRead: {
			// Read as an indirect buffer for drawing or dispatch (THSVS_ACCESS_INDIRECT_BUFFER)
			return {
				vk::PipelineStageFlagBits::eDrawIndirect,
				vk::AccessFlagBits::eIndirectCommandRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::IndexBufferRead: {
			// Read as an index buffer for drawing (THSVS_ACCESS_INDEX_BUFFER)
			return {
				vk::PipelineStageFlagBits::eVertexInput,
				vk::AccessFlagBits::eIndexRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::VertexBufferRead: {
			// Read as a vertex buffer for drawing (THSVS_ACCESS_VERTEX_BUFFER)
			return {
				vk::PipelineStageFlagBits::eVertexInput,
				vk::AccessFlagBits::eVertexAttributeRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::VertexShaderUniformBufferRead: {
			// Read as a uniform buffer in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eVertexShader,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::VertexShaderUniformTexelBufferRead: {
			// Read as a sampled image/uniform texel buffer in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eVertexShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::TessellationControlShaderUniformBufferRead: {
			// Read as a uniform buffer in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eTessellationControlShader,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::TessellationControlShaderUniformTexelBufferRead: {
			// Read as a sampled image/uniform texel buffer  in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eTessellationControlShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::TessellationEvaluationShaderUniformBufferRead: {
			// Read as a uniform buffer in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eTessellationEvaluationShader,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::TessellationEvaluationShaderUniformTexelBufferRead: {
			// Read as a sampled image/uniform texel buffer in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eTessellationEvaluationShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::GeometryShaderUniformBufferRead: {
			// Read as a uniform buffer in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eGeometryShader,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::GeometryShaderUniformTexelBufferRead: {
			// Read as a sampled image/uniform texel buffer  in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eGeometryShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::FragmentShaderUniformBufferRead: {
			// Read as a uniform buffer in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::FragmentShaderUniformTexelBufferRead: {
			// Read as a sampled image/uniform texel buffer  in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::ComputeShaderUniformBufferRead: {
			// Read as a uniform buffer in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eComputeShader,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::ComputeShaderUniformTexelBufferRead: {
			// Read as a sampled image/uniform texel buffer in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eComputeShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::AnyShaderUniformBufferRead: {
			// Read as a uniform buffer in any shader (THSVS_ACCESS_ANY_SHADER_READ_UNIFORM_BUFFER)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eUniformRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::AnyShaderUniformBufferOrVertexBufferRead: {
			// Read as a uniform buffer in any shader, or a vertex buffer (THSVS_ACCESS_ANY_SHADER_READ_UNIFORM_BUFFER_OR_VERTEX_BUFFER)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eUniformRead | vk::AccessFlagBits::eVertexAttributeRead,
				vk::ImageLayout::eUndefined
			};
		}
		case rn::BufferAccess::AnyShaderUniformTexelBufferRead: {
			// Read as a sampled image in any shader (THSVS_ACCESS_ANY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::BufferAccess::TransferRead: {
			// Read as the source of a transfer operation (THSVS_ACCESS_TRANSFER_READ)
			return {
				vk::PipelineStageFlagBits::eTransfer,
				vk::AccessFlagBits::eTransferRead,
				vk::ImageLayout::eTransferSrcOptimal
			};
		}
		case rn::BufferAccess::HostRead: {
			// Read on the host (THSVS_ACCESS_HOST_READ)
			return {
				vk::PipelineStageFlagBits::eHost,
				vk::AccessFlagBits::eHostRead,
				vk::ImageLayout::eGeneral
			};
		}
		// write
		case rn::BufferAccess::VertexShaderWrite: {
			// Written as any resource in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eVertexShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::TessellationControlShaderWrite: {
			// Written as any resource in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eTessellationControlShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::TessellationEvaluationShaderWrite: {
			// Written as any resource in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eTessellationEvaluationShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::GeometryShaderWrite: {
			// Written as any resource in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eGeometryShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::FragmentShaderWrite: {
			// Written as any resource in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::ComputeShaderWrite: {
			// Written as any resource in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eComputeShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::AnyShaderWrite: {
			// Written as any resource in any shader (THSVS_ACCESS_ANY_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::TransferWrite: {
			// Written as the destination of a transfer operation (THSVS_ACCESS_TRANSFER_WRITE)
			return {
				vk::PipelineStageFlagBits::eTransfer,
				vk::AccessFlagBits::eTransferWrite,
				vk::ImageLayout::eTransferDstOptimal
			};
		}
		case rn::BufferAccess::HostPreinitialized: {
			// Data pre-filled by host before device access starts (THSVS_ACCESS_HOST_PREINITIALIZED)
			return {
				vk::PipelineStageFlagBits::eHost,
				vk::AccessFlagBits::eHostWrite,
				vk::ImageLayout::ePreinitialized
			};
		}
		case rn::BufferAccess::HostWrite: {
			// Written on the host (THSVS_ACCESS_HOST_WRITE)
			return {
				vk::PipelineStageFlagBits::eHost,
				vk::AccessFlagBits::eHostWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::BufferAccess::General: {
			// Covers any access - useful for debug, generally avoid for performance reasons (THSVS_ACCESS_GENERAL)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
				vk::ImageLayout::eGeneral
			};
		}
	}
}

AccessInfo getAccessInfo(rn::TextureAccess access) {
	switch (access) {
		default: [[fallthrough]];
		case rn::TextureAccess::None: {
			return {};
		}
		// read
		case rn::TextureAccess::VertexShaderSampledRead: {
			// Read as a sampled image/uniform texel buffer in a vertex shader (THSVS_ACCESS_VERTEX_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eVertexShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::TessellationControlShaderSampledRead: {
			// Read as a sampled image/uniform texel buffer  in a tessellation control shader (THSVS_ACCESS_TESSELLATION_CONTROL_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eTessellationControlShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::TessellationEvaluationShaderSampledRead: {
			// Read as a sampled image/uniform texel buffer in a tessellation evaluation shader (THSVS_ACCESS_TESSELLATION_EVALUATION_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eTessellationEvaluationShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::GeometryShaderSampledRead: {
			// Read as a sampled image/uniform texel buffer  in a geometry shader (THSVS_ACCESS_GEOMETRY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eGeometryShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::FragmentShaderSampledRead: {
			// Read as a sampled image/uniform texel buffer  in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::FragmentShaderColorInputAttachmentRead: {
			// Read as an input attachment with a color format in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_COLOR_INPUT_ATTACHMENT)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eInputAttachmentRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::FragmentShaderDepthStencilInputAttachmentRead: {
			// Read as an input attachment with a depth/stencil format in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_READ_DEPTH_STENCIL_INPUT_ATTACHMENT)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eInputAttachmentRead,
				vk::ImageLayout::eDepthStencilReadOnlyOptimal
			};
		}
		case rn::TextureAccess::ColorAttachmentRead: {
			// Read by standard blending/logic operations or subpass load operations (THSVS_ACCESS_COLOR_ATTACHMENT_READ)
			return {
				vk::PipelineStageFlagBits::eColorAttachmentOutput,
				vk::AccessFlagBits::eColorAttachmentRead,
				vk::ImageLayout::eColorAttachmentOptimal
			};
		}
		case rn::TextureAccess::DepthStencilAttachmentRead: {
			// Read by depth/stencil tests or subpass load operations (THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ)
			return {
				vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
				vk::AccessFlagBits::eDepthStencilAttachmentRead,
				vk::ImageLayout::eDepthStencilReadOnlyOptimal
			};
		}
		case rn::TextureAccess::ComputeShaderSampledRead: {
			// Read as a sampled image/uniform texel buffer in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eComputeShader,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::AnyShaderSampledRead: {
			// Read as a sampled image in any shader (THSVS_ACCESS_ANY_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eShaderRead,
				vk::ImageLayout::eShaderReadOnlyOptimal
			};
		}
		case rn::TextureAccess::TransferRead: {
			// Read as the source of a transfer operation (THSVS_ACCESS_TRANSFER_READ)
			return {
				vk::PipelineStageFlagBits::eTransfer,
				vk::AccessFlagBits::eTransferRead,
				vk::ImageLayout::eTransferSrcOptimal
			};
		}
		// write
		case rn::TextureAccess::Present: {
			// Read by the presentation engine (i.e. vkQueuePresentKHR) (THSVS_ACCESS_PRESENT)
			return {
				vk::PipelineStageFlags{},
				vk::AccessFlags{},
				vk::ImageLayout::ePresentSrcKHR
			};
		}
		case rn::TextureAccess::FragmentShaderWrite: {
			// Written as any resource in a fragment shader (THSVS_ACCESS_FRAGMENT_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eFragmentShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::TextureAccess::ColorAttachmentWrite: {
			// Written as a color attachment during rendering, or via a subpass store op (THSVS_ACCESS_COLOR_ATTACHMENT_WRITE)
			return {
				vk::PipelineStageFlagBits::eColorAttachmentOutput,
				vk::AccessFlagBits::eColorAttachmentWrite,
				vk::ImageLayout::eColorAttachmentOptimal
			};
		}
		case rn::TextureAccess::DepthStencilAttachmentWrite: {
			// Written as a depth/stencil attachment during rendering, or via a subpass store op (THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE)
			return {
				vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
				vk::AccessFlagBits::eDepthStencilAttachmentWrite,
				vk::ImageLayout::eDepthStencilAttachmentOptimal
			};
		}
		case rn::TextureAccess::ComputeShaderWrite: {
			// Written as any resource in a compute shader (THSVS_ACCESS_COMPUTE_SHADER_WRITE)
			return {
				vk::PipelineStageFlagBits::eComputeShader,
				vk::AccessFlagBits::eShaderWrite,
				vk::ImageLayout::eGeneral
			};
		}
		case rn::TextureAccess::TransferWrite: {
			// Written as the destination of a transfer operation (THSVS_ACCESS_TRANSFER_WRITE)
			return {
				vk::PipelineStageFlagBits::eTransfer,
				vk::AccessFlagBits::eTransferWrite,
				vk::ImageLayout::eTransferDstOptimal
			};
		}
		case rn::TextureAccess::HostPreinitialized: {
			// Data pre-filled by host before device access starts (THSVS_ACCESS_HOST_PREINITIALIZED)
			return {
				vk::PipelineStageFlagBits::eHost,
				vk::AccessFlagBits::eHostWrite,
				vk::ImageLayout::ePreinitialized
			};
		}
		case rn::TextureAccess::HostWrite: {
			// Written on the host (THSVS_ACCESS_HOST_WRITE)
			return {
				vk::PipelineStageFlagBits::eHost,
				vk::AccessFlagBits::eHostWrite,
				vk::ImageLayout::eGeneral
			};
		}
		// read-write
		case rn::TextureAccess::DepthAttachmentWriteStencilRead: {
			// Written as a depth aspect of a depth/stencil attachment during rendering, whilst the stencil aspect is read-only (THSVS_ACCESS_DEPTH_ATTACHMENT_WRITE_STENCIL_READ_ONLY)
			return {
				vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
				vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead,
				vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimalKHR
			};
		}
		case rn::TextureAccess::StencilAttachmentWriteDepthRead: {
			// Written as a stencil aspect of a depth/stencil attachment during rendering, whilst the depth aspect is read-only (THSVS_ACCESS_STENCIL_ATTACHMENT_WRITE_DEPTH_READ_ONLY)
			return {
				vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
				vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead,
				vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimalKHR
			};
		}
		case rn::TextureAccess::ColorAttachmentWriteRead: {
			// Read or written as a color attachment during rendering (THSVS_ACCESS_COLOR_ATTACHMENT_READ_WRITE)
			return {
				vk::PipelineStageFlagBits::eColorAttachmentOutput,
				vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
				vk::ImageLayout::eColorAttachmentOptimal
			};
		}
		case rn::TextureAccess::General: {
			// Covers any access - useful for debug, generally avoid for performance reasons (THSVS_ACCESS_GENERAL)
			return {
				vk::PipelineStageFlagBits::eAllCommands,
				vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
				vk::ImageLayout::eGeneral
			};
		}
	}
}

uint32_t getQueueFamily(rn::QueueType queueType, const rn::vki::Context &context) {
	switch (queueType) {
		case rn::QueueType::Graphic: {
			return context.queueFamilyIndex.graphic.family;
		}
		case rn::QueueType::Compute: {
			return context.queueFamilyIndex.compute.family;
		}
		case rn::QueueType::Transfer: {
			return context.queueFamilyIndex.transfer.family;
		}
		case rn::QueueType::Presentation: {
			return context.queueFamilyIndex.presentation.family;
		}
		default: {
			return VK_QUEUE_FAMILY_IGNORED;
		}
	}
}

void applyCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::TransitionCommand &command, const rn::vki::Context &context) {
	if (command.textures.empty() && command.buffers.empty()) {
		return;
	}

	vk::PipelineStageFlags srcStageMask{};
	vk::PipelineStageFlags dstStageMask{};

	util::TrivialVector<vk::BufferMemoryBarrier, 1> bufferMemoryBarriers{};
	util::TrivialVector<vk::ImageMemoryBarrier, 1> imageMemoryBarriers{};

	bufferMemoryBarriers.reserve(command.buffers.size());
	imageMemoryBarriers.reserve(command.textures.size());

	for (auto &&transition : command.buffers) {
		const auto oldAccessInfo = getAccessInfo(transition.oldAccess);
		const auto newAccessInfo = getAccessInfo(transition.newAccess);

		srcStageMask = (srcStageMask | oldAccessInfo.stageMask);
		dstStageMask = (dstStageMask | newAccessInfo.stageMask);

		uint32_t srcQueueFamilyIndex = transition.oldQueueType != rn::QueueType::None ? getQueueFamily(transition.oldQueueType, context) : getQueueFamily(rn::QueueType::Transfer, context);
		uint32_t dstQueueFamilyIndex = transition.newQueueType != rn::QueueType::None ? getQueueFamily(transition.newQueueType, context) : getQueueFamily(rn::QueueType::Transfer, context);

		vk::Buffer buffer = context.bufferSlots[transition.handle.index].buffer.get();

		bufferMemoryBarriers.push_back({
			/*.srcAccessMask=*/ oldAccessInfo.accessMask,
			/*.dstAccessMask=*/ newAccessInfo.accessMask,
			/*.srcQueueFamilyIndex=*/ srcQueueFamilyIndex != dstQueueFamilyIndex ? srcQueueFamilyIndex : VK_QUEUE_FAMILY_IGNORED,
			/*.dstQueueFamilyIndex=*/ srcQueueFamilyIndex != dstQueueFamilyIndex ? dstQueueFamilyIndex : VK_QUEUE_FAMILY_IGNORED,
			/*.buffer=*/ buffer,
			/*.offset=*/ static_cast<vk::DeviceSize>(transition.offset),
			/*.size=*/ static_cast<vk::DeviceSize>(transition.size),
		});
	}

	for (auto &&transition : command.textures) {
		const auto oldAccessInfo = getAccessInfo(transition.oldAccess);
		const auto newAccessInfo = getAccessInfo(transition.newAccess);

		srcStageMask = (srcStageMask | oldAccessInfo.stageMask);
		dstStageMask = (dstStageMask | newAccessInfo.stageMask);

		uint32_t srcQueueFamilyIndex = transition.oldQueueType != rn::QueueType::None ? getQueueFamily(transition.oldQueueType, context) : getQueueFamily(rn::QueueType::Transfer, context);
		uint32_t dstQueueFamilyIndex = transition.newQueueType != rn::QueueType::None ? getQueueFamily(transition.newQueueType, context) : getQueueFamily(rn::QueueType::Transfer, context);

		vk::Image image = context.textureSlots[transition.handle.index].image.get();

		imageMemoryBarriers.push_back({
			/*.srcAccessMask=*/ oldAccessInfo.accessMask,
			/*.dstAccessMask=*/ newAccessInfo.accessMask,
			/*.oldLayout=*/ oldAccessInfo.imageLayout,
			/*.newLayout=*/ newAccessInfo.imageLayout,
			/*.srcQueueFamilyIndex=*/ srcQueueFamilyIndex != dstQueueFamilyIndex ? srcQueueFamilyIndex : VK_QUEUE_FAMILY_IGNORED,
			/*.dstQueueFamilyIndex=*/ srcQueueFamilyIndex != dstQueueFamilyIndex ? dstQueueFamilyIndex : VK_QUEUE_FAMILY_IGNORED,
			/*.image=*/ image,
			/*.subresourceRange=*/ {
				/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor, // TODO: get aspect from image format
				/*.baseMipLevel=*/ transition.baseMipLevel,
				/*.levelCount=*/ transition.mipCount,
				/*.baseArrayLayer=*/ transition.baseArrayLayer,
				/*.layerCount=*/ transition.layerCount,
			},
		});
	}

	if (srcStageMask == vk::PipelineStageFlags{}) {
		srcStageMask = vk::PipelineStageFlagBits::eTopOfPipe;
	}

	if (dstStageMask == vk::PipelineStageFlags{}) {
		dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	}

	RN_VKI_TRACE(commandBufferHandle->pipelineBarrier(srcStageMask, dstStageMask, vk::DependencyFlags{}, {}, { static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data() }, { static_cast<uint32_t>(imageMemoryBarriers.size()), imageMemoryBarriers.data() }, commandBufferHandle.table()));
}

void applyCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::CopyBufferToBufferCommand &command, const rn::vki::Context &context) {
	vk::Buffer srcBuffer = context.bufferSlots[command.source.index].buffer.get();
	vk::Buffer dstBuffer = context.bufferSlots[command.destination.index].buffer.get();

	util::TrivialVector<vk::BufferCopy, decltype(command.regions)::stackCapacity> regions{};
	regions.reserve(command.regions.size());
	for (auto &&region : command.regions) {
		regions.push_back({ region.sourceOffset, region.destinationOffset, region.size });
	}

	RN_VKI_TRACE(commandBufferHandle->copyBuffer(srcBuffer, dstBuffer, { static_cast<uint32_t>(regions.size()), regions.data() }, commandBufferHandle.table()));
}

void applyCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::CopyBufferToTextureCommand &command, const rn::vki::Context &context) {
	vk::Buffer srcBuffer = context.bufferSlots[command.source.index].buffer.get();
	vk::Image dstImage = context.textureSlots[command.destination.index].image.get();

	util::TrivialVector<vk::BufferImageCopy, decltype(command.regions)::stackCapacity> regions{};
	regions.reserve(command.regions.size());
	for (auto &&region : command.regions) {
		vk::ImageSubresourceLayers subresourceLayers{
			/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor, // TODO: get aspect from image format
			/*.mipLevel=*/ region.mipLevel,
			/*.baseArrayLayer=*/ region.baseArrayLayer,
			/*.layerCount=*/ region.layerCount,
		};

		vk::Offset3D destinationOffset{static_cast<int32_t>(region.destinationOffset.x), static_cast<int32_t>(region.destinationOffset.y), static_cast<int32_t>(region.destinationOffset.z)};
		vk::Extent3D destinationExtent{region.destinationExtent.width, region.destinationExtent.height, region.destinationExtent.depth};

		regions.push_back({ region.sourceOffset, region.sourceRowLength, region.sourceRowCount, subresourceLayers, destinationOffset, destinationExtent });
	}

	RN_VKI_TRACE(commandBufferHandle->copyBufferToImage(srcBuffer, dstImage, vk::ImageLayout::eTransferDstOptimal, { static_cast<uint32_t>(regions.size()), regions.data() }, commandBufferHandle.table()));
}

void applyCommand([[maybe_unused]] rn::vki::HandleCommandBuffer &&commandBufferHandle, [[maybe_unused]] const rn::CopyTextureToTextureCommand &command, [[maybe_unused]] const rn::vki::Context &context) {
	vk::Image srcImage = context.textureSlots[command.source.index].image.get();
	vk::Image dstImage = context.textureSlots[command.destination.index].image.get();

	util::TrivialVector<vk::ImageCopy, decltype(command.regions)::stackCapacity> regions{};
	regions.reserve(command.regions.size());
	for (auto &&region : command.regions) {
		vk::ImageSubresourceLayers sourceSubresourceLayers{
			/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor, // TODO: get aspect from image format
			/*.mipLevel=*/ region.sourceMipLevel,
			/*.baseArrayLayer=*/ region.sourceBaseArrayLayer,
			/*.layerCount=*/ region.sourceLayerCount,
		};

		vk::ImageSubresourceLayers destinationSubresourceLayers{
			/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor, // TODO: get aspect from image format
			/*.mipLevel=*/ region.destinationMipLevel,
			/*.baseArrayLayer=*/ region.destinationBaseArrayLayer,
			/*.layerCount=*/ region.destinationLayerCount,
		};

		vk::Offset3D sourceOffset{static_cast<int32_t>(region.sourceOffset.x), static_cast<int32_t>(region.sourceOffset.y), static_cast<int32_t>(region.sourceOffset.z)};
		vk::Offset3D destinationOffset{static_cast<int32_t>(region.destinationOffset.x), static_cast<int32_t>(region.destinationOffset.y), static_cast<int32_t>(region.destinationOffset.z)};
		vk::Extent3D extent{region.extent.width, region.extent.height, region.extent.depth};

		regions.push_back({
			/*.srcSubresource=*/ sourceSubresourceLayers,
			/*.srcOffset=*/ sourceOffset,
			/*.dstSubresource=*/ destinationSubresourceLayers,
			/*.dstOffset=*/ destinationOffset,
			/*.extent=*/ extent,
		});
	}

	RN_VKI_TRACE(commandBufferHandle->copyImage(srcImage, vk::ImageLayout::eTransferSrcOptimal, dstImage, vk::ImageLayout::eTransferDstOptimal, { static_cast<uint32_t>(regions.size()), regions.data() }, commandBufferHandle.table()));
}

void applyCommand([[maybe_unused]] rn::vki::HandleCommandBuffer &&commandBufferHandle, [[maybe_unused]] const rn::CopyTextureToBufferCommand &command, [[maybe_unused]] const rn::vki::Context &context) {
	vk::Image srcImage = context.textureSlots[command.source.index].image.get();
	vk::Buffer dstBuffer = context.bufferSlots[command.destination.index].buffer.get();

	util::TrivialVector<vk::BufferImageCopy, decltype(command.regions)::stackCapacity> regions{};
	regions.reserve(command.regions.size());
	for (auto &&region : command.regions) {
		vk::ImageSubresourceLayers subresourceLayers{
			/*.aspectMask=*/ vk::ImageAspectFlagBits::eColor, // TODO: get aspect from image format
			/*.mipLevel=*/ region.mipLevel,
			/*.baseArrayLayer=*/ region.baseArrayLayer,
			/*.layerCount=*/ region.layerCount,
		};

		vk::Offset3D sourceOffset{static_cast<int32_t>(region.sourceOffset.x), static_cast<int32_t>(region.sourceOffset.y), static_cast<int32_t>(region.sourceOffset.z)};
		vk::Extent3D sourceExtent{region.sourceExtent.width, region.sourceExtent.height, region.sourceExtent.depth};

		regions.push_back({ region.destinationOffset, region.destinationRowLength, region.destinationRowCount, subresourceLayers, sourceOffset, sourceExtent });
	}

	RN_VKI_TRACE(commandBufferHandle->copyImageToBuffer(srcImage, vk::ImageLayout::eTransferSrcOptimal, dstBuffer, { static_cast<uint32_t>(regions.size()), regions.data() }, commandBufferHandle.table()));
}

void Executor::executeCommand([[maybe_unused]] rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::GraphicCommandVariant &commandVariant) {
	NGN_PROF_SCOPE("rn::vki::Executor::executeCommand<Graphic>");

	if (false) {
	} else {
		ngn::log::warn("rn::vki::Executor::executeCommand({}) => unknown graphic command", commandVariant.index());
	}
}

void Executor::executeCommand([[maybe_unused]] rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::ComputeCommandVariant &commandVariant) {
	NGN_PROF_SCOPE("rn::vki::Executor::executeCommand<Compute>");

	if (false) {
	} else {
		ngn::log::warn("rn::vki::Executor::executeCommand({}) => unknown compute command", commandVariant.index());
	}
}

void Executor::executeCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::TransferCommandVariant &commandVariant) {
	NGN_PROF_SCOPE("rn::vki::Executor::executeCommand<Transfer>");

	if (auto command = std::get_if<rn::TransitionCommand>(&commandVariant)) {
		applyCommand(std::move(commandBufferHandle), *command, context);
	} else if (auto command = std::get_if<rn::CopyBufferToBufferCommand>(&commandVariant)) {
		applyCommand(std::move(commandBufferHandle), *command, context);
	} else if (auto command = std::get_if<rn::CopyBufferToTextureCommand>(&commandVariant)) {
		applyCommand(std::move(commandBufferHandle), *command, context);
	} else if (auto command = std::get_if<rn::CopyTextureToTextureCommand>(&commandVariant)) {
		applyCommand(std::move(commandBufferHandle), *command, context);
	} else if (auto command = std::get_if<rn::CopyTextureToBufferCommand>(&commandVariant)) {
		applyCommand(std::move(commandBufferHandle), *command, context);
	} else {
		ngn::log::debug("rn::vki::Executor::executeCommand({}) => unknown transfer command variant", commandVariant.index());
	}
}

} // rn::vki