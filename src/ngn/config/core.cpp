#include "core.hpp"

#include <nlohmann/json.hpp>
#include "../fs.hpp"

using json = nlohmann::json;

namespace ngn::config {

std::string toString(const VSync &value) {
	switch (value) {
		default:
		case VSync::Immediate: { return "immediate"; }
		case VSync::Fifo: { return "fifo"; }
		case VSync::FifoRelaxed: { return "fiforelaxed"; }
		case VSync::Mailbox: { return "mailbox"; }
	}
}

VSync fromString(const std::string_view &name, const VSync &defaultValue) {
	if (name == "immediate") { return VSync::Immediate; }
	if (name == "fifo") { return VSync::Fifo; }
	if (name == "fiforelaxed") { return VSync::FifoRelaxed; }
	if (name == "mailbox") { return VSync::Mailbox; }
	return defaultValue;
}

std::string toString(const WindowMode &value) {
	switch (value) {
		default:
		case WindowMode::Windowed: { return "windowed"; }
		case WindowMode::Borderless: { return "borderless"; }
		case WindowMode::Fullscreen: { return "fullscreen"; }
	}
}

WindowMode fromString(const std::string_view &name, const WindowMode &defaultValue) {
	if (name == "windowed") { return WindowMode::Windowed; }
	if (name == "borderless") { return WindowMode::Borderless; }
	if (name == "fullscreen") { return WindowMode::Fullscreen; }
	return defaultValue;
}

std::string toString(const PixelFormat &value) {
	switch (value) {
		default:
		case PixelFormat::Undefined: { return "undefined"; }
		case PixelFormat::R8UNorm: { return "r8unorm"; }
		case PixelFormat::R8SNorm: { return "r8snorm"; }
		case PixelFormat::R8UInt: { return "r8uint"; }
		case PixelFormat::R8SInt: { return "r8sint"; }
		case PixelFormat::R8G8UNorm: { return "r8g8unorm"; }
		case PixelFormat::R8G8SNorm: { return "r8g8snorm"; }
		case PixelFormat::R8G8UInt: { return "r8g8uint"; }
		case PixelFormat::R8G8SInt: { return "r8g8sint"; }
		case PixelFormat::R8G8B8A8UNorm: { return "r8g8b8a8unorm"; }
		case PixelFormat::R8G8B8A8SNorm: { return "r8g8b8a8snorm"; }
		case PixelFormat::R8G8B8A8UInt: { return "r8g8b8a8uint"; }
		case PixelFormat::R8G8B8A8SInt: { return "r8g8b8a8sint"; }
		case PixelFormat::B8G8R8A8UNorm: { return "b8g8r8a8unorm"; }
		case PixelFormat::R16UNorm: { return "r16unorm"; }
		case PixelFormat::R16SNorm: { return "r16snorm"; }
		case PixelFormat::R16UInt: { return "r16uint"; }
		case PixelFormat::R16SInt: { return "r16sint"; }
		case PixelFormat::R16Float: { return "r16float"; }
		case PixelFormat::R16G16UNorm: { return "r16g16unorm"; }
		case PixelFormat::R16G16SNorm: { return "r16g16snorm"; }
		case PixelFormat::R16G16UInt: { return "r16g16uint"; }
		case PixelFormat::R16G16SInt: { return "r16g16sint"; }
		case PixelFormat::R16G16Float: { return "r16g16float"; }
		case PixelFormat::R16G16B16A16UNorm: { return "r16g16b16a16unorm"; }
		case PixelFormat::R16G16B16A16SNorm: { return "r16g16b16a16snorm"; }
		case PixelFormat::R16G16B16A16UInt: { return "r16g16b16a16uint"; }
		case PixelFormat::R16G16B16A16SInt: { return "r16g16b16a16sint"; }
		case PixelFormat::R16G16B16A16Float: { return "r16g16b16a16float"; }
		case PixelFormat::R32UInt: { return "r32uint"; }
		case PixelFormat::R32SInt: { return "r32sint"; }
		case PixelFormat::R32Float: { return "r32float"; }
		case PixelFormat::R32G32UInt: { return "r32g32uint"; }
		case PixelFormat::R32G32SInt: { return "r32g32sint"; }
		case PixelFormat::R32G32Float: { return "r32g32float"; }
		case PixelFormat::R32G32B32UInt: { return "r32g32b32uint"; }
		case PixelFormat::R32G32B32SInt: { return "r32g32b32sint"; }
		case PixelFormat::R32G32B32Float: { return "r32g32b32float"; }
		case PixelFormat::R32G32B32A32UInt: { return "r32g32b32a32uint"; }
		case PixelFormat::R32G32B32A32SInt: { return "r32g32b32a32sint"; }
		case PixelFormat::R32G32B32A32Float: { return "r32g32b32a32float"; }
		case PixelFormat::D16UNorm: { return "d16unorm"; }
		case PixelFormat::D32Float: { return "d32float"; }
		case PixelFormat::D24UNormS8UInt: { return "d24unorms8uint"; }
		case PixelFormat::D32FloatS8UInt: { return "d32floats8uint"; }
		case PixelFormat::BC1UNorm: { return "bc1unorm"; }
		case PixelFormat::BC1SRGB: { return "bc1srgb"; }
		case PixelFormat::BC2UNorm: { return "bc2unorm"; }
		case PixelFormat::BC2SRGB: { return "bc2srgb"; }
		case PixelFormat::BC3UNorm: { return "bc3unorm"; }
		case PixelFormat::BC3SRGB: { return "bc3srgb"; }
		case PixelFormat::BC4UNorm: { return "bc4unorm"; }
		case PixelFormat::BC4SNorm: { return "bc4snorm"; }
		case PixelFormat::BC5UNorm: { return "bc5unorm"; }
		case PixelFormat::BC5SNorm: { return "bc5snorm"; }
		case PixelFormat::BC6HUFloat: { return "bc6hufloat"; }
		case PixelFormat::BC6HSFloat: { return "bc6hsfloat"; }
		case PixelFormat::BC7UNorm: { return "bc7unorm"; }
		case PixelFormat::BC7SRGB: { return "bc7srgb"; }
		case PixelFormat::B4G4R4A4UNormPack16: { return "b4g4r4a4unormpack16"; }
		case PixelFormat::B5G6R5UNormPack16: { return "b5g6r5unormpack16"; }
		case PixelFormat::B5G5R5A1UNormPack16: { return "b5g5r5a1unormpack16"; }
		case PixelFormat::R10G10B10A2UNorm: { return "r10g10b10a2unorm"; }
		case PixelFormat::R10G10B10A2UInt: { return "r10g10b10a2uint"; }
		case PixelFormat::R11G11B10Float: { return "r11g11b10float"; }
	}
}

PixelFormat fromString(const std::string_view &name, const PixelFormat &defaultValue) {
	if (name == "undefined") { return PixelFormat::Undefined; }
	if (name == "r8unorm") { return PixelFormat::R8UNorm; }
	if (name == "r8snorm") { return PixelFormat::R8SNorm; }
	if (name == "r8uint") { return PixelFormat::R8UInt; }
	if (name == "r8sint") { return PixelFormat::R8SInt; }
	if (name == "r8g8unorm") { return PixelFormat::R8G8UNorm; }
	if (name == "r8g8snorm") { return PixelFormat::R8G8SNorm; }
	if (name == "r8g8uint") { return PixelFormat::R8G8UInt; }
	if (name == "r8g8sint") { return PixelFormat::R8G8SInt; }
	if (name == "r8g8b8a8unorm") { return PixelFormat::R8G8B8A8UNorm; }
	if (name == "r8g8b8a8snorm") { return PixelFormat::R8G8B8A8SNorm; }
	if (name == "r8g8b8a8uint") { return PixelFormat::R8G8B8A8UInt; }
	if (name == "r8g8b8a8sint") { return PixelFormat::R8G8B8A8SInt; }
	if (name == "b8g8r8a8unorm") { return PixelFormat::B8G8R8A8UNorm; }
	if (name == "r16unorm") { return PixelFormat::R16UNorm; }
	if (name == "r16snorm") { return PixelFormat::R16SNorm; }
	if (name == "r16uint") { return PixelFormat::R16UInt; }
	if (name == "r16sint") { return PixelFormat::R16SInt; }
	if (name == "r16float") { return PixelFormat::R16Float; }
	if (name == "r16g16unorm") { return PixelFormat::R16G16UNorm; }
	if (name == "r16g16snorm") { return PixelFormat::R16G16SNorm; }
	if (name == "r16g16uint") { return PixelFormat::R16G16UInt; }
	if (name == "r16g16sint") { return PixelFormat::R16G16SInt; }
	if (name == "r16g16float") { return PixelFormat::R16G16Float; }
	if (name == "r16g16b16a16unorm") { return PixelFormat::R16G16B16A16UNorm; }
	if (name == "r16g16b16a16snorm") { return PixelFormat::R16G16B16A16SNorm; }
	if (name == "r16g16b16a16uint") { return PixelFormat::R16G16B16A16UInt; }
	if (name == "r16g16b16a16sint") { return PixelFormat::R16G16B16A16SInt; }
	if (name == "r16g16b16a16float") { return PixelFormat::R16G16B16A16Float; }
	if (name == "r32uint") { return PixelFormat::R32UInt; }
	if (name == "r32sint") { return PixelFormat::R32SInt; }
	if (name == "r32float") { return PixelFormat::R32Float; }
	if (name == "r32g32uint") { return PixelFormat::R32G32UInt; }
	if (name == "r32g32sint") { return PixelFormat::R32G32SInt; }
	if (name == "r32g32float") { return PixelFormat::R32G32Float; }
	if (name == "r32g32b32uint") { return PixelFormat::R32G32B32UInt; }
	if (name == "r32g32b32sint") { return PixelFormat::R32G32B32SInt; }
	if (name == "r32g32b32float") { return PixelFormat::R32G32B32Float; }
	if (name == "r32g32b32a32uint") { return PixelFormat::R32G32B32A32UInt; }
	if (name == "r32g32b32a32sint") { return PixelFormat::R32G32B32A32SInt; }
	if (name == "r32g32b32a32float") { return PixelFormat::R32G32B32A32Float; }
	if (name == "d16unorm") { return PixelFormat::D16UNorm; }
	if (name == "d32float") { return PixelFormat::D32Float; }
	if (name == "d24unorms8uint") { return PixelFormat::D24UNormS8UInt; }
	if (name == "d32floats8uint") { return PixelFormat::D32FloatS8UInt; }
	if (name == "bc1unorm") { return PixelFormat::BC1UNorm; }
	if (name == "bc1srgb") { return PixelFormat::BC1SRGB; }
	if (name == "bc2unorm") { return PixelFormat::BC2UNorm; }
	if (name == "bc2srgb") { return PixelFormat::BC2SRGB; }
	if (name == "bc3unorm") { return PixelFormat::BC3UNorm; }
	if (name == "bc3srgb") { return PixelFormat::BC3SRGB; }
	if (name == "bc4unorm") { return PixelFormat::BC4UNorm; }
	if (name == "bc4snorm") { return PixelFormat::BC4SNorm; }
	if (name == "bc5unorm") { return PixelFormat::BC5UNorm; }
	if (name == "bc5snorm") { return PixelFormat::BC5SNorm; }
	if (name == "bc6hufloat") { return PixelFormat::BC6HUFloat; }
	if (name == "bc6hsfloat") { return PixelFormat::BC6HSFloat; }
	if (name == "bc7unorm") { return PixelFormat::BC7UNorm; }
	if (name == "bc7srgb") { return PixelFormat::BC7SRGB; }
	if (name == "b4g4r4a4unormpack16") { return PixelFormat::B4G4R4A4UNormPack16; }
	if (name == "b5g6r5unormpack16") { return PixelFormat::B5G6R5UNormPack16; }
	if (name == "b5g5r5a1unormpack16") { return PixelFormat::B5G5R5A1UNormPack16; }
	if (name == "r10g10b10a2unorm") { return PixelFormat::R10G10B10A2UNorm; }
	if (name == "r10g10b10a2uint") { return PixelFormat::R10G10B10A2UInt; }
	if (name == "r11g11b10float") { return PixelFormat::R11G11B10Float; }
	return defaultValue;
}

std::string toString(const ColorSpace &value) {
	switch (value) {
		default:
		case ColorSpace::LDR: { return "Ldr"; }
		case ColorSpace::HDR: { return "hdr"; }
	}
}

ColorSpace fromString(const std::string_view &name, const ColorSpace &defaultValue) {
	if (name == "ldr") { return ColorSpace::LDR; }
	if (name == "hdr") { return ColorSpace::HDR; }
	return defaultValue;
}

struct PixelFormatVisitor {
	json operator()(const PixelFormat &value) {
		return json{toString(value)};
	}
	json operator()(uint32_t value) {
		return json{value};
	}
};

struct ColorSpaceVisitor {
	json operator()(const ColorSpace &value) {
		return json{toString(value)};
	}
	json operator()(uint32_t value) {
		return json{value};
	}
};

void Core::load() {
	std::string contents = ngn::fs::contents(source(), false);
	if (contents.empty()) {
		contents = "{}";
		dirty(true);
	}

	json body = json::parse(contents);

	try {
		physicalDevice.deviceId(body.at("/physicalDevice/deviceId"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		physicalDevice.deviceId(0);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		physicalDevice.deviceId(0);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		physicalDevice.deviceId(0);
		dirty(true);
	}

	try {
		physicalDevice.vendorId(body.at("/physicalDevice/vendorId"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		physicalDevice.vendorId(0);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		physicalDevice.vendorId(0);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		physicalDevice.vendorId(0);
		dirty(true);
	}

	try {
		db.mesh.forceSynchronusLoading(body.at("/db/mesh/forceSynchronusLoading"_json_pointer).get<bool>());
	} catch (const std::out_of_range &/*e*/) {
		db.mesh.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		db.mesh.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		db.mesh.forceSynchronusLoading(false);
		dirty(true);
	}

	try {
		db.texture.forceSynchronusLoading(body.at("/db/texture/forceSynchronusLoading"_json_pointer).get<bool>());
	} catch (const std::out_of_range &/*e*/) {
		db.texture.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		db.texture.forceSynchronusLoading(false);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		db.texture.forceSynchronusLoading(false);
		dirty(true);
	}

	try {
		window.width(body.at("/window/width"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.width(1600);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.width(1600);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.width(1600);
		dirty(true);
	}

	try {
		window.height(body.at("/window/height"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.height(900);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.height(900);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.height(900);
		dirty(true);
	}

	try {
		window.mode(fromString(body.at("/window/mode"_json_pointer).get<std::string>(), WindowMode::Borderless));
	} catch (const std::out_of_range &/*e*/) {
		window.mode(WindowMode::Borderless);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.mode(WindowMode::Borderless);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.mode(WindowMode::Borderless);
		dirty(true);
	}

	try {
		window.monitor(body.at("/window/monitor"_json_pointer).get<int32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.monitor(-1);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.monitor(-1);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.monitor(-1);
		dirty(true);
	}

	try {
		window.vsync(fromString(body.at("/window/vsync"_json_pointer).get<std::string>(), VSync::Mailbox));
	} catch (const std::out_of_range &/*e*/) {
		window.vsync(VSync::Mailbox);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.vsync(VSync::Mailbox);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.vsync(VSync::Mailbox);
		dirty(true);
	}

	try {
		window.imageBuffers(body.at("/window/imageBuffers"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.imageBuffers(3);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.imageBuffers(3);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.imageBuffers(3);
		dirty(true);
	}

	try {
		if (body.at("/window/surfaceFormat"_json_pointer).is_number_unsigned()) {
			window.surfaceFormat(body.at("/window/surfaceFormat"_json_pointer).get<uint32_t>());
		} else {
			window.surfaceFormat(fromString(body.at("/window/surfaceFormat"_json_pointer).get<std::string>(), PixelFormat::B8G8R8A8UNorm));
		}

		// window.surfaceFormat(fromString(body.at("/window/surfaceFormat"_json_pointer).get<std::string>(), PixelFormat::B8G8R8A8UNorm));
	} catch (const std::out_of_range &/*e*/) {
		window.surfaceFormat(PixelFormat::B8G8R8A8UNorm);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.surfaceFormat(PixelFormat::B8G8R8A8UNorm);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.surfaceFormat(PixelFormat::B8G8R8A8UNorm);
		dirty(true);
	}

	try {
		if (body.at("/window/surfaceColorSpace"_json_pointer).is_number_unsigned()) {
			window.surfaceColorSpace(body.at("/window/surfaceColorSpace"_json_pointer).get<uint32_t>());
		} else {
			window.surfaceColorSpace(fromString(body.at("/window/surfaceColorSpace"_json_pointer).get<std::string>(), ColorSpace::LDR));
		}
		// window.surfaceColorSpace(fromString(body.at("/window/surfaceColorSpace"_json_pointer).get<std::string>(), ColorSpace::LDR));
	} catch (const std::out_of_range &/*e*/) {
		window.surfaceColorSpace(ColorSpace::LDR);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		window.surfaceColorSpace(ColorSpace::LDR);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		window.surfaceColorSpace(ColorSpace::LDR);
		dirty(true);
	}

	try {
		debug.vk.useRenderDoc(body.at("/debug/vk/useRenderDoc"_json_pointer).get<bool>());
	} catch (const std::out_of_range &/*e*/) {
		debug.vk.useRenderDoc(false);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		debug.vk.useRenderDoc(false);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		debug.vk.useRenderDoc(false);
		dirty(true);
	}

	try {
		debug.vk.logLevel(body.at("/debug/vk/logLevel"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		debug.vk.logLevel(1);
		dirty(true);
	} catch (const std::domain_error &/*e*/) {
		debug.vk.logLevel(1);
		dirty(true);
	} catch (const std::invalid_argument &/*e*/) {
		debug.vk.logLevel(1);
		dirty(true);
	}
}

std::string Core::dump(const int indent) {
	json body = {
		{"physicalDevice", {
			{"deviceId", physicalDevice.deviceId()},
			{"vendorId", physicalDevice.vendorId()}
		}},
		{"db", {
			{"mesh", {
				{"forceSynchronusLoading", db.mesh.forceSynchronusLoading()}
			}},
			{"texture", {
				{"forceSynchronusLoading", db.texture.forceSynchronusLoading()}
			}}
		}},
		{"window", {
			{"width", window.width()},
			{"height", window.height()},
			{"mode", toString(window.mode())},
			{"monitor", window.monitor()},
			{"vsync", toString(window.vsync())},
			{"imageBuffers", window.imageBuffers()},
			{"surfaceFormat", std::visit(PixelFormatVisitor{}, window.surfaceFormat())},
			{"surfaceColorSpace", std::visit(ColorSpaceVisitor{}, window.surfaceColorSpace())}
		}},
		{"debug", {
			{"vk", {
				{"useRenderDoc", debug.vk.useRenderDoc()},
				{"logLevel", debug.vk.logLevel()}
			}}
		}}
	};

	return body.dump(indent);
}

bool Core::store() {
	if (ngn::fs::write(source(), dump(2), false)) {
		dirty(false);
		return true;
	} else {
		return false;
	}
}

} // ngn::config
