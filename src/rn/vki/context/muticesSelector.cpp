#include "muticesSelector.hpp"

namespace rn::vki::context {

rn::vki::context::Mutex MuticesSelector::select(rn::vki::context::Queue &queue) {
	uint32_t mutexCount = 1;

	uint32_t presentationIdx;
	uint32_t graphicIdx;
	uint32_t computeIdx;
	uint32_t transferIdx;

	// presentation
	presentationIdx = 0;

	// graphic
	if (queue.graphic == queue.presentation) {
		graphicIdx = presentationIdx;
	} else {
		graphicIdx = mutexCount++;
	}

	// compute
	if (queue.compute == queue.presentation) {
		computeIdx = presentationIdx;
	} else if (queue.compute == queue.graphic) {
		computeIdx = graphicIdx;
	} else {
		computeIdx = mutexCount++;
	}

	// transfer
	if (queue.transfer == queue.presentation) {
		transferIdx = presentationIdx;
	} else if (queue.transfer == queue.graphic) {
		transferIdx = graphicIdx;
	} else if (queue.transfer == queue.compute) {
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

} // rn::vki::context
