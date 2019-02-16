#pragma once

#include <ngn/prof.hpp>

#include <rn/window.hpp>
#include <rn/vki/context.hpp>

namespace app::main {

class State {
public:
	rn::Window &window;
	rn::vki::Context &context;

	State(rn::Window &window, rn::vki::Context &context) noexcept :
		window(window),
		context(context)
	{}
};

} // app::main
