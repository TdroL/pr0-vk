#pragma once

#include <string>
#include <vector>
#include <optional>

namespace rn::graph {

using BufferLayoutHandle = uint32_t;
using AttributeIndex = uint32_t;

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
	std::vector<std::pair<BufferLayoutHandle, AttributeIndex>> attributes;
	std::optional<std::vector<BufferLayoutHandle>> buffers{};
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

struct PipelineState {
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

	PipelineState operator|(GraphicShadersState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.graphicShaders = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(InputLayoutState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.inputLayout = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(InputAssemblyState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.inputAssembly = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(ViewportState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.viewport = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(ScissorState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.scissor = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(ViewportScissorState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.viewport = ViewportState{state.width, state.height};
		pipelineState.scissor = ScissorState{state.width, state.height};

		return pipelineState;
	}

	PipelineState operator|(RasterizationState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.rasterization = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(DiscardState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.discard = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(DepthClampState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.depthClamp = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(DepthBiasState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.depthBias = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(DepthState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.depth = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(StencilState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.stencil = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(StencilReferenceState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.stencilReference = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(AlphaState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.alpha = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(MultisampleState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.multisample = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(ColorBlendState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.colorBlend = std::move(state);

		return pipelineState;
	}

	PipelineState operator|(ColorBlendConstantsState &&state) {
		PipelineState pipelineState = *this;
		pipelineState.colorBlendConstants = std::move(state);

		return pipelineState;
	}
};

} // rn::graph
