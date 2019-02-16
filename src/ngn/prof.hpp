#pragma once

#include <cstdint>
#include <cstdlib>

#include "../ngn/log.hpp"
#include "../util/symbol.hpp"

namespace ngn::prof {

#define NGN_PROF_SCOPEIMPL(name, ID) ngn::prof::Sampler profSampler_##ID{}; do { static uint32_t profSamplerHash_##ID = 0; profSampler_##ID.start(name, profSamplerHash_##ID); } while(0)
#define NGN_PROF_SCOPE(name) NGN_PROF_SCOPEIMPL(name, __COUNTER__)

class Sampler {
public:
	bool started = false;

	Sampler() noexcept = default;

	Sampler(Sampler &&) = delete;
	Sampler & operator=(Sampler &&) = delete;

	Sampler(const Sampler &) = delete;
	Sampler & operator=(const Sampler &) = delete;

	void start(const char *name, uint32_t &hash);

	~Sampler();
};

double now();

} // ngn::prof
