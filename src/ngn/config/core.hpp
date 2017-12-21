#pragma once

#include <string>

namespace ngn::config {

enum class VSync {
	Immediate,
	Fifo,
	FifoRelaxed,
	Mailbox
};

std::string toString(const VSync &value);
VSync fromString(const std::string_view &name, const VSync &defaultValue);

enum class WindowMode {
	Windowed,
	Borderless,
	Fullscreen
};

std::string toString(const WindowMode &value);
WindowMode fromString(const std::string_view &name, const WindowMode &defaultValue);

enum class ImageFormat {
	Undefined,
	R4G4UnormPack8,
	R4G4B4A4UnormPack16,
	B4G4R4A4UnormPack16,
	R5G6B5UnormPack16,
	B5G6R5UnormPack16,
	R5G5B5A1UnormPack16,
	B5G5R5A1UnormPack16,
	A1R5G5B5UnormPack16,
	R8Unorm,
	R8Snorm,
	R8Uscaled,
	R8Sscaled,
	R8Uint,
	R8Sint,
	R8Srgb,
	R8G8Unorm,
	R8G8Snorm,
	R8G8Uscaled,
	R8G8Sscaled,
	R8G8Uint,
	R8G8Sint,
	R8G8Srgb,
	R8G8B8Unorm,
	R8G8B8Snorm,
	R8G8B8Uscaled,
	R8G8B8Sscaled,
	R8G8B8Uint,
	R8G8B8Sint,
	R8G8B8Srgb,
	B8G8R8Unorm,
	B8G8R8Snorm,
	B8G8R8Uscaled,
	B8G8R8Sscaled,
	B8G8R8Uint,
	B8G8R8Sint,
	B8G8R8Srgb,
	R8G8B8A8Unorm,
	R8G8B8A8Snorm,
	R8G8B8A8Uscaled,
	R8G8B8A8Sscaled,
	R8G8B8A8Uint,
	R8G8B8A8Sint,
	R8G8B8A8Srgb,
	B8G8R8A8Unorm,
	B8G8R8A8Snorm,
	B8G8R8A8Uscaled,
	B8G8R8A8Sscaled,
	B8G8R8A8Uint,
	B8G8R8A8Sint,
	B8G8R8A8Srgb,
	A8B8G8R8UnormPack32,
	A8B8G8R8SnormPack32,
	A8B8G8R8UscaledPack32,
	A8B8G8R8SscaledPack32,
	A8B8G8R8UintPack32,
	A8B8G8R8SintPack32,
	A8B8G8R8SrgbPack32,
	A2R10G10B10UnormPack32,
	A2R10G10B10SnormPack32,
	A2R10G10B10UscaledPack32,
	A2R10G10B10SscaledPack32,
	A2R10G10B10UintPack32,
	A2R10G10B10SintPack32,
	A2B10G10R10UnormPack32,
	A2B10G10R10SnormPack32,
	A2B10G10R10UscaledPack32,
	A2B10G10R10SscaledPack32,
	A2B10G10R10UintPack32,
	A2B10G10R10SintPack32,
	R16Unorm,
	R16Snorm,
	R16Uscaled,
	R16Sscaled,
	R16Uint,
	R16Sint,
	R16Sfloat,
	R16G16Unorm,
	R16G16Snorm,
	R16G16Uscaled,
	R16G16Sscaled,
	R16G16Uint,
	R16G16Sint,
	R16G16Sfloat,
	R16G16B16Unorm,
	R16G16B16Snorm,
	R16G16B16Uscaled,
	R16G16B16Sscaled,
	R16G16B16Uint,
	R16G16B16Sint,
	R16G16B16Sfloat,
	R16G16B16A16Unorm,
	R16G16B16A16Snorm,
	R16G16B16A16Uscaled,
	R16G16B16A16Sscaled,
	R16G16B16A16Uint,
	R16G16B16A16Sint,
	R16G16B16A16Sfloat,
	R32Uint,
	R32Sint,
	R32Sfloat,
	R32G32Uint,
	R32G32Sint,
	R32G32Sfloat,
	R32G32B32Uint,
	R32G32B32Sint,
	R32G32B32Sfloat,
	R32G32B32A32Uint,
	R32G32B32A32Sint,
	R32G32B32A32Sfloat,
	R64Uint,
	R64Sint,
	R64Sfloat,
	R64G64Uint,
	R64G64Sint,
	R64G64Sfloat,
	R64G64B64Uint,
	R64G64B64Sint,
	R64G64B64Sfloat,
	R64G64B64A64Uint,
	R64G64B64A64Sint,
	R64G64B64A64Sfloat,
	B10G11R11UfloatPack32,
	E5B9G9R9UfloatPack32,
	D16Unorm,
	X8D24UnormPack32,
	D32Sfloat,
	S8Uint,
	D16UnormS8Uint,
	D24UnormS8Uint,
	D32SfloatS8Uint,
	Bc1RgbUnormBlock,
	Bc1RgbSrgbBlock,
	Bc1RgbaUnormBlock,
	Bc1RgbaSrgbBlock,
	Bc2UnormBlock,
	Bc2SrgbBlock,
	Bc3UnormBlock,
	Bc3SrgbBlock,
	Bc4UnormBlock,
	Bc4SnormBlock,
	Bc5UnormBlock,
	Bc5SnormBlock,
	Bc6HUfloatBlock,
	Bc6HSfloatBlock,
	Bc7UnormBlock,
	Bc7SrgbBlock,
	Etc2R8G8B8UnormBlock,
	Etc2R8G8B8SrgbBlock,
	Etc2R8G8B8A1UnormBlock,
	Etc2R8G8B8A1SrgbBlock,
	Etc2R8G8B8A8UnormBlock,
	Etc2R8G8B8A8SrgbBlock,
	EacR11UnormBlock,
	EacR11SnormBlock,
	EacR11G11UnormBlock,
	EacR11G11SnormBlock,
	Astc4x4UnormBlock,
	Astc4x4SrgbBlock,
	Astc5x4UnormBlock,
	Astc5x4SrgbBlock,
	Astc5x5UnormBlock,
	Astc5x5SrgbBlock,
	Astc6x5UnormBlock,
	Astc6x5SrgbBlock,
	Astc6x6UnormBlock,
	Astc6x6SrgbBlock,
	Astc8x5UnormBlock,
	Astc8x5SrgbBlock,
	Astc8x6UnormBlock,
	Astc8x6SrgbBlock,
	Astc8x8UnormBlock,
	Astc8x8SrgbBlock,
	Astc10x5UnormBlock,
	Astc10x5SrgbBlock,
	Astc10x6UnormBlock,
	Astc10x6SrgbBlock,
	Astc10x8UnormBlock,
	Astc10x8SrgbBlock,
	Astc10x10UnormBlock,
	Astc10x10SrgbBlock,
	Astc12x10UnormBlock,
	Astc12x10SrgbBlock,
	Astc12x12UnormBlock,
	Astc12x12SrgbBlock,
	Pvrtc12BppUnormBlockIMG,
	Pvrtc14BppUnormBlockIMG,
	Pvrtc22BppUnormBlockIMG,
	Pvrtc24BppUnormBlockIMG,
	Pvrtc12BppSrgbBlockIMG,
	Pvrtc14BppSrgbBlockIMG,
	Pvrtc22BppSrgbBlockIMG,
	Pvrtc24BppSrgbBlockIMG,
	G8B8G8R8422UnormKHR,
	B8G8R8G8422UnormKHR,
	G8B8R83Plane420UnormKHR,
	G8B8R82Plane420UnormKHR,
	G8B8R83Plane422UnormKHR,
	G8B8R82Plane422UnormKHR,
	G8B8R83Plane444UnormKHR,
	R10X6UnormPack16KHR,
	R10X6G10X6Unorm2Pack16KHR,
	R10X6G10X6B10X6A10X6Unorm4Pack16KHR,
	G10X6B10X6G10X6R10X6422Unorm4Pack16KHR,
	B10X6G10X6R10X6G10X6422Unorm4Pack16KHR,
	G10X6B10X6R10X63Plane420Unorm3Pack16KHR,
	G10X6B10X6R10X62Plane420Unorm3Pack16KHR,
	G10X6B10X6R10X63Plane422Unorm3Pack16KHR,
	G10X6B10X6R10X62Plane422Unorm3Pack16KHR,
	G10X6B10X6R10X63Plane444Unorm3Pack16KHR,
	R12X4UnormPack16KHR,
	R12X4G12X4Unorm2Pack16KHR,
	R12X4G12X4B12X4A12X4Unorm4Pack16KHR,
	G12X4B12X4G12X4R12X4422Unorm4Pack16KHR,
	B12X4G12X4R12X4G12X4422Unorm4Pack16KHR,
	G12X4B12X4R12X43Plane420Unorm3Pack16KHR,
	G12X4B12X4R12X42Plane420Unorm3Pack16KHR,
	G12X4B12X4R12X43Plane422Unorm3Pack16KHR,
	G12X4B12X4R12X42Plane422Unorm3Pack16KHR,
	G12X4B12X4R12X43Plane444Unorm3Pack16KHR,
	G16B16G16R16422UnormKHR,
	B16G16R16G16422UnormKHR,
	G16B16R163Plane420UnormKHR,
	G16B16R162Plane420UnormKHR,
	G16B16R163Plane422UnormKHR,
	G16B16R162Plane422UnormKHR,
	G16B16R163Plane444UnormKHR
};

std::string toString(const ImageFormat &value);
ImageFormat fromString(const std::string_view &name, const ImageFormat &defaultValue);

enum class ColorSpace {
	SrgbNonlinear,
	DisplayP3NonlinearEXT,
	ExtendedSrgbLinearEXT,
	DciP3LinearEXT,
	DciP3NonlinearEXT,
	Bt709LinearEXT,
	Bt709NonlinearEXT,
	Bt2020LinearEXT,
	Hdr10St2084EXT,
	DolbyvisionEXT,
	Hdr10HlgEXT,
	AdobergbLinearEXT,
	AdobergbNonlinearEXT,
	PassThroughEXT,
	ExtendedSrgbNonlinearEXT
};

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

		ImageFormat prop_surfaceFormat{ImageFormat::B8G8R8A8Unorm};
		const ImageFormat & surfaceFormat() {
			return prop_surfaceFormat;
		}

		void surfaceFormat(const ImageFormat &newValue) {
			root.dirty(root.dirty() || prop_surfaceFormat != newValue);
			prop_surfaceFormat = newValue;
		}

		ColorSpace prop_surfaceColorSpace{ColorSpace::SrgbNonlinear};
		const ColorSpace & surfaceColorSpace() {
			return prop_surfaceColorSpace;
		}

		void surfaceColorSpace(const ColorSpace &newValue) {
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
