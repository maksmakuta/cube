#include "cube/utils/ThreadPool.hpp"

namespace cube {

    ThreadPool::ThreadPool(const size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { this->worker(); });
        }
    }

    ThreadPool::~ThreadPool() {
        if (!stop) {
            stop = true;
            condition.notify_all();
        }
        for (auto &worker : workers) {
            worker.join();
        }
    }

    void ThreadPool::unload() {
        stop = true;
        condition.notify_all();
    }

    void ThreadPool::worker() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock lock(queueMutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop || tasks.empty()) {
                    return;
                }
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

}