/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/spin_lock.hpp"

#include <mutex>        // std::lock_guard
#include <utility>      // std::forward
#include <type_traits>  // std::true_type, std::false_type

namespace capo {

template <class TargetT, class MutexT = capo::spin_lock>
class thread_wrapper : public TargetT, public std::true_type
{
public:
    using mutex_type = MutexT;
    using target_type = TargetT;

private:
    mutex_type lc_;

public:
    using target_type::target_type;

#define CAPO_THREAD_WRAPPER_CALL__(...) \
    template <typename R, typename C, typename... P1, typename... P2> \
    R call(R(C::*mem_f)(P1...) __VA_ARGS__, P2&&...args) __VA_ARGS__ \
    { \
        std::lock_guard<MutexT> lc_scope(lc_); \
        return (this->*mem_f)(std::forward<P2>(args)...); \
    }

    CAPO_THREAD_WRAPPER_CALL__()
    CAPO_THREAD_WRAPPER_CALL__(const)
    CAPO_THREAD_WRAPPER_CALL__(volatile)
    CAPO_THREAD_WRAPPER_CALL__(const volatile)

#undef CAPO_THREAD_WRAPPER_CALL__
};

template <class TargetT, class MutexT = capo::spin_lock>
class non_lock_wrapper : public TargetT, public std::false_type
{
public:
    using mutex_type = MutexT;
    using target_type = TargetT;
    using target_type::target_type;
};

} // namespace capo
