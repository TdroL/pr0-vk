#include "texture.hpp"

#include <algorithm>

#include "../../../ngn/log.hpp"

namespace rn::vki::db {

texture::Handle Texture::emplace(const std::string_view &source) {
	uint32_t index;

	std::unique_lock lock{entriesMutex};

	auto findIt = std::find_if(std::begin(entries), std::end(entries), [&] (const auto &entry) {
		return entry.source == source;
	});

	if (findIt != std::end(entries)) {
		index = static_cast<uint32_t>(findIt - std::begin(entries));
		lock.unlock();
	} else {
		// find unused
		auto unusedIt = std::find_if(std::begin(entries), std::end(entries), [&] (const auto &entry) {
			return entry.status == TextureEntryStatus::UNUSED;
		});

		if (unusedIt != std::end(entries)) {
			index = static_cast<uint32_t>(unusedIt - std::begin(entries));
		} else {
			// create new
			index = static_cast<uint32_t>(entries.size());
			entries.resize(entries.size() + 1);
		}

		entries[index].source = std::string{source};
		entries[index].status = TextureEntryStatus::PENDING;

		lock.unlock();
	}

	acquire(index);

	return texture::Handle{index, *this};
}

void Texture::acquire(uint32_t index) {
	map(index, [] (auto &entry) {
		entry.counter++;
	});
}

void Texture::release(uint32_t index) {
	map(index, [] (auto &entry) {
		entry.counter--;

		if (entry.counter == 0) {
			entry = TextureEntry{};
		}
	});
}

} // rn::vki::db
