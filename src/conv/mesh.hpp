#pragma once

#include <algorithm>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

#include <assimp/scene.h>

#include "common.hpp"

namespace conv {

namespace {

char metaDescriptionContent[] = "Format:\n"
	"struct MeshHeader {\n"
	"	uint8_t code[4] { 'B', 'M', 0xff, 0x01 };\n"
	"	uint32_t meshCount;\n"
	"	uint32_t descriptionLength;\n"
	"	char description[descriptionLength];\n"
	"};\n"
	"\n"
	"enum MeshFlags {\n"
	"	MhF_none = 0x00000000,\n"
	"	MhF_hasNormals = 0x00000001,\n"
	"	MhF_hasCoords = 0x00000002,\n"
	"	MhF_hasBones = 0x00000004,\n"
	"	MhF_hasIndices = 0x00000008,\n"
	"};\n"
	"\n"
	"enum MeshTopology {\n"
	"	MhT_triangleList = 0x00000000,\n"
	"	MhT_triangleStrip = 0x00000001,\n"
	"	MhT_triangleFan = 0x00000002,\n"
	"};\n"
	"\n"
	"struct MeshMeta {\n"
	"	uint64_t offset;\n"
	"	uint64_t size;\n"
	"	uint32_t vertices;\n"
	"	uint32_t bones;\n"
	"	uint32_t indices;\n"
	"	uint32_t flags;\n"
	"	uint32_t topology;\n"
	"};\n"
	"\n"
	"struct MeshPosition {\n"
	"	float x, y, z;\n"
	"};\n"
	"\n"
	"struct MeshNormal {\n"
	"	float x, y, z;\n"
	"};\n"
	"\n"
	"struct MeshCoord {\n"
	"	float u, v;\n"
	"};\n"
	"\n"
	"struct MeshIndex {\n"
	"	uint32_t v0, v1, v2;\n"
	"};\n"
	"\n"
	"<...> - start, end\n"
	"[...] - repeatable (one or more)\n"
	"(...) - optional\n"
	"\n"
	"<\n"
	"	MeshHeader,\n"
	"	[MeshMeta],\n"
	"	[\n"
	"		MeshPosition,\n"
	"		(MeshNormal),\n"
	"		(MeshCoord),\n"
	"		(MeshIndex)\n"
	"	]\n"
	">";

}

struct MeshHeader {
	uint8_t code[4] { 'B', 'M', 0xff, 0x01 };
	uint32_t meshCount = 0;
	uint32_t descriptionLength = (sizeof(metaDescriptionContent) + 3u) & ~3u;
	char description[(sizeof(metaDescriptionContent) + 3u) & ~3u];
};
static_assert(sizeof(MeshHeader) == sizeof(uint8_t) * 4 + sizeof(uint32_t) * 2 + ((sizeof(metaDescriptionContent) + 3u) & ~3u));
static_assert(sizeof(MeshHeader) % 4 == 0);

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
	uint64_t offset = 0;
	uint64_t size = 0;
	uint32_t vertices = 0;
	uint32_t bones = 0;
	uint32_t indices = 0;
	uint32_t flags = MhF_none;
	uint32_t topology = MhT_triangleList;
};
static_assert(sizeof(MeshMeta) == sizeof(uint32_t) * 6 + sizeof(uint64_t) * 2);
static_assert(sizeof(MeshMeta) % 4 == 0);

std::optional<std::vector<std::byte>> mesh(const aiScene *scene, const std::string_view &/*ext*/) {
	std::vector<MeshMeta> meshMetas{};
	meshMetas.reserve(scene->mNumMeshes);

	MeshHeader meshHeader{};
	meshHeader.meshCount = scene->mNumMeshes;
	std::fill_n(std::begin(meshHeader.description), meshHeader.descriptionLength, '\0');
	std::copy(std::begin(metaDescriptionContent), std::end(metaDescriptionContent), std::begin(meshHeader.description));

	size_t size = sizeof(MeshHeader) + sizeof(MeshMeta) * scene->mNumMeshes;
	for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
		MeshMeta meshMeta{};

		if ( ! scene->mMeshes[i]->HasPositions()) {
			std::cerr << "Mesh " << i << " does not contain positions\n";
			return std::nullopt;
		}

		if (scene->mMeshes[i]->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
			std::cerr << "Mesh " << i << " has invalid promitive type (" << scene->mMeshes[i]->mPrimitiveTypes << ")\n";
			return std::nullopt;
		}

		meshMeta.size += scene->mMeshes[i]->mNumVertices * sizeof(float) * 3;
		meshMeta.vertices = scene->mMeshes[i]->mNumVertices;

		if (scene->mMeshes[i]->HasNormals()) {
			meshMeta.flags |= MhF_hasNormals;
			meshMeta.size += scene->mMeshes[i]->mNumVertices * sizeof(float) * 3;
		}

		if (scene->mMeshes[i]->HasTextureCoords(0)) {
			if (scene->mMeshes[i]->mNumUVComponents[0] != 2) {
				std::cerr << "Mesh " << i << " has invalid number of coordinate components (" << scene->mMeshes[i]->mNumUVComponents[0] << ")\n";
				return std::nullopt;
			}

			meshMeta.flags |= MhF_hasCoords;
			meshMeta.size += scene->mMeshes[i]->mNumVertices * sizeof(float) * 2;
		}

		/*
		// TODO
		if (scene->mMeshes[i]->HasBones()) {
			meshMeta.flags |= MhF_hasBones;
		}
		*/

		if (scene->mMeshes[i]->HasFaces()) {
			for (size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
				if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3) {
					std::cerr << "Mesh " << i << " has invalid face " << j << " (" << scene->mMeshes[i]->mFaces[j].mNumIndices << ")\n";
					return std::nullopt;
				}
			}

			meshMeta.flags |= MhF_hasIndices;
			meshMeta.size += scene->mMeshes[i]->mNumFaces * sizeof(uint32_t) * 3;
			meshMeta.indices = scene->mMeshes[i]->mNumFaces * 3;
		}

		meshMeta.offset = size;

		meshMetas.push_back(meshMeta);

		size += meshMeta.size;
	}

	std::vector<std::byte> buffer{};
	buffer.resize(size);
	std::byte *data = buffer.data();

	data = copy(data, &meshHeader);

	data = copy(data, meshMetas.data(), meshMetas.size());

	for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
		assert(buffer.data() + meshMetas[i].offset == data);
		// copy positions
		data = copy(data, scene->mMeshes[i]->mVertices, scene->mMeshes[i]->mNumVertices);

		// copy normals
		if (scene->mMeshes[i]->HasNormals()) {
			data = copy(data, scene->mMeshes[i]->mNormals, scene->mMeshes[i]->mNumVertices);
		}

		// copy coords
		if (scene->mMeshes[i]->HasTextureCoords(0)) {
			for (uint32_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
				data = copy(data, &scene->mMeshes[i]->mTextureCoords[0][j].x, 1);
				data = copy(data, &scene->mMeshes[i]->mTextureCoords[0][j].y, 1);
			}
		}

		/*
		// TODO
		if (scene->mMeshes[i]->HasBones()) {
			meshMeta.flags |= MhF_hasBones;
		}
		*/

		// copy indices
		if (scene->mMeshes[i]->HasFaces()) {
			for (size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
				data = copy(data, scene->mMeshes[i]->mFaces[j].mIndices, 3);
			}
		}

		assert(buffer.data() + meshMetas[i].offset + meshMetas[i].size == data);
	}

	assert(buffer.data() + size == data);

	return buffer;
}

} // conv
