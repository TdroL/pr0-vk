#pragma once

// Based on https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h

// Copyright (c) 2012 Jakob Progsch, Václav Zeman

// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:

//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.

//    2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.

//    3. This notice may not be removed or altered from any source
//    distribution.

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

namespace util {

class ThreadPool {
public:
	// need to keep track of threads so we can join them
	std::vector<std::thread> workers;
	// the task queue
	std::queue<std::function<void (uint32_t threadIndex)>> tasks;

	// synchronization
	std::mutex mutex;
	std::condition_variable condition;
	std::atomic<bool> shouldStop{false};

	explicit ThreadPool(uint32_t poolSize) {
		workers.reserve(poolSize);

		for(uint32_t i = 0; i < poolSize; i++) {
			workers.emplace_back([&, i] () {
				for(;;) {
					std::function<void (uint32_t threadIndex)> task;

					{
						std::unique_lock<std::mutex> lock{mutex};
						condition.wait(lock, [&]{ return shouldStop.load() || ! tasks.empty(); });

						if (shouldStop.load() && tasks.empty()) {
							return;
						}

						task = std::move(tasks.front());
						tasks.pop();
					}

					task(i);
				}
			});
		}
	}

	size_t size() const {
		return workers.size();
	}

	void stop() {
		if (shouldStop.load()) {
			return;
		}

		shouldStop.store(true);
		condition.notify_all();

		for (auto &&worker: workers) {
			worker.join();
		}
	}

	template<typename F>
	auto enqueue(F &&callback)-> std::future<typename std::result_of<F (uint32_t)>::type> {
		using ReturnType = typename std::result_of<F (uint32_t)>::type;

		if (shouldStop.load()) {
			std::promise<ReturnType> promise{};
			promise.set_value(callback(uint32_t{0}));

			return promise.get_future();
		} else {
			auto task = std::make_shared<std::packaged_task<ReturnType (uint32_t threadIndex)>>(std::forward<F>(callback));

			std::future<ReturnType> future = task->get_future();

			{
				std::unique_lock<std::mutex> lock{mutex};

				tasks.emplace([task] (uint32_t threadIndex) { (*task)(threadIndex); });
			}

			condition.notify_one();

			return future;
		}
	}

	~ThreadPool() {
		stop();
	}
};

} // util
