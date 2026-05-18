#ifndef CUBE_THREADPOOL_HPP
#define CUBE_THREADPOOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

namespace cube {

    class ThreadPool final {
    public:
        explicit ThreadPool(size_t threads);
        ~ThreadPool();

        void enqueue(std::function<void()> task);

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

    private:
        std::vector<std::jthread> m_workers;
        std::queue<std::function<void()>> m_tasks;

        std::mutex m_queueMutex;
        std::condition_variable m_cv;
        bool m_stop;
    };

}

#endif //CUBE_THREADPOOL_HPP
