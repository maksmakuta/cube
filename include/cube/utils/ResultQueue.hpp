#ifndef CUBE_RESULTQUEUE_HPP
#define CUBE_RESULTQUEUE_HPP

#include <vector>
#include <mutex>
#include <utility>

namespace cube {

    template <typename T>
    class ResultQueue final {
    public:
        ResultQueue() = default;

        void push(T item) {
            std::scoped_lock lock(m_mutex);
            m_items.push_back(std::move(item));
        }

        [[nodiscard]] std::vector<T> pop_all() {
            std::vector<T> local_items;
            {
                std::scoped_lock lock(m_mutex);
                local_items.swap(m_items);
            }
            return local_items;
        }

        [[nodiscard]] size_t approximate_size() const {
            std::scoped_lock lock(m_mutex);
            return m_items.size();
        }

    private:
        std::vector<T> m_items;
        mutable std::mutex m_mutex; 
    };

}

#endif //CUBE_RESULTQUEUE_HPP