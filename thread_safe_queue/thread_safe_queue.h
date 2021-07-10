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
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        cv_.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&]{return !queue_.empty();});
        auto temp = queue_.front();
        queue_.pop();
        return temp;
    }

    std::optional<T> TryPop() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return std::nullopt;
        }
        auto temp = queue_.front();
        queue_.pop();

        return temp;
    }


private:
    std::condition_variable cv_;
    std::mutex mutex_;
    std::queue<T> queue_;
};
