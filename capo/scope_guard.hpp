/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/noncopyable.hpp"
#include "capo/unused.hpp"

#include <utility>      // std::forward, std::move
#include <algorithm>    // std::swap
#include <functional>   // std::function

namespace capo {

////////////////////////////////////////////////////////////////
/// Execute guard function when the enclosing scope exits
////////////////////////////////////////////////////////////////

template <typename F = std::function<void()>>
class scope_guard : capo::noncopyable
{
    F destructor_;
    mutable bool dismiss_;

public:
    template <typename F_>
    scope_guard(F_&& destructor)
        : destructor_(std::forward<F_>(destructor))
        , dismiss_(false)
    {}

    scope_guard(scope_guard&& rhs)
        : destructor_(std::move(rhs.destructor_))
        , dismiss_(true) // dismiss rhs
    {
        std::swap(dismiss_, rhs.dismiss_);
    }

    ~scope_guard(void)
    {
        if (!dismiss_) try { destructor_(); }
        /*
            In the realm of exceptions, it is fundamental that you can do nothing
            if your "undo/recover" action fails.
        */
        catch(...) { /* Do nothing */ }
    }

    void dismiss(void) const noexcept
    {
        dismiss_ = true;
    }

    void swap(scope_guard& rhs)
    {
        std::swap(destructor_, rhs.destructor_);
        std::swap(dismiss_,    rhs.dismiss_);
    }
};

#define CAPO_SCOPE_GUARD_V_(L)  CAPO_UNUSED_ scope_guard_##L##__
#define CAPO_SCOPE_GUARD_L_(L)  capo::scope_guard<> CAPO_SCOPE_GUARD_V_(L)

/*
    Do things like this:
    -->
    CAPO_SCOPE_GUARD_ = [ptr]
    {
        if (ptr) free(ptr);
    };
*/

#define CAPO_SCOPE_GUARD_ CAPO_SCOPE_GUARD_L_(__LINE__)

////////////////////////////////////////////////////////////////

} // namespace capo
