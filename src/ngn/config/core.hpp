#pragma once

#include <optional>
#include <string>
#include <variant>
#include "../../rn/types.hpp"

namespace ngn::config {

struct Core {
	bool dirty{false};

	struct Application {
		std::string name{"pr0-vk"};

		struct Version {
			uint32_t major{0};
			uint32_t minor{1};
			uint32_t patch{0};
		} version{};
	} application{};

	struct Engine {
		std::string name{"ngn"};

		struct Version {
			uint32_t major{0};
			uint32_t minor{1};
			uint32_t patch{0};
		} version{};
	} engine{};

	struct Threading {
		int32_t forcePoolSize{-1};
	} threading;

	// struct PhysicalDevice {
	// 	uint32_t deviceId{0};
	// 	uint32_t vendorId{0};
	// } physicalDevice{};

	struct Context {
		struct Vki {
			uint32_t physicalDeviceId{0};
			uint32_t physicalDeviceVendorId{0};

			struct FamilyIndex {
				int32_t family{-1};
				int32_t index{-1};
			};

			FamilyIndex forceGraphicQueue{};
			FamilyIndex forceComputeQueue{};
			FamilyIndex forceTransferQueue{};
			FamilyIndex forcePresentQueue{};
		} vki{};
	} context{};

	struct Window {
		uint32_t width{1600};
		uint32_t height{900};
		rn::WindowMode mode{rn::WindowMode::Borderless};
		int32_t monitor{-1};
		rn::VSync vsync{rn::VSync::Mailbox};
		uint32_t swapchainSize{3};
		std::variant<rn::PixelFormat, uint32_t> surfaceFormat{rn::PixelFormat::B8G8R8A8UNorm};
		std::variant<rn::ColorSpace, uint32_t> surfaceColorSpace{rn::ColorSpace::Undefined};
	} window{};

	struct Debug {
		struct Vki {
			bool useRenderDoc{false};
			uint32_t logLevel{1};
		} vki{};
	} debug{};

	static Core load(std::string_view filePath = "core.json");
	static std::string dump(const Core &core, int indent = 2);
	static std::optional<std::string> store(const Core &core, std::string_view filePath = "core.json");
};

bool operator==(const Core::Context::Vki &rhs, const Core::Context::Vki &lhs);
bool operator!=(const Core::Context::Vki &rhs, const Core::Context::Vki &lhs);
bool operator==(const Core::Debug::Vki &rhs, const Core::Debug::Vki &lhs);
bool operator!=(const Core::Debug::Vki &rhs, const Core::Debug::Vki &lhs);
bool operator==(const Core::Engine::Version &rhs, const Core::Engine::Version &lhs);
bool operator!=(const Core::Engine::Version &rhs, const Core::Engine::Version &lhs);
bool operator==(const Core::Application::Version &rhs, const Core::Application::Version &lhs);
bool operator!=(const Core::Application::Version &rhs, const Core::Application::Version &lhs);
bool operator==(const Core::Debug &rhs, const Core::Debug &lhs);
bool operator!=(const Core::Debug &rhs, const Core::Debug &lhs);
bool operator==(const Core::Window &rhs, const Core::Window &lhs);
bool operator!=(const Core::Window &rhs, const Core::Window &lhs);
// bool operator==(const Core::PhysicalDevice &rhs, const Core::PhysicalDevice &lhs);
// bool operator!=(const Core::PhysicalDevice &rhs, const Core::PhysicalDevice &lhs);
bool operator==(const Core::Context &rhs, const Core::Context &lhs);
bool operator!=(const Core::Context &rhs, const Core::Context &lhs);
bool operator==(const Core::Engine &rhs, const Core::Engine &lhs);
bool operator!=(const Core::Engine &rhs, const Core::Engine &lhs);
bool operator==(const Core::Application &rhs, const Core::Application &lhs);
bool operator!=(const Core::Application &rhs, const Core::Application &lhs);
bool operator==(const Core &rhs, const Core &lhs);
bool operator!=(const Core &rhs, const Core &lhs);

} // ngn::config
