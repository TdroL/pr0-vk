#pragma once

#include <mutex>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../memory/pool.hpp"
#include "mesh/handle.hpp"
#include "mesh/loader.hpp"

namespace rn::vlk::db {

enum class MeshEntryStatus {
	UNUSED,
	PENDING,
	STAGED,
	TRANSFERING,
	READY,
	INVALID_FILE,
	INVALID_FORMAT,
	INVALID_DATA,
	CANCELLED,
	READER_FAILED,
	TRANSFERRER_FAILED,
};

struct MeshEntry {
	std::string source{};
	MeshEntryStatus status{MeshEntryStatus::UNUSED};

	// ref counter
	uint32_t counter = 0;
};

class Mesh {
public:
	std::vector<MeshEntry> entries{};
	std::mutex entriesMutex{};

	mesh::Handle emplace(const std::string_view &source);

	void acquire(uint32_t index);
	void release(uint32_t index);

	template<class T>
	auto map(uint32_t index, T callback) -> decltype(callback(entries[index])) {
		std::lock_guard lock{entriesMutex};

		if (entries.size() < index) {
			entries.resize(index + 1);
		}

		return callback(entries[index]);
	}

	template<class T>
	auto map(T callback) -> decltype(callback()) {
		std::lock_guard lock{entriesMutex};

		return callback();
	}
};

} // rn::vlk::db
