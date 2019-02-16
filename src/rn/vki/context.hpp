#pragma once

#include <mutex>
#include <vector>

#include "dispatch.hpp"
#include "context/types.hpp"

namespace ngn::config {

struct Config;

}

namespace rn {

class Window;

}

namespace rn::vki {

class Context {
public:
	rn::vki::UniqueTableInstance instance{};
	rn::vki::HandlePhysicalDevice physicalDevice{};
	rn::vki::context::Debug debug{};
	rn::vki::UniqueSurfaceKHR surface{};
	rn::vki::UniqueTableDevice device{};
	rn::vki::UniqueSwapchainKHR swapchain{};
	std::vector<rn::vki::UniqueImageView> surfaceImageViews{};
	rn::vki::context::PhysicalDeviceDescription physicalDeviceDescription{};
	rn::vki::context::Queue queue{};
	rn::vki::context::Family family{};
	rn::vki::context::Mutex mutex{};
	rn::vki::context::SurfaceDescription surfaceDescription{};
	rn::vki::context::Allocator allocator{};

	static Context factory(ngn::config::Config &config, rn::Window &window);

	Context() = default;

	Context(const Context &other) = delete;
	Context(Context &&other);

	Context & operator=(const Context &other) = delete;
	Context & operator=(Context &&other);

	~Context();

	void reset();

	void waitIdle();
};

} // rn::vki
