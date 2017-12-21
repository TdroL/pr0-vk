#include "loader.hpp"

#include "../../../ngn/log.hpp"
#include "../../context.hpp"
#include "../mesh.hpp"

namespace rn::vlk::db::mesh {

Loader::Loader(rn::vlk::Context &context, rn::vlk::db::Mesh &db) :
	db(db),
	reader{context, db},
	transferrer{context, db}
{
	if (ngn::config::core.db.mesh.forceSynchronusLoading()) {
		ngn::log::warn("rn::vlk::db::mesh::Loader::Loader() => force synchronus loading");
	}

	reader.event.listen([&] (Payload &&payload) {
		transferrer.transfer(std::move(payload));
	});
}

std::future<bool> Loader::load(Handle &handle) {
	uint32_t index = handle.index;
	std::string source = handle.db->map(index, [] (auto &entry) {
		return entry.source;
	});

	std::promise<bool> promise{};
	std::future<bool> future = promise.get_future();

	Payload payload{
		std::move(index),
		std::move(source),
		std::move(promise),
	};

	reader.read(std::move(payload));

	return future;
}

} // rn::vlk::db::mesh
