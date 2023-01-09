#include "rendererVK.hpp"

namespace ware::rendererVK {

State setup(ware::windowGLFW::State &window, ware::contextVK::State &context, ware::swapchainVK::State &swapchain) {
	return State{
		.stateSimple = passes::simple::setup(window, context, swapchain),
	};
}

void refresh([[maybe_unused]] State &state) {
	passes::simple::refresh(state.stateSimple);
}

void process([[maybe_unused]] State &state) {
	passes::simple::process(state.stateSimple);
}

} // ware::rendererVK
