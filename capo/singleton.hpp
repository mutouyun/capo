/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/thread_local_ptr.hpp"
#include "capo/assert.hpp"

#include <utility>  // std::forward

namespace capo {
namespace use {

////////////////////////////////////////////////////////////////
/// Global(thread-shared) singleton policy
////////////////////////////////////////////////////////////////

template <typename T>
struct single_shared
{
public:
    /*
        <Remarks> ISO/IEC 14882-2011 (The C++11 Standard), §6.7.4:
        If control enters the declaration concurrently while the variable is being initialized,
        the concurrent execution shall wait for completion of the initialization.
    */
    static T& instance(void)
    {
        static T ir;
        return ir;
    }

    template <typename P1, typename... P>
    static T& instance(P1&& a1, P&&... args)
    {
        static T ir(std::forward<P1>(a1), std::forward<P>(args)...);
        return ir;
    }
};

////////////////////////////////////////////////////////////////
/// Thread-local singleton policy
////////////////////////////////////////////////////////////////

template <typename T>
struct single_local
{
public:
    template <typename... P>
    static T& instance(P&&... args)
    {
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
        return (*pi);
    }
};

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
