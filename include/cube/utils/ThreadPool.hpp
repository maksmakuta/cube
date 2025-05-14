#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace cube {

    class ThreadPool {
    public:
        explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
        ~ThreadPool();

        template<class F>
        auto submit(F&& f) -> std::future<std::invoke_result_t<F>> {
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
    private:
        void worker();

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stop;
    };

}

#endif //THREADPOOL_HPP
