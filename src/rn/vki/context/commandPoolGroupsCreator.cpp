#include "commandPoolGroupsCreator.hpp"

#include <thread>
#include <vector>

#include "../../../ngn/threading.hpp"
#include "../trace.hpp"

namespace rn::vki::context {

rn::vki::context::CommandPoolGroups CommandPoolGroupsCreator::create(const rn::vki::context::SurfaceDescription &surfaceDescription, const ngn::config::Config &config) {
	std::vector<rn::vki::context::CommandPoolGroups::Group> graphicGroup{};
	std::vector<rn::vki::context::CommandPoolGroups::Group> computeGroup{};
	std::vector<rn::vki::context::CommandPoolGroups::Group> transferGroup{};

	graphicGroup.resize(surfaceDescription.images.size());
	computeGroup.resize(surfaceDescription.images.size());
	transferGroup.resize(surfaceDescription.images.size());

	uint32_t concurrency = ngn::threading::concurrency(config);
	for (size_t i = 0; i < surfaceDescription.images.size(); i++) {
		graphicGroup[i].owners.reserve(concurrency);
		graphicGroup[i].handles.reserve(concurrency);
		computeGroup[i].owners.reserve(concurrency);
		computeGroup[i].handles.reserve(concurrency);
		transferGroup[i].owners.reserve(concurrency);
		transferGroup[i].handles.reserve(concurrency);
		// graphicGroup[i].resize(concurrency);
		// computeGroup[i].resize(concurrency);
		// transferGroup[i].resize(concurrency);

		// for (uint32_t j = 0; j < concurrency; j++) {
		// 	graphicGroup[i][j] = {
		// 		RN_VKI_TRACE(device->createCommandPoolUnique({
		// 			/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
		// 			/*.queueFamilyIndex=*/ queueFamilyIndex.graphic.family
		// 		}, nullptr, device.table())),
		// 		device.table()
		// 	};

		// 	computeGroup[i][j] = {
		// 		RN_VKI_TRACE(device->createCommandPoolUnique({
		// 			/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
		// 			/*.queueFamilyIndex=*/ queueFamilyIndex.compute.family
		// 		}, nullptr, device.table())),
		// 		device.table()
		// 	};

		// 	transferGroup[i][j] = {
		// 		RN_VKI_TRACE(device->createCommandPoolUnique({
		// 			/*.flags=*/ vk::CommandPoolCreateFlagBits::eTransient,
		// 			/*.queueFamilyIndex=*/ queueFamilyIndex.transfer.family
		// 		}, nullptr, device.table())),
		// 		device.table()
		// 	};
		// }
	}

	return rn::vki::context::CommandPoolGroups{
		/*.graphicGroup=*/ std::move(graphicGroup),
		/*.computeGroup=*/ std::move(computeGroup),
		/*.transferGroup=*/ std::move(transferGroup)
	};
}

} // rn::vki::context
