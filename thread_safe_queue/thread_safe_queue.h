#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {
    }

    void Push(const T& value) {
        std::unique_lock<std::mutex> lock(m_);
        q_.push(value);
        cv_.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait(lock, [&]{return !q_.empty();});
        auto temp = q_.front();
        q_.pop();
        return temp;
    }

    std::optional<T> TryPop() {
        std::unique_lock<std::mutex> lock(m_);
        if (q_.empty()) {
            return std::nullopt;
        }
        auto temp = q_.front();
        q_.pop();

        return temp;
    }


private:
    std::condition_variable cv_;
    std::mutex m_;
    std::queue<T> q_;
};
