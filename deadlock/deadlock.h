#pragma once
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

class Deadlock {
public:
    Deadlock() {
    }

    void ThreadOne() {
        std::unique_lock<std::mutex> x_lock(x_mutex_);
        std::this_thread::sleep_for(10ms);
        std::unique_lock<std::mutex> y_lock(y_mutex_);
    }

    void ThreadTwo() {
        std::unique_lock<std::mutex> y_lock(y_mutex_);
        std::this_thread::sleep_for(10ms);
        std::unique_lock<std::mutex> x_lock(x_mutex_);
    }

private:
    std::mutex x_mutex_, y_mutex_;
};

