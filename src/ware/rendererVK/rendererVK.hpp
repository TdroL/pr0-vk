#pragma once

#include "passes/simple.hpp"

namespace ware::rendererVK {

struct State {
	passes::simple::State stateSimple;
};

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::swapchainVK::State &swapchain);

void refresh(State &state);

void process(State &state);

} // ware::rendererVK
