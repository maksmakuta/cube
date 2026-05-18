#include "cube/utils/ThreadPool.hpp"

namespace cube {

    ThreadPool::ThreadPool(const size_t threads) : m_stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            m_workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock lock(m_queueMutex);
                        m_cv.wait(lock, [this] {
                            return m_stop || !m_tasks.empty();
                        });

                        if (m_stop && m_tasks.empty()) {
                            return;
                        }

                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void ThreadPool::enqueue(std::function<void()> task) {
        {
            std::unique_lock lock(m_queueMutex);
            if (m_stop) return;
            m_tasks.push(std::move(task));
        }
        m_cv.notify_one();
    }

    ThreadPool::~ThreadPool() {
        {
            std::unique_lock lock(m_queueMutex);
            m_stop = true;
        }
        m_cv.notify_all();
    }

}