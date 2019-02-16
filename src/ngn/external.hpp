#pragma once

namespace ngn::external {

template<class ...Ts>
struct Initialize {};

template<class ...Ts>
Initialize<Ts...> initialize() {
	(Ts::initialize(), ...);

	return Initialize<Ts...>{};
}

} // ngn::external
