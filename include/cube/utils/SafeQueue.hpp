#ifndef CUBE_SAFEQUEUE_HPP
#define CUBE_SAFEQUEUE_HPP

#include <mutex>
#include <queue>
#include <optional>

namespace cube {

    template<typename T>
    class SafeQueue {
    public:
        void push(const T& value) {
            std::lock_guard lock(m_mutex);
            m_queue.push(value);
        }

        std::optional<T> pop() {
            std::lock_guard lock(m_mutex);
            if (m_queue.empty()) return std::nullopt;
            T val = m_queue.front();
            m_queue.pop();
            return val;
        }

        bool empty() {
            std::lock_guard lock(m_mutex);
            return m_queue.empty();
        }

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
    };

}

#endif //CUBE_SAFEQUEUE_HPP
