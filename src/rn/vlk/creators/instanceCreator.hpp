#pragma once

#include <vulkan/vulkan.hpp>

#include "debugCallbackCreator.hpp"
#include "surfaceCreator.hpp"

#include "../context.hpp"

namespace rn::vlk {

class InstanceCreator {
public:
	struct Version {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};

	vk::UniqueInstance create(Context &context, DebugCallbackCreator &debugCallback, const SurfaceCreator &surface);
};

} // rn::vlk
