#pragma once

#include "capo/spin_lock.hpp"
#include "capo/range.hpp"
#include "capo/singleton.hpp"
#include "capo/countof.hpp"
#include "capo/stopwatch.hpp"

#include <thread>
#include <mutex>
#include <iostream>
#include <cstdint>

namespace ut_spin_lock_ {

uint64_t g_counter;

template <class Lock>
void thread_proc(void)
{
    for (auto i : capo::range(10000))
    {
        std::lock_guard<Lock> lc_scope(capo::singleton<Lock>());
        uint64_t n = g_counter;
        g_counter = n + i;
    }
}

template <class Lock>
auto do_test(void)
{
    g_counter = 0;
    std::thread threads[10];
    capo::stopwatch<> sw(true);
    for (auto i : capo::range(capo::countof(threads)))
    {
        threads[i] = std::thread(thread_proc<Lock>);
    }
    for (auto& th : threads) th.join();
    return sw.elapsed<std::chrono::milliseconds>();
}

} // namespace ut_spin_lock_
