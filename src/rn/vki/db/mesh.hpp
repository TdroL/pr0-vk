#pragma once

#include <mutex>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../memory/pool.hpp"
#include "mesh/handle.hpp"
#include "mesh/loader.hpp"

namespace rn::vki::db {

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

	vk::UniqueBuffer vertexBuffer{};
	memory::Handle vertexMemory{};

	vk::UniqueBuffer indexBuffer{};
	memory::Handle indexMemory{};

	enum class Topology {
		triangleList = 0x00000000,
		triangleStrip = 0x00000001,
		triangleFan = 0x00000002,
	};

	struct SubMesh {
		vk::DeviceSize vertexBufferOffset = 0;
		vk::DeviceSize vertexBufferSize = 0;

		vk::DeviceSize indexBufferOffset = 0;
		vk::DeviceSize indexBufferSize = 0;

		uint32_t vertices = 0;
		uint32_t bones = 0;
		uint32_t indices = 0;

		bool hasNormals = false;
		bool hasCoords = false;
		bool hasBones = false;
		bool hasIndices = false;

		vk::DeviceSize positionOffset = 0;
		vk::DeviceSize normalOffset = 0;
		vk::DeviceSize coordOffset = 0;
		vk::DeviceSize boneOffset = 0;

		Topology topology = Topology::triangleList;
	};

	std::vector<SubMesh> subMeshes{};

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
	auto map(const mesh::Handle &handle, T callback) -> decltype(map(handle.index, std::forward<T>(callback))) {
		return map(handle.index, std::forward<T>(callback));
	}

	template<class T>
	auto map(T callback) -> decltype(callback()) {
		std::lock_guard lock{entriesMutex};

		return callback();
	}
};

} // rn::vki::db
