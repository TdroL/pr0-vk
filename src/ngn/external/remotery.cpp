#include "remotery.hpp"

#include "../../util/scope.hpp"
#include "../log.hpp"

namespace ngn::external {

void Remotery::initialize() {
	static ::Remotery *rmt = nullptr;
	static auto instance = util::createScopeExit(
		[&] () {
			ngn::log::debug("ngn::external::Remotery::initialize() => initialize Remotery");

			rmt_CreateGlobalInstance(&rmt);
		},
		[&] () {
			ngn::log::debug("ngn::external::Remotery::initialize() => destroy Remotery");

			rmt_DestroyGlobalInstance(rmt);
		}
	);
}

} // ngn::external
