#pragma once
#include <iostream>
#include <thread>
#include <atomic>


class RWSpinLock {
public:
    RWSpinLock() {
    }

    void LockRead() {
        while (wrighter_waiting_.load()) {
            std::this_thread::yield();
        }
        while (count_.load() % 2 != 0) {
            std::this_thread::yield();
        }
        count_.fetch_add(2);
    }

    void UnlockRead() {
        count_.fetch_sub(2);
    }

    void LockWrite() {
        wrighter_waiting_.store(true);
        while (count_.load() != 0) {
            std::this_thread::yield();
        }
        while (count_.exchange(1)) {
            std::this_thread::yield();
        }
        wrighter_waiting_.store(false);
    }

    void UnlockWrite() {
        count_.fetch_sub(1);
    }

private:
    std::atomic<uint64_t> count_{0};
    std::atomic<bool> wrighter_waiting_{false};
};