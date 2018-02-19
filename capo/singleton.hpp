/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/thread_local_ptr.hpp"
#include "capo/scope_guard.hpp"
#include "capo/assert.hpp"
#include "capo/unused.hpp"

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
        /*
            <Remarks> ISO/IEC 14882-2011 (The C++11 Standard), §6.7.4:
            If control enters the declaration concurrently while the variable is being initialized,
            the concurrent execution shall wait for completion of the initialization.
        */
        static CreatorT<T> CAPO_UNUSED_ creator { std::forward<P>(args)... };
        return (*CreatorT<T>::InstPtr_);
    }
};

////////////////////////////////////////////////////////////////
/// Global(thread-shared) singleton policy
////////////////////////////////////////////////////////////////

namespace detail_single_shared {

template <typename T>
struct single_creator
{
    static T* InstPtr_;

    template <typename... P>
    single_creator(P&&... args)
    {
        if (InstPtr_ != nullptr) return;
        InstPtr_ = new T(std::forward<P>(args)...);
        static CAPO_SCOPE_GUARD_ = []{ delete InstPtr_; };
    }
};

template <typename T>
T* single_creator<T>::InstPtr_ = nullptr;

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
    static T* InstPtr_;

    template <typename... P>
    single_creator(P&&... args)
    {
        if (InstPtr_ != nullptr) return;
        /*
            Did not use thread_local, cause thread_local's performance is unfavorable.
            See: https://gcc.gnu.org/gcc-4.8/changes.html#cxx
                 https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55812

            And the construction on first usage/destruction on thread exit does not work properly.
            See: https://connect.microsoft.com/VisualStudio/Feedback/Details/955546
        */
        static thread_local_ptr<T> tls_ptr;
        T* pi = tls_ptr;
        if (pi == nullptr) pi = tls_ptr = new T(std::forward<P>(args)...);
        CAPO_ASSERT_(pi != nullptr);
        InstPtr_ = pi;
    }
};

template <typename T>
T* single_creator<T>::InstPtr_ = nullptr;

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
