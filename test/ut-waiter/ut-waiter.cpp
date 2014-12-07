#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

#include "capo/waiter.hpp"
#include "capo/spin_lock.hpp"
#include "capo/range.hpp"
#include "capo/singleton.hpp"
#include "capo/countof.hpp"

#include <thread>
#include <mutex>
#include <iostream>

////////////////////////////////////////////////////////////////

capo::waiter consumer_waiter;

void test_proc(unsigned id)
{
    // print a starting message
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "[tester " << id << "]\tis waiting..." << std::endl;
    }

    consumer_waiter.wait();

    // print a ending message
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "[tester " << id << "]\tis ending..." << std::endl;
    }
}

#define TEST_METHOD(TEST_NAME) TEST(waiter, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(waiter)
{
    // start the test threads
    std::thread threads[10];
    for (auto i : capo::range(capo::countof(threads)))
    {
        threads[i] = std::thread(test_proc, i + 1);
    }

    // test for notify
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::endl << "test for notify" << std::endl;
    for (int i = 0; i < 3; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        consumer_waiter.notify_one();
    }

    // test for broadcast
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::endl << "test for broadcast" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    consumer_waiter.notify_all();

    // wait for finish
    for (auto& th : threads) th.join();
}
