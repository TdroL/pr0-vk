#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "debugCallbackCreator.hpp"
#include "surfaceCreator.hpp"

namespace rn::vlk {

struct InstancePlannerResult {
	std::vector<std::string> extensions{};
	std::vector<std::string> layers{};

	struct Missing {
		std::vector<std::string> list{};
		std::string type{};
	};

	Missing missing{};

	std::vector<std::string> missingList{};
	std::string missingType{};

	bool all = false;
	bool surface = false;
	bool debugCallback = false;
};

struct InstancePlanner {
	InstancePlannerResult selectExtensionsAndLayers(const DebugCallbackCreator &debugCallbackCreator, const SurfaceCreator &surfaceCreator);
};

} // rn::vlk
