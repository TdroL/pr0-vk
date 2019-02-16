#include <ngn/log.hpp>
#include <ngn/prof.hpp>

#include <rn/graph.hpp>
#include <rn/graph/compile.hpp>
#include <rn/graph/passes.hpp>
#include <rn/graph/types.hpp>

int main() {
	size_t loops = 10000;

	double addDiffsSum = 0.0;
	double addDiffsMin = 1000000.0;
	double addDiffsMax = 0.0;

	double compileDiffsSum = 0.0;
	double compileDiffsMin = 1000000.0;
	double compileDiffsMax = 0.0;

	for (uint32_t o = 0; o < loops; o++) {
	rn::graph::Passes passes{};

	auto addStart = ngn::prof::now();

	enum VertexAttribute {
		POSITION = 0,
		NORMAL = 1,
		TEXCOORDS = 2,
	};

	// const rn::BufferLayout<VertexAttribute> vertexBufferLayout{
	// 	{ /*.name=*/ VertexAttribute::POSITION,  /*.format=*/ rn::DataFormat::R32G32B32Float },
	// 	{ /*.name=*/ VertexAttribute::NORMAL,    /*.format=*/ rn::DataFormat::R32G32B32Float },
	// 	{ /*.name=*/ VertexAttribute::TEXCOORDS, /*.format=*/ rn::DataFormat::R32G32Float },
	// };

	// std::cout << "vertexBufferLayout (" << vertexBufferLayout.stride << "):\n";
	// for (const auto &entry : vertexBufferLayout.entries) {
	// 	std::cout << "  " << static_cast<size_t>(entry.name) << " " << entry.size << " " << entry.offset << "\n";
	// }

	passes.push_back(rn::graph::GraphicPass{"z-prepass", { 1920, 1080 }, [] () {
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

	passes.push_back(rn::graph::GraphicPass{"sao", { 1920, 1080 }, [] () {
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
			/*.usage=*/ rn::graph::BufferUsage::Uniform
		});

		const auto lightListBuf = resources.buffer.create("light-list", {
			/*.size=*/ 1024,
			/*.usage=*/ rn::graph::BufferUsage::Uniform
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

	passes.push_back(rn::graph::GraphicPass{"gbuffer", { 1920, 1080 }, [] () {
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

	passes.push_back(rn::graph::GraphicPass{"lighting", { 1920, 1080 }, [] () {
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
		});
	}});

	auto addEnd = ngn::prof::now();

	try {
		auto compileStart = ngn::prof::now();
		auto compileResult = rn::graph::compile(std::move(passes), "lighting");
		auto compileEnd = ngn::prof::now();

		auto addDiff{addEnd - addStart};
		auto compileDiff{compileEnd - compileStart};

		addDiffsSum += addDiff;
		addDiffsMin = addDiffsMin > addDiff ? addDiff : addDiffsMin;
		addDiffsMax = addDiffsMax > addDiff ? addDiffsMax : addDiff;

		compileDiffsSum += compileDiff;
		compileDiffsMin = compileDiffsMin > compileDiff ? compileDiff : compileDiffsMin;
		compileDiffsMax = compileDiffsMax > compileDiff ? compileDiffsMax : compileDiff;

		// std::cout << "tick=" << hrc::period::num << "\n";
		// std::cout << "den=" << hrc::period::den << "\n";
		// std::cout << "addDiff=" << std::chrono::duration<float, std::micro>(addDiff).count() << "\n";
		// ngn::log::info("add={}us", std::chrono::duration<float, std::micro>(addDiff).count());
		// ngn::log::info("add={}us", addDiff * 1000.0);
		// std::cout << "compileDiff=" << std::chrono::duration<float, std::micro>(compileDiff).count() << "\n";
		// ngn::log::info("compile={}us", std::chrono::duration<float, std::micro>(compileDiff).count());
		// ngn::log::info("compile={}us", compileDiff * 1000.0);
		// if (compileResult.isRight()) {
		// 	const auto passList = compileResult.right();
		// 	for (const auto &pass : passList) {
		// 		std::cout << pass << "\n";
		// 	}
		// } else {
		// 	std::cerr << "Compile error: " << compileResult.left().message << "\n";
		// }
	} catch (std::runtime_error &e) {
		// std::cerr << e.what() << "\n";
		ngn::log::error("runtime error: {}", e.what());
		return -1;
	}
	}

	ngn::log::info("addDiffsAvg={}us addDiffsMin={}us addDiffsMax={}us", addDiffsSum / static_cast<double>(loops) * 1000000.0, addDiffsMin * 1000000.0, addDiffsMax * 1000000.0);
	ngn::log::info("compileDiffsAvg={}us compileDiffsMin={}us compileDiffsMax={}us", compileDiffsSum / static_cast<double>(loops) * 1000000.0, compileDiffsMin * 1000000.0, compileDiffsMax * 1000000.0);

	return 0;
}
