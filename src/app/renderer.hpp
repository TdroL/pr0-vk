#pragma once

#include <rn/graph.hpp>
#include <rn/resources.hpp>
#include <rn/window.hpp>

#include "rni.hpp"

namespace app {

class Renderer {
public:
	rn::Window &window;
	app::rni::Context &context;
	rn::Resources<app::rni::Context> &resources;

	Renderer(rn::Window &window, app::rni::Context &context, rn::Resources<app::rni::Context> &resources) noexcept :
		window(window),
		context(context),
		resources(resources)
	{}

	rn::graph::CompileResult compile();
};

} // app
