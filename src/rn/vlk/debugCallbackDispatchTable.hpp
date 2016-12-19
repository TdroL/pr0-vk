#include "debugCallbackCreator.hpp"
#include "instanceOwner.hpp"
#include "../../ngn/log.hpp"

#include <unordered_map>
#include <sstream>

namespace rn {

namespace vlk {

struct DebugCallbackDispatchTable {
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{};
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT{};
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT{};
};

void initDebugCallbackDispatchTable(vk::Instance &instance);
void deinitDebugCallbackDispatchTable(vk::Instance &instance);
DebugCallbackDispatchTable & getDebugCallbackDispatchTable(vk::Instance &instance);

} // vlk

} // rn