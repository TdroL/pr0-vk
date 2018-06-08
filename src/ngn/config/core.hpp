#pragma once

#include <string>
#include <variant>
#include "../../rn/types.hpp"

namespace ngn::config {

using VSync = rn::VSync;

std::string toString(const VSync &value);
VSync fromString(const std::string_view &name, const VSync &defaultValue);

using WindowMode = rn::WindowMode;

std::string toString(const WindowMode &value);
WindowMode fromString(const std::string_view &name, const WindowMode &defaultValue);

using PixelFormat = rn::PixelFormat;

std::string toString(const PixelFormat &value);
PixelFormat fromString(const std::string_view &name, const PixelFormat &defaultValue);

using ColorSpace = rn::ColorSpace;

std::string toString(const ColorSpace &value);
ColorSpace fromString(const std::string_view &name, const ColorSpace &defaultValue);

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

			uint32_t prop_minor{1};
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

			uint32_t prop_minor{1};
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
			root.dirty(root.dirty() || prop_deviceId != newValue);
			prop_deviceId = newValue;
		}

		uint32_t prop_vendorId{0};
		uint32_t vendorId() {
			return prop_vendorId;
		}

		void vendorId(uint32_t newValue) {
			root.dirty(root.dirty() || prop_vendorId != newValue);
			prop_vendorId = newValue;
		}
	} physicalDevice{root};

	struct Db {
		Core &root;

		Db(Core &root) : root(root) {}

		struct Mesh {
			Core &root;

			Mesh(Core &root) : root(root) {}

			bool prop_forceSynchronusLoading{false};
			bool forceSynchronusLoading() {
				return prop_forceSynchronusLoading;
			}

			void forceSynchronusLoading(bool newValue) {
				root.dirty(root.dirty() || prop_forceSynchronusLoading != newValue);
				prop_forceSynchronusLoading = newValue;
			}
		} mesh{root};

		struct Texture {
			Core &root;

			Texture(Core &root) : root(root) {}

			bool prop_forceSynchronusLoading{false};
			bool forceSynchronusLoading() {
				return prop_forceSynchronusLoading;
			}

			void forceSynchronusLoading(bool newValue) {
				root.dirty(root.dirty() || prop_forceSynchronusLoading != newValue);
				prop_forceSynchronusLoading = newValue;
			}
		} texture{root};
	} db{root};

	struct Window {
		Core &root;

		Window(Core &root) : root(root) {}

		uint32_t prop_width{1600};
		uint32_t width() {
			return prop_width;
		}

		void width(uint32_t newValue) {
			root.dirty(root.dirty() || prop_width != newValue);
			prop_width = newValue;
		}

		uint32_t prop_height{900};
		uint32_t height() {
			return prop_height;
		}

		void height(uint32_t newValue) {
			root.dirty(root.dirty() || prop_height != newValue);
			prop_height = newValue;
		}

		WindowMode prop_mode{WindowMode::Borderless};
		const WindowMode & mode() {
			return prop_mode;
		}

		void mode(const WindowMode &newValue) {
			root.dirty(root.dirty() || prop_mode != newValue);
			prop_mode = newValue;
		}

		int32_t prop_monitor{-1};
		int32_t monitor() {
			return prop_monitor;
		}

		void monitor(int32_t newValue) {
			root.dirty(root.dirty() || prop_monitor != newValue);
			prop_monitor = newValue;
		}

		VSync prop_vsync{VSync::Mailbox};
		const VSync & vsync() {
			return prop_vsync;
		}

		void vsync(const VSync &newValue) {
			root.dirty(root.dirty() || prop_vsync != newValue);
			prop_vsync = newValue;
		}

		uint32_t prop_imageBuffers{3};
		uint32_t imageBuffers() {
			return prop_imageBuffers;
		}

		void imageBuffers(uint32_t newValue) {
			root.dirty(root.dirty() || prop_imageBuffers != newValue);
			prop_imageBuffers = newValue;
		}

		std::variant<PixelFormat, uint32_t> prop_surfaceFormat{PixelFormat::B8G8R8A8UNorm};
		const std::variant<PixelFormat, uint32_t> & surfaceFormat() {
			return prop_surfaceFormat;
		}

		void surfaceFormat(const std::variant<PixelFormat, uint32_t> &newValue) {
			root.dirty(root.dirty() || prop_surfaceFormat != newValue);
			prop_surfaceFormat = newValue;
		}

		std::variant<ColorSpace, uint32_t> prop_surfaceColorSpace{ColorSpace::LDR};
		const std::variant<ColorSpace, uint32_t> & surfaceColorSpace() {
			return prop_surfaceColorSpace;
		}

		void surfaceColorSpace(const std::variant<ColorSpace, uint32_t> &newValue) {
			root.dirty(root.dirty() || prop_surfaceColorSpace != newValue);
			prop_surfaceColorSpace = newValue;
		}
	} window{root};

	struct Debug {
		Core &root;

		Debug(Core &root) : root(root) {}

		struct Vk {
			Core &root;

			Vk(Core &root) : root(root) {}

			bool prop_useRenderDoc{false};
			bool useRenderDoc() {
				return prop_useRenderDoc;
			}

			void useRenderDoc(bool newValue) {
				root.dirty(root.dirty() || prop_useRenderDoc != newValue);
				prop_useRenderDoc = newValue;
			}

			uint32_t prop_logLevel{1};
			uint32_t logLevel() {
				return prop_logLevel;
			}

			void logLevel(uint32_t newValue) {
				root.dirty(root.dirty() || prop_logLevel != newValue);
				prop_logLevel = newValue;
			}
		} vk{root};
	} debug{root};

	bool dirty_value = false;
	void dirty(bool newValue) {
		dirty_value = newValue;
	}

	bool dirty() {
		return dirty_value;
	}

	std::string source_value{"core.json"};
	void source(const std::string_view &newValue) {
		source_value = newValue;
	}

	std::string & source() {
		return source_value;
	}

	void load();
	std::string dump(const int indent = 2);
	bool store();
};

} // ngn::config
