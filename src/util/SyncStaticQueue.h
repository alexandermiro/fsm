
#ifndef FSM_SRC_UTIL_SYNC_STATIC_QUEUE_H
#define FSM_SRC_UTIL_SYNC_STATIC_QUEUE_H


#include <array>
#include <mutex>
#include <condition_variable>

using std::string;

namespace fsm::util {

/**
 * @brief
 */
template <typename T, std::size_t N>
class SyncStaticQueue final {
public:
    /**
     * @brief
     * @param
     */
    SyncStaticQueue() noexcept {}

    /**
     * @brief Deleted constructors
     */
    SyncStaticQueue(SyncStaticQueue const&)             = delete;
    SyncStaticQueue& operator=(SyncStaticQueue const&)  = delete;
    SyncStaticQueue(SyncStaticQueue&&)                  = delete;
    SyncStaticQueue& operator=(SyncStaticQueue&&)       = delete;

    /**
     * @brief Dctor
     */
    ~SyncStaticQueue() {}

    /**
     * @brief size
     * @return
     */
    std::size_t size() {
        std::size_t size{0u};
        {
            std::unique_lock<std::mutex> lock(m_size_mtx);
            size = m_size;
        }
        return size;
    }

    /**
      * @brief
      * @return
      */
    constexpr bool is_zero_length_queue() const {
        return N == 0u;
    }

    /**
     * @brief clear
     */
    void clear() {
        m_front = 0u;
        m_back = 0u;
    }

    /**
     * @brief push
     * @param p_new_item
     * @return
     */
    bool push(T&& p_new_item) {
        if (is_zero_length_queue()) {
            return false;
        }
        std::unique_lock<std::mutex> lock(m_size_mtx);
        m_cv_wait_for_size_change.wait(lock, [&]() { return m_size < N; });
        m_queue[m_front] = std::move(p_new_item);
        m_front = (m_front + 1u) % N;
        m_size++;
        lock.unlock();
        return true;
    }

    /**
     * @brief pop
     * @return
     */
    T pop() {
        std::size_t size{0u};
        {
            std::scoped_lock<std::mutex> lock(m_size_mtx);
            size = m_size;
        }
        if (size == 0u) {
            return T{};
        }
        auto&& ret = std::move(m_queue[m_back]);
        m_back = (m_back + 1u) % N;
        {
            std::unique_lock<std::mutex> lock(m_size_mtx);
            m_size--;
            m_cv_wait_for_size_change.notify_all();
        }
        return ret;
    }

private:
    std::size_t                 m_size{0u};
    std::size_t                 m_front{0u};
    std::size_t                 m_back{0u};
    std::array<T, N>            m_queue;
    std::mutex                  m_size_mtx;
    std::condition_variable     m_cv_wait_for_size_change;
};

}  // namespace fsm::util 

#endif  //FSM_SRC_UTIL_SYNC_STATIC_QUEUE_H
