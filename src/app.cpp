#include <thread>

#include <rn/window.hpp>
#include <rn/context.hpp>
#include <rn/resources.hpp>
#include <rn/vki/context.hpp>
#include <rn/vki/resources.hpp>

#include <ngn/log.hpp>
#include <ngn/config.hpp>
#include <ngn/prof.hpp>

#include <util/join.hpp>
#include <util/map.hpp>

#include "app/renderer.hpp"

int main() {
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

		rn::Context<rn::vki::Context> context{};
		{
			auto contextStart = ngn::prof::now();
			context = rn::vki::Context::factory(config, window);
			auto contextEnd = ngn::prof::now();

			ngn::log::debug("context#create: {}ms", 1000.0 * (contextEnd - contextStart));
		}

		if (config.core.dirty) {
			ngn::log::warn("Config.core is dirty after context creation");

			ngn::log::debug("{}", ngn::config::Core::dump(config.core));
		}

		rn::Resources<rn::vki::Resources> resources = rn::vki::Resources::create(context);

		app::Renderer renderer{window, context, resources};
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
		}

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
