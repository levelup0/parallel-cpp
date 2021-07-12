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
        std::shared_lock<std::shared_mutex> lock(shared_mutex_);
        return vector_.size();
    }

    void PushBack(const T& value) {
        std::unique_lock<std::mutex> outer_lock(mutex_);
        if (vector_.size() == vector_.capacity()) {
            shared_mutex_.lock();
            vector_.push_back(value);
            shared_mutex_.unlock();
        } else {
            shared_mutex_.lock_shared();
            vector_.push_back(value);
            shared_mutex_.unlock_shared();
        }
    }

private:
    mutable std::shared_mutex shared_mutex_;
    mutable std::mutex mutex_;
    std::vector<T> vector_;
};

