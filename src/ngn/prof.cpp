#include "prof.hpp"

// #include <mutex>
#include "external/glfw.hpp"
#include "external/remotery.hpp"
#include "external.hpp"

namespace ngn::prof {

const auto initialize = ngn::external::initialize<ngn::external::GLFW, ngn::external::Remotery>();

// class RemoteryInstance {
// public:
// 	Remotery *rmt;

// 	explicit RemoteryInstance(Remotery *rmt = nullptr) noexcept :
// 		rmt{std::move(rmt)}
// 	{}

// 	RemoteryInstance(const RemoteryInstance &other) = delete;
// 	RemoteryInstance( RemoteryInstance &&other) = delete;

// 	RemoteryInstance & operator=(RemoteryInstance &&other) {
// 		rmt = std::move(other.rmt);
// 		other.rmt = nullptr;

// 		return *this;
// 	}

// 	~RemoteryInstance() {
// 		if (rmt != nullptr) {
// 			rmt_DestroyGlobalInstance(rmt);
// 		}
// 	}
// };

// RemoteryInstance remoteryInstance{};

// void initializeRemotery() {
// 	static std::once_flag createRemoteryFlag;
// 	std::call_once(createRemoteryFlag, [&] () {
// 		Remotery *rmt;
// 		rmt_CreateGlobalInstance(&rmt);

// 		remoteryInstance = RemoteryInstance{rmt};
// 	});
// }

void Sampler::start(const char *name, uint32_t &hash) {
	// initializeRemotery();

	started = true;

	_rmt_BeginCPUSample(name, 0, &hash);
}

Sampler::~Sampler() {
	if (started) {
		_rmt_EndCPUSample();
	}
}

double now() {
	return glfwGetTime();
}

} // ngn::prof
