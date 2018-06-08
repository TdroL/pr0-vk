#pragma once

#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>
#include <set>

#include <assimp/scene.h>

#include "common.hpp"

namespace conv {

struct DescMaterial {
	std::string name{};
	std::optional<std::string> texAlbedo{};
	std::optional<std::string> texEmissive{};
	std::optional<std::string> texNormal{};
	std::optional<std::string> texRoughnessMetallic{};
};

struct DescMapping {
	uint32_t meshIndex = 0;
	std::string materialName = "";
};

std::optional<std::vector<std::byte>> desc(const aiScene *scene, const std::string_view &ext) {
	std::vector<DescMaterial> descMaterials{};
	descMaterials.reserve(scene->mNumMaterials);
	std::set<std::string> descMaterialNames{};

	for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
		DescMaterial material{};

		// name
		{
			aiString value;
			aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_NAME, value);
			if (result == aiReturn_SUCCESS) {
				material.name = std::string{value.C_Str()};
			}
		}

		if (material.name.empty()) {
			material.name = "material-" + std::to_string(i);
		} else {
			std::transform(std::begin(material.name), std::end(material.name), std::begin(material.name), [] (unsigned char c) {
				return std::isspace(c) ? '-' : c;
			});
		}

		// ensure name uniqueness
		{
			std::string materialName = material.name;

			for (uint32_t j = 1; descMaterialNames.find(materialName) != std::end(descMaterialNames); j++) {
				materialName = material.name + "-" + std::to_string(j);
			}

			material.name = materialName;

			descMaterialNames.insert(material.name);
		}

		// texAlbedo
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path{};
			scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			material.texAlbedo = std::string{path.C_Str()};
		}

		// texEmissive
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
			aiString path{};
			scene->mMaterials[i]->GetTexture(aiTextureType_EMISSIVE, 0, &path);

			material.texEmissive = std::string{path.C_Str()};
		}

		// texNormal
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString path{};
			scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path);

			material.texNormal = std::string{path.C_Str()};
		}

		// texRoughnessMetallic
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR) > 0) {
			aiString path{};
			scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &path);

			material.texRoughnessMetallic = std::string{path.C_Str()};
		} else if (ext == ".gltf" && scene->mMaterials[i]->GetTextureCount(aiTextureType_UNKNOWN) > 0) {
			aiString path{};
			scene->mMaterials[i]->GetTexture(aiTextureType_UNKNOWN, 0, &path);

			material.texRoughnessMetallic = std::string{path.C_Str()};
		}

		descMaterials.push_back(material);
	}

	std::vector<DescMapping> descMappings{};
	descMappings.reserve(scene->mNumMeshes);

	for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
		DescMapping mapping{};
		mapping.meshIndex = i;
		mapping.materialName = descMaterials[scene->mMeshes[i]->mMaterialIndex].name;

		descMappings.push_back(mapping);
	}

	std::string contents{};
	for (const auto &material : descMaterials) {
		if (material.texAlbedo.has_value()) {
			contents += "mat " + material.name + " tex-albedo             " + material.texAlbedo.value() + "\n";
		}

		if (material.texEmissive.has_value()) {
			contents += "mat " + material.name + " tex-emissive           " + material.texEmissive.value() + "\n";
		}

		if (material.texNormal.has_value()) {
			contents += "mat " + material.name + " tex-normal             " + material.texNormal.value() + "\n";
		}

		if (material.texRoughnessMetallic.has_value()) {
			contents += "mat " + material.name + " tex-roughness-metallic " + material.texRoughnessMetallic.value() + "\n";
		}
	}

	if ( ! contents.empty()) {
		contents += "\n";
	}

	for (const auto &mapping : descMappings) {
		contents += "map " + std::to_string(mapping.meshIndex) + " " + mapping.materialName + "\n";
	}

	std::vector<std::byte> buffer{};
	buffer.resize(contents.size());

	std::byte *data = buffer.data();

	copy(data, contents.data(), contents.size());

	return buffer;
}

} // conv
