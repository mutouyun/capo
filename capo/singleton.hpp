/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/thread_local_ptr.hpp"
#include "capo/unused.hpp"

#include <atomic>   // std::atomic
#include <mutex>    // std::unique_lock, std::mutex
#include <utility>  // std::forward

namespace capo {
namespace use {

template <typename T, template <typename> class CreatorT>
struct single
{
public:
    template <typename... P>
    static T& instance(P&&... args)
    {
        static CreatorT<T> CAPO_UNUSED_ creator { std::forward<P>(args)... };
        return (*CreatorT<T>::get());
    }
};

////////////////////////////////////////////////////////////////
/// Global(thread-shared) singleton policy
////////////////////////////////////////////////////////////////

namespace detail_single_shared {

template <typename T>
struct single_creator
{
    static std::atomic<T*> InstPtr_;
    static std::mutex InstLock_;

    template <typename... P>
    single_creator(P&&... args)
    {
        // DCLP: https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
        if (InstPtr_.load(std::memory_order_acquire) == nullptr)
        {
            std::unique_lock<std::mutex> CAPO_UNUSED_ guard { InstLock_ };
            if (InstPtr_.load(std::memory_order_relaxed) == nullptr)
            {
                InstPtr_ .store(new T(std::forward<P>(args)...), std::memory_order_release);
            }
        }
    }

    ~single_creator(void)
    {
        delete InstPtr_.load(std::memory_order_relaxed);
    }

    static T* get(void)
    {
        return InstPtr_.load(std::memory_order_relaxed);
    }
};

template <typename T> std::atomic<T*> single_creator<T>::InstPtr_ { nullptr };
template <typename T> std::mutex single_creator<T>::InstLock_;

} // namespace detail_single_shared

template <typename T>
using single_shared = single<T, detail_single_shared::single_creator>;

////////////////////////////////////////////////////////////////
/// Thread-local singleton policy
////////////////////////////////////////////////////////////////

namespace detail_single_local {

template <typename T>
struct single_creator
{
    /*
        Did not use thread_local, cause thread_local's performance is unfavorable.
        See: https://gcc.gnu.org/gcc-4.8/changes.html#cxx
             https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55812

        And the construction on first usage/destruction on thread exit does not work properly.
        See: https://connect.microsoft.com/VisualStudio/Feedback/Details/955546
    */
    static thread_local_ptr<T> InstPtr_;

    template <typename... P>
    single_creator(P&&... args)
    {
        if (static_cast<T*>(InstPtr_) == nullptr)
        {
            InstPtr_ = new T(std::forward<P>(args)...);
        }
    }

    static T* get(void)
    {
        return static_cast<T*>(InstPtr_);
    }
};

template <typename T>
thread_local_ptr<T> single_creator<T>::InstPtr_;

} // namespace detail_single_local

template <typename T>
using single_local = single<T, detail_single_local::single_creator>;

} // namespace use

////////////////////////////////////////////////////////////////
/// Singleton pattern
////////////////////////////////////////////////////////////////

template <typename T,
          template <typename...> class PolicyT = capo::use::single_shared,
          typename... P>
T& singleton(P&&... args)
{
    return PolicyT<T>::instance(std::forward<P>(args)...);
}

} // namespace capo
