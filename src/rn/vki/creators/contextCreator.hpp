#pragma once

#include "../../window.hpp"
#include "../context.hpp"

namespace rn::vki {

class ContextCreator {
public:
	rn::Window &window;

	ContextCreator(rn::Window &window) :
		window(window)
	{}

	Context create();
};

} // rn::vki
