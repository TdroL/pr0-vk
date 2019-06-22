#pragma once

#include <functional>
#include <string>
#include <vector>
#include <optional>

#include "types.hpp"

namespace rn {

class BufferLayout {
public:
	using AttributeIndex = uint32_t;

	struct Attribute {
		AttributeIndex name;
		rn::DataFormat format;
	};

	struct Entry {
		AttributeIndex name;
		rn::DataFormat format;
		size_t size;
		size_t offset;
	};

	std::vector<Entry> entries;
	size_t stride;

	explicit BufferLayout(const std::vector<Attribute> &attributes) {
		entries.reserve(static_cast<size_t>(std::end(attributes) - std::begin(attributes)));

		stride = 0;
		for (const auto &attribute : attributes) {
			size_t size = rn::numberOfComponents(attribute.format);
			entries.push_back({attribute.name, attribute.format, size, stride});
			stride += size;
		}
	}

	explicit BufferLayout(std::initializer_list<Attribute> attributes) {
		entries.reserve(static_cast<size_t>(std::end(attributes) - std::begin(attributes)));

		stride = 0;
		for (const auto &attribute : attributes) {
			size_t size = rn::numberOfComponents(attribute.format);
			entries.push_back({attribute.name, attribute.format, size, stride});
			stride += size;
		}
	}

	std::reference_wrapper<const BufferLayout> handle() const {
		return std::ref(*this);
	}
};

enum class Topology {
	PointList,
	LineList,
	LineStrip,
	TriangleList,
	TriangleStrip,
};

enum class PolygonMode {
	Fill,
	Line,
	Point,
};

enum class CullMode {
	None,
	Front,
	Back,
};

enum class CompareOp {
	Always,
	Never,
	Equal,
	NotEqual,
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
};

enum class StencilOp {
	Keep,
	Zero,
	Replace,
	IncrementAndClamp,
	DecrementAndClamp,
	Invert,
	IncrementAndWrap,
	DecrementAndWrap,
};

enum class BlendOp {
	Add,
	Subtract,
	RevSubtract,
	Min,
	Max,
};

enum class ColorBlendFactor {
	Zero,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha,
	SrcAlphaSaturate,
	Src1Color,
	OneMinusSrc1Color,
	Src1Alpha,
	OneMinusSrc1Alpha,
};

enum class AlphaBlendFactor {
	Zero,
	One,
	SrcAlpha,
	OneMinusSrcAlpha,
	DestAlpha,
	OneMinusDstAlpha,
	SrcAlphaSaturate,
	ConstantAlpha,
	OneMinusConstantAlpha,
	Src1Alpha,
	OneMinusSrc1Alpha,
};

enum class LogicOp {
	Noop,
	Clear,
	And,
	AndReverse,
	Copy,
	AndInverted,
	Xor,
	Or,
	Nor,
	Equivalent,
	Invert,
	OrReverse,
	CopyInverted,
	OrInverted,
	Nand,
	Set,
};

enum class ColorComponents {
	None = 0x00000000,
	R = 0x00000001,
	G = 0x00000002,
	B = 0x00000004,
	A = 0x00000008,
	RGB = 0x00000001 | 0x00000002 | 0x00000004,
	RGBA = 0x00000001 | 0x00000002 | 0x00000004 | 0x00000008,
};

constexpr ColorComponents operator|(ColorComponents left, ColorComponents right) {
	return static_cast<ColorComponents>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

// https://computergraphics.stackexchange.com/questions/4422/directx-openglvulkan-concepts-mapping-chart

struct GraphicSubpassState {
	uint32_t subpass;
};

struct GraphicShadersState {
	std::string vertex;
	std::optional<std::string> fragment{};
	std::optional<std::string> geometry{};
	std::optional<std::string> hull{};
	std::optional<std::string> domain{};
};

struct ComputeShadersState {
	std::string compute;
};

struct InputLayoutState {
	std::vector<BufferLayout::AttributeIndex> attributes;
	std::vector<std::reference_wrapper<const BufferLayout>> buffers{};
};

struct InputAssemblyState {
	Topology topology{Topology::TriangleList};
};

struct ViewportState {
	uint32_t width;
	uint32_t height;
	uint32_t x{0};
	uint32_t y{0};
	float minDepth{1.0f};
	float maxDepth{0.0f};
};

struct ScissorState {
	uint32_t width;
	uint32_t height;
	uint32_t x{0};
	uint32_t y{0};
};

struct ViewportScissorState {
	uint32_t width;
	uint32_t height;
};

struct RasterizationState {
	CullMode cullMode{CullMode::Back};
	bool frontCounterClockwise{false};
	PolygonMode polygonMode{PolygonMode::Fill};
	float lineWidth{1.0f};
};

struct DiscardState {
	bool enable{false};
};

struct DepthClampState {
	bool enable{false};
};

struct DepthBiasState {
	bool enable{false};
	float constantFactor{0.0f};
	float clamp{0.0f};
	float slopeFactor{0.0f};
};

struct DepthState {
	bool testEnable{true};
	CompareOp compareOp{CompareOp::Greater};
	bool writeEnable{true};
};

struct StencilOpState {
	StencilOp failOp{StencilOp::Keep};
	StencilOp depthFailOp{StencilOp::Keep};
	StencilOp passOp{StencilOp::Keep};
	CompareOp compareOp{CompareOp::Always};
};

struct StencilState {
	bool enable{false};
	StencilOpState front{};
	StencilOpState back{};
	uint8_t readMask{0xff};
	uint8_t writeMask{0xff};
};

struct StencilReferenceState {
	uint8_t reference{0xff};
};

struct AlphaState {
	bool alphaToCoverageEnable{false};
	bool alphaToOneEnable{false};
};

struct MultisampleState {
	uint32_t rasterizationSamples{1u};
};

struct ColorBlendAttachmentState {
	bool enable{false};

	// color = srcColor * srcAlpha + dstColor * (1 - srcAlpha)
	ColorBlendFactor srcColor{ColorBlendFactor::SrcAlpha};
	ColorBlendFactor dstColor{ColorBlendFactor::OneMinusSrcAlpha};
	BlendOp colorOp{BlendOp::Add};

	// alpha = srcAlpha * 1 + dstAlpha * 0
	AlphaBlendFactor srcAlpha{AlphaBlendFactor::One};
	AlphaBlendFactor dstAlpha{AlphaBlendFactor::Zero};
	BlendOp alphaOp{BlendOp::Add};

	ColorComponents mask{ColorComponents::RGBA};
};

struct ColorBlendState {
	bool logicOpEnable{false};
	LogicOp logicOp{LogicOp::Noop};
	std::vector<ColorBlendAttachmentState> attachments{};
};

struct ColorBlendConstantsState {
	float constants[4]{1.0f, 1.0f, 1.0f, 1.0f};
};

struct GraphicPipelineState {
	std::optional<GraphicSubpassState> subpass{};
	std::optional<GraphicShadersState> graphicShaders{};
	std::optional<InputLayoutState> inputLayout{};
	std::optional<InputAssemblyState> inputAssembly{};
	std::optional<ViewportState> viewport{};
	std::optional<ScissorState> scissor{};
	std::optional<RasterizationState> rasterization{};
	std::optional<DiscardState> discard{};
	std::optional<DepthClampState> depthClamp{};
	std::optional<DepthBiasState> depthBias{};
	std::optional<DepthState> depth{};
	std::optional<StencilState> stencil{};
	std::optional<StencilReferenceState> stencilReference{};
	std::optional<AlphaState> alpha{};
	std::optional<MultisampleState> multisample{};
	std::optional<ColorBlendState> colorBlend{};
	std::optional<ColorBlendConstantsState> colorBlendConstants{};

	// GraphicPipelineState operator|(GraphicShadersState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.graphicShaders = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(InputLayoutState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.inputLayout = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(InputAssemblyState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.inputAssembly = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(ViewportState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.viewport = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(ScissorState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.scissor = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(ViewportScissorState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.viewport = ViewportState{state.width, state.height};
	// 	pipelineState.scissor = ScissorState{state.width, state.height};

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(RasterizationState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.rasterization = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(DiscardState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.discard = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(DepthClampState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.depthClamp = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(DepthBiasState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.depthBias = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(DepthState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.depth = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(StencilState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.stencil = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(StencilReferenceState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.stencilReference = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(AlphaState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.alpha = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(MultisampleState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.multisample = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(ColorBlendState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.colorBlend = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(ColorBlendConstantsState &&state) {
	// 	GraphicPipelineState pipelineState = *this;
	// 	pipelineState.colorBlendConstants = std::move(state);

	// 	return pipelineState;
	// }

	// GraphicPipelineState operator|(const GraphicShadersState &state) {
	// 	return *this | GraphicShadersState{state};
	// }

	// GraphicPipelineState operator|(const InputLayoutState &state) {
	// 	return *this | InputLayoutState{state};
	// }

	// GraphicPipelineState operator|(const InputAssemblyState &state) {
	// 	return *this | InputAssemblyState{state};
	// }

	// GraphicPipelineState operator|(const ViewportState &state) {
	// 	return *this | ViewportState{state};
	// }

	// GraphicPipelineState operator|(const ScissorState &state) {
	// 	return *this | ScissorState{state};
	// }

	// GraphicPipelineState operator|(const ViewportScissorState &state) {
	// 	return *this | ViewportScissorState{state};
	// }

	// GraphicPipelineState operator|(const RasterizationState &state) {
	// 	return *this | RasterizationState{state};
	// }

	// GraphicPipelineState operator|(const DiscardState &state) {
	// 	return *this | DiscardState{state};
	// }

	// GraphicPipelineState operator|(const DepthClampState &state) {
	// 	return *this | DepthClampState{state};
	// }

	// GraphicPipelineState operator|(const DepthBiasState &state) {
	// 	return *this | DepthBiasState{state};
	// }

	// GraphicPipelineState operator|(const DepthState &state) {
	// 	return *this | DepthState{state};
	// }

	// GraphicPipelineState operator|(const StencilState &state) {
	// 	return *this | StencilState{state};
	// }

	// GraphicPipelineState operator|(const StencilReferenceState &state) {
	// 	return *this | StencilReferenceState{state};
	// }

	// GraphicPipelineState operator|(const AlphaState &state) {
	// 	return *this | AlphaState{state};
	// }

	// GraphicPipelineState operator|(const MultisampleState &state) {
	// 	return *this | MultisampleState{state};
	// }

	// GraphicPipelineState operator|(const ColorBlendState &state) {
	// 	return *this | ColorBlendState{state};
	// }

	// GraphicPipelineState operator|(const ColorBlendConstantsState &state) {
	// 	return *this | ColorBlendConstantsState{state};
	// }
};

GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, GraphicSubpassState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, GraphicShadersState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, InputLayoutState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, InputAssemblyState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ViewportState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ScissorState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ViewportScissorState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, RasterizationState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DiscardState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DepthClampState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DepthBiasState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, DepthState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, StencilState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, StencilReferenceState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, AlphaState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, MultisampleState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ColorBlendState &&state);
GraphicPipelineState operator|(GraphicPipelineState &&pipelineState, ColorBlendConstantsState &&state);

template<typename T>
GraphicPipelineState operator|(const GraphicPipelineState &pipelineState, const T &state) {
	return GraphicPipelineState{pipelineState} | T{state};
}

template<typename T>
GraphicPipelineState operator|(const GraphicPipelineState &pipelineState, T &&state) {
	return GraphicPipelineState{pipelineState} | std::move(state);
}

struct ComputePipelines {
	std::optional<ComputeShadersState> computeShaders{};
};

ComputePipelines operator|(ComputePipelines &&pipelineState, ComputeShadersState &&state);

template<typename T>
ComputePipelines operator|(const ComputePipelines &pipelineState, const T &state) {
	return ComputePipelines{pipelineState} | T{state};
}

template<typename T>
ComputePipelines operator|(const ComputePipelines &pipelineState, T &&state) {
	return ComputePipelines{pipelineState} | std::move(state);
}

} // rn
