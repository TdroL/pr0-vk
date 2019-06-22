#pragma once

#include "context.hpp"

#include <tuple>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../util/threadPool.hpp"
#include "../commands.hpp"

namespace rn::vki {

class Recorder {
public:
	rn::vki::Context &context;
	util::ThreadPool &threadPool;

	explicit Recorder(rn::vki::Context &context, util::ThreadPool &threadPool) :
		context{context},
		threadPool{threadPool}
	{}

	vk::CommandBuffer record(std::vector<std::vector<rn::GraphicCommandVariant>> &&commandLists);
	vk::CommandBuffer record(std::vector<std::vector<rn::ComputeCommandVariant>> &&commandLists);
	vk::CommandBuffer record(std::vector<std::vector<rn::TransferCommandVariant>> &&commandLists);
};

} // rn::vki