#pragma once

#include <ngn/prof.hpp>

#include <rn/resources.hpp>
#include <rn/window.hpp>
#include <rn/db/loader.hpp>

#include "rni.hpp"
#include "action/bootstrap.hpp"
#include "action/loadModel.hpp"

namespace app {

class Dispatcher {
public:
	rn::Window &window;
	app::rni::Context &context;
	rn::Resources<app::rni::Context> &resources;

	Dispatcher(rn::Window &window, app::rni::Context &context, rn::Resources<app::rni::Context> &resources) noexcept :
		window(window),
		context(context),
		resources(resources)
	{}

	void dispatch(app::action::Bootstrap &&action);
	void dispatch(app::action::LoadModel &&action);
};

} // app
