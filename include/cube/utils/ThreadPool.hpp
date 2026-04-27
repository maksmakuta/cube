#ifndef CUBE_THREADPOOL_HPP
#define CUBE_THREADPOOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace cube {

    class ThreadPool final {
    public:
        explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) {
            const size_t actual_threads = num_threads > 0 ? num_threads : 1;
            
            for (size_t i = 0; i < actual_threads; ++i) {
                m_workers.emplace_back([this](const std::stop_token &st) {
                    while (!st.stop_requested()) {
                        std::move_only_function<void()> task;
                        {
                            std::unique_lock lock(m_queueMutex);
                            
                            m_cv.wait(lock, st, [this] { return !m_tasks.empty(); });
                            
                            if (st.stop_requested() && m_tasks.empty()) {
                                return;
                            }

                            task = std::move(m_tasks.front());
                            m_tasks.pop();
                        }

                        if (task) {
                            task();
                        }
                    }
                });
            }
        }

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        ~ThreadPool() {
            for (auto& worker : m_workers) {
                worker.request_stop();
            }
            m_cv.notify_all();
        }

        void enqueue(std::move_only_function<void()> task) {
            {
                std::scoped_lock lock(m_queueMutex);
                m_tasks.push(std::move(task));
            }
        }

        void notifyAll() {
            m_cv.notify_all();
        }

    private:
        std::vector<std::jthread> m_workers;
        std::queue<std::move_only_function<void()>> m_tasks;
        std::mutex m_queueMutex;
        std::condition_variable_any m_cv;
    };

}

#endif //CUBE_THREADPOOL_HPP