#ifndef CUBE_SAFEQUEUE_HPP
#define CUBE_SAFEQUEUE_HPP

#include <queue>
#include <mutex>
#include <optional>

namespace cube {

    template<typename T>
    class ConcurrentQueue final {
    public:
        void push(T&& result) {
            std::lock_guard lock(m_mutex);
            m_queue.push(std::move(result));
        }

        std::optional<T> pop() {
            std::lock_guard lock(m_mutex);
            if (m_queue.empty()) {
                return std::nullopt;
            }

            T result = std::move(m_queue.front());
            m_queue.pop();
            return result;
        }

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
    };

}

#endif //CUBE_SAFEQUEUE_HPP
