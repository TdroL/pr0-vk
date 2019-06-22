#include "pipeline.hpp"

namespace rn {

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, GraphicSubpassState &&state) {
	pipelineState.subpass = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, GraphicShadersState &&state) {
	pipelineState.graphicShaders = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, InputLayoutState &&state) {
	pipelineState.inputLayout = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, InputAssemblyState &&state) {
	pipelineState.inputAssembly = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ViewportState &&state) {
	pipelineState.viewport = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ScissorState &&state) {
	pipelineState.scissor = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ViewportScissorState &&state) {
	pipelineState.viewport = ViewportState{state.width, state.height};
	pipelineState.scissor = ScissorState{state.width, state.height};

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, RasterizationState &&state) {
	pipelineState.rasterization = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DiscardState &&state) {
	pipelineState.discard = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DepthClampState &&state) {
	pipelineState.depthClamp = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DepthBiasState &&state) {
	pipelineState.depthBias = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DepthState &&state) {
	pipelineState.depth = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, StencilState &&state) {
	pipelineState.stencil = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, StencilReferenceState &&state) {
	pipelineState.stencilReference = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, AlphaState &&state) {
	pipelineState.alpha = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, MultisampleState &&state) {
	pipelineState.multisample = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ColorBlendState &&state) {
	pipelineState.colorBlend = std::move(state);

	return pipelineState;
}

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ColorBlendConstantsState &&state) {
	pipelineState.colorBlendConstants = std::move(state);

	return pipelineState;
}

ComputePipelines operator|(ComputePipelines &&pipelineState, ComputeShadersState &&state) {
	pipelineState.computeShaders = std::move(state);

	return pipelineState;
}

} // rn