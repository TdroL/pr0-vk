#pragma once

#include "context.hpp"

#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../util/threadPool.hpp"
#include "../commands.hpp"
#include "executor.hpp"

namespace rn::vki {

class Recorder {
public:
	rn::vki::Context &context;
	util::ThreadPool &threadPool;
	rn::vki::Executor executor;

	explicit Recorder(rn::vki::Context &context, util::ThreadPool &threadPool) :
		context{context},
		threadPool{threadPool},
		executor{context}
	{}

	rn::vki::HandleCommandBuffer record(std::vector<std::vector<rn::GraphicCommandVariant>> &&commandLists);
	rn::vki::HandleCommandBuffer record(std::vector<std::vector<rn::ComputeCommandVariant>> &&commandLists);
	rn::vki::HandleCommandBuffer record(std::vector<std::vector<rn::TransferCommandVariant>> &&commandLists);
};

} // rn::vki