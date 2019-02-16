#include "renderer.hpp"

#include <ngn/prof.hpp>

namespace app {

rn::graph::CompileResult Renderer::compile() {
	rn::graph::Passes passes{};

	const rn::Extent3D windowExtents{ window.currentProperties.width, window.currentProperties.height };

	passes.push_back(rn::graph::GraphicPass{"z-prepass", { windowExtents }, [] () {
		rn::graph::Resources resources{};

		const auto depthTex = resources.texture.create("depth", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto depthSubpass = subpasses.add("depth", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Modify,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ depthSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::GraphicPass{"cascade-shadowmap", { 2048, 2048 }, [] () {
		rn::graph::Resources resources{};

		const auto cascadeShadowmapTex = resources.texture.create("cascade-shadowmap", {
			/*.format=*/ rn::PixelFormat::D32Float,
			/*.dimensions=*/ {{ 4 * 2048, 2048 }},
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto cascadeShadowmapSubpass = subpasses.add("cascade-shadowmap", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ cascadeShadowmapTex,
				/*.usage=*/ rn::graph::Access::Write,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ cascadeShadowmapSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::GraphicPass{"spotlight-shadowmap", { 1024, 1024 }, [] () {
		rn::graph::Resources resources{};

		const auto spotlightShadowmapTex = resources.texture.create("spotlight-shadowmap", {
			/*.format=*/ rn::PixelFormat::D32Float,
			/*.dimensions=*/ {{ 4 * 2048, 2048 }},
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto spotlightShadowmapSubpass = subpasses.add("spotlight-shadowmap", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ spotlightShadowmapTex,
				/*.usage=*/ rn::graph::Access::Write,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ {},
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ spotlightShadowmapSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::GraphicPass{"sao", { windowExtents }, [] () {
		rn::graph::Resources resources{};

		const auto depthTex = resources.texture.read("depth");

		const auto saoTempTex = resources.texture.create("sao-temp", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		const auto saoTex = resources.texture.create("sao", {
			/*.format=*/ rn::PixelFormat::R8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto saoNoiseSubpass = subpasses.add("sao-noise", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Read,
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
			/*.textures=*/ { saoTex },
			/*.buffers=*/ {}
		});

		const auto saoBlurVSubpass = subpasses.add("sao-blur-v", { saoBlurHSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ {},
			/*.outputs=*/ { saoTex },
			/*.textures=*/ { saoTempTex },
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ saoNoiseSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
			{ saoBlurHSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
			{ saoBlurVSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::ComputePass{"classify", [] () {
		rn::graph::Resources resources{};

		const auto depthTex = resources.texture.read("depth");

		const auto lightGridBuf = resources.buffer.create("light-grid", {
			/*.size=*/ 1024,
			/*.usage=*/ rn::BufferUsage::Uniform
		});

		const auto lightListBuf = resources.buffer.create("light-list", {
			/*.size=*/ 1024,
			/*.usage=*/ rn::BufferUsage::Uniform
		});

		rn::graph::ComputeSubpasses subpasses{};

		const auto classifySubpass = subpasses.add("classify", {}, rn::graph::ComputeSubpassDesc{
			/*.textures=*/ { depthTex },
			/*.buffers=*/ { lightGridBuf, lightListBuf }
		});

		return std::make_tuple(resources, subpasses, rn::graph::ComputeSubpassRecorders{
			{ classifySubpass, [=] () {
				return rn::graph::ComputeCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::GraphicPass{"gbuffer", { windowExtents }, [] () {
		rn::graph::Resources resources{};

		const auto depthTex = resources.texture.read("depth");

		const auto gbufferNormalTex = resources.texture.create("gbuffer-normal", {
			/*.format=*/ rn::PixelFormat::R16G16Float,
		});

		const auto gbufferAlbedoTex = resources.texture.create("gbuffer-albedo", {
			/*.format=*/ rn::PixelFormat::R8G8B8A8UNorm,
		});

		const auto gbufferMetalnessRoughnessTex = resources.texture.create("gbuffer-metalness-roughness", {
			/*.format=*/ rn::PixelFormat::R8G8UNorm,
		});

		rn::graph::GraphicSubpasses subpasses{};

		const auto gbufferSubpass = subpasses.add("gbuffer", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { gbufferNormalTex, gbufferAlbedoTex, gbufferMetalnessRoughnessTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ gbufferSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::GraphicPass{"lighting", { windowExtents }, [] () {
		rn::graph::Resources resources{};

		const auto depthTex = resources.texture.read("depth");
		const auto spotlightShadowmapTex = resources.texture.read("spotlight-shadowmap");
		const auto cascadeShadowmapTex = resources.texture.read("cascade-shadowmap");
		const auto gbufferNormalTex = resources.texture.read("gbuffer-normal");
		const auto gbufferAlbedoTex = resources.texture.read("gbuffer-albedo");
		const auto gbufferMetalnessRoughnessTex = resources.texture.read("gbuffer-metalness-roughness");
		const auto saoTex = resources.texture.read("sao");
		const auto lightGridBuf = resources.buffer.read("light-grid");
		const auto lightListBuf = resources.buffer.read("light-list");
		const auto gbufferLightingTex = resources.texture.create("gbuffer-lighting", {
			/*.format=*/ rn::PixelFormat::R16G16B16A16UNorm,
		});
		const auto swapchainTex = resources.swapchain();

		rn::graph::GraphicSubpasses subpasses{};

		const auto opaqueSubpass = subpasses.add("opaque", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Modify,
			},
			/*.inputs=*/ { saoTex, gbufferNormalTex, gbufferAlbedoTex, gbufferMetalnessRoughnessTex },
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ { spotlightShadowmapTex, cascadeShadowmapTex },
			/*.buffers=*/ { lightGridBuf, lightListBuf }
		});

		const auto skyboxSubpass = subpasses.add("skybox", { opaqueSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		const auto transparentSubpass = subpasses.add("transparent", { skyboxSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Read,
			},
			/*.inputs=*/ { saoTex },
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ { spotlightShadowmapTex, cascadeShadowmapTex },
			/*.buffers=*/ { lightGridBuf, lightListBuf }
		});

		const auto particlesSubpass = subpasses.add("particles", { transparentSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {
				/*.texture=*/ depthTex,
				/*.usage=*/ rn::graph::Access::Read,
			},
			/*.inputs=*/ {},
			/*.outputs=*/ { gbufferLightingTex },
			/*.textures=*/ {},
			/*.buffers=*/ { lightGridBuf, lightListBuf }
		});

		const auto swapchainSubpass = subpasses.add("swapchain", { particlesSubpass }, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ { gbufferLightingTex },
			/*.outputs=*/ { swapchainTex },
			/*.textures=*/ {},
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ opaqueSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
			{ skyboxSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
			{ transparentSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
			{ particlesSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
			{ swapchainSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	passes.push_back(rn::graph::GraphicPass{"debug", { windowExtents }, [] () {
		rn::graph::Resources resources{};

		const auto debugTex = resources.texture.create("debug", {
			/*.format=*/ rn::PixelFormat::R8G8B8A8UNorm,
		});
		const auto swapchainTex = resources.swapchain();

		rn::graph::GraphicSubpasses subpasses{};

		const auto debugSubpass = subpasses.add("debug", {}, rn::graph::GraphicSubpassDesc{
			/*.depthStencil=*/ {},
			/*.inputs=*/ {},
			/*.outputs=*/ { debugTex },
			/*.textures=*/ { swapchainTex },
			/*.buffers=*/ {}
		});

		return std::make_tuple(resources, subpasses, rn::graph::GraphicSubpassRecorders{
			{ debugSubpass, [=] () {
				return rn::graph::GraphicCommandList{};
			} },
		});
	}});

	rn::graph::CompileResult compileResultE = rn::graph::compile(std::move(passes), "lighting");

	if (compileResultE.isRight()) {
		auto &compileData = compileResultE.right();

		rn::graph::resolve(context, compileData.resourcesUsageList);
	}

	return compileResultE;
}

} // app
