/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_PATTERN_SINGLETON_HPP___
#define CAPO_PATTERN_SINGLETON_HPP___

#include "../thread/spin_lock.hpp"

#include <atomic>   // std::atomic
#include <mutex>    // std::lock_guard
#include <utility>  // std::forward

namespace capo {
namespace use {

////////////////////////////////////////////////////////////////
/// Global(thread-shared) singleton policy
////////////////////////////////////////////////////////////////

template <typename T>
struct single_shared
{
    static capo::spin_lock lc_;
    static std::atomic<T*> ip_;

private:
    static T* thread_shared_instance(void)
    {
        static T ir;
        return (&ir);
    }

    template <typename P1, typename... P>
    static T* thread_shared_instance(P1&& a1, P&&... args)
    {
        static T ir(std::forward<P1>(a1), std::forward<P>(args)...);
        return (&ir);
    }

public:
    template <typename... P>
    static T& instance(P&&... args)
    {
        T* tp = ip_.load(std::memory_order_acquire);
        if (!tp)
        {
            std::lock_guard<capo::spin_lock> lc_scope(lc_);
            tp = ip_.load(std::memory_order_relaxed);
            if (!tp)
            {
                tp = thread_shared_instance(std::forward<P>(args)...);
                ip_.store(tp, std::memory_order_release);
            }
        }
        return (*tp);
    }
};

template <typename T> capo::spin_lock single_shared<T>::lc_;
template <typename T> std::atomic<T*> single_shared<T>::ip_;

////////////////////////////////////////////////////////////////
/// Thread-local singleton policy
////////////////////////////////////////////////////////////////

template <typename T>
struct single_local
{
public:
    static T& instance(void)
    {
        thread_local T ir;
        return ir;
    }

    template <typename P1, typename... P>
    static T& instance(P1&& a1, P&&... args)
    {
        thread_local T ir(std::forward<P1>(a1), std::forward<P>(args)...);
        return ir;
    }
};

} // namespace use

////////////////////////////////////////////////////////////////
/// Singleton pattern
////////////////////////////////////////////////////////////////

template <typename T,
          template <typename...> class PolicyT = capo::use::single_shared,
          typename... P>
inline T& singleton(P&&... args)
{
    return PolicyT<T>::instance(std::forward<P>(args)...);
}

} // namespace capo

#endif // CAPO_PATTERN_SINGLETON_HPP___
