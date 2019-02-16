#pragma once

#include <rn/context.hpp>
#include <rn/graph.hpp>
#include <rn/resources.hpp>
#include <rn/window.hpp>
#include <rn/vki/context.hpp>
#include <rn/vki/resources.hpp>

namespace app {

class Renderer {
public:
	rn::Window &window;
	rn::Context<rn::vki::Context> &context;
	rn::Resources<rn::vki::Resources> &resources;

	Renderer(rn::Window &window, rn::Context<rn::vki::Context> &context, rn::Resources<rn::vki::Resources> &resources) noexcept :
		window(window),
		context(context),
		resources(resources)
	{}

	rn::graph::CompileResult compile();
};

} // app
