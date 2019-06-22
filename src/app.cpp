#include <thread>
#include <filesystem>

#include <rn/window.hpp>
#include <rn/resources.hpp>

#include <ngn/log.hpp>
#include <ngn/config.hpp>
#include <ngn/prof.hpp>
#include <ngn/threading.hpp>

#include <util/join.hpp>
#include <util/map.hpp>

#include "app/rni.hpp"
#include "app/dispatcher.hpp"
#include "app/renderer.hpp"

int main(int argc, char *argv[]) {
	if (argc > 0) {
		ngn::fs::searchDirectories.emplace_back(ngn::fs::base(argv[0]));
	}

	try {
		ngn::config::Config config = ngn::config::Config::factory();

		rn::Window window{{
			/*.title=*/ config.core.application.name,
			/*.width=*/ config.core.window.width,
			/*.height=*/ config.core.window.height,
			/*.monitor=*/ config.core.window.monitor,
			/*.mode=*/ config.core.window.mode
		}};
		window.create();

		app::rni::Context context{};
		{
			auto contextStart = ngn::prof::now();
			context = app::rni::Context::factory(config, window);
			auto contextEnd = ngn::prof::now();

			ngn::log::debug("context#create: {}ms", 1000.0 * (contextEnd - contextStart));
		}

		if (config.core.dirty) {
			ngn::log::warn("Config.core is dirty after context creation");

			ngn::log::debug("{}", ngn::config::Core::dump(config.core));
		}

		util::ThreadPool threadPool{ngn::threading::concurrency(config)};

		rn::Resources<app::rni::Context> resources{context};

		app::Dispatcher dispatcher{window, context, resources};
		dispatcher.dispatch(app::action::Bootstrap{});
		dispatcher.dispatch(app::action::LoadModel{"assets/models/sponza/Sponza.gltf"});
		// dispatcher.dispatch(app::action::LoadModel{"assets/models/minimal/minimal.gltf"});
		// dispatcher.dispatch(app::action::LoadModel{"assets/models/BoxTextured/glTF/BoxTextured.gltf"});
		// dispatcher.dispatch(app::action::LoadModel{"assets/models/BoxTextured/glTF-Binary/BoxTextured.glb"});
		// dispatcher.dispatch(app::action::LoadModel{"assets/models/BoxTextured/glTF-Embedded/BoxTextured.gltf"});

		app::Renderer renderer{window, context, resources};
		if (false)
		{
			auto rendererCompileStart = ngn::prof::now();
			auto rendererCompileE = renderer.compile();
			auto rendererCompileEnd = ngn::prof::now();

			ngn::log::debug("rendererCompile#create: {}ms", 1000.0 * (rendererCompileEnd - rendererCompileStart));

			if (rendererCompileE.isLeft()) {
				ngn::log::error("Failed to compile renderer resources: {}", rendererCompileE.left().message);

				return EXIT_FAILURE;
			}
		}

		for (size_t i = 0; i < 1 && ! window.shouldClose(); i++) {
			NGN_PROF_SCOPE("main loop");

			{
				NGN_PROF_SCOPE("context advance");
				context.advance();
			}

			{
				NGN_PROF_SCOPE("resource advance");
				resources.advance();
			}

			{
				NGN_PROF_SCOPE("window event polling");
				window.pollEvents();
			}

			{
				NGN_PROF_SCOPE("window refreshing");

				if ( ! window.needsRefresh()) {
					window.refresh();
					// mainState.refresh();
				}
			}

			// mainState.render();

			{
				NGN_PROF_SCOPE("resource commit");
				resources.commit();
			}

			{
				NGN_PROF_SCOPE("context commit");
				context.commit(threadPool);
			}
		}

		threadPool.stop();

		context.waitIdle();

		ngn::log::debug("context.graphicCommandLists=<{}> [{}]", context.graphicCommandLists.size(), util::join(util::map(context.graphicCommandLists, [] (const auto &list) {
			return std::to_string(list.size());
		})));
		ngn::log::debug("context.computeCommandLists=<{}> [{}]", context.computeCommandLists.size(), util::join(util::map(context.computeCommandLists, [] (const auto &list) {
			return std::to_string(list.size());
		})));
		ngn::log::debug("context.transferCommandLists=<{}> [{}]", context.transferCommandLists.size(), util::join(util::map(context.transferCommandLists, [] (const auto &list) {
			return std::to_string(list.size());
		})));

		ngn::log::info("done, cleanup");
		return EXIT_SUCCESS;
	} catch (std::runtime_error const &e) {
		ngn::log::critical("Runtime exception: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		ngn::log::critical("Unknown exception");
		return EXIT_FAILURE;
	}
}
