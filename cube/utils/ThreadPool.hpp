#ifndef CUBE_THREADPOOL_HPP
#define CUBE_THREADPOOL_HPP

#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <ranges>

namespace cube {

    class ThreadPool {
    public:
        explicit ThreadPool(size_t numThreads = 0);
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        template<typename Func, typename... Args>
        auto submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>>;

        template<typename Func, typename... Args>
        void execute(Func&& func, Args&&... args);

        template<std::ranges::range Range, typename Func>
        void parallelFor(Range&& range, Func&& func);

        template<typename Func>
        void parallelForIndex(size_t count, Func&& func);

        template<typename Func, typename... Args>
        void submitBatch(size_t count, Func&& func, Args&&... args);

        void waitAll() const;

        size_t getPendingTaskCount() const;

        size_t getThreadCount() const;

        size_t getActiveThreadCount() const;

        void shutdown();

    private:
        struct WorkQueue {
            mutable std::mutex mutex;
            std::condition_variable cv;
            std::queue<std::function<void()>> tasks;
        };

        void workerThread(size_t threadId);

        std::vector<std::thread> m_workers;
        std::vector<std::unique_ptr<WorkQueue>> m_queues;
        std::atomic<size_t> m_nextQueue{0};
        std::atomic<size_t> m_activeWorkers{0};
        std::atomic<bool> m_stop{false};
    };

    using TPoolPtr = std::shared_ptr<ThreadPool>;

}

#endif //CUBE_THREADPOOL_HPP