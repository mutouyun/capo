/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_THREAD_SEMAPHORE_HPP___
#define CAPO_THREAD_SEMAPHORE_HPP___

#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>   // std::condition_variable
#include <chrono>               // std::chrono

namespace capo {

////////////////////////////////////////////////////////////////
/// Semaphore
////////////////////////////////////////////////////////////////

class semaphore
{
    mutable std::mutex      lock_;
    std::condition_variable cond_;
    long counter_;

public:
    semaphore(long init_count = 0)
        : counter_(init_count)
    {}

public:
    long count(void) const
    {
        std::unique_lock<std::mutex> lc_scope(lock_);
        return counter_;
    }

    void wait(void)
    {
        std::unique_lock<std::mutex> lc_scope(lock_);
        cond_.wait(lc_scope, [this]{ return (counter_ > 0); });
        -- counter_;
    }

    template <typename Rep, typename Period>
    std::cv_status wait_until(const std::chrono::duration<Rep, Period>& abs_time)
    {
        std::unique_lock<std::mutex> lc_scope(lock_);
        while (counter_ <= 0)
        {
            if (cond_.wait_until(lc_scope, abs_time) == std::cv_status::timeout)
                return std::cv_status::timeout;
        }
        -- counter_;
        return std::cv_status::no_timeout;
    }

    template <typename Rep, typename Period>
    std::cv_status wait_for(const std::chrono::duration<Rep, Period>& rel_time)
    {
        return wait_until(std::chrono::steady_clock::now() + rel_time);
    }

    void post(long count = 1)
    {
        std::unique_lock<std::mutex> lc_scope(lock_);
        counter_ += count;
        cond_.notify_all();
    }
};

} // namespace capo

#endif // CAPO_THREAD_SEMAPHORE_HPP___
