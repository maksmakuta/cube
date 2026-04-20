#ifndef CUBE_THREADWORKER_HPP
#define CUBE_THREADWORKER_HPP

#include <condition_variable>
#include <queue>
#include <thread>

#include "Meshing.hpp"
#include "cube/data/World.hpp"

namespace cube {

    class ThreadWorker {
    public:
        explicit ThreadWorker(World&);

        void push_task(MeshTask& task);
        std::vector<MeshResult> collect_results();

    private:
        void worker_loop(const std::stop_token &st);

        World& m_world;
        std::queue<MeshTask> m_tasks;
        std::vector<MeshResult> m_results;

        std::mutex m_queue_mutex;
        std::mutex m_result_mutex;
        std::condition_variable_any m_cv;
        std::vector<std::jthread> m_workers;
    };

}

#endif //CUBE_THREADWORKER_HPP
