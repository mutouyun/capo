#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

#include "capo/spin_lock.hpp"
#include "capo/range.hpp"
#include "capo/singleton.hpp"
#include "capo/countof.hpp"
#include "capo/stopwatch.hpp"

#include <thread>
#include <mutex>
#include <iostream>
#include <cstdint>

////////////////////////////////////////////////////////////////

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

#define TEST_METHOD(TEST_NAME) TEST(assert, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(ensure)
{
    auto sec_1 = do_test<capo::spin_lock>();
    EXPECT_EQ(499950000, g_counter);

    auto sec_2 = do_test<std::mutex>();
    EXPECT_EQ(499950000, g_counter);

    std::cout << "capo::spin_lock: " << sec_1 << " ms" << std::endl;
    std::cout << "std::mutex:      " << sec_2 << " ms" << std::endl;
    EXPECT_LT(sec_1, sec_2);
}
