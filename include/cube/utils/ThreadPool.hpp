#ifndef CUBE_THREADPOOL_HPP
#define CUBE_THREADPOOL_HPP

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace cube {

    class ThreadPool final {
    public:
        explicit ThreadPool(
            std::size_t thread_count = std::thread::hardware_concurrency()
        );

        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        template<typename F, typename... Args>
        auto submit(F&& func, Args&&... args)
            -> std::future<std::invoke_result_t<F, Args...>>
        {
            using result_t = std::invoke_result_t<F, Args...>;

            auto task = std::packaged_task<result_t()>(
                [f = std::forward<F>(func),
                 ... a = std::forward<Args>(args)]() mutable {
                    return std::invoke(std::move(f), std::move(a)...);
                }
            );

            auto future = task.get_future();

            {
                std::lock_guard lock(m_mutex);

                m_tasks.emplace(
                    [t = std::move(task)]() mutable {
                        t();
                    }
                );
            }

            m_cv.notify_one();

            return future;
        }

        template<typename F, typename... Args>
        void enqueue_detached(F&& func, Args&&... args) {
            {
                std::lock_guard lock(m_mutex);

                m_tasks.emplace(
                    [f = std::forward<F>(func),
                     ... a = std::forward<Args>(args)]() mutable {
                        std::invoke(std::move(f), std::move(a)...);
                    }
                );
            }

            m_cv.notify_one();
        }

        [[nodiscard]]
        std::size_t thread_count() const noexcept;

    private:
        using Task = std::move_only_function<void()>;

        void start(std::size_t count);
        void stop() noexcept;
        void worker_loop(const std::stop_token &stop_token);

        std::mutex m_mutex;
        std::queue<Task> m_tasks;
        std::condition_variable_any m_cv;
        std::vector<std::jthread> m_workers;
    };

}

#endif