#pragma once

#include <vector>

#include "../dispatch.hpp"

namespace ngn::config {

struct Config;

} // ngn::config

namespace rn::vki::context {

struct SurfaceDescription;

class SurfaceImageViewsCreator {
public:
	std::vector<rn::vki::UniqueImageView> create(rn::vki::context::SurfaceDescription &surfaceDescription, rn::vki::HandleDevice &&device, ngn::config::Config &config);
};

} // rn::vki::context
