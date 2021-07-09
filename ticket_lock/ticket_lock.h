#pragma once
#include <atomic>
#include <thread>


class TicketLock {
public:
    TicketLock() {
    }

    void Lock() {
        auto cur_ticket = last_ticket_number_.fetch_add(1);
        while (cur_serving_ticket_.load() != cur_ticket) {
            std::this_thread::yield();
        }
    }

    void Unlock() {
        cur_serving_ticket_.fetch_add(1);
    }

private:
    std::atomic<uint64_t> last_ticket_number_{0};
    std::atomic<uint64_t> cur_serving_ticket_{0};
};

