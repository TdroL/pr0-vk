#include "muticesSelector.hpp"

namespace rn::vki {

Context::Mutex MuticesSelector::select(Context &context) {
	uint32_t mutexCount = 1;

	uint32_t presentationIdx;
	uint32_t graphicIdx;
	uint32_t computeIdx;
	uint32_t transferIdx;

	// presentation
	presentationIdx = 0;

	// graphic
	if (context.queue.graphic == context.queue.presentation) {
		graphicIdx = presentationIdx;
	} else {
		graphicIdx = mutexCount++;
	}

	// compute
	if (context.queue.compute == context.queue.presentation) {
		computeIdx = presentationIdx;
	} else if (context.queue.compute == context.queue.graphic) {
		computeIdx = graphicIdx;
	} else {
		computeIdx = mutexCount++;
	}

	// transfer
	if (context.queue.transfer == context.queue.presentation) {
		transferIdx = presentationIdx;
	} else if (context.queue.transfer == context.queue.graphic) {
		transferIdx = graphicIdx;
	} else if (context.queue.transfer == context.queue.compute) {
		transferIdx = computeIdx;
	} else {
		transferIdx = mutexCount++;
	}

	return {
		std::vector<std::mutex>{ mutexCount },
		presentationIdx,
		graphicIdx,
		computeIdx,
		transferIdx
	};
}

} // rn::vki
