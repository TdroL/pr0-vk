#pragma once

#include <string>

namespace ngn {

namespace config {

enum class VSync {
	Immediate,
	Double,
	Triple,
	Mailbox
};

std::string toString(const VSync &value);
VSync fromString(const std::string &name, const VSync &defaultValue);

enum class WindowMode {
	Windowed,
	Borderless,
	Fullscreen
};

std::string toString(const WindowMode &value);
WindowMode fromString(const std::string &name, const WindowMode &defaultValue);

struct Core {
	Core &root{*this};

	Core() { load(); }

	struct Application {
		Core &root;

		Application(Core &root) : root(root) {}

		std::string prop_name{"pr0-vk"};
		const std::string & name() {
			return prop_name;
		}

		struct Version {
			Core &root;

			Version(Core &root) : root(root) {}

			uint32_t prop_major{0};
			uint32_t major() {
				return prop_major;
			}

			uint32_t prop_minor{0};
			uint32_t minor() {
				return prop_minor;
			}

			uint32_t prop_patch{0};
			uint32_t patch() {
				return prop_patch;
			}
		} version{root};
	} application{root};

	struct Engine {
		Core &root;

		Engine(Core &root) : root(root) {}

		std::string prop_name{"ngn"};
		const std::string & name() {
			return prop_name;
		}

		struct Version {
			Core &root;

			Version(Core &root) : root(root) {}

			uint32_t prop_major{0};
			uint32_t major() {
				return prop_major;
			}

			uint32_t prop_minor{0};
			uint32_t minor() {
				return prop_minor;
			}

			uint32_t prop_patch{0};
			uint32_t patch() {
				return prop_patch;
			}
		} version{root};
	} engine{root};

	struct PhysicalDevice {
		Core &root;

		PhysicalDevice(Core &root) : root(root) {}

		uint32_t prop_deviceId{0};
		uint32_t deviceId() {
			return prop_deviceId;
		}

		void deviceId(uint32_t newValue) {
			prop_deviceId = newValue;
			root.dirty(true);
		}

		uint32_t prop_vendorId{0};
		uint32_t vendorId() {
			return prop_vendorId;
		}

		void vendorId(uint32_t newValue) {
			prop_vendorId = newValue;
			root.dirty(true);
		}
	} physicalDevice{root};

	struct Window {
		Core &root;

		Window(Core &root) : root(root) {}

		uint32_t prop_width{1600};
		uint32_t width() {
			return prop_width;
		}

		void width(uint32_t newValue) {
			prop_width = newValue;
			root.dirty(true);
		}

		uint32_t prop_height{900};
		uint32_t height() {
			return prop_height;
		}

		void height(uint32_t newValue) {
			prop_height = newValue;
			root.dirty(true);
		}

		VSync prop_vsync{VSync::Double};
		const VSync & vsync() {
			return prop_vsync;
		}

		void vsync(const VSync &newValue) {
			prop_vsync = newValue;
			root.dirty(true);
		}

		WindowMode prop_mode{WindowMode::Borderless};
		const WindowMode & mode() {
			return prop_mode;
		}

		void mode(const WindowMode &newValue) {
			prop_mode = newValue;
			root.dirty(true);
		}
	} window{root};

	struct Debug {
		Core &root;

		Debug(Core &root) : root(root) {}

		uint32_t prop_vulkanLogLevel{1};
		uint32_t vulkanLogLevel() {
			return prop_vulkanLogLevel;
		}

		void vulkanLogLevel(uint32_t newValue) {
			prop_vulkanLogLevel = newValue;
			root.dirty(true);
		}
	} debug{root};

	bool dirty_value = false;
	void dirty(bool newValue) {
		dirty_value = newValue;
	}

	bool dirty() {
		return dirty_value;
	}

	std::string source_value{"core.json"};
	void source(const std::string &newValue) {
		source_value = newValue;
	}

	std::string & source() {
		return source_value;
	}

	void load();
	std::string dump(const int indent = 2);
	bool store();
};

} // config

} // ngn
