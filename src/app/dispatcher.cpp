#include "dispatcher.hpp"

namespace app {

void Dispatcher::dispatch([[maybe_unused]] app::action::Bootstrap &&action) {
}

void Dispatcher::dispatch(app::action::LoadModel &&action) {
	rn::db::Loader loader{resources};

	const auto resultEither = loader.loadModel(action.fileName);
	if (resultEither.isLeft()) {
		ngn::log::error("app::Dispatcher::dispatch({}) => error: {}", action.fileName, resultEither.left().message);
	} else {
		ngn::log::info("app::Dispatcher::dispatch({}) => done", action.fileName);
	}
}

} // app
