#include "cube/utils/ThreadPool.hpp"

namespace cube {

    ThreadPool::ThreadPool(const std::size_t thread_count) {
        start(thread_count);
    }

    ThreadPool::~ThreadPool() {
        stop();
    }

    std::size_t ThreadPool::thread_count() const noexcept {
        return m_workers.size();
    }

    void ThreadPool::start(const std::size_t count) {
        m_workers.reserve(count);

        for (std::size_t i = 0; i < count; ++i) {
            m_workers.emplace_back(
                [this](const std::stop_token &stop_token) {
                    worker_loop(stop_token);
                }
            );
        }
    }

    void ThreadPool::stop() noexcept {
        for (auto& worker : m_workers) {
            worker.request_stop();
        }

        m_cv.notify_all();

        m_workers.clear();
    }

    void ThreadPool::worker_loop(const std::stop_token &stop_token) {
        while (!stop_token.stop_requested()) {
            Task task;

            {
                std::unique_lock lock(m_mutex);

                m_cv.wait(lock, stop_token, [this] {
                    return !m_tasks.empty();
                });

                if (m_tasks.empty()) {
                    continue;
                }

                task = std::move(m_tasks.front());
                m_tasks.pop();
            }

            task();
        }
    }

}