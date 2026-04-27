#ifndef CUBE_THREADPOOL_HPP
#define CUBE_THREADPOOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace cube {

    struct PrioritizedTask {
        uint64_t sequence;
        std::move_only_function<void()> work;

        bool operator<(const PrioritizedTask& other) const {
            return sequence > other.sequence;
        }
    };

    class ThreadPool final {
    public:
        explicit ThreadPool(const size_t num_threads = std::thread::hardware_concurrency()) {
            for (size_t i = 0; i < num_threads; ++i) {
                m_workers.emplace_back([this](const std::stop_token& st) {
                    while (!st.stop_requested()) {
                        std::move_only_function<void()> task;
                        {
                            std::unique_lock lock(m_mutex);
                            m_cv.wait(lock, st, [this] { return !m_tasks.empty(); });

                            if (st.stop_requested()) return;

                            task = std::move(const_cast<PrioritizedTask&>(m_tasks.top()).work);
                            m_tasks.pop();
                        }
                        if (task) task();
                    }
                });
            }
        }

        ~ThreadPool() {
            for (auto& worker : m_workers) worker.request_stop();
            m_cv.notify_all();
        }

        void enqueue(std::move_only_function<void()> work) {
            {
                std::scoped_lock lock(m_mutex);
                m_tasks.push({m_counter++, std::move(work)});
            }
            m_cv.notify_one();
        }

        void enqueue_batch(std::vector<PrioritizedTask>& batch) {
            {
                std::scoped_lock lock(m_mutex);
                for (auto& task : batch) {
                    task.sequence = m_counter++;
                    m_tasks.push(std::move(task));
                }
            }
            m_cv.notify_all();
        }

    private:
        std::vector<std::jthread> m_workers;
        std::priority_queue<PrioritizedTask> m_tasks;
        std::mutex m_mutex;
        std::condition_variable_any m_cv;
        uint64_t m_counter = 0;
    };

}

#endif //CUBE_THREADPOOL_HPP