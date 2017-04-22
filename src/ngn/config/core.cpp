#include "core.hpp"

#include <json.hpp>
#include "../fs.hpp"

using json = nlohmann::json;

namespace ngn {

namespace config {

std::string toString(const VSync &value) {
	switch (value) {
		default:
		case VSync::Immediate: { return "immediate"; }
		case VSync::Double: { return "double"; }
		case VSync::Triple: { return "triple"; }
		case VSync::Mailbox: { return "mailbox"; }
	}
}

VSync fromString(const std::string &name, const VSync &defaultValue) {
	if (name == "immediate") { return VSync::Immediate; }
	if (name == "double") { return VSync::Double; }
	if (name == "triple") { return VSync::Triple; }
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

WindowMode fromString(const std::string &name, const WindowMode &defaultValue) {
	if (name == "windowed") { return WindowMode::Windowed; }
	if (name == "borderless") { return WindowMode::Borderless; }
	if (name == "fullscreen") { return WindowMode::Fullscreen; }
	return defaultValue;
}

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
	} catch (const std::domain_error &/*e*/) {
		physicalDevice.deviceId(0);
	} catch (const std::invalid_argument &/*e*/) {
		physicalDevice.deviceId(0);
	}

	try {
		physicalDevice.vendorId(body.at("/physicalDevice/vendorId"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		physicalDevice.vendorId(0);
	} catch (const std::domain_error &/*e*/) {
		physicalDevice.vendorId(0);
	} catch (const std::invalid_argument &/*e*/) {
		physicalDevice.vendorId(0);
	}

	try {
		window.width(body.at("/window/width"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.width(1600);
	} catch (const std::domain_error &/*e*/) {
		window.width(1600);
	} catch (const std::invalid_argument &/*e*/) {
		window.width(1600);
	}

	try {
		window.height(body.at("/window/height"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		window.height(900);
	} catch (const std::domain_error &/*e*/) {
		window.height(900);
	} catch (const std::invalid_argument &/*e*/) {
		window.height(900);
	}

	try {
		window.vsync(fromString(body.at("/window/vsync"_json_pointer).get<std::string>(), VSync::Double));
	} catch (const std::out_of_range &/*e*/) {
		window.vsync(VSync::Double);
	} catch (const std::domain_error &/*e*/) {
		window.vsync(VSync::Double);
	} catch (const std::invalid_argument &/*e*/) {
		window.vsync(VSync::Double);
	}

	try {
		window.mode(fromString(body.at("/window/mode"_json_pointer).get<std::string>(), WindowMode::Borderless));
	} catch (const std::out_of_range &/*e*/) {
		window.mode(WindowMode::Borderless);
	} catch (const std::domain_error &/*e*/) {
		window.mode(WindowMode::Borderless);
	} catch (const std::invalid_argument &/*e*/) {
		window.mode(WindowMode::Borderless);
	}

	try {
		debug.vulkanLogLevel(body.at("/debug/vulkanLogLevel"_json_pointer).get<uint32_t>());
	} catch (const std::out_of_range &/*e*/) {
		debug.vulkanLogLevel(1);
	} catch (const std::domain_error &/*e*/) {
		debug.vulkanLogLevel(1);
	} catch (const std::invalid_argument &/*e*/) {
		debug.vulkanLogLevel(1);
	}
}

std::string Core::dump(const int indent) {
	json body = {
		{"physicalDevice", {
			{"deviceId", physicalDevice.deviceId()},
			{"vendorId", physicalDevice.vendorId()}
		}},
		{"window", {
			{"width", window.width()},
			{"height", window.height()},
			{"vsync", toString(window.vsync())},
			{"mode", toString(window.mode())}
		}},
		{"debug", {
			{"vulkanLogLevel", debug.vulkanLogLevel()}
		}}
	};

	return body.dump(indent);
}

bool Core::store() {
	if (ngn::fs::contents(source(), dump(2), false)) {
		dirty(false);
		return true;
	} else {
		return false;
	}
}

} // config

} // ngn
