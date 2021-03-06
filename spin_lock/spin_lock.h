#pragma once
#include <atomic>
#include <thread>

class SpinLock {
public:
    SpinLock() {
    }

    void Lock() {
        while (locked_.exchange(true)) {
            std::this_thread::yield();
        }
    }

    void Unlock() {
        locked_.store(false);
    }

private:
    std::atomic<bool> locked_{false};
};
