#include <thread>

#include "rn/window.hpp"
#include "rn/vlk/creators/contextCreator.hpp"
#include "rn/vlk/context.hpp"
#include "rn/vlk/db/texture.hpp"
#include "rn/vlk/db/mesh.hpp"

#include "ngn/log.hpp"
#include "ngn/config.hpp"

#include "app/main/state.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/DefaultLogger.hpp>     // Post processing flags

#include <stb/stb_image.h>

int main() {
	try {
		if (ngn::config::core.dirty()) {
			if ( ! ngn::config::core.store()) {
				ngn::log::error("Failed to store core config");
			}
		}

		// rn::GLFW glfw{};
		rn::Window window{};
		window.create();

		rn::vlk::Context context{};
		{
			auto contextStart = glfwGetTime();

			context = rn::vlk::ContextCreator{window}.create();

			auto contextEnd = glfwGetTime();
			ngn::log::debug("context#create: {}ms", 1000.0 * (contextEnd - contextStart));
		}

		// ngn::Inputs inputs{context};

		if (ngn::config::core.dirty()) {
			ngn::log::warn("Config is dirty after context creation");

			ngn::log::debug("{}", ngn::config::core.dump());
		}

		// if (false)
		for (auto &filePath : std::vector<std::string>{"../assets/meshes/Arch.obj", "../assets/meshes/FerrisWheel_lowpoly.obj", "../assets/meshes/Tree1.obj", "../assets/meshes/Suzanne head rig.blend", "../assets/meshes/Wolf.fbx"})
		{
			Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);

			Assimp::Importer importer{};

			uint32_t flags = aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes | aiProcess_TransformUVCoords | aiProcess_GenNormals | aiProcess_SortByPType;

			const aiScene *scene = importer.ReadFile(filePath, flags);

			if (scene == nullptr) {
				ngn::log::debug("Failed to load: {}", importer.GetErrorString());

				return 1;
			}

			ngn::log::debug("Scene loaded ({})", filePath);
			ngn::log::debug("mNumMeshes: {}", scene->mNumMeshes);
			for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
				ngn::log::debug("  [{}]", i);
				ngn::log::debug("    - mPrimitiveTypes: {}", scene->mMeshes[i]->mPrimitiveTypes);
				ngn::log::debug("    - mNumVertices: {}", scene->mMeshes[i]->mNumVertices);
				ngn::log::debug("    - mNumFaces: {}", scene->mMeshes[i]->mNumFaces);
				ngn::log::debug("    - mNumUVComponents[0]: {}", scene->mMeshes[i]->mNumUVComponents[0]);
				ngn::log::debug("    - mNumUVComponents[1]: {}", scene->mMeshes[i]->mNumUVComponents[1]);
				ngn::log::debug("    - mNumUVComponents[2]: {}", scene->mMeshes[i]->mNumUVComponents[2]);
				ngn::log::debug("    - mNumUVComponents[3]: {}", scene->mMeshes[i]->mNumUVComponents[3]);
				ngn::log::debug("    - mNumUVComponents[4]: {}", scene->mMeshes[i]->mNumUVComponents[4]);
				ngn::log::debug("    - mNumUVComponents[5]: {}", scene->mMeshes[i]->mNumUVComponents[5]);
				ngn::log::debug("    - mNumUVComponents[6]: {}", scene->mMeshes[i]->mNumUVComponents[6]);
				ngn::log::debug("    - mNumUVComponents[7]: {}", scene->mMeshes[i]->mNumUVComponents[7]);
				ngn::log::debug("    - mNumBones: {}", scene->mMeshes[i]->mNumBones);
				ngn::log::debug("    - mMaterialIndex: {}", scene->mMeshes[i]->mMaterialIndex);
				ngn::log::debug("    - mNumAnimMeshes: {}", scene->mMeshes[i]->mNumAnimMeshes);
				ngn::log::debug("    - mMethod: {}", scene->mMeshes[i]->mMethod);
			}
			ngn::log::debug("mNumMaterials: {}", scene->mNumMaterials);
			for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
				ngn::log::debug("  [{}]", i);
				ngn::log::debug("    - GetTextureCount(NONE): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_NONE));
				ngn::log::debug("    - GetTextureCount(DIFFUSE): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE));
				ngn::log::debug("    - GetTextureCount(SPECULAR): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR));
				ngn::log::debug("    - GetTextureCount(AMBIENT): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_AMBIENT));
				ngn::log::debug("    - GetTextureCount(EMISSIVE): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_EMISSIVE));
				ngn::log::debug("    - GetTextureCount(HEIGHT): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_HEIGHT));
				ngn::log::debug("    - GetTextureCount(NORMALS): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS));
				ngn::log::debug("    - GetTextureCount(SHININESS): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_SHININESS));
				ngn::log::debug("    - GetTextureCount(OPACITY): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_OPACITY));
				ngn::log::debug("    - GetTextureCount(DISPLACEMENT): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_DISPLACEMENT));
				ngn::log::debug("    - GetTextureCount(LIGHTMAP): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_LIGHTMAP));
				ngn::log::debug("    - GetTextureCount(REFLECTION): {}", scene->mMaterials[i]->GetTextureCount(aiTextureType_REFLECTION));
			}
			ngn::log::debug("mNumAnimations: {}", scene->mNumAnimations);
			ngn::log::debug("mNumTextures: {}", scene->mNumTextures);
		}

		// if (false)
		{
			rn::vlk::db::Texture texDb{};
			rn::vlk::db::texture::Loader texLoader{context, texDb};

			rn::vlk::db::texture::Handle handle1 = texDb.emplace("../assets/textures/TreeTexture.png");
			rn::vlk::db::texture::Handle handle2 = texDb.emplace("../assets/textures/kueken7_rgba8_unorm.dds");
			rn::vlk::db::texture::Handle handle3 = texDb.emplace("../assets/textures/kueken7_rgba8_unorm.ktx");

			auto future1 = texLoader.load(handle1);
			auto future2 = texLoader.load(handle2);
			auto future3 = texLoader.load(handle3);

			ngn::log::debug("waiting for handle 1 {}...", handle1.index);
			ngn::log::debug("waiting for handle 2 {}...", handle2.index);
			ngn::log::debug("waiting for handle 3 {}...", handle3.index);

			ngn::log::debug("got handle 1 {}", future1.get());
			ngn::log::debug("got handle 2 {}", future2.get());
			ngn::log::debug("got handle 3 {}", future3.get());
		}

		if (false)
		{
			rn::vlk::db::Mesh meshDb{};
			rn::vlk::db::mesh::Loader meshLoader{context, meshDb};

			// rn::vlk::db::mesh::Handle handle1 = meshDb.emplace("../assets/textures/TreeTexture.png");
			// rn::vlk::db::mesh::Handle handle2 = meshDb.emplace("../assets/textures/kueken7_rgba8_unorm.dds");
			rn::vlk::db::mesh::Handle handle3 = meshDb.emplace("../assets/meshes/Tree1.blend");

			// auto future1 = meshLoader.load(handle1);
			// auto future2 = meshLoader.load(handle2);
			// auto future3 = meshLoader.load(handle3);

			// ngn::log::debug("waiting for handle 1 {}...", handle1.index);
			// ngn::log::debug("waiting for handle 2 {}...", handle2.index);
			// ngn::log::debug("waiting for handle 3 {}...", handle3.index);

			// ngn::log::debug("got handle 1 {}", future1.get());
			// ngn::log::debug("got handle 2 {}", future2.get());
			// ngn::log::debug("handle 3 resolved {}", future3.get());
		}

		if (true) {
			return EXIT_SUCCESS;
		}

		app::main::State mainState{window, context};

		{
			auto initStart = glfwGetTime();

			mainState.init();

			auto initEnd = glfwGetTime();
			ngn::log::debug("mainState#init: {}ms", 1000.0 * (initEnd - initStart));
		}

		/*
		{
			rn::vlk::BuddyPool buddyPool{context.owners.device.get(), 64 * 1024, 0};

			ngn::log::debug("alloc #0 pre  [allocs: {}]", std::size(buddyPool.allocators));
			auto alloc0 = buddyPool.alloc(vk::MemoryRequirements{64 * 1024, 256, 0});
			ngn::log::debug("alloc #0 post [allocs: {}] offset={}", std::size(buddyPool.allocators), alloc0.offset);

			ngn::log::debug("alloc #1 pre  [allocs: {}]", std::size(buddyPool.allocators));
			auto alloc1 = buddyPool.alloc(vk::MemoryRequirements{32 * 1024, 256, 0});
			ngn::log::debug("alloc #1 post [allocs: {}] offset={}", std::size(buddyPool.allocators), alloc1.offset);

			ngn::log::debug("alloc #2 pre  [allocs: {}]", std::size(buddyPool.allocators));
			auto alloc2 = buddyPool.alloc(vk::MemoryRequirements{32 * 1024, 256, 0});
			ngn::log::debug("alloc #2 post [allocs: {}] offset={}", std::size(buddyPool.allocators), alloc2.offset);

			alloc0.destroy();

			ngn::log::debug("alloc #3 pre  [allocs: {}]", std::size(buddyPool.allocators));
			auto alloc3 = buddyPool.alloc(vk::MemoryRequirements{32 * 1024, 256, 0});
			ngn::log::debug("alloc #3 post [allocs: {}] offset={}", std::size(buddyPool.allocators), alloc3.offset);

			ngn::log::debug("alloc #4 pre  [allocs: {}]", std::size(buddyPool.allocators));
			auto alloc4 = buddyPool.alloc(vk::MemoryRequirements{96 * 1024, 256, 0});
			ngn::log::debug("alloc #4 post [allocs: {}] offset={}", std::size(buddyPool.allocators), alloc3.offset);
		}
		*/

		/*
		{
			rn::vlk::BuddyMemory buddyMemory{context.handles.physicalDevice.memoryProperties, context.owners.device.get(), 64 * 1024};
		}
		*/

		/*
			rn::vlk::BuddyAllocator buddyAllocator{vk::UniqueDeviceMemory{}, 64 * 1024, 0};

			ngn::log::debug("masks:");

			ngn::log::debug("[0b01111111111111111111111111111111] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[0], rn::vlk::BuddyAllocator::masks[0]);
			ngn::log::debug("[0b00000000011111111000011110011011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[1], rn::vlk::BuddyAllocator::masks[1]);
			ngn::log::debug("[0b01111111100000000111100001100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[2], rn::vlk::BuddyAllocator::masks[2]);
			ngn::log::debug("[0b00000000000001111000000110001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[3], rn::vlk::BuddyAllocator::masks[3]);
			ngn::log::debug("[0b00000000011110000000011000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[4], rn::vlk::BuddyAllocator::masks[4]);
			ngn::log::debug("[0b00000111100000000001100000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[5], rn::vlk::BuddyAllocator::masks[5]);
			ngn::log::debug("[0b01111000000000000110000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[6], rn::vlk::BuddyAllocator::masks[6]);
			ngn::log::debug("[0b00000000000000011000000010001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[7], rn::vlk::BuddyAllocator::masks[7]);
			ngn::log::debug("[0b00000000000001100000000100001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[8], rn::vlk::BuddyAllocator::masks[8]);
			ngn::log::debug("[0b00000000000110000000001000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[9], rn::vlk::BuddyAllocator::masks[9]);
			ngn::log::debug("[0b00000000011000000000010000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[10], rn::vlk::BuddyAllocator::masks[10]);
			ngn::log::debug("[0b00000001100000000000100000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[11], rn::vlk::BuddyAllocator::masks[11]);
			ngn::log::debug("[0b00000110000000000001000000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[12], rn::vlk::BuddyAllocator::masks[12]);
			ngn::log::debug("[0b00011000000000000010000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[13], rn::vlk::BuddyAllocator::masks[13]);
			ngn::log::debug("[0b01100000000000000100000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[14], rn::vlk::BuddyAllocator::masks[14]);
			ngn::log::debug("[0b00000000000000001000000010001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[15], rn::vlk::BuddyAllocator::masks[15]);
			ngn::log::debug("[0b00000000000000010000000010001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[16], rn::vlk::BuddyAllocator::masks[16]);
			ngn::log::debug("[0b00000000000000100000000100001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[17], rn::vlk::BuddyAllocator::masks[17]);
			ngn::log::debug("[0b00000000000001000000000100001011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[18], rn::vlk::BuddyAllocator::masks[18]);
			ngn::log::debug("[0b00000000000010000000001000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[19], rn::vlk::BuddyAllocator::masks[19]);
			ngn::log::debug("[0b00000000000100000000001000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[20], rn::vlk::BuddyAllocator::masks[20]);
			ngn::log::debug("[0b00000000001000000000010000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[21], rn::vlk::BuddyAllocator::masks[21]);
			ngn::log::debug("[0b00000000010000000000010000010011] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[22], rn::vlk::BuddyAllocator::masks[22]);
			ngn::log::debug("[0b00000000100000000000100000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[23], rn::vlk::BuddyAllocator::masks[23]);
			ngn::log::debug("[0b00000001000000000000100000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[24], rn::vlk::BuddyAllocator::masks[24]);
			ngn::log::debug("[0b00000010000000000001000000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[25], rn::vlk::BuddyAllocator::masks[25]);
			ngn::log::debug("[0b00000100000000000001000000100101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[26], rn::vlk::BuddyAllocator::masks[26]);
			ngn::log::debug("[0b00001000000000000010000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[27], rn::vlk::BuddyAllocator::masks[27]);
			ngn::log::debug("[0b00010000000000000010000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[28], rn::vlk::BuddyAllocator::masks[28]);
			ngn::log::debug("[0b00100000000000000100000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[29], rn::vlk::BuddyAllocator::masks[29]);
			ngn::log::debug("[0b01000000000000000100000001000101] [{:0>32b}] = {}", rn::vlk::BuddyAllocator::masks[30], rn::vlk::BuddyAllocator::masks[30]);

			ngn::log::debug("alloc #0 pre  [{:0>32b}]", buddyAllocator.mask);
			auto alloc0 = buddyAllocator.alloc(64 * 1024, 256);
			ngn::log::debug("alloc #0 post [{:0>32b}] offset={}", buddyAllocator.mask, alloc0.offset);

			ngn::log::debug("free #0 pre   [{:0>32b}]", buddyAllocator.mask);
			buddyAllocator.free(std::move(alloc0));
			ngn::log::debug("free #0 post  [{:0>32b}]", buddyAllocator.mask);

			ngn::log::debug("alloc #1 pre  [{:0>32b}]", buddyAllocator.mask);
			auto alloc1 = buddyAllocator.alloc(1024 * 4, 256);
			ngn::log::debug("alloc #1 post [{:0>32b}] offset={}", buddyAllocator.mask, alloc1.offset);

			ngn::log::debug("alloc #2 pre  [{:0>32b}]", buddyAllocator.mask);
			auto alloc2 = buddyAllocator.alloc(1024, 1024 * 8 - 1);
			ngn::log::debug("alloc #2 post [{:0>32b}] offset={}", buddyAllocator.mask, alloc2.offset);

			ngn::log::debug("free #1 pre   [{:0>32b}]", buddyAllocator.mask);
			buddyAllocator.free(std::move(alloc1));
			ngn::log::debug("free #1 post  [{:0>32b}]", buddyAllocator.mask);

			ngn::log::debug("free #2 pre   [{:0>32b}]", buddyAllocator.mask);
			buddyAllocator.free(std::move(alloc2));
			ngn::log::debug("free #2 post  [{:0>32b}]", buddyAllocator.mask);

			ngn::log::debug("alloc #3 pre  [{:0>32b}]", buddyAllocator.mask);
			auto alloc3 = buddyAllocator.alloc(1024 * 4, 256);
			ngn::log::debug("alloc #3 post [{:0>32b}] offset={}", buddyAllocator.mask, alloc3.offset);

			ngn::log::debug("alloc #4 pre  [{:0>32b}]", buddyAllocator.mask);
			auto alloc4 = buddyAllocator.alloc(1024, 1024 * 8 - 1);
			ngn::log::debug("alloc #4 post [{:0>32b}] offset={}", buddyAllocator.mask, alloc4.offset);

			ngn::log::debug("free #4 pre   [{:0>32b}]", buddyAllocator.mask);
			buddyAllocator.free(std::move(alloc4));
			ngn::log::debug("free #4 post  [{:0>32b}]", buddyAllocator.mask);

			ngn::log::debug("free #3 pre   [{:0>32b}]", buddyAllocator.mask);
			buddyAllocator.free(std::move(alloc3));
			ngn::log::debug("free #3 post  [{:0>32b}]", buddyAllocator.mask);
			*/

		/*
			:    0111 1111 1000 0111 0111 1001 0110 0100
			:     -----------------------------------------------------------------
			: 512 |                               1                               |
			:     -----------------------------------------------------------------
			: 256 |               1               |               1               |
			:     -----------------------------------------------------------------
			: 128 |       1       |       1       |       1       |       1       |
			:     -----------------------------------------------------------------
			:  64 |   1   |   0   |   0   |   0   |   0   |   1   |   1   |   1   |
			:     -----------------------------------------------------------------
			:  32 | 0 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 1 | 1 | 1 | 0 | 1 | 1 | 0 | 0 |
			:     -----------------------------------------------------------------

			:      0111 1111 1000 0111 1111 1001 1110 1100
			:     -----------------------------------------------------------------
			: 512 |                               1                               |
			:     -----------------------------------------------------------------
			: 256 |               1               |               1               |
			:     -----------------------------------------------------------------
			: 128 |       1       |       1       |       1       |       1       |
			:     -----------------------------------------------------------------
			:  64 |   1   |   0   |   0   |   0   |   0   |   1   |   1   |   1   |
			:     -----------------------------------------------------------------
			:  32 | 1 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 1 | 1 | 1 | 0 | 1 | 1 | 0 | 0 |
			:     -----------------------------------------------------------------
		*/
		for (size_t i = 0; /*i < 24 &&*/ ! glfwWindowShouldClose(window.handle); i++) {
			NGN_PROF_SCOPE("main loop");

			{
				NGN_PROF_SCOPE("event polling");
				glfwPollEvents();
			}

			{
				NGN_PROF_SCOPE("window refreshing");

				if ( ! window.needsRefresh()) {
					window.refresh();
					mainState.refresh();
				}
			}

			mainState.render();
		}

		{
			auto deinitStart = glfwGetTime();

			mainState.deinit();

			auto deinitEnd = glfwGetTime();
			ngn::log::debug("mainState#deinit: {}ms", 1000.0 * (deinitEnd - deinitStart));
		}

		return EXIT_SUCCESS;
	} catch (std::runtime_error const &e) {
		ngn::log::critical("Runtime exception: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		ngn::log::critical("Unknown exception");
		return EXIT_FAILURE;
	}
}
