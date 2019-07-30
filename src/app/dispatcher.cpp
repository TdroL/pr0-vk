#include "dispatcher.hpp"

namespace app {

void Dispatcher::dispatch([[maybe_unused]] app::action::Bootstrap &&action) {
}

void Dispatcher::dispatch([[maybe_unused]] app::action::CheckModels &&action) {
	const auto currentFenceStamp = context.resolvedFenceStamp(rn::QueueType::Transfer);
	for (auto &&[name, model] : modelCollection.storage) {
		if (model.isLoading()) {
			model.advanceIfTransferred(currentFenceStamp);
		}
	}
}

void Dispatcher::dispatch(app::action::LoadModel &&action) {
	rn::db::Loader loader{resources};

	auto foundO = modelCollection.access(action.modelHandle);

	if ( ! foundO) {
		ngn::log::warn("app::Dispatcher::dispatch(app::action::LoadModel<{}>) => invalid model handle", action.modelHandle);
		return;
	}

	if ( ! foundO->get().second.isInitialized()) {
		ngn::log::warn("app::Dispatcher::dispatch(app::action::LoadModel<{}>) => model not initialized", action.modelHandle);
		return;
	}

	auto resultEither = loader.loadModel(foundO->get().second.filePath);
	if (resultEither.isLeft()) {
		ngn::log::error("app::Dispatcher::dispatch(app::action::LoadModel<{}>) => error: {}", action.modelHandle, resultEither.left().message);
		return;
	}

	foundO->get().second = std::move(resultEither.right());

	ngn::log::info("app::Dispatcher::dispatch(app::action::LoadModel<{}>) => model \"{}\" loaded from file: \"{}\"", action.modelHandle, foundO->get().first, foundO->get().second.filePath);
}

} // app
