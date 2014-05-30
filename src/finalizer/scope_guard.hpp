/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_FINALIZER_SCOPE_GUARD_HPP___
#define CAPO_FINALIZER_SCOPE_GUARD_HPP___

#include "../utility/noncopyable.hpp"
#include "../macro/macro_unused.hpp"

#include <utility>      // std::forward
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

    void dismiss() const noexcept
    {
        dismiss_ = true;
    }

    void swap(scope_guard& rhs)
    {
        std::swap(destructor_, rhs.destructor_);
        std::swap(dismiss_,    rhs.dismiss_);
    }
};

#define CAPO_SCOPE_GUARD_VAR__(L) CAPO_UNUSED_ scope_guard_##L##___
#define CAPO_SCOPE_GUARD__(L)     scope_guard<> CAPO_SCOPE_GUARD_VAR__(L)

/*
    Do things like this:
    -->
    capo_scope_exit = [ptr]
    {
        if (ptr) free(ptr);
    };
*/

#define capo_scope_exit CAPO_SCOPE_GUARD__(__LINE__)

////////////////////////////////////////////////////////////////

} // namespace capo

#endif // CAPO_FINALIZER_SCOPE_GUARD_HPP___
