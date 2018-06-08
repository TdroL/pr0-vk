#include "reader.hpp"

#include <algorithm>
#include <cctype>

#include "../../../../ngn/fs.hpp"
#include "../../../../ngn/log.hpp"
#include "../../../../ngn/prof.hpp"
#include "../../../../util/scope.hpp"
#include "../../context.hpp"
#include "../../trace.hpp"
#include "../mesh.hpp"

namespace rn::vki::db::mesh {

struct ReaderDetails {

};

Reader::Reader(rn::vki::Context &context, rn::vki::db::Mesh &db) :
	context(context),
	db(db),
	readerDetails{new ReaderDetails{}}
{}

Reader::~Reader() {}

std::optional<Payload> Reader::run(Payload &&payload) {
	uint32_t index = payload.index;
	std::string source = payload.source;

	try {
		NGN_PROF_SCOPE("Texture::Reader");

		if (isMsbReadable(payload.source)) {
			return readWithMsb(std::move(payload));
		} else {
			ngn::log::debug("rn::vki::db::mesh::Reader::run({}, \"{}\") => unsupported mesh file", index, payload.source);

			db.map(payload.index, [&] (auto &entry) {
				entry.status = rn::vki::db::MeshEntryStatus::INVALID_FILE;
			});

			payload.promise.set_value(false);
		}
	} catch (std::runtime_error const &e) {
		ngn::log::debug("rn::vki::db::texture::Reader::run({}, \"{}\") => error {}", index, payload.source, e.what());

		if ( ! payload.source.empty()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = MeshEntryStatus::READER_FAILED;
			});

			payload.promise.set_value(false);
		}
	} catch (...) {
		ngn::log::debug("rn::vki::db::texture::Reader::run({}, \"{}\") => unknown error", index, payload.source);

		if ( ! payload.source.empty()) {
			db.map(payload.index, [&] (auto &entry) {
				entry.status = MeshEntryStatus::READER_FAILED;
			});

			payload.promise.set_value(false);
		}
	}

	return std::nullopt;
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

bool Reader::isMsbReadable(const std::string_view &source) const {
	std::vector<std::string_view> supportedExts{ ".msb" };

	return hasExt(source, supportedExts);
}

std::string toString(const rn::vki::db::MeshEntry::Topology &topology) {
	switch (topology) {
		case rn::vki::db::MeshEntry::Topology::triangleList: {
			return "{Topology::triangleList}";
		}
		case rn::vki::db::MeshEntry::Topology::triangleStrip: {
			return "{Topology::triangleStrip}";
		}
		case rn::vki::db::MeshEntry::Topology::triangleFan: {
			return "{Topology::triangleFan}";
		}
		default: {
			return "{Topology::(nil)}";
		}
	}
}

std::optional<Payload> Reader::readWithMsb(Payload &&payload) {
	std::vector<std::byte> sourceContents = ngn::fs::read(payload.source);

	struct MsbHeader {
		std::array<uint8_t, 4> code;
		uint32_t meshCount;
		uint32_t descriptionLength;
		char *description;
	};

	if (sourceContents.size() < sizeof(MsbHeader)) {
		ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => unable to load", payload.index, payload.source);

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vki::db::MeshEntryStatus::INVALID_DATA;
		});

		payload.promise.set_value(false);

		return std::nullopt;
	}

	std::byte *data = sourceContents.data();

	MsbHeader *header = reinterpret_cast<MsbHeader *>(data);

	if (std::array<uint8_t, 4>{'B', 'M', 0xff, 0x01} != header->code) {
		ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => invalid magic code", payload.index, payload.source);

		db.map(payload.index, [&] (auto &entry) {
			entry.status = rn::vki::db::MeshEntryStatus::INVALID_DATA;
		});

		payload.promise.set_value(false);

		return std::nullopt;
	}

	data += sizeof(header->code) + sizeof(header->meshCount) + sizeof(header->descriptionLength) + sizeof(char) * header->descriptionLength;

	enum MeshFlags {
		MhF_none = 0x00000000,
		MhF_hasNormals = 0x00000001,
		MhF_hasCoords = 0x00000002,
		MhF_hasBones = 0x00000004,
		MhF_hasIndices = 0x00000008,
	};

	enum MeshTopology {
		MhT_triangleList = 0x00000000,
		MhT_triangleStrip = 0x00000001,
		MhT_triangleFan = 0x00000002,
	};

	struct MeshMeta {
		uint64_t offset;
		uint64_t size;
		uint32_t vertices;
		uint32_t bones;
		uint32_t indices;
		uint32_t flags;
		uint32_t topology;
	};

	struct DataTransfer {
		uint64_t srcOffset = 0;
		uint64_t dstOffset = 0;
		uint64_t size = 0;
	};

	std::vector<rn::vki::db::MeshEntry::SubMesh> subMeshes{};
	subMeshes.reserve(header->meshCount);

	std::vector<DataTransfer> vertexDataTransfers{};
	vertexDataTransfers.reserve(header->meshCount);

	std::vector<DataTransfer> indexDataTransfers{};
	indexDataTransfers.reserve(header->meshCount);

	uint64_t dataOffset = static_cast<uint64_t>(data - sourceContents.data()) + header->meshCount * sizeof(MeshMeta);

	uint64_t vertexBufferOffset = 0;
	uint64_t indexBufferOffset = 0;

	MeshMeta *meta = reinterpret_cast<MeshMeta *>(data);
	for (uint32_t i = 0; i < header->meshCount; i++) {
		rn::vki::db::MeshEntry::Topology topology = rn::vki::db::MeshEntry::Topology::triangleList;
		switch (meta->topology) {
			case MhT_triangleFan: {
				topology = rn::vki::db::MeshEntry::Topology::triangleList;
				break;
			}
			case MhT_triangleStrip: {
				topology = rn::vki::db::MeshEntry::Topology::triangleStrip;
				break;
			}
			case MhT_triangleList: {
				topology = rn::vki::db::MeshEntry::Topology::triangleFan;
				break;
			}
			default: {
				ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => invalid topology {:x} for submesh {}", payload.index, payload.source, meta->topology, i);

				db.map(payload.index, [&] (auto &entry) {
					entry.status = rn::vki::db::MeshEntryStatus::INVALID_DATA;
				});

				payload.promise.set_value(false);

				return std::nullopt;
			}
		}

		if (meta->offset < dataOffset) {
			ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => offset out-of-bounds ({} < {}) for submesh {}", payload.index, payload.source, meta->offset, dataOffset, i);

			db.map(payload.index, [&] (auto &entry) {
				entry.status = rn::vki::db::MeshEntryStatus::INVALID_DATA;
			});

			payload.promise.set_value(false);

			return std::nullopt;
		}

		if (meta->offset > sourceContents.size()) {
			ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => offset out-of-bounds ({} > {}) for submesh {}", payload.index, payload.source, meta->offset, dataOffset, i);

			db.map(payload.index, [&] (auto &entry) {
				entry.status = rn::vki::db::MeshEntryStatus::INVALID_DATA;
			});

			payload.promise.set_value(false);

			return std::nullopt;
		}

		if (meta->size > sourceContents.size() - (meta->offset - dataOffset)) {
			ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => size out-of-bounds ({} > {}) for submesh {}", payload.index, payload.source, meta->size, sourceContents.size() - (meta->offset - dataOffset), i);

			db.map(payload.index, [&] (auto &entry) {
				entry.status = rn::vki::db::MeshEntryStatus::INVALID_DATA;
			});

			payload.promise.set_value(false);

			return std::nullopt;
		}

		bool hasNormals= (meta->flags & MhF_hasNormals) == MhF_hasNormals;
		bool hasCoords= (meta->flags & MhF_hasCoords) == MhF_hasCoords;
		bool hasBones= (meta->flags & MhF_hasBones) == MhF_hasBones;
		bool hasIndices= (meta->flags & MhF_hasIndices) == MhF_hasIndices;

		uint64_t vertexBufferSize = 0;
		uint64_t indexBufferSize = 0;

		vertexBufferSize += meta->vertices * sizeof(float) * 3;
		vertexBufferSize += hasNormals ? meta->vertices * sizeof(float) * 3 : 0;
		vertexBufferSize += hasCoords ? meta->vertices * sizeof(float) * 2 : 0;
		vertexBufferSize += hasBones ? meta->bones * sizeof(float) * 8 : 0;

		vk::DeviceSize positionOffset = vertexBufferOffset;
		vk::DeviceSize normalOffset = positionOffset + (hasNormals ? meta->vertices * sizeof(float) * 3: 0);
		vk::DeviceSize coordOffset = normalOffset + (hasCoords ? meta->vertices * sizeof(float) * 3 : 0);
		vk::DeviceSize boneOffset = coordOffset + (hasBones ? meta->vertices * sizeof(float) * 2 : 0);

		indexBufferSize += hasIndices ? meta->indices * sizeof(uint32_t) * 3 : 0;

		rn::vki::db::MeshEntry::SubMesh subMesh{
			/*.vertexBufferOffset=*/ vertexBufferOffset,
			/*.vertexBufferSize=*/ vertexBufferSize,
			/*.indexBufferOffset=*/ indexBufferOffset,
			/*.indexBufferSize=*/ indexBufferSize,
			/*.vertices=*/ meta->vertices,
			/*.bones=*/ meta->bones,
			/*.indices=*/ meta->indices,
			/*.hasNormals=*/ hasNormals,
			/*.hasCoords=*/ hasCoords,
			/*.hasBones=*/ hasBones,
			/*.hasIndices=*/ hasIndices,
			/*.positionOffset=*/ positionOffset,
			/*.normalOffset=*/ normalOffset,
			/*.coordOffset=*/ coordOffset,
			/*.boneOffset=*/ boneOffset,
			/*.topology=*/ topology
		};

		// meta->offset - dataOffset

		// vk::DeviceSize verticesOffset = subMesh.offset;
		// vk::DeviceSize verticesSize = meta->vertices * sizeof(float) * 3;

		// vk::DeviceSize normalsOffset = verticesOffset + verticesSize;
		// vk::DeviceSize normalsSize = subMesh.hasNormals ? meta->vertices * sizeof(float) * 3 : 0;

		// vk::DeviceSize coordsOffset = normalsOffset + normalsSize;
		// vk::DeviceSize coordsSize = subMesh.hasCoords ? meta->vertices * sizeof(float) * 2 : 0;

		// vk::DeviceSize bonesOffset = coordsOffset + coordsSize;
		// vk::DeviceSize bonesSize = subMesh.hasBones ? meta->bones * sizeof(float) * 8 : 0;

		// vk::DeviceSize indicesOffset = bonesOffset + bonesSize;
		// vk::DeviceSize indicesSize = subMesh.hasIndices ? meta->indices * sizeof(uint32_t) * 3 : 0;

		DataTransfer vertexDataTransfer{
			/*.srcOffset=*/ meta->offset - dataOffset,
			/*.dstOffset=*/ vertexBufferOffset,
			/*.size=*/ vertexBufferSize
		};

		DataTransfer indexDataTransfer{
			/*.srcOffset=*/ vertexDataTransfer.srcOffset + vertexDataTransfer.size,
			/*.dstOffset=*/ indexBufferOffset,
			/*.size=*/ indexBufferSize
		};

		vertexBufferOffset += vertexBufferSize;
		indexBufferOffset += indexBufferSize;

		subMeshes.push_back(subMesh);
		vertexDataTransfers.push_back(vertexDataTransfer);
		indexDataTransfers.push_back(indexDataTransfer);

		meta++;
	}

	uint64_t vertexBufferSize = vertexBufferOffset;
	uint64_t indexBufferSize = indexBufferOffset;

	data = reinterpret_cast<std::byte *>(meta);

	vk::UniqueBuffer vertexBuffer{};
	memory::Handle vertexBufferMemory{};

	vk::UniqueBuffer indexBuffer{};
	memory::Handle indexBufferMemory{};


	// vertex buffer
	{
		vk::BufferCreateInfo vertexBufferCreateInfo{
			/*.flags=*/ vk::BufferCreateFlags{},
			/*.size=*/ vertexBufferSize,
			/*.usage=*/ vk::BufferUsageFlagBits::eTransferSrc,
			/*.sharingMode=*/ vk::SharingMode::eExclusive,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/ nullptr
		};
		vertexBuffer = RN_VLK_TRACE(context.device.createBufferUnique(vertexBufferCreateInfo));

		vk::MemoryRequirements memoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(vertexBuffer.get()));
		vertexBufferMemory = context.allocator.staging.alloc(memory::Usage::CPU_TO_GPU, memoryRequirements);

		RN_VLK_TRACE(context.device.bindBufferMemory(vertexBuffer.get(), vertexBufferMemory.memory, vertexBufferMemory.offset));

		std::byte * buffer = reinterpret_cast<std::byte *>(vertexBufferMemory.pointer);
		for (const auto &dataTransfer : vertexDataTransfers) {
			std::copy_n(data + dataTransfer.srcOffset, dataTransfer.size, buffer + dataTransfer.dstOffset);
		}

		if (vertexBufferMemory.needsFlushing()) {
			RN_VLK_TRACE(context.device.flushMappedMemoryRanges({
				{
					/*.memory=*/ vertexBufferMemory.memory,
					/*.offset=*/ vertexBufferMemory.offset,
					/*.size=*/ memoryRequirements.size
				}
			}));
		}
	}

	// index buffer
	if (indexBufferSize > 0) {
		vk::BufferCreateInfo indexBufferCreateInfo{
			/*.flags=*/ vk::BufferCreateFlags{},
			/*.size=*/ indexBufferSize,
			/*.usage=*/ vk::BufferUsageFlagBits::eTransferSrc,
			/*.sharingMode=*/ vk::SharingMode::eExclusive,
			/*.queueFamilyIndexCount=*/ 0,
			/*.pQueueFamilyIndices=*/ nullptr
		};
		indexBuffer = RN_VLK_TRACE(context.device.createBufferUnique(indexBufferCreateInfo));

		vk::MemoryRequirements memoryRequirements = RN_VLK_TRACE(context.device.getBufferMemoryRequirements(indexBuffer.get()));
		indexBufferMemory = context.allocator.staging.alloc(memory::Usage::CPU_TO_GPU, memoryRequirements);

		RN_VLK_TRACE(context.device.bindBufferMemory(indexBuffer.get(), indexBufferMemory.memory, indexBufferMemory.offset));

		std::byte * buffer = reinterpret_cast<std::byte *>(indexBufferMemory.pointer);
		for (const auto &dataTransfer : indexDataTransfers) {
			std::copy_n(data + dataTransfer.srcOffset, dataTransfer.size, buffer + dataTransfer.dstOffset);
		}

		if (indexBufferMemory.needsFlushing()) {
			RN_VLK_TRACE(context.device.flushMappedMemoryRanges({
				{
					/*.memory=*/ indexBufferMemory.memory,
					/*.offset=*/ indexBufferMemory.offset,
					/*.size=*/ memoryRequirements.size
				}
			}));
		}
	}

	db.map(payload.index, [&] (auto &entry) {
		entry.subMeshes = std::move(subMeshes);
		entry.status = rn::vki::db::MeshEntryStatus::STAGED;
	});

	ngn::log::debug("rn::vki::db::mesh::Reader::readWithMsb({}, {}) => {} bytes ({} bytes vertex, {} bytes index)", payload.index, payload.source, vertexBufferSize + indexBufferSize, vertexBufferSize, indexBufferSize);

	return Payload{
		std::move(payload.index),
		std::move(payload.source),
		std::move(payload.promise),
		std::move(vertexBuffer),
		std::move(vertexBufferMemory),
		std::move(vertexBufferSize),
		std::move(indexBuffer),
		std::move(indexBufferMemory),
		std::move(indexBufferSize)
	};
}

} // rn::vki::db::mesh
