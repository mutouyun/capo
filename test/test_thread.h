#ifndef TEST_THREAD_H
#define TEST_THREAD_H

////////////////////////////////////////////////////////////////

#include "../src/utility/countof.hpp"
#include "../src/thread/spin_lock.hpp"
#include "../src/algorithm/range.hpp"
#include "../src/pattern/singleton.hpp"
#include <thread>
#include <mutex>

namespace np_test_spin_lock
{
    using namespace capo;

    void thread_proc(int id)
    {
        for (auto i : range(10))
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
            std::cout << "thread #" << id << " - " << i << std::endl;
        }
    }
}

void test_spin_lock(void)
{
    TEST_CASE();
    using namespace np_test_spin_lock;

    std::thread threads[10];
    for (auto i : range(countof(threads)))
    {
        threads[i] = std::thread(thread_proc, i + 1);
    }
    for (auto& th : threads) th.join();
}

////////////////////////////////////////////////////////////////

#include "../src/thread/semaphore.hpp"
#include "../src/random/random.hpp"
#include <atomic>

namespace np_test_semaphore
{
    using namespace capo;

    random<>  rdm(100, 2000);
    bool      warehouse[10] = { false };
    int       counter = 0;
    spin_lock warehouse_lock, random_lock;
    semaphore producer_sema(countof(warehouse));
    semaphore consumer_sema;
    bool      is_done = false;

    void producer(unsigned id)
    {
        char id_c = 'A' - 1 + id;

        // print a starting message
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
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
            std::lock_guard<spin_lock> lc_scope(warehouse_lock);
            {
                std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
                std::cout << "[producer " << id_c << "]\tputting product No." << counter << std::endl;
            }
            warehouse[counter++] = true;
            consumer_sema.post();
        }

        // print a ending message
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
            std::cout << "[producer " << id_c << "]\tfinish..." << std::endl;
        }
    }

    void consumer(unsigned id)
    {
        unsigned st = 0;

        // print a starting message
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
            std::cout << "[consumer-" << id << "]\trunning..." << std::endl;
        }

        // loop until end is signaled
        while (!is_done)
        {
            consumer_sema.wait();
            if (is_done) break;

            // simulate work
            {
                std::lock_guard<spin_lock> lc_scope(random_lock);
                st = rdm();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(st));

            // get a product from warehouse
            std::lock_guard<spin_lock> lc_scope(warehouse_lock);
            warehouse[--counter] = false;
            {
                std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
                std::cout << "[consumer-" << id << "]\tget product No." << counter << std::endl;
            }
            producer_sema.post();
        }

        // print a ending message
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
            std::cout << "[consumer-" << id << "]\tfinish..." << std::endl;
        }
    }
}

void test_semaphore(void)
{
    TEST_CASE();
    using namespace np_test_semaphore;

    // start the producer threads
    std::thread producer_threads[2];
    for (auto i : range(countof(producer_threads)))
    {
        producer_threads[i] = std::thread(producer, i + 1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // start the consumer threads
    std::thread consumer_threads[10];
    for (auto i : range(countof(consumer_threads)))
    {
        consumer_threads[i] = std::thread(consumer, i + 1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // notify the producers and consumers to finish and wait for it
    {
        std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
        std::cout << "Works are over..." << std::endl;
    }
    is_done = true;
    producer_sema.post(countof(producer_threads));
    for (auto& th : producer_threads) th.join();
    consumer_sema.post(countof(consumer_threads));
    for (auto& th : consumer_threads) th.join();
}

////////////////////////////////////////////////////////////////

#include "../src/thread/waiter.hpp"

namespace np_test_waiter
{
    using namespace capo;

    waiter consumer_waiter;

    void test_proc(unsigned id)
    {
        // print a starting message
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
            std::cout << "[tester " << id << "]\tis waiting..." << std::endl;
        }

        consumer_waiter.wait();

        // print a ending message
        {
            std::lock_guard<spin_lock> lc_scope(singleton<spin_lock>());
            std::cout << "[tester " << id << "]\tis ending..." << std::endl;
        }
    }
}

void test_waiter(void)
{
    TEST_CASE();
    using namespace np_test_waiter;

    // start the test threads
    std::thread threads[10];
    for (auto i : range(countof(threads)))
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

////////////////////////////////////////////////////////////////

void test_thread(void)
{
    test_spin_lock();
    test_semaphore();
    test_waiter();
}

////////////////////////////////////////////////////////////////

#endif // TEST_THREAD_H
