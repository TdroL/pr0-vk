#pragma once

#include <cstdint>

namespace ngn::prof
{

class Scope {
public:
	uint32_t &hash;

	Scope(const char *name);

	~Scope();
};

} // ngn::prof
