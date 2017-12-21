#include "reader.hpp"

#include <algorithm>
#include <cctype>

#include <gli/gli.hpp>
#include <stb/stb_image.h>

#include "../../../../ngn/fs.hpp"
#include "../../../../ngn/log.hpp"
#include "../../../../ngn/prof.hpp"
#include "../../context.hpp"
#include "../../trace.hpp"
#include "../texture.hpp"
#include "format/vk_format.h"

namespace rn::vlk::db::texture {

std::mutex stbiMutex{};

vk::ComponentSwizzle translateSwizzle(gli::swizzle swizzle) {
	switch (swizzle) {
		default: return vk::ComponentSwizzle::eIdentity;
		case gli::SWIZZLE_ZERO: return vk::ComponentSwizzle::eZero;
		case gli::SWIZZLE_ONE: return vk::ComponentSwizzle::eOne;
		case gli::SWIZZLE_RED: return vk::ComponentSwizzle::eR;
		case gli::SWIZZLE_GREEN: return vk::ComponentSwizzle::eG;
		case gli::SWIZZLE_BLUE: return vk::ComponentSwizzle::eB;
		case gli::SWIZZLE_ALPHA: return vk::ComponentSwizzle::eA;
	}
}

Reader::Reader(rn::vlk::Context &context, rn::vlk::db::Texture &db) :
	context(context),
	db(db)
{
	workerThread = std::thread{[&] () {
		while (true) {
			Payload payload{};

			{
				std::unique_lock lock{tasksMutex};

				tasksCond.wait(lock, [&] () {
					return shouldStop.load() || ! tasks.empty();
				});


				if (shouldStop.load()) {
					if (tasks.empty()) {
						return;
					}

					db.map([&] () {
						for (auto &payload : tasks) {
							db.entries[payload.index].status = TextureEntryStatus::CANCELLED;
						}
					});

					for (auto &payload : tasks) {
						payload.promise.set_value(false);
					}

					ngn::log::debug("rn::vlk::db::texture::Reader::workerThread[] => {} tasks cancelled", tasks.size());

					return;
				}

				payload = std::move(tasks.front());
				tasks.pop_front();
			}

			run(std::move(payload));
		}
	}};
}

Reader::~Reader() {
	shouldStop.store(true);
	tasksCond.notify_one();

	if (workerThread.joinable()) {
		workerThread.join();
	}
}

void Reader::run(Payload &&payload) {
	try {
		NGN_PROF_SCOPE("Texture::Reader");

		if (isStbiReadable(payload.source)) {
			readWithStbi(std::move(payload));
		} else if (isGliReadable(payload.source)) {
			readWithGli(std::move(payload));
		} else {
			ngn::log::debug("rn::vlk::db::texture::Reader::workerThread[{}, {}] => unsupported texture file", payload.index, payload.source);

			db.map(payload.index, [&] (auto &entry) {
				entry.status = rn::vlk::db::TextureEntryStatus::INVALID_FILE;
			});

			payload.promise.set_value(false);
		}
	} catch (std::runtime_error const &e) {
		ngn::log::debug("rn::vlk::db::texture::Reader::workerThread[?] => error {}", e.what());

		if ( ! payload.source.empty()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = TextureEntryStatus::READER_FAILED;
			});

			payload.promise.set_value(false);
		}
	} catch (...) {
		ngn::log::debug("rn::vlk::db::texture::Reader::workerThread[?] => unknown error");

		if ( ! payload.source.empty()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = TextureEntryStatus::READER_FAILED;
			});

			payload.promise.set_value(false);
		}
	}
}

void Reader::read(Payload &&payload) {
	uint32_t index = payload.index;
	std::string source = std::string{payload.source};

	{
		std::unique_lock lock{tasksMutex};

		if (shouldStop.load()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = TextureEntryStatus::CANCELLED;
			});

			payload.promise.set_value(false);

			ngn::log::debug("rn::vlk::db::texture::Reader::read({}, {}) => cancelled", index, source);

			return;
		}

		if (forceSynchronusLoading) {
			lock.unlock();

			run(std::move(payload));

			return;
		}

		tasks.push_back(std::move(payload));

		ngn::log::debug("rn::vlk::db::texture::Reader::read({}, {}) => item {}", index, source, tasks.size() - 1);
	}

	tasksCond.notify_one();
}

bool Reader::hasExt(const std::string_view &source, const std::vector<std::string_view> &exts) const {
	auto isSame = [] (char a, char b) {
		return std::tolower(a) == std::tolower(b);
	};

	for (auto &ext : exts) {
		if (source.size() < ext.size()) {
			continue;
		}

		if (std::equal(std::rbegin(ext), std::rend(ext), std::rbegin(source), isSame)) {
			return true;
		}
	}

	return false;
}

bool Reader::isStbiReadable(const std::string_view &source) const {
	std::vector<std::string_view> stbiExts{ ".bmp", ".gif", ".hdr", ".jpeg", ".jpg", ".pgm", ".pic", ".png", ".ppm", ".psd", ".tga" };

	return hasExt(source, stbiExts);
}

bool Reader::isGliReadable(const std::string_view &source) const {
	std::vector<std::string_view> gliExts{ ".dds", ".ktx" };

	return hasExt(source, gliExts);
}

void Reader::readWithStbi(Payload &&payload) {
	std::vector<std::byte> sourceContents = ngn::fs::read(payload.source);

	int x = 0;
	int y = 0;
	int n = 0;
	std::unique_ptr<stbi_uc, void (*)(void *)> bitmap{nullptr, stbi_image_free};
	{
		std::lock_guard lock{stbiMutex};

		static_assert(sizeof(std::byte) == sizeof(stbi_uc));
		bitmap.reset(stbi_load_from_memory(reinterpret_cast<stbi_uc *>(sourceContents.data()), sourceContents.size(), &x, &y, &n, STBI_rgb_alpha));

		if (bitmap == nullptr) {
			ngn::log::debug("rn::vlk::db::texture::Reader::readWithStbi({}, {}) => {}", payload.index, payload.source, stbi_failure_reason());

			db.map(payload.index, [&] (auto &entry) {
				entry.status = rn::vlk::db::TextureEntryStatus::INVALID_DATA;
			});

			payload.promise.set_value(false);

			return;
		}
	}

	if (x <= 0 || y <= 0) {
		ngn::log::debug("rn::vlk::db::texture::Reader::readWithStbi({}, {}) => invalid image extents: {}, {}", payload.index, payload.source, x, y);

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vlk::db::TextureEntryStatus::INVALID_FORMAT;
		});

		payload.promise.set_value(false);

		return;
	}

	if (n <= 0 || n > 4) {
		ngn::log::debug("rn::vlk::db::texture::Reader::readWithStbi({}, {}) => invalid number of channels: {}", payload.index, payload.source, n);

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vlk::db::TextureEntryStatus::INVALID_FORMAT;
		});

		payload.promise.set_value(false);

		return;
	}

	vk::ImageCreateFlags flags{};
	vk::Format format{vk::Format::eR8G8B8A8Unorm};
	vk::ImageType imageType{vk::ImageType::e2D};
	uint32_t layers = 1;
	uint32_t faces = 1;
	uint32_t levels = 1;
	std::vector<vk::Extent3D> extents{
		{
			/*.width=*/ static_cast<uint32_t>(x),
			/*.height=*/ static_cast<uint32_t>(y),
			/*.depth=*/ 1,
		}
	};
	std::vector<vk::DeviceSize> sizes{
		/*level 0*/ x * y * n * sizeof(stbi_uc)
	};

	vk::ComponentMapping swizzles{
		/*.r=*/ vk::ComponentSwizzle::eIdentity,
		/*.g=*/ vk::ComponentSwizzle::eIdentity,
		/*.b=*/ vk::ComponentSwizzle::eIdentity,
		/*.a=*/ vk::ComponentSwizzle::eIdentity,
	};

	vk::BufferCreateInfo bufferCreateInfo{
		/*.flags=*/ vk::BufferCreateFlags{},
		/*.size=*/ x * y * n * sizeof(stbi_uc),
		/*.usage=*/ vk::BufferUsageFlagBits::eTransferSrc,
		/*.sharingMode=*/ vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ 0,
		/*.pQueueFamilyIndices=*/ nullptr
	};
	vk::UniqueBuffer buffer = RN_VLK_TRACE(context.device.createBufferUnique(bufferCreateInfo));

	vk::MemoryRequirements memoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(buffer.get()));
	memory::Handle handle = context.allocator.staging.alloc(memory::Usage::CPU_TO_GPU, memoryRequirements);

	RN_VLK_TRACE(context.device.bindBufferMemory(buffer.get(), handle.memory, handle.offset));

	std::copy_n(bitmap.get(), x * y * n, reinterpret_cast<stbi_uc *>(handle.pointer));

	if (handle.needsFlushing()) {
		RN_VLK_TRACE(context.device.flushMappedMemoryRanges({
			{
				/*.memory=*/ handle.memory,
				/*.offset=*/ handle.offset,
				/*.size=*/ memoryRequirements.size
			}
		}));
	}

	db.map(payload.index, [&] (auto &entry) {
		entry.format = format;
		entry.imageType = imageType;
		entry.cubeCompatible = false;
		entry.layers = layers;
		entry.faces = faces;
		entry.levels = levels;
		entry.extents = std::vector<vk::Extent3D>{extents};
		entry.swizzles = swizzles;
		entry.status = rn::vlk::db::TextureEntryStatus::STAGED;
	});

	ngn::log::debug("rn::vlk::db::texture::Reader::readWithStbi({}, {}) => {} bytes", payload.index, payload.source, x * y * n);

	event.emit(Payload{
		std::move(payload.index),
		std::move(payload.source),
		std::move(payload.promise),
		std::move(flags),
		std::move(format),
		std::move(imageType),
		std::move(layers),
		std::move(faces),
		std::move(levels),
		std::move(extents),
		std::move(sizes),
		std::move(swizzles),
		std::move(buffer),
		std::move(handle)
	});
}

void Reader::readWithGli(Payload &&payload) {
	std::vector<std::byte> sourceContents = ngn::fs::read(payload.source);
	gli::texture tex = gli::load(reinterpret_cast<char *>(sourceContents.data()), sourceContents.size());

	if (tex.empty()) {
		ngn::log::debug("rn::vlk::db::texture::Reader::readWithGli({}, {}) => unable to load", payload.index, payload.source);

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vlk::db::TextureEntryStatus::INVALID_DATA;
		});

		payload.promise.set_value(false);

		return;
	}

	if (gli::is_compressed(tex.format())) {
		ngn::log::debug("rn::vlk::db::texture::Reader::readWithGli({}, {}) => compressed images not supported", payload.index, payload.source);

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vlk::db::TextureEntryStatus::INVALID_FORMAT;
		});

		payload.promise.set_value(false);

		return;
	}

	gli::gl glHelper{gli::gl::PROFILE_GL33};
	gli::gl::format gliFormat = glHelper.translate(tex.format(), tex.swizzles());
	vk::Format format = static_cast<vk::Format>(vkGetFormatFromOpenGLInternalFormat(gliFormat.Internal));

	vk::FormatProperties formatProperties = context.physicalDevice.handle.getFormatProperties(format);
	vk::FormatFeatureFlags requiredProperties = vk::FormatFeatureFlagBits::eSampledImage | vk::FormatFeatureFlagBits:: eTransferDstKHR;
	if ((formatProperties.optimalTilingFeatures & requiredProperties) != requiredProperties) {
		ngn::log::debug("rn::vlk::db::texture::Reader::readWithGli({}, {}) => format \"{}\" not supported", payload.index, payload.source, vk::to_string(format));

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vlk::db::TextureEntryStatus::INVALID_FORMAT;
		});

		payload.promise.set_value(false);

		return;
	}

	vk::ImageCreateFlags flags{};
	vk::ImageType imageType{vk::ImageType::e1D};
	switch (tex.target()) {
		default:
		case gli::target::TARGET_1D:
		case gli::target::TARGET_1D_ARRAY: {
			imageType = vk::ImageType::e1D;
			break;
		}
		case gli::target::TARGET_CUBE:
		case gli::target::TARGET_CUBE_ARRAY: {
			flags = vk::ImageCreateFlagBits::eCubeCompatible;
			[[fallthrough]];
		}
		case gli::target::TARGET_2D:
		case gli::target::TARGET_RECT:
		case gli::target::TARGET_2D_ARRAY:
		case gli::target::TARGET_RECT_ARRAY: {
			imageType = vk::ImageType::e2D;
			break;
		}
		case gli::target::TARGET_3D: {
			imageType = vk::ImageType::e3D;
			break;
		}
	}

	uint32_t layers = tex.layers();
	uint32_t faces = tex.faces();
	uint32_t levels = tex.levels();
	std::vector<vk::Extent3D> extents{};
	extents.resize(tex.levels());
	std::vector<vk::DeviceSize> sizes{};
	sizes.resize(tex.levels());

	for (uint32_t level = 0; level < static_cast<uint32_t>(tex.levels()); level++) {
		extents[level] = vk::Extent3D{
			/*.width=*/ static_cast<uint32_t>(tex.extent(level).x),
			/*.height=*/ static_cast<uint32_t>(tex.extent(level).y),
			/*.depth=*/ static_cast<uint32_t>(tex.extent(level).z),
		};

		sizes[level] = tex.size(level);
	}

	vk::ComponentMapping swizzles{
		/*.r=*/translateSwizzle(tex.swizzles().r),
		/*.g=*/translateSwizzle(tex.swizzles().g),
		/*.b=*/translateSwizzle(tex.swizzles().b),
		/*.a=*/translateSwizzle(tex.swizzles().a),
	};

	vk::BufferCreateInfo bufferCreateInfo{
		/*.flags=*/ vk::BufferCreateFlags{},
		/*.size=*/ tex.size(),
		/*.usage=*/ vk::BufferUsageFlagBits::eTransferSrc,
		/*.sharingMode=*/ vk::SharingMode::eExclusive,
		/*.queueFamilyIndexCount=*/ 0,
		/*.pQueueFamilyIndices=*/ nullptr
	};
	vk::UniqueBuffer buffer = RN_VLK_TRACE(context.device.createBufferUnique(bufferCreateInfo));

	vk::MemoryRequirements memoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(buffer.get()));
	memory::Handle bufferMemory = context.allocator.staging.alloc(memory::Usage::CPU_TO_GPU, memoryRequirements);

	RN_VLK_TRACE(context.device.bindBufferMemory(buffer.get(), bufferMemory.memory, bufferMemory.offset));

	std::copy_n(reinterpret_cast<std::byte *>(tex.data()), tex.size(), reinterpret_cast<std::byte *>(bufferMemory.pointer));

	if (bufferMemory.needsFlushing()) {
		RN_VLK_TRACE(context.device.flushMappedMemoryRanges({
			{
				/*.memory=*/ bufferMemory.memory,
				/*.offset=*/ bufferMemory.offset,
				/*.size=*/ memoryRequirements.size
			}
		}));
	}

	db.map(payload.index, [&] (auto &entry) {
		entry.format = format;
		entry.imageType = imageType;
		entry.cubeCompatible = (flags & vk::ImageCreateFlagBits::eCubeCompatible) == vk::ImageCreateFlagBits::eCubeCompatible;
		entry.layers = layers;
		entry.faces = faces;
		entry.levels = levels;
		entry.extents = std::vector<vk::Extent3D>{extents};
		entry.swizzles = swizzles;
		entry.status = rn::vlk::db::TextureEntryStatus::STAGED;
	});

	ngn::log::debug("rn::vlk::db::texture::Reader::readWithGli({}, {}) => {} bytes", payload.index, payload.source, tex.size());

	event.emit(Payload{
		std::move(payload.index),
		std::move(payload.source),
		std::move(payload.promise),
		std::move(flags),
		std::move(format),
		std::move(imageType),
		std::move(layers),
		std::move(faces),
		std::move(levels),
		std::move(extents),
		std::move(sizes),
		std::move(swizzles),
		std::move(buffer),
		std::move(bufferMemory)
	});
}

} // rn::vlk::db::texture
