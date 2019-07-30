#pragma once

namespace rn::db {

enum ModelState : uint32_t {
	Empty = 0,
	Initialized = 1,
	Loading = 2,
	Ready = 3,
};

} // rn::db