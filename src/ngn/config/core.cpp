#include "core.hpp"

#include <tuple>

#include <nlohmann/json.hpp>

#include "../fs.hpp"
#include "../log.hpp"

using json = nlohmann::json;

namespace rn {

NLOHMANN_JSON_SERIALIZE_ENUM(VSync, {
	{VSync::Fifo, "fifo"},
	{VSync::FifoRelaxed, "fiforelaxed"},
	{VSync::Mailbox, "mailbox"},
	{VSync::Immediate, "immediate"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(WindowMode, {
	{WindowMode::Windowed, "windowed"},
	{WindowMode::Borderless, "borderless"},
	{WindowMode::Fullscreen, "fullscreen"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(PixelFormat, {
	{PixelFormat::Undefined, "undefined"},
	{PixelFormat::R8UNorm, "r8unorm"},
	{PixelFormat::R8SNorm, "r8snorm"},
	{PixelFormat::R8UInt, "r8uint"},
	{PixelFormat::R8SInt, "r8sint"},
	{PixelFormat::R8G8UNorm, "r8g8unorm"},
	{PixelFormat::R8G8SNorm, "r8g8snorm"},
	{PixelFormat::R8G8UInt, "r8g8uint"},
	{PixelFormat::R8G8SInt, "r8g8sint"},
	{PixelFormat::R8G8B8A8UNorm, "r8g8b8a8unorm"},
	{PixelFormat::R8G8B8A8SNorm, "r8g8b8a8snorm"},
	{PixelFormat::R8G8B8A8UInt, "r8g8b8a8uint"},
	{PixelFormat::R8G8B8A8SInt, "r8g8b8a8sint"},
	{PixelFormat::B8G8R8A8UNorm, "b8g8r8a8unorm"},
	{PixelFormat::R16UNorm, "r16unorm"},
	{PixelFormat::R16SNorm, "r16snorm"},
	{PixelFormat::R16UInt, "r16uint"},
	{PixelFormat::R16SInt, "r16sint"},
	{PixelFormat::R16Float, "r16float"},
	{PixelFormat::R16G16UNorm, "r16g16unorm"},
	{PixelFormat::R16G16SNorm, "r16g16snorm"},
	{PixelFormat::R16G16UInt, "r16g16uint"},
	{PixelFormat::R16G16SInt, "r16g16sint"},
	{PixelFormat::R16G16Float, "r16g16float"},
	{PixelFormat::R16G16B16A16UNorm, "r16g16b16a16unorm"},
	{PixelFormat::R16G16B16A16SNorm, "r16g16b16a16snorm"},
	{PixelFormat::R16G16B16A16UInt, "r16g16b16a16uint"},
	{PixelFormat::R16G16B16A16SInt, "r16g16b16a16sint"},
	{PixelFormat::R16G16B16A16Float, "r16g16b16a16float"},
	{PixelFormat::R32UInt, "r32uint"},
	{PixelFormat::R32SInt, "r32sint"},
	{PixelFormat::R32Float, "r32float"},
	{PixelFormat::R32G32UInt, "r32g32uint"},
	{PixelFormat::R32G32SInt, "r32g32sint"},
	{PixelFormat::R32G32Float, "r32g32float"},
	{PixelFormat::R32G32B32UInt, "r32g32b32uint"},
	{PixelFormat::R32G32B32SInt, "r32g32b32sint"},
	{PixelFormat::R32G32B32Float, "r32g32b32float"},
	{PixelFormat::R32G32B32A32UInt, "r32g32b32a32uint"},
	{PixelFormat::R32G32B32A32SInt, "r32g32b32a32sint"},
	{PixelFormat::R32G32B32A32Float, "r32g32b32a32float"},
	{PixelFormat::D16UNorm, "d16unorm"},
	{PixelFormat::D32Float, "d32float"},
	{PixelFormat::D24UNormS8UInt, "d24unorms8uint"},
	{PixelFormat::D32FloatS8UInt, "d32floats8uint"},
	{PixelFormat::BC1UNorm, "bc1unorm"},
	{PixelFormat::BC1SRGB, "bc1srgb"},
	{PixelFormat::BC2UNorm, "bc2unorm"},
	{PixelFormat::BC2SRGB, "bc2srgb"},
	{PixelFormat::BC3UNorm, "bc3unorm"},
	{PixelFormat::BC3SRGB, "bc3srgb"},
	{PixelFormat::BC4UNorm, "bc4unorm"},
	{PixelFormat::BC4SNorm, "bc4snorm"},
	{PixelFormat::BC5UNorm, "bc5unorm"},
	{PixelFormat::BC5SNorm, "bc5snorm"},
	{PixelFormat::BC6HUFloat, "bc6hufloat"},
	{PixelFormat::BC6HSFloat, "bc6hsfloat"},
	{PixelFormat::BC7UNorm, "bc7unorm"},
	{PixelFormat::BC7SRGB, "bc7srgb"},
	{PixelFormat::B4G4R4A4UNormPack16, "b4g4r4a4unormpack16"},
	{PixelFormat::B5G6R5UNormPack16, "b5g6r5unormpack16"},
	{PixelFormat::B5G5R5A1UNormPack16, "b5g5r5a1unormpack16"},
	{PixelFormat::R10G10B10A2UNorm, "r10g10b10a2unorm"},
	{PixelFormat::R10G10B10A2UInt, "r10g10b10a2uint"},
	{PixelFormat::R11G11B10Float, "r11g11b10float"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(ColorSpace, {
	{ColorSpace::Undefined, "undefined"},
	{ColorSpace::LDR, "ldr"},
	{ColorSpace::HDR, "hdr"},
})

} // rn

namespace nlohmann {

template <>
struct adl_serializer<std::variant<rn::PixelFormat, uint32_t>> {
	static void to_json(json &j, const std::variant<rn::PixelFormat, uint32_t> &pixelFormat) {
		if (pixelFormat.index() == 1) {
			j = std::get<1>(pixelFormat);
		} else {
			j = std::get<0>(pixelFormat);
		}
	}

	static void from_json(const json &j, std::variant<rn::PixelFormat, uint32_t> &pixelFormat) {
		if (j.is_number_unsigned()) {
			pixelFormat = j.get<uint32_t>();
		} else if (j.is_string()) {
			pixelFormat = j.get<rn::PixelFormat>();
		}
	}
};

template <>
struct adl_serializer<std::variant<rn::ColorSpace, uint32_t>> {
	static void to_json(json &j, const std::variant<rn::ColorSpace, uint32_t> &colorSpace) {
		if (colorSpace.index() == 1) {
			j = std::get<1>(colorSpace);
		} else {
			j = std::get<0>(colorSpace);
		}
	}

	static void from_json(const json &j, std::variant<rn::ColorSpace, uint32_t> &colorSpace) {
		if (j.is_number_unsigned()) {
			colorSpace = j.get<uint32_t>();
		} else if (j.is_string()) {
			colorSpace = j.get<rn::ColorSpace>();
		}
	}
};

template <>
struct adl_serializer<ngn::config::Core::Context::Vki::FamilyIndex> {
	static void to_json(json& j, const ngn::config::Core::Context::Vki::FamilyIndex& familyIndex) {
		j.emplace("index", familyIndex.index);
		j.emplace("family", familyIndex.family);
	}

	static void from_json(const json& j, ngn::config::Core::Context::Vki::FamilyIndex& familyIndex) {
		familyIndex.index = j.value("index", familyIndex.index);
		familyIndex.family = j.value("family", familyIndex.family);
	}
};

template <>
struct adl_serializer<ngn::config::Core::Context::Vki> {
	static void to_json(json& j, const ngn::config::Core::Context::Vki& vki) {
		j.emplace("physicalDeviceId", vki.physicalDeviceId);
		j.emplace("physicalDeviceVendorId", vki.physicalDeviceVendorId);

		if (vki.forceGraphicQueue.family >= 0) {
			j.emplace("forceGraphicQueue", vki.forceGraphicQueue);
		}

		if (vki.forceComputeQueue.family >= 0) {
			j.emplace("forceComputeQueue", vki.forceComputeQueue);
		}

		if (vki.forceTransferQueue.family >= 0) {
			j.emplace("forceTransferQueue", vki.forceTransferQueue);
		}

		if (vki.forcePresentQueue.family >= 0) {
			j.emplace("forcePresentQueue", vki.forcePresentQueue);
		}
	}

	static void from_json(const json& j, ngn::config::Core::Context::Vki& vki) {
		vki.physicalDeviceId = j.value("physicalDeviceId", vki.physicalDeviceId);
		vki.physicalDeviceVendorId = j.value("physicalDeviceVendorId", vki.physicalDeviceVendorId);
		vki.forceGraphicQueue = j.value("forceGraphicQueue", vki.forceGraphicQueue);
		vki.forceComputeQueue = j.value("forceComputeQueue", vki.forceComputeQueue);
		vki.forceTransferQueue = j.value("forceTransferQueue", vki.forceTransferQueue);
		vki.forcePresentQueue = j.value("forcePresentQueue", vki.forcePresentQueue);
	}
};

template <>
struct adl_serializer<ngn::config::Core::Debug::Vki> {
	static void to_json(json& j, const ngn::config::Core::Debug::Vki& vki) {
		j.emplace("useRenderDoc", vki.useRenderDoc);
		j.emplace("logLevel", vki.logLevel);
	}

	static void from_json(const json& j, ngn::config::Core::Debug::Vki& vki) {
		vki.useRenderDoc = j.value("useRenderDoc", vki.useRenderDoc);
		vki.logLevel = j.value("logLevel", vki.logLevel);
	}
};

template <>
struct adl_serializer<ngn::config::Core::Threading> {
	static void to_json(json& j, const ngn::config::Core::Threading& threading) {
		j.emplace("forcePoolSize", threading.forcePoolSize);
	}

	static void from_json(const json& j, ngn::config::Core::Threading& threading) {
		threading.forcePoolSize = j.value("forcePoolSize", threading.forcePoolSize);
	}
};

template <>
struct adl_serializer<ngn::config::Core::Debug> {
	static void to_json(json& j, const ngn::config::Core::Debug& debug) {
		j.emplace("vki", debug.vki);
	}

	static void from_json(const json& j, ngn::config::Core::Debug& debug) {
		debug.vki = j.value("vki", debug.vki);
	}
};

template <>
struct adl_serializer<ngn::config::Core::Window> {
	static void to_json(json& j, const ngn::config::Core::Window& window) {
		j.emplace("width", window.width);
		j.emplace("height", window.height);
		j.emplace("mode", window.mode);
		j.emplace("monitor", window.monitor);
		j.emplace("vsync", window.vsync);
		j.emplace("swapchainSize", window.swapchainSize);
		j.emplace("surfaceFormat", window.surfaceFormat);
		j.emplace("surfaceColorSpace", window.surfaceColorSpace);
	}

	static void from_json(const json& j, ngn::config::Core::Window& window) {
		window.width = j.value("width", window.width);
		window.height = j.value("height", window.height);
		window.mode = j.value("mode", window.mode);
		window.monitor = j.value("monitor", window.monitor);
		window.vsync = j.value("vsync", window.vsync);
		window.swapchainSize = j.value("swapchainSize", window.swapchainSize);
		window.surfaceFormat = j.value("surfaceFormat", window.surfaceFormat);
		window.surfaceColorSpace = j.value("surfaceColorSpace", window.surfaceColorSpace);
	}
};

// template <>
// struct adl_serializer<ngn::config::Core::PhysicalDevice> {
// 	static void to_json(json& j, const ngn::config::Core::PhysicalDevice& physicalDevice) {
// 		j.emplace("deviceId", physicalDevice.deviceId);
// 		j.emplace("vendorId", physicalDevice.vendorId);
// 	}

// 	static void from_json(const json& j, ngn::config::Core::PhysicalDevice& physicalDevice) {
// 		physicalDevice.deviceId = j.value("deviceId", physicalDevice.deviceId);
// 		physicalDevice.vendorId = j.value("vendorId", physicalDevice.vendorId);
// 	}
// };

template <>
struct adl_serializer<ngn::config::Core::Context> {
	static void to_json(json& j, const ngn::config::Core::Context& context) {
		j.emplace("vki", context.vki);
	}

	static void from_json(const json& j, ngn::config::Core::Context& context) {
		context.vki = j.value("vki", context.vki);
	}
};

template <>
struct adl_serializer<ngn::config::Core> {
	static void to_json(json& j, const ngn::config::Core& core) {
		// j.emplace("physicalDevice", core.physicalDevice);
		j.emplace("threading", core.threading);
		j.emplace("context", core.context);
		j.emplace("window", core.window);
		j.emplace("debug", core.debug);
	}

	static void from_json(const json& j, ngn::config::Core& core) {
		// core.physicalDevice = j.value("physicalDevice", core.physicalDevice);
		core.threading = j.value("threading", core.threading);
		core.context = j.value("context", core.context);
		core.window = j.value("window", core.window);
		core.debug = j.value("debug", core.debug);
	}
};

} // nlohmann

namespace ngn::config {

bool operator==(const Core::Context::Vki::FamilyIndex &rhs, const Core::Context::Vki::FamilyIndex &lhs) {
	return std::tie(rhs.index, rhs.family) == std::tie(lhs.index, lhs.family);
}

bool operator==(const Core::Context::Vki &rhs, const Core::Context::Vki &lhs) {
	return std::tie(rhs.physicalDeviceId, rhs.physicalDeviceVendorId, rhs.forceGraphicQueue, rhs.forceComputeQueue, rhs.forceTransferQueue, rhs.forcePresentQueue) == std::tie(lhs.physicalDeviceId, lhs.physicalDeviceVendorId, lhs.forceGraphicQueue, lhs.forceComputeQueue, lhs.forceTransferQueue, lhs.forcePresentQueue);
}

bool operator==(const Core::Debug::Vki &rhs, const Core::Debug::Vki &lhs) {
	return std::tie(rhs.useRenderDoc, rhs.logLevel) == std::tie(lhs.useRenderDoc, lhs.logLevel);
}

bool operator==(const Core::Engine::Version &rhs, const Core::Engine::Version &lhs) {
	return std::tie(rhs.major, rhs.minor, rhs.patch) == std::tie(lhs.major, lhs.minor, lhs.patch);
}

bool operator==(const Core::Application::Version &rhs, const Core::Application::Version &lhs) {
	return std::tie(rhs.major, rhs.minor, rhs.patch) == std::tie(lhs.major, lhs.minor, lhs.patch);
}

bool operator==(const Core::Debug &rhs, const Core::Debug &lhs) {
	return std::tie(rhs.vki) == std::tie(lhs.vki);
}

bool operator==(const Core::Window &rhs, const Core::Window &lhs) {
	return std::tie(rhs.width, rhs.height, rhs.mode, rhs.monitor, rhs.vsync, rhs.swapchainSize, rhs.surfaceFormat, rhs.surfaceColorSpace) == std::tie(lhs.width, lhs.height, lhs.mode, lhs.monitor, lhs.vsync, lhs.swapchainSize, lhs.surfaceFormat, lhs.surfaceColorSpace);
}

// bool operator==(const Core::PhysicalDevice &rhs, const Core::PhysicalDevice &lhs) {
// 	return std::tie(rhs.deviceId, rhs.vendorId) == std::tie(lhs.deviceId, lhs.vendorId);
// }

bool operator==(const Core::Context &rhs, const Core::Context &lhs) {
	return std::tie(rhs.vki) == std::tie(lhs.vki);
}

bool operator==(const Core::Threading &rhs, const Core::Threading &lhs) {
	return std::tie(rhs.forcePoolSize) == std::tie(lhs.forcePoolSize);
}

bool operator==(const Core::Engine &rhs, const Core::Engine &lhs) {
	return std::tie(rhs.name, rhs.version) == std::tie(lhs.name, lhs.version);
}

bool operator==(const Core::Application &rhs, const Core::Application &lhs) {
	return std::tie(rhs.name, rhs.version) == std::tie(lhs.name, lhs.version);
}

bool operator==(const Core &rhs, const Core &lhs) {
	return std::tie(rhs.dirty, rhs.application, rhs.engine, /*rhs.physicalDevice*/ rhs.context, rhs.window, rhs.debug) == std::tie(lhs.dirty, lhs.application, lhs.engine, /*lhs.physicalDevice*/ lhs.context, lhs.window, lhs.debug);
}

bool operator!=(const Core::Context::Vki::FamilyIndex &rhs, const Core::Context::Vki::FamilyIndex &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Context::Vki &rhs, const Core::Context::Vki &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Debug::Vki &rhs, const Core::Debug::Vki &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Engine::Version &rhs, const Core::Engine::Version &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Application::Version &rhs, const Core::Application::Version &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Debug &rhs, const Core::Debug &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Window &rhs, const Core::Window &lhs) {
	return ! (rhs == lhs);
}

// bool operator!=(const Core::PhysicalDevice &rhs, const Core::PhysicalDevice &lhs) {
// 	return ! (rhs == lhs);
// }

bool operator!=(const Core::Threading &rhs, const Core::Threading &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Engine &rhs, const Core::Engine &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core::Application &rhs, const Core::Application &lhs) {
	return ! (rhs == lhs);
}

bool operator!=(const Core &rhs, const Core &lhs) {
	return ! (rhs == lhs);
}

Core Core::load(std::string_view filePath) {
	std::optional<std::string> contentsO = ngn::fs::contents(filePath);
	if ( ! contentsO || contentsO->empty()) {
		contentsO = "{}";
	}

	// return json::parse(contentsO.value()).get<Core>();
	Core core = json::parse(contentsO.value()).get<Core>();

	/*
	ngn::log::debug("{}={}", filePath, contentsO.value());
	ngn::log::debug("core.dirty={}", core.dirty);
	ngn::log::debug("core.application.name={}", core.application.name);
	ngn::log::debug("core.application.version.major={}", core.application.version.major);
	ngn::log::debug("core.application.version.minor={}", core.application.version.minor);
	ngn::log::debug("core.application.version.patch={}", core.application.version.patch);
	ngn::log::debug("core.engine.name={}", core.engine.name);
	ngn::log::debug("core.engine.version.major={}", core.engine.version.major);
	ngn::log::debug("core.engine.version.minor={}", core.engine.version.minor);
	ngn::log::debug("core.engine.version.patch={}", core.engine.version.patch);
	ngn::log::debug("core.context.vki.physicalDeviceId={}", core.context.vki.physicalDeviceId);
	ngn::log::debug("core.context.vki.physicalDeviceVendorId={}", core.context.vki.physicalDeviceVendorId);
	ngn::log::debug("core.context.vki.forceGraphicQueueFamily={}", core.context.vki.forceGraphicQueueFamily);
	ngn::log::debug("core.context.vki.forceGraphicQueueIndex={}", core.context.vki.forceGraphicQueueIndex);
	ngn::log::debug("core.context.vki.forceComputeQueueFamily={}", core.context.vki.forceComputeQueueFamily);
	ngn::log::debug("core.context.vki.forceComputeQueueIndex={}", core.context.vki.forceComputeQueueIndex);
	ngn::log::debug("core.context.vki.forceTransferQueueFamily={}", core.context.vki.forceTransferQueueFamily);
	ngn::log::debug("core.context.vki.forceTransferQueueIndex={}", core.context.vki.forceTransferQueueIndex);
	ngn::log::debug("core.context.vki.forcePresentQueueFamily={}", core.context.vki.forcePresentQueueFamily);
	ngn::log::debug("core.context.vki.forcePresentQueueIndex={}", core.context.vki.forcePresentQueueIndex);
	ngn::log::debug("core.window.width={}", core.window.width);
	ngn::log::debug("core.window.height={}", core.window.height);
	switch (core.window.mode) {
		case rn::WindowMode::Windowed: {
			ngn::log::debug("core.window.mode=Windowed");
			break;
		}
		case rn::WindowMode::Borderless: {
			ngn::log::debug("core.window.mode=Borderless");
			break;
		}
		case rn::WindowMode::Fullscreen: {
			ngn::log::debug("core.window.mode=Fullscreen");
			break;
		}
		default: {
			ngn::log::debug("core.window.mode=Unknown");
		}
	}
	ngn::log::debug("core.window.monitor={}", core.window.monitor);
	switch (core.window.vsync) {
		case rn::VSync::Fifo: {
			ngn::log::debug("core.window.vsync=Fifo");
			break;
		}
		case rn::VSync::FifoRelaxed: {
			ngn::log::debug("core.window.vsync=FifoRelaxed");
			break;
		}
		case rn::VSync::Mailbox: {
			ngn::log::debug("core.window.vsync=Mailbox");
			break;
		}
		case rn::VSync::Immediate: {
			ngn::log::debug("core.window.vsync=Immediate");
			break;
		}
		default: {
			ngn::log::debug("core.window.vsync=Unknown");
		}
	}
	ngn::log::debug("core.window.swapchainSize={}", core.window.swapchainSize);
	if (auto value = std::get_if<rn::PixelFormat>(&core.window.surfaceFormat)) {
		ngn::log::debug("core.window.surfaceFormat=PixelFormat[{}]", static_cast<size_t>(*value));
	} else if (auto value = std::get_if<uint32_t>(&core.window.surfaceFormat)) {
		ngn::log::debug("core.window.surfaceFormat={:#4x}", *value);
	} else {
		ngn::log::debug("core.window.surfaceFormat=Unknown");
	}

	// ngn::log::debug("core.window.surfaceColorSpace={}", core.window.surfaceColorSpace);
	if (auto value = std::get_if<rn::ColorSpace>(&core.window.surfaceColorSpace)) {
		ngn::log::debug("core.window.surfaceColorSpace=ColorSpace[{}]", static_cast<size_t>(*value));
	} else if (auto value = std::get_if<uint32_t>(&core.window.surfaceColorSpace)) {
		ngn::log::debug("core.window.surfaceColorSpace={:#4x}", *value);
	} else {
		ngn::log::debug("core.window.surfaceColorSpace=Unknown");
	}
	ngn::log::debug("core.debug.vki.useRenderDoc={}", core.debug.vki.useRenderDoc);
	ngn::log::debug("core.debug.vki.logLevel={}", core.debug.vki.logLevel);

	// rn::WindowMode mode{rn::WindowMode::Borderless};
	// rn::VSync vsync{rn::VSync::Mailbox};
	// std::variant<rn::PixelFormat, uint32_t> surfaceFormat{rn::PixelFormat::B8G8R8A8UNorm};
	// std::variant<rn::ColorSpace, uint32_t> surfaceColorSpace{rn::ColorSpace::Undefined};
	*/

	return core;
}

std::string Core::dump(const Core &core, int indent) {
	json body = core;

	return body.dump(indent);
}

std::optional<std::string> Core::store(const Core &core, std::string_view filePath) {
	return ngn::fs::write(filePath, Core::dump(core, 2));
}

} // ngn::config
