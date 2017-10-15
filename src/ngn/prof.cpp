#include "prof.hpp"

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

void Sampler::start(const char *name, uint32_t &hash) {
	started = true;

	_rmt_BeginCPUSample(name, 0, &hash);
}

Sampler::~Sampler() {
	if (started) {
		_rmt_EndCPUSample();
	}
}

} // ngn::prof
