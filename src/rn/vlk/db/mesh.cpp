#include "mesh.hpp"

#include <algorithm>

#include "../../../ngn/log.hpp"

namespace rn::vlk::db {

mesh::Handle Mesh::emplace(const std::string_view &source) {
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
			return entry.status == MeshEntryStatus::UNUSED;
		});

		if (unusedIt != std::end(entries)) {
			index = static_cast<uint32_t>(unusedIt - std::begin(entries));
		} else {
			// create new
			index = static_cast<uint32_t>(entries.size());
			entries.resize(entries.size() + 1);
		}

		entries[index].source = std::string{source};
		entries[index].status = MeshEntryStatus::PENDING;

		lock.unlock();
	}

	acquire(index);

	return mesh::Handle{index, *this};
}

void Mesh::acquire(uint32_t index) {
	map(index, [] (auto &entry) {
		entry.counter++;
	});
}

void Mesh::release(uint32_t index) {
	map(index, [] (auto &entry) {
		entry.counter--;

		if (entry.counter == 0) {
			entry = MeshEntry{};
		}
	});
}

} // rn::vlk::db
