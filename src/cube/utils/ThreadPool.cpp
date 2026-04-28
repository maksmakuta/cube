#include "cube/utils/ThreadPool.hpp"

namespace cube {

    ThreadPool::ThreadPool(const size_t threadCount) {
        for (size_t i = 0; i < threadCount; ++i) {
            m_workers.emplace_back([this] { workerLoop(); });
        }
    }

    ThreadPool::~ThreadPool() {
        m_stop = true;
        m_cv.notify_all();
    }

    void ThreadPool::enqueue(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_tasks.push(std::move(task));
        }
        m_cv.notify_one();
    }

    void ThreadPool::workerLoop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock lock(m_queueMutex);
                m_cv.wait(lock, [this] { return m_stop || !m_tasks.empty(); });

                if (m_stop && m_tasks.empty()) return;

                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
            task();
        }
    }

}