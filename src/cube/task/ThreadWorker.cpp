#include <cube/task/ThreadWorker.hpp>

namespace cube {

    ThreadWorker::ThreadWorker(World& world) : m_world(world) {
        uint32_t threads = std::thread::hardware_concurrency();
        for (uint32_t i = 0; i < threads; ++i) {
            m_workers.emplace_back([this](const std::stop_token &st) { worker_loop(st); });
        }
    }

    void ThreadWorker::push_task(MeshTask& task) {
        {
            std::lock_guard lock(m_queue_mutex);
            m_tasks.push(std::move(task));
        }
        m_cv.notify_one();
    }


    std::vector<MeshResult> ThreadWorker::collect_results() {
        std::lock_guard lock(m_result_mutex);
        return std::move(m_results);
    }

    void ThreadWorker::worker_loop(const std::stop_token &st) {
        while (!st.stop_requested()) {
            MeshTask task{};
            {
                std::unique_lock lock(m_queue_mutex);
                m_cv.wait(lock, st, [this] { return !m_tasks.empty(); });
                if (st.stop_requested()) return;

                task = m_tasks.front();
                m_tasks.pop();
            }

            {
                MeshResult result = mesh(m_world,task);
                std::lock_guard lock(m_result_mutex);
                m_results.push_back(std::move(result));
            }
        }
    }

}
