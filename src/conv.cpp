#include <algorithm>
#include <cassert>
#include <ios>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "ngn/fs.hpp"
#include "ngn/log.hpp"
#include "ngn/config.hpp"

#include "util/implode.hpp"
#include "util/opts.hpp"
#include "util/map.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>

#include "conv/mesh.hpp"
#include "conv/desc.hpp"

bool hasExt(const std::string_view &source, const std::vector<std::string_view> &exts) {
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

std::string getExt(const std::string_view &source) {
	const auto pos = source.rfind('.');

	if(pos != std::string::npos) {
		std::string ext{source.substr(pos)};

		std::transform(std::begin(ext), std::end(ext), std::begin(ext), [] (unsigned char c) { return std::tolower(c); });

		return ext;
	}
	else {
		return "";
	}
}

std::string propertyToString(aiMaterialProperty *property) {
	switch (property->mType) {
		case aiPTI_Float: {
			std::vector<float> values{reinterpret_cast<float *>(property->mData), reinterpret_cast<float *>(property->mData + property->mDataLength)};
			return "[" + util::implode(util::map(values, [] (float value) { return std::to_string(value); })) + "]";
		}
		case aiPTI_Double: {
			std::vector<double> values{reinterpret_cast<double *>(property->mData), reinterpret_cast<double *>(property->mData + property->mDataLength)};
			return "[" + util::implode(util::map(values, [] (double value) { return std::to_string(value); })) + "]";
		}
		case aiPTI_String: {
			return "[" + std::string{property->mData, property->mData + property->mDataLength} + "]";
		}
		case aiPTI_Integer: {
			std::vector<int> values{reinterpret_cast<int *>(property->mData), reinterpret_cast<int *>(property->mData + property->mDataLength)};
			return "[" + util::implode(util::map(values, [] (int value) { return std::to_string(value); })) + "]";
		}
		case aiPTI_Buffer: {
			if (property->mDataLength == 4) {
				return "[buffer: " + std::to_string(*reinterpret_cast<uint32_t *>(property->mData)) + ":" + std::to_string(*reinterpret_cast<float* >(property->mData)) + "]";
			} else {
				return std::string{"[buffer]"};
			}

		}
		default: {
			return std::string{"[unknown]"};
		}
	}
}

enum MaterialFlags {
	MtF_none = 0x00000000,
	MtF_hasTexNormals = 0x00000001,
	MtF_hasTexAlbedo = 0x00000002,
	MtF_hasTexRoughnessMetallic = 0x00000004,
	MtF_hasTexEmissive = 0x00000008,
};

enum MaterialAlphaMode {
	MtAM_opaque = 0x00000000,
	MtAM_mask = 0x00000001,
	MtAM_transparent = 0x00000002,
};

struct MaterialMeta {
	uint64_t offset = 0;
	uint64_t size = 0;
	uint32_t alphaMode = MtAM_opaque;
	uint32_t flags = MtF_none;
};
static_assert(sizeof(MaterialMeta) == sizeof(uint32_t) * 2 + sizeof(uint64_t) * 2);

int main(int argc, char *argv[]) {
	try {
		util::Opts opts{argc, argv};

		std::string input{opts.get("-i")};
		std::string output{opts.get("-o")};

		if (opts.has("-h")) {
			std::cout << "-i input\n";
			std::cout << "-o output\n";
			std::cout << "-m run mesh converter\n";
			std::cout << "-d run description builder\n";
			std::cout << "-h help\n";
			std::cout << "-v verbose\n";

			return EXIT_SUCCESS;
		}

		if (input.empty()) {
			std::cerr << "-i input required\n";
			return EXIT_FAILURE;
		}

		if (output.empty()) {
			output = input;

			auto dotPos = output.find_last_of('.');
			if (dotPos != std::string::npos) {
				output = output.substr(0, dotPos);
			}
		}

		std::vector<std::string_view> exts{ ".3ds", ".ac", ".ase", ".b3d", ".blend", ".bvh", ".dae", ".fbx", ".glb", ".gltf", ".hmp", ".ifc", ".irrmesh", ".lwo", ".lws", ".lxo", ".md2", ".md3", ".md5", ".mdl", ".mdl", ".ms3d", ".ndo", ".nff", ".nff", ".obj", ".off", ".pk3", ".ply", ".q3d", ".q3s", ".raw", ".stl", ".ter", ".x", ".xgl", ".xml", ".zgl" };

		if ( ! hasExt(input, exts)) {
			std::cerr << "-i input file extension not supported\n";
			return EXIT_FAILURE;
		}

		std::string ext = getExt(input);

		bool verbose = opts.has("-v");
		if (verbose) {
			Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);
		}

		bool runMesh = true;
		bool runDesc = true;
		if (opts.has("-m") || opts.has("-d")) {
			runMesh = opts.has("-m");
			runDesc = opts.has("-d");
		}

		Assimp::Importer importer{};
		const aiScene *scene = importer.ReadFile(input, aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene == nullptr) {
			std::cerr << "Failed to load scene: " << importer.GetErrorString() << "\n";
			return EXIT_FAILURE;
		}

		if ( ! scene->HasMeshes()) {
			std::cerr << "Scene does not contain meshes\n";
			return EXIT_FAILURE;
		}

		if ( ! scene->HasMaterials()) {
			std::cerr << "Scene does not contain materials\n";
			return EXIT_FAILURE;
		}

		if (runMesh) {
			const auto meshOpt = conv::mesh(scene, ext);
			if ( ! meshOpt) {
				return EXIT_FAILURE;
			}

			std::string meshOutput = output + ".msb";
			std::fstream meshFile(meshOutput, meshFile.binary | meshFile.trunc | meshFile.out);
			if ( ! meshFile.is_open()) {
				std::cout << "Failed to open " << meshOutput << "\n";
				return EXIT_FAILURE;
			}

			const auto &meshBinary = meshOpt.value();
			meshFile.write(reinterpret_cast<const char *>(meshBinary.data()), meshBinary.size());

			std::cout << "Mesh file " << meshOutput << " saved (" << meshBinary.size() << " bytes)\n";
		}

		if (runDesc) {
			const auto descOpt = conv::desc(scene, ext);
			if ( ! descOpt) {
				return EXIT_FAILURE;
			}

			std::string descOutput = output + ".dcs";
			std::fstream descFile(descOutput, descFile.binary | descFile.trunc | descFile.out);
			if ( ! descFile.is_open()) {
				std::cout << "Failed to open " << descOutput << "\n";
				return EXIT_FAILURE;
			}

			const auto &descBinary = descOpt.value();
			descFile.write(reinterpret_cast<const char *>(descBinary.data()), descBinary.size());

			std::cout << "Description file " << descOutput << " saved (" << descBinary.size() << " bytes)\n";
		}

		/*
		// std::vector<MeshMeta> meshMetas{};
		// std::vector<MaterialMeta> materialMetas{};

		// meshMetas.reserve(scene->mNumMeshes);
		size_t offset = 0;
		if (false) {
		// for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
			// MeshMeta meshMeta{};

			if ( ! scene->mMeshes[i]->HasPositions()) {
				std::cerr << "Mesh " << i << " does not contain positions\n";
				return EXIT_FAILURE;
			}

			if (scene->mMeshes[i]->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
				std::cerr << "Mesh " << i << " has invalid promitive type (" << scene->mMeshes[i]->mPrimitiveTypes << ")\n";
				return EXIT_FAILURE;
			}

			meshMeta.size += scene->mMeshes[i]->mNumVertices * sizeof(float) * 3;
			meshMeta.vertices = scene->mMeshes[i]->mNumVertices;

			if (scene->mMeshes[i]->HasNormals()) {
				meshMeta.flags |= MhF_hasNormals;
				meshMeta.size += scene->mMeshes[i]->mNumVertices * sizeof(float) * 3;
			}

			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				if (scene->mMeshes[i]->mNumUVComponents[0] < 2) {
					std::cerr << "Mesh " << i << " has invalid number of coordinate components (" << scene->mMeshes[i]->mNumUVComponents[0] << ")\n";
					return EXIT_FAILURE;
				}

				meshMeta.flags |= MhF_hasCoords;
				meshMeta.size += scene->mMeshes[i]->mNumVertices * sizeof(float) * 2;
			}

			// TODO
			// if (scene->mMeshes[i]->HasBones()) {
			// 	meshMeta.flags |= MhF_hasBones;
			// }

			if (scene->mMeshes[i]->HasFaces()) {
				for (size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3) {
						std::cerr << "Mesh " << i << " has invalid face " << j << " (" << scene->mMeshes[i]->mFaces[j].mNumIndices << ")\n";
						return EXIT_FAILURE;
					}
				}

				meshMeta.flags |= MhF_hasIndices;
				meshMeta.size += scene->mMeshes[i]->mNumFaces * sizeof(uint32_t) * 3;
				meshMeta.indices = scene->mMeshes[i]->mNumFaces;
			}

			meshMeta.offset = offset;
			meshMeta.materialIndex = scene->mMeshes[i]->mMaterialIndex;

			meshMetas.push_back(meshMeta);

			// std::cout << " [" << i << "] meshMeta.offset = " << meshMeta.offset << "\n";
			// std::cout << " [" << i << "] meshMeta.size = " << meshMeta.size << "\n";
			// std::cout << " [" << i << "] meshMeta.vertices = " << meshMeta.vertices << "\n";
			// std::cout << " [" << i << "] meshMeta.bones = " << meshMeta.bones << "\n";
			// std::cout << " [" << i << "] meshMeta.indices = " << meshMeta.indices << "\n";
			// std::cout << " [" << i << "] meshMeta.materialIndex = " << meshMeta.materialIndex << "\n";
			// std::cout << " [" << i << "] meshMeta.flags = " << std::hex << reinterpret_cast<uint64_t>(meshMeta.flags) << std::dec << "\n";
			// std::cout << " [" << i << "] meshMeta.topology = " << std::hex << reinterpret_cast<uint64_t>(meshMeta.topology) << std::dec << "\n";

			offset += meshMeta.size;

			// ngn::log::debug("Scene loaded ({})", input);
			// ngn::log::debug("mNumMeshes: {}", scene->mNumMeshes);
			// ngn::log::debug("  [{}]", i);
			// ngn::log::debug("    - mPrimitiveTypes: {}", scene->mMeshes[i]->mPrimitiveTypes);
			// ngn::log::debug("    - mNumVertices: {}", scene->mMeshes[i]->mNumVertices);
			// ngn::log::debug("    - mNumFaces: {}", scene->mMeshes[i]->mNumFaces);
			// ngn::log::debug("    - mVertices: {}", reinterpret_cast<void *>(scene->mMeshes[i]->mVertices));
			// ngn::log::debug("    - mNormals: {}", reinterpret_cast<void *>(scene->mMeshes[i]->mNormals));
			// ngn::log::debug("    - HasPositions: {}", scene->mMeshes[i]->HasPositions());
			// ngn::log::debug("    - HasNormals: {}", scene->mMeshes[i]->HasNormals());
			// ngn::log::debug("    - HasTextureCoords: {}", scene->mMeshes[i]->HasTextureCoords(0));
			// ngn::log::debug("    - HasBones: {}", scene->mMeshes[i]->HasBones());
			// ngn::log::debug("    - HasFaces: {}", scene->mMeshes[i]->HasFaces());
			// ngn::log::debug("    - mNumUVComponents[0]: {}", scene->mMeshes[i]->mNumUVComponents[0]);
			// ngn::log::debug("    - mNumUVComponents[1]: {}", scene->mMeshes[i]->mNumUVComponents[1]);
			// ngn::log::debug("    - mNumUVComponents[2]: {}", scene->mMeshes[i]->mNumUVComponents[2]);
			// ngn::log::debug("    - mNumUVComponents[3]: {}", scene->mMeshes[i]->mNumUVComponents[3]);
			// ngn::log::debug("    - mNumUVComponents[4]: {}", scene->mMeshes[i]->mNumUVComponents[4]);
			// ngn::log::debug("    - mNumUVComponents[5]: {}", scene->mMeshes[i]->mNumUVComponents[5]);
			// ngn::log::debug("    - mNumUVComponents[6]: {}", scene->mMeshes[i]->mNumUVComponents[6]);
			// ngn::log::debug("    - mNumUVComponents[7]: {}", scene->mMeshes[i]->mNumUVComponents[7]);
			// ngn::log::debug("    - mNumBones: {}", scene->mMeshes[i]->mNumBones);
			// ngn::log::debug("    - mMaterialIndex: {}", scene->mMeshes[i]->mMaterialIndex);
			// ngn::log::debug("    - mNumAnimMeshes: {}", scene->mMeshes[i]->mNumAnimMeshes);
			// ngn::log::debug("    - mMethod: {}", scene->mMeshes[i]->mMethod);
		}
		// materialMetas.reserve(scene->mNumMaterials);
		offset = 0;
		*/
		if (false)
		for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
			// MaterialMeta materialMeta{};

			// materialMeta.alphaMode = ;

			// enum MaterialFlags {
			// 	MtF_none = 0x00000000,
			// 	MtF_hasTexNormals = 0x00000001,
			// 	MtF_hasTexAlbedo = 0x00000002,
			// 	MtF_hasTexRoughnessMetallic = 0x00000004,
			// 	MtF_hasTexEmissive = 0x00000008,
			// };

			// enum MaterialAlphaMode {
			// 	MtAM_opaque = 0x00000000,
			// 	MtAM_mask = 0x00000001,
			// 	MtAM_transparent = 0x00000002,
			// };

			// struct MaterialMeta {
			// 	uint64_t offset = 0;
			// 	uint64_t size = 0;
			// 	uint32_t alphaMode = MtAM_opaque;
			// 	uint32_t flags = MtF_none;
			// };
			// static_assert(sizeof(MaterialMeta) == sizeof(uint64_t) * 8);

			ngn::log::debug("  [{}]", i);

			for (uint32_t j = 0; j < scene->mMaterials[i]->mNumProperties; j++) {
				if (scene->mMaterials[i]->mProperties[j]) {
					const auto &property = scene->mMaterials[i]->mProperties[j];

					ngn::log::debug("    - [{}] {}: {} byte(s) {}", j, property->mKey.C_Str(), property->mDataLength, propertyToString(property));
				}
			}


			{
				aiString value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_NAME, value);
				ngn::log::debug("    - NAME: ({}) {}", result, value.C_Str());
			}
			{
				bool value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_TWOSIDED, value);
				ngn::log::debug("    - TWOSIDED: ({}) {}", result, value);
			}
			{
				aiShadingMode value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_SHADING_MODEL, value);
				ngn::log::debug("    - SHADING_MODEL: ({}) {}", result, value);
			}
			{
				bool value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_ENABLE_WIREFRAME, value);
				ngn::log::debug("    - ENABLE_WIREFRAME: ({}) {}", result, value);
			}
			{
				aiBlendMode value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_BLEND_FUNC, value);
				ngn::log::debug("    - BLEND_FUNC: ({}) {}", result, value);
			}
			{
				float value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_OPACITY, value);
				ngn::log::debug("    - OPACITY: ({}) {}", result, value);
			}
			{
				float value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_BUMPSCALING, value);
				ngn::log::debug("    - BUMPSCALING: ({}) {}", result, value);
			}
			{
				float value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, value);
				ngn::log::debug("    - SHININESS: ({}) {}", result, value);
			}
			{
				float value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_REFLECTIVITY, value);
				ngn::log::debug("    - REFLECTIVITY: ({}) {}", result, value);
			}
			{
				float value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, value);
				ngn::log::debug("    - SHININESS_STRENGTH: ({}) {}", result, value);
			}
			{
				float value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_REFRACTI, value);
				ngn::log::debug("    - REFRACTI: ({}) {}", result, value);
			}
			{
				aiColor3D value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, value);
				ngn::log::debug("    - COLOR_DIFFUSE: ({}) {}, {}, {}", result, value.r, value.g, value.b);
			}
			{
				aiColor3D value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, value);
				ngn::log::debug("    - COLOR_AMBIENT: ({}) {}, {}, {}", result, value.r, value.g, value.b);
			}
			{
				aiColor3D value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, value);
				ngn::log::debug("    - COLOR_SPECULAR: ({}) {}, {}, {}", result, value.r, value.g, value.b);
			}
			{
				aiColor3D value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, value);
				ngn::log::debug("    - COLOR_EMISSIVE: ({}) {}, {}, {}", result, value.r, value.g, value.b);
			}
			{
				aiColor3D value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_COLOR_TRANSPARENT, value);
				ngn::log::debug("    - COLOR_TRANSPARENT: ({}) {}, {}, {}", result, value.r, value.g, value.b);
			}
			{
				aiColor3D value;
				aiReturn result = scene->mMaterials[i]->Get(AI_MATKEY_COLOR_REFLECTIVE, value);
				ngn::log::debug("    - COLOR_REFLECTIVE: ({}) {}, {}, {}", result, value.r, value.g, value.b);
			}

			ngn::log::debug("    - GetTextureCount(NONE): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_NONE));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_NONE); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_NONE, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(DIFFUSE): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(SPECULAR): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(AMBIENT): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_AMBIENT));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_AMBIENT); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_AMBIENT, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(EMISSIVE): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_EMISSIVE));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_EMISSIVE); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_EMISSIVE, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(HEIGHT): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_HEIGHT));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_HEIGHT); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_HEIGHT, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(NORMALS): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(SHININESS): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_SHININESS));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_SHININESS); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_SHININESS, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(OPACITY): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_OPACITY));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_OPACITY); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_OPACITY, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(DISPLACEMENT): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_DISPLACEMENT));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_DISPLACEMENT); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_DISPLACEMENT, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(LIGHTMAP): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_LIGHTMAP));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_LIGHTMAP); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_LIGHTMAP, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(REFLECTION): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_REFLECTION));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_REFLECTION); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_REFLECTION, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
			ngn::log::debug("    - GetTextureCount(UNKNOWN): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_UNKNOWN));
			for (uint32_t j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_UNKNOWN); j++) {
				aiString path{};
				scene->mMaterials[i]->GetTexture(aiTextureType_UNKNOWN, j, &path);
				ngn::log::debug("      - [{}]: {}", j, path.C_Str());
			}
		}
		/**/

		/*
		ngn::log::debug("mNumAnimations: {}", scene->mNumAnimations);
		for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
			ngn::log::debug("  [{}]", i);
			// mAnimations
			//
		}
		ngn::log::debug("mNumTextures: {}", scene->mNumTextures);
		for (uint32_t i = 0; i < scene->mNumTextures; i++) {
			ngn::log::debug("  [{}]", i);
			ngn::log::debug("    - mWidth: {}", scene->mTextures[i]->mWidth);
			ngn::log::debug("    - mHeight: {}", scene->mTextures[i]->mHeight);
			ngn::log::debug("    - achFormatHint: {}", scene->mTextures[i]->achFormatHint);
			ngn::log::debug("    - pcData: {:x}", reinterpret_cast<size_t>(scene->mTextures[i]->pcData));
		}
		*/

		return EXIT_SUCCESS;
	} catch (std::runtime_error const &e) {
		ngn::log::critical("Runtime exception: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		ngn::log::critical("Unknown exception");
		return EXIT_FAILURE;
	}
}
