#include "renderer.hpp"

#include <glm/glm.hpp>

#include <ngn/prof.hpp>

namespace app {

struct UniformBufferDraw {
	alignas(16) glm::mat4 M;
	alignas(16) uint32_t texIdxOffset;
	// alignas(16) glm::ivec4 texIdx; // normal, diffuse, specular, metalness
};

struct UniformBufferModel {
};

struct UniformBufferPipeline {
	alignas(16) glm::mat4 V;
};

struct UniformBufferRenderpass {
	alignas(16) glm::mat4 P;
};

struct UniformBufferGlobal {

};

rn::graph::CompileResult Renderer::compile() {
	rn::graph::Passes passes{};

	enum VertexAttribute : uint32_t {
		Position = 0,
		Normal = 1,
		TexCoords = 2,
	};

	const rn::BufferLayout vertexBufferPositionLayout{
		{ /*.name=*/ VertexAttribute::Position,  /*.format=*/ rn::DataFormat::Float3 },
	};

	const rn::BufferLayout vertexBufferNormalCoordsLayout{
		{ /*.name=*/ VertexAttribute::Normal,    /*.format=*/ rn::DataFormat::Float3 },
		{ /*.name=*/ VertexAttribute::TexCoords, /*.format=*/ rn::DataFormat::Float2 },
	};

	// VK_SHADER_STAGE_VERTEX_BIT
	// VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
	// VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT
	// VK_SHADER_STAGE_GEOMETRY_BIT
	// VK_SHADER_STAGE_FRAGMENT_BIT
	// VK_SHADER_STAGE_COMPUTE_BIT

	// const rn::DescriptorSetLayout descriptorSetLayout{
		// // per draw
		// {
		// 	{
		// 		/*.type=*/ rn::DescriptorType::UniformBuffer,
		// 		// /*.count=*/ 3,
		// 		/*.stages=*/ rn::ShaderStage::Vertex | rn::ShaderStage::Fragment,
		// 	}
		// },
		// // per vertex/index buffer
		// {
		// 	{
		// 		/*.type=*/ rn::DescriptorType::SampledTexture,
		// 		/*.count=*/ 1,
		// 		/*.stages=*/ rn::ShaderStage::Fragment,
		// 	}
		// },
		// // per pipeline
		// {

		// },
		// // per renderpass
		// {
		// 	{
		// 		/*.type=*/ rn::DescriptorType::Sampler,
		// 		/*.count=*/ 1,
		// 		/*.stages=*/ rn::ShaderStage::Fragment,
		// 	}
		// }
		// "uniform",
		// "dynamic-uniform",
		// "storage",
		// "dynamic-storage",
		// "sampler",
		// "texture",
	// };

	// rn::GraphicPipelineState baseGraphicPipelineState = rn::GraphicPipelineState{}
	// 	| rn::ViewportScissorState{
	// 		/*.width=*/ window.currentProperties.width,
	// 		/*.height=*/ window.currentProperties.height
	// 	};

	const rn::Extent3D windowExtents{ window.currentProperties.width, window.currentProperties.height };

	passes.push_back(rn::graph::GraphicPass{"z-prepass", { windowExtents }, [&] () {
		rn::graph::ResourceDescriptors resources{};

		const auto depthTex = resources.texture.create("app/rn:depth", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto depthSubpass = subpasses.add("depth", {}, /*rn::graph::GraphicSubpassDesc*/{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Modify,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicPipelines pipelines{};

		const auto depthPipeline = pipelines.add("depth", rn::GraphicPipelineState{}
			| rn::GraphicSubpassState{
				/*.subpass=*/ depthSubpass.index,
			}
			| rn::GraphicShadersState{
				/*.vertex=*/ "shaders/zpass.vert"
			}
			| rn::InputLayoutState{
				/*.attributes=*/ {
					VertexAttribute::Position
				},
				/*.buffers=*/ {
					/*[0]=*/ vertexBufferPositionLayout.handle()
				}
			}
		);

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(depthSubpass, [=] () {
				ngn::log::debug("zpass:depthTex={}", depthTex.index);
				ngn::log::debug("zpass:depthSubpass={}", depthSubpass.index);
				ngn::log::debug("zpass:depthPipeline={}", depthPipeline.index);
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(pipelines), std::move(recorders) };
	}});
#if 0
	passes.push_back(rn::graph::GraphicPass{"cascade-shadowmap", { 2048, 2048 }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto cascadeShadowmapTex = resources.texture.create("app/rn:cascade-shadowmap", {
			/*.format=*/ rn::PixelFormat::D32Float,
			/*.dimensions=*/ { 4 * 2048, 2048 },
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto cascadeShadowmapSubpass = subpasses.add("cascade-shadowmap", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ cascadeShadowmapTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Write,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(cascadeShadowmapSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

	passes.push_back(rn::graph::GraphicPass{"spotlight-shadowmap", { 1024, 1024 }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto spotlightShadowmapTex = resources.texture.create("app/rn:spotlight-shadowmap", {
			/*.format=*/ rn::PixelFormat::D32Float,
			/*.dimensions=*/ { 4 * 2048, 2048 },
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto spotlightShadowmapSubpass = subpasses.add("spotlight-shadowmap", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ spotlightShadowmapTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Write,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(spotlightShadowmapSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

	passes.push_back(rn::graph::GraphicPass{"sao", { windowExtents }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto depthTex = resources.texture.read("app/rn:depth");

		const auto saoTempTex = resources.texture.create("app/rn:sao-temp", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		const auto saoTex = resources.texture.create("app/rn:sao", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto saoNoiseSubpass = subpasses.add("sao-noise", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { saoTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		const auto saoBlurHSubpass = subpasses.add("sao-blur-h", { saoNoiseSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ {},
			/*.outputs=*/ { saoTempTex },
			/*.textures=*/ {
				{
					/*.texture=*/ saoTex,
					/*.usage=*/ rn::graph::GraphicTextureUsage::Sampled,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			},
			/*.buffers=*/ {}
		});

		const auto saoBlurVSubpass = subpasses.add("sao-blur-v", { saoBlurHSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ {},
			/*.outputs=*/ { saoTex },
			/*.textures=*/ {
				{
					/*.texture=*/ saoTempTex,
					/*.usage=*/ rn::graph::GraphicTextureUsage::Sampled,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(saoNoiseSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
			std::make_tuple(saoBlurHSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
			std::make_tuple(saoBlurVSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

	passes.push_back(rn::graph::ComputePass{"classify", [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto depthTex = resources.texture.read("app/rn:depth");

		const auto lightGridBuf = resources.buffer.create("app/rn:light-grid", {
			/*.size=*/ 1024,
			/*.usage=*/ rn::BufferUsage::Uniform
		});

		const auto lightListBuf = resources.buffer.create("app/rn:light-list", {
			/*.size=*/ 1024,
			/*.usage=*/ rn::BufferUsage::Uniform
		});

		rn::graph::ComputeSubpasses subpasses{};

		const auto classifySubpass = subpasses.add("classify", {}, rn::graph::ComputeSubpassDesc{
			/*.textures=*/ {
				{
					/*.texture=*/ depthTex,
					/*.usage=*/ rn::graph::ComputeTextureUsage::Sampled
				}
			},
			/*.buffers=*/ {
				{
					/*.buffer=*/ lightGridBuf,
					/*.usage=*/ rn::graph::ComputeBufferUsage::Storage
				},
				{
					/*.buffer=*/ lightListBuf,
					/*.usage=*/ rn::graph::ComputeBufferUsage::Storage
				}
			}
		});

		rn::graph::ComputeSubpassRecorders recorders{
			std::make_tuple(classifySubpass, [=] () {
				return rn::graph::ComputeCommandList{};
			}),
		};

		return rn::graph::ComputeSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

	passes.push_back(rn::graph::GraphicPass{"gbuffer", { windowExtents }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto depthTex = resources.texture.read("app/rn:depth");

		const auto gbufferNormalTex = resources.texture.create("app/rn:gbuffer-normal", {
			/*.format=*/ rn::PixelFormat::R16G16Float,
		});

		const auto gbufferAlbedoTex = resources.texture.create("app/rn:gbuffer-albedo", {
			/*.format=*/ rn::PixelFormat::R8G8B8A8UNorm,
		});

		const auto gbufferMetalnessRoughnessTex = resources.texture.create("app/rn:gbuffer-metalness-roughness", {
			/*.format=*/ rn::PixelFormat::R8G8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto gbufferSubpass = subpasses.add("gbuffer", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { gbufferNormalTex, gbufferAlbedoTex, gbufferMetalnessRoughnessTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(gbufferSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

	passes.push_back(rn::graph::GraphicPass{"lighting", { windowExtents }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto depthTex = resources.texture.read("app/rn:depth");
		const auto spotlightShadowmapTex = resources.texture.read("app/rn:spotlight-shadowmap");
		const auto cascadeShadowmapTex = resources.texture.read("app/rn:cascade-shadowmap");
		const auto gbufferNormalTex = resources.texture.read("app/rn:gbuffer-normal");
		const auto gbufferAlbedoTex = resources.texture.read("app/rn:gbuffer-albedo");
		const auto gbufferMetalnessRoughnessTex = resources.texture.read("app/rn:gbuffer-metalness-roughness");
		const auto saoTex = resources.texture.read("app/rn:sao");
		const auto lightGridBuf = resources.buffer.read("app/rn:light-grid");
		const auto lightListBuf = resources.buffer.read("app/rn:light-list");
		const auto gbufferLightingTex = resources.texture.create("app/rn:gbuffer-lighting", {
			/*.format=*/ rn::PixelFormat::R16G16B16A16UNorm,
		});
		const auto swapchainTex = resources.swapchain();

		rn::graph::GraphicSubpasses subpasses{};

		const auto opaqueSubpass = subpasses.add("opaque", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Modify,
			},
			/*.inputs=*/ { saoTex, gbufferNormalTex, gbufferAlbedoTex, gbufferMetalnessRoughnessTex },
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ {
				{
					/*.texture=*/ spotlightShadowmapTex,
					/*.usage=*/ rn::graph::GraphicTextureUsage::Sampled,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				},
				{
					/*.texture=*/ cascadeShadowmapTex,
					/*.usage=*/ rn::graph::GraphicTextureUsage::Sampled,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			},
			/*.buffers=*/ {
				{
					/*.buffer=*/ lightGridBuf,
					/*.usage=*/ rn::graph::GraphicBufferUsage::Uniform,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				},
				{
					/*.buffer=*/ lightListBuf,
					/*.usage=*/ rn::graph::GraphicBufferUsage::Uniform,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			}
		});

		const auto skyboxSubpass = subpasses.add("skybox", { opaqueSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		const auto transparentSubpass = subpasses.add("transparent", { skyboxSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Read,
			},
			/*.inputs=*/ { saoTex },
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ {
				{
					/*.texture=*/ spotlightShadowmapTex,
					/*.usage=*/ rn::graph::GraphicTextureUsage::Sampled,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				},
				{
					/*.texture=*/ cascadeShadowmapTex,
					/*.usage=*/ rn::graph::GraphicTextureUsage::Sampled,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			},
			/*.buffers=*/ {
				{
					/*.buffer=*/ lightGridBuf,
					/*.usage=*/ rn::graph::GraphicBufferUsage::Uniform,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				},
				{
					/*.buffer=*/ lightListBuf,
					/*.usage=*/ rn::graph::GraphicBufferUsage::Uniform,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			}
		});

		const auto particlesSubpass = subpasses.add("particles", { transparentSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ {},
			/*.buffers=*/ {
				{
					/*.buffer=*/ lightGridBuf,
					/*.usage=*/ rn::graph::GraphicBufferUsage::Uniform,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				},
				{
					/*.buffer=*/ lightListBuf,
					/*.usage=*/ rn::graph::GraphicBufferUsage::Uniform,
					/*.stages=*/ rn::graph::GraphicStage::Fragment
				}
			}
		});

		const auto swapchainSubpass = subpasses.add("swapchain", { particlesSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ { gbufferLightingTex },
			/*.outputs=*/ { swapchainTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(opaqueSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
			std::make_tuple(skyboxSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
			std::make_tuple(transparentSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
			std::make_tuple(particlesSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
			std::make_tuple(swapchainSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

	passes.push_back(rn::graph::GraphicPass{"debug", { windowExtents }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto debugTex = resources.texture.create("app/rn:debug", {
			/*.format=*/ rn::PixelFormat::R8G8B8A8UNorm,
		});
		const auto swapchainTex = resources.swapchain();

		rn::graph::GraphicSubpasses subpasses{};

		const auto debugSubpass = subpasses.add("debug", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ { debugTex },
			/*.outputs=*/ { swapchainTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(debugSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});
#endif
	rn::graph::CompileResult compileResultE = rn::graph::compile(std::move(passes), "lighting");

	if (compileResultE.isRight()) {
		auto &compileData = compileResultE.right();

		rn::graph::resolve(context, resources, compileData.setupResults);
	}

	return compileResultE;
}

} // app
