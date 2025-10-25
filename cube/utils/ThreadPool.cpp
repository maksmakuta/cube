#include "ThreadPool.hpp"

namespace cube {

    template<typename Func, typename... Args>
    auto ThreadPool::submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {
        using ReturnType = std::invoke_result_t<Func, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

        std::future<ReturnType> result = task->get_future();

        const size_t targetQueue = m_nextQueue.fetch_add(1, std::memory_order_relaxed) % m_queues.size();

        {
            std::unique_lock lock(m_queues[targetQueue]->mutex);
            if (m_stop) {
                throw std::runtime_error("Cannot submit to stopped ThreadPool");
            }

            m_queues[targetQueue]->tasks.emplace([task] { (*task)(); });
        }

        m_queues[targetQueue]->cv.notify_one();
        return result;
    }

    template<typename Func, typename... Args>
    void ThreadPool::execute(Func&& func, Args&&... args) {
        const size_t targetQueue = m_nextQueue.fetch_add(1, std::memory_order_relaxed) % m_queues.size();

        {
            std::unique_lock lock(m_queues[targetQueue]->mutex);
            if (m_stop) return;

            m_queues[targetQueue]->tasks.emplace(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );
        }

        m_queues[targetQueue]->cv.notify_one();
    }

    template<std::ranges::range Range, typename Func>
    void ThreadPool::parallelFor(Range&& range, Func&& func) {
        auto begin = std::ranges::begin(range);
        auto end = std::ranges::end(range);
        const size_t totalSize = std::ranges::distance(begin, end);

        if (totalSize == 0) return;

        const size_t numWorkers = std::min(totalSize, m_workers.size());
        const size_t chunkSize = (totalSize + numWorkers - 1) / numWorkers;

        std::vector<std::future<void>> futures;
        futures.reserve(numWorkers);

        auto it = begin;
        for (size_t i = 0; i < numWorkers && it != end; ++i) {
            auto chunkBegin = it;
            auto chunkEnd = it;

            for (size_t j = 0; j < chunkSize && chunkEnd != end; ++j) {
                ++chunkEnd;
            }

            futures.push_back(submit([&func, chunkBegin, chunkEnd] {
                for (auto it1 = chunkBegin; it1 != chunkEnd; ++it1) {
                    func(*it1);
                }
            }));

            it = chunkEnd;
        }

        for (auto& future : futures) {
            future.wait();
        }
    }

    template<typename Func>
    void ThreadPool::parallelForIndex(const size_t count, Func&& func) {
        if (count == 0) return;

        const size_t numWorkers = std::min(count, m_workers.size());
        const size_t chunkSize = (count + numWorkers - 1) / numWorkers;

        std::vector<std::future<void>> futures;
        futures.reserve(numWorkers);

        for (size_t i = 0; i < numWorkers; ++i) {
            size_t start = i * chunkSize;
            size_t end = std::min(start + chunkSize, count);

            if (start >= count) break;

            futures.push_back(submit([&func, start, end] {
                for (size_t idx = start; idx < end; ++idx) {
                    func(idx);
                }
            }));
        }

        for (auto& future : futures) {
            future.wait();
        }
    }

    template<typename Func, typename... Args>
    void ThreadPool::submitBatch(const size_t count, Func&& func, Args&&... args) {
        if (count == 0) return;

        const size_t tasksPerQueue = (count + m_queues.size() - 1) / m_queues.size();
        size_t remaining = count;

        for (size_t qIdx = 0; qIdx < m_queues.size() && remaining > 0; ++qIdx) {
            const size_t batchSize = std::min(tasksPerQueue, remaining);

            {
                std::unique_lock lock(m_queues[qIdx]->mutex);
                if (m_stop) return;

                for (size_t i = 0; i < batchSize; ++i) {
                    m_queues[qIdx]->tasks.emplace(
                        std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
                    );
                }
            }

            m_queues[qIdx]->cv.notify_all();
            remaining -= batchSize;
        }
    }


    ThreadPool::ThreadPool(size_t numThreads) {
        if (numThreads == 0) {
            numThreads = std::thread::hardware_concurrency();
        }

        m_workers.reserve(numThreads);
        m_queues.resize(numThreads);

        for (size_t i = 0; i < numThreads; ++i) {
            m_workers.emplace_back([this, i] { workerThread(i); });
        }
    }

    ThreadPool::~ThreadPool() {
        shutdown();
    }

    void ThreadPool::workerThread(size_t threadId) {
        while (true) {
            std::function<void()> task;

            // Try to get task from own queue
            {
                std::unique_lock lock(m_queues[threadId]->mutex);
                m_queues[threadId]->cv.wait(lock, [this, threadId] {
                    return m_stop || !m_queues[threadId]->tasks.empty();
                });

                if (m_stop && m_queues[threadId]->tasks.empty()) {
                    return;
                }

                if (!m_queues[threadId]->tasks.empty()) {
                    task = std::move(m_queues[threadId]->tasks.front());
                    m_queues[threadId]->tasks.pop();
                }
            }

            // If no task found, try work stealing from other queues
            if (!task) {
                bool found = false;
                for (size_t i = 1; i < m_queues.size() && !found; ++i) {
                    const size_t targetQueue = (threadId + i) % m_queues.size();

                    std::unique_lock lock(m_queues[targetQueue]->mutex, std::try_to_lock);
                    if (lock.owns_lock() && !m_queues[targetQueue]->tasks.empty()) {
                        task = std::move(m_queues[targetQueue]->tasks.front());
                        m_queues[targetQueue]->tasks.pop();
                        found = true;
                    }
                }

                if (!found) continue;
            }

            // Execute task
            m_activeWorkers.fetch_add(1, std::memory_order_relaxed);
            try {
                task();
            } catch (...) {
                // Silently catch exceptions to prevent thread termination
            }
            m_activeWorkers.fetch_sub(1, std::memory_order_relaxed);

            // Notify waitAll
            for (const auto& queue : m_queues) {
                queue->cv.notify_all();
            }
        }
    }

    void ThreadPool::waitAll() const {
        for (const auto & m_queue : m_queues) {
            std::unique_lock lock(m_queue->mutex);
            m_queue->cv.wait(lock, [this, &m_queue] {
                return m_queue->tasks.empty() && m_activeWorkers.load() == 0;
            });
        }
    }

    size_t ThreadPool::getPendingTaskCount() const {
        size_t count = 0;
        for (const auto& queue : m_queues) {
            std::unique_lock lock(queue->mutex);
            count += queue->tasks.size();
        }
        return count;
    }

    size_t ThreadPool::getThreadCount() const {
        return m_workers.size();
    }

    size_t ThreadPool::getActiveThreadCount() const {
        return m_activeWorkers.load(std::memory_order_relaxed);
    }

    void ThreadPool::shutdown() {
        {
            std::unique_lock lock(m_queues[0]->mutex);
            if (m_stop) return;
            m_stop = true;
        }

        for (const auto& queue : m_queues) {
            queue->cv.notify_all();
        }

        for (auto& worker : m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

}