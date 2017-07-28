#include "prof.hpp"

#include <string>
#include <map>

#include <Remotery.h>

namespace ngn::prof {

Remotery *rmt;

class RemoteryFactory {
public:

	RemoteryFactory() {
		rmt_CreateGlobalInstance(&rmt);
	}

	~RemoteryFactory() {
		rmt_DestroyGlobalInstance(rmt);
	}
};

RemoteryFactory rf{};
std::map<std::string, uint32_t> hashCache{};

uint32_t & findHash(const std::string &name) {
	auto found = hashCache.find(name);
	if (found == std::end(hashCache)) {
		auto inserted = hashCache.insert_or_assign(name, 0);
		return inserted.first->second;
	} else {
		return found->second;
	}
}

Scope::Scope(const char *name) :
	hash(findHash(name))
{
	_rmt_BeginCPUSample(name, 0, &hash);
}

Scope::~Scope() {
	_rmt_EndCPUSample();
}

} // ngn::prof
