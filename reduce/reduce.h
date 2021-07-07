#pragma once

#include <cstddef>
#include <vector>
#include <thread>
#include <numeric>


template <typename Iter, class T, class Func>
T parallel_reduce(Iter begin, Iter end,
                  const T& init, Func f, size_t n) {
    std::vector<std::thread> threads;
    size_t size = std::distance(begin, end);
    size_t size_of_block = (size - 1) / n + 1;
    size_t temp_res_ind = 0;
    std::vector<T> temp_res(n);

    for (; temp_res_ind < n; ++temp_res_ind) {
        Iter cur_begin = begin, cur_end;

        if (temp_res_ind == n - 1) {
            cur_end = end;
        } else {
            begin += size_of_block;
            cur_end = begin;
        }

        threads.emplace_back(
                [] (Iter begin, Iter end, T& result, T init, Func f) {
                    result = std::reduce(begin, end, init, f);
                },
                cur_begin,
                cur_end,
                std::ref(temp_res[temp_res_ind]),
                init,
                f);
    }

    for (auto &t : threads) {
        t.join();
    }

    return std::reduce(temp_res.begin(), temp_res.end(), init, f);
}

