#pragma once

#include "../commands.hpp"
#include "context.hpp"
#include "dispatch.hpp"

namespace rn::vki {

class Executor {
public:
	rn::vki::Context &context;

	void executeCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::GraphicCommandVariant &commandVariant);
	void executeCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::ComputeCommandVariant &commandVariant);
	void executeCommand(rn::vki::HandleCommandBuffer &&commandBufferHandle, const rn::TransferCommandVariant &commandVariant);
};

} // rn::vki