#pragma once

#include <mutex>

#include "../memory/pool.hpp"
#include "../dispatch.hpp"

namespace rn::vki::context {

struct Debug {
  rn::vki::UniqueDebugReportCallbackEXT debugReportCallback{};
  rn::vki::UniqueDebugUtilsMessengerEXT debugUtilsMessenger{};
};

struct PhysicalDeviceDescription {
  vk::PhysicalDeviceProperties2 properties{};
  vk::PhysicalDeviceMemoryProperties2 memoryProperties{};
  vk::PhysicalDeviceFeatures2 availableFeatures{};
  vk::PhysicalDeviceFeatures2 requiredFeatures{};
};

struct Queue {
  vk::Queue presentation{};
  vk::Queue graphic{};
  vk::Queue compute{};
  vk::Queue transfer{};
};

struct Family {
  uint32_t presentation{};
  uint32_t graphic{};
  uint32_t compute{};
  uint32_t transfer{};
};

struct Mutex {
  std::vector<std::mutex> list{};
  uint32_t presentationIdx = 0;
  uint32_t graphicIdx = 0;
  uint32_t computeIdx = 0;
  uint32_t transferIdx = 0;

  std::mutex & presentation() { return list[presentationIdx]; }
  std::mutex & graphic() { return list[graphicIdx]; }
  std::mutex & compute() { return list[computeIdx]; }
  std::mutex & transfer() { return list[transferIdx]; }
};

struct SurfaceDescription {
  vk::Extent2D extent{};
  vk::SurfaceFormatKHR format{};
  std::vector<vk::Image> images{};
  std::vector<vk::ImageView> imageViews{};
};

struct Allocator {
  rn::vki::memory::Pool buffer{};
  rn::vki::memory::Pool texture{};
  rn::vki::memory::Pool staging{};
};

} // rn::vki::context
