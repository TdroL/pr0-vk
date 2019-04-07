#include "renderer.hpp"

#include <ngn/prof.hpp>

namespace app {

rn::graph::CompileResult Renderer::compile() {
	rn::graph::Passes passes{};

	const rn::Extent3D windowExtents{ window.currentProperties.width, window.currentProperties.height };

	passes.push_back(rn::graph::GraphicPass{"z-prepass", { windowExtents }, [] () {
		rn::graph::ResourceDescriptors resources{};

		const auto depthTex = resources.texture.create("app/rn:depth", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto depthSubpass = subpasses.add("depth", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::DepthStencilTextureUsage::Modify,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		rn::graph::GraphicSubpassRecorders recorders{
			std::make_tuple(depthSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			}),
		};

		return rn::graph::GraphicSetupResult{ std::move(resources), std::move(subpasses), std::move(recorders) };
	}});

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

	rn::graph::CompileResult compileResultE = rn::graph::compile(std::move(passes), "lighting");

	if (compileResultE.isRight()) {
		auto &compileData = compileResultE.right();

		rn::graph::resolve(context, resources, compileData.setupResults);
	}

	return compileResultE;
}

} // app
