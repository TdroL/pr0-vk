#include "rn/window.hpp"
#include "rn/vlk/creators/contextCreator.hpp"
#include "rn/vlk/context.hpp"

// #include "rn/vlk/memory.hpp"

#include "ngn/log.hpp"
#include "ngn/config.hpp"

#include "app/main/state.hpp"

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
			ngn::prof::Scope("main loop");

			{
				ngn::prof::Scope("event polling");
				glfwPollEvents();
			}

			{
				ngn::prof::Scope("window refreshing");

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
	} catch (...) {
		ngn::log::critical("Unknown exception");
	}
}
