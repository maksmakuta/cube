#include "cube/utils/ThreadPool.hpp"

namespace cube {

    ThreadPool::ThreadPool(const size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { this->worker(); });
        }
    }

    ThreadPool::~ThreadPool() {
        stop = true;
        condition.notify_all();
        for (auto &worker : workers) {
            worker.join();
        }
    }

    template<class F>
    auto ThreadPool::submit(F&& f) -> std::future<std::invoke_result_t<F>> {
        using return_type = std::invoke_result_t<F>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock lock(queueMutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    void ThreadPool::worker() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock lock(queueMutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty()) {
                    return;
                }
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

}