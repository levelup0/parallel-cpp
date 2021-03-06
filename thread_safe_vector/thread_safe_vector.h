#pragma once

#include <vector>
#include <mutex>
#include <shared_mutex>


template <typename T>
class ThreadSafeVector {
public:
    ThreadSafeVector() {
    }

    T operator[](size_t index) const {
        std::shared_lock<std::shared_mutex> lock(shared_mutex_);
        return vector_[index];
    }

    size_t Size() const {
        std::unique_lock<std::mutex> u_lock(mutex_);
        std::shared_lock<std::shared_mutex> sh_lock(shared_mutex_);
        return vector_.size();
    }

    void PushBack(const T& value) {
        std::unique_lock<std::mutex> outer_lock(mutex_);
        if (vector_.size() == vector_.capacity()) {
            std::unique_lock<std::shared_mutex> inner_lock(shared_mutex_);
            vector_.push_back(value);
        } else {
            std::shared_lock<std::shared_mutex> inner_lock(shared_mutex_);
            vector_.push_back(value);
        }
    }

private:
    mutable std::shared_mutex shared_mutex_;
    mutable std::mutex mutex_;
    std::vector<T> vector_;
};

