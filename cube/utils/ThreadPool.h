#ifndef CUBE_THREADPOOL_H
#define CUBE_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace cube {

    class ThreadPool {
    public:
        explicit ThreadPool(const size_t n = std::thread::hardware_concurrency()) : stop(false) {
            for (size_t i = 0; i < n; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock lock(queueMutex);
                            cv.wait(lock, [this] { return stop || !tasks.empty(); });
                            if (stop && tasks.empty()) return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPool() {
            {
                std::unique_lock lock(queueMutex);
                stop = true;
            }
            cv.notify_all();
            for (auto& t : workers) t.join();
        }

        template<class F>
        void enqueue(F&& f) {
            {
                std::unique_lock lock(queueMutex);
                tasks.emplace(std::forward<F>(f));
            }
            cv.notify_one();
        }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable cv;
        bool stop;
    };


}

#endif //CUBE_THREADPOOL_H