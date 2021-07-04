#pragma once
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

class Deadlock {
 public:
  Deadlock() {
    x = y = 0;
  }

  void ThreadOne() {
    x_mutex.lock();
    std::this_thread::sleep_for(10ms);
    y_mutex.lock();
    x_mutex.unlock();
    y_mutex.unlock();
  }

  void ThreadTwo() {
    y_mutex.lock();
    std::this_thread::sleep_for(10ms);
    x_mutex.lock();
    y_mutex.unlock();
    x_mutex.unlock();
  }

 private:
  std::mutex x_mutex, y_mutex;
  int x, y;
};

