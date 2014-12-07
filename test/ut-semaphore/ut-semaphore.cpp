#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

#include "capo/semaphore.hpp"
#include "capo/spin_lock.hpp"
#include "capo/range.hpp"
#include "capo/singleton.hpp"
#include "capo/countof.hpp"
#include "capo/random.hpp"

#include <thread>
#include <mutex>
#include <iostream>

////////////////////////////////////////////////////////////////

capo::random<>  rdm(100, 2000);
bool            warehouse[10] = { false };
int             counter = 0;
capo::spin_lock warehouse_lock, random_lock;
capo::semaphore producer_sema(capo::countof(warehouse));
capo::semaphore consumer_sema;
bool            is_done = false;

void producer(unsigned id)
{
    char id_c = 'A' - 1 + id;

    // print a starting message
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "[producer " << id_c << "]\trunning..." << std::endl;
    }

    // loop until end is signaled
    while (!is_done)
    {
        producer_sema.wait();
        if (is_done) break;

        // simulate work
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // put the product into warehouse
        std::lock_guard<capo::spin_lock> lc_scope(warehouse_lock);
        {
            std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
            std::cout << "[producer " << id_c << "]\tputting product No." << counter << std::endl;
        }
        warehouse[counter++] = true;
        consumer_sema.post();
    }

    // print a ending message
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "[producer " << id_c << "]\tfinish..." << std::endl;
    }
}

void consumer(unsigned id)
{
    unsigned st = 0;

    // print a starting message
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "[consumer-" << id << "]\trunning..." << std::endl;
    }

    // loop until end is signaled
    while (!is_done)
    {
        consumer_sema.wait();
        if (is_done) break;

        // simulate work
        {
            std::lock_guard<capo::spin_lock> lc_scope(random_lock);
            st = rdm();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(st));

        // get a product from warehouse
        std::lock_guard<capo::spin_lock> lc_scope(warehouse_lock);
        warehouse[--counter] = false;
        {
            std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
            std::cout << "[consumer-" << id << "]\tget product No." << counter << std::endl;
        }
        producer_sema.post();
    }

    // print a ending message
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "[consumer-" << id << "]\tfinish..." << std::endl;
    }
}

#define TEST_METHOD(TEST_NAME) TEST(semaphore, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(semaphore)
{
    // start the producer threads
    std::thread producer_threads[2];
    for (auto i : capo::range(capo::countof(producer_threads)))
    {
        producer_threads[i] = std::thread(producer, i + 1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // start the consumer threads
    std::thread consumer_threads[10];
    for (auto i : capo::range(capo::countof(consumer_threads)))
    {
        consumer_threads[i] = std::thread(consumer, i + 1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // notify the producers and consumers to finish and wait for it
    {
        std::lock_guard<capo::spin_lock> lc_scope(capo::singleton<capo::spin_lock>());
        std::cout << "Works are over..." << std::endl;
    }
    is_done = true;
    producer_sema.post(capo::countof(producer_threads));
    for (auto& th : producer_threads) th.join();
    consumer_sema.post(capo::countof(consumer_threads));
    for (auto& th : consumer_threads) th.join();
}
