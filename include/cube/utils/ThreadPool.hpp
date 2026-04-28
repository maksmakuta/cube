#ifndef CUBE_THREADPOOL_HPP
#define CUBE_THREADPOOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace cube {

    class ThreadPool {
    public:
        explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency() - 1);
        ~ThreadPool();

        void enqueue(std::function<void()> task);

    private:
        std::vector<std::jthread> m_workers;
        std::queue<std::function<void()>> m_tasks;
        std::mutex m_queueMutex;
        std::condition_variable m_cv;
        std::atomic<bool> m_stop{false};

        void workerLoop();
    };

}

#endif //CUBE_THREADPOOL_HPP
