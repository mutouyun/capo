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

// names for waiter status
enum class waiter_status
{
    Resting,
    Arrived,
    Excited
};

////////////////////////////////////////////////////////////////
/// Wait until notification arrives
////////////////////////////////////////////////////////////////

class waiter
{
    mutable std::mutex      lock_;
    std::condition_variable cond_;
    waiter_status signaled_ = waiter_status::Resting;

public:
    waiter_status status(void) const
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        return signaled_;
    }

    bool is_signaled(void) const
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        return (signaled_ != waiter_status::Resting);
    }

    void reset(void)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        signaled_ = waiter_status::Resting;
    }

public:
    void wait(void)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        cond_.wait(lc, [this]{ return (signaled_ != waiter_status::Resting); });
        if (signaled_ == waiter_status::Arrived)
            signaled_ = waiter_status::Resting;
    }

    template <typename Rep, typename Period>
    std::cv_status wait_until(const std::chrono::duration<Rep, Period>& abs_time)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        while (signaled_ == waiter_status::Resting)
        {
            if (cond_.wait_until(lc, abs_time) == std::cv_status::timeout)
                return std::cv_status::timeout;
        }
        if (signaled_ == waiter_status::Arrived)
            signaled_ = waiter_status::Resting;
        return std::cv_status::no_timeout;
    }

    template <typename Rep, typename Period>
    std::cv_status wait_for(const std::chrono::duration<Rep, Period>& rel_time)
    {
        return wait_until(std::chrono::steady_clock::now() + rel_time);
    }

    void notify_one(void)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        signaled_ = waiter_status::Arrived;
        cond_.notify_all();
    }

    void notify_all(void)
    {
        auto lc = capo::make<std::unique_lock>(lock_);
        signaled_ = waiter_status::Excited;
        cond_.notify_all();
    }
};

} // namespace capo
