/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/make.hpp"

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
        auto lc = capo::make<std::unique_lock>(lock_);
        return counter_;
    }

    void wait(void)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        cond_.wait(lc, [this]{ return (counter_ > 0); });
        -- counter_;
    }

    template <typename Rep, typename Period>
    std::cv_status wait_until(const std::chrono::duration<Rep, Period>& abs_time)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        while (counter_ <= 0)
        {
            if (cond_.wait_until(lc, abs_time) == std::cv_status::timeout)
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
        auto lc = capo::make<std::unique_lock>(lock_);
        counter_ += count;
        cond_.notify_all();
    }
};

} // namespace capo
