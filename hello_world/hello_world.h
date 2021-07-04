#pragma once

#include <ostream>
#include <string_view>
#include <vector>
#include <thread> 
#include <iostream>
#include <mutex>

class HelloWorld {
 public:
  static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

  HelloWorld(size_t n_threads) : mN_threads(n_threads) {}

  void SayHello(std::ostream& out) {
      std::vector<std::thread> threads;
      std::mutex m;
      for (size_t i = 0; i < mN_threads; ++i) {
          threads.emplace_back([&] {
                  m.lock();
                  out << kHelloPrefix << std::this_thread::get_id() << '\n';
                  m.unlock();
                  });
      }

      for (auto &t : threads) {
          t.join();
      }
  }

 private:
  size_t mN_threads;
};

