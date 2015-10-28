#pragma once

#include "capo/range.hpp"
#include "capo/operator.hpp"
#include "capo/type_traits.hpp"
#include "capo/concept.hpp"

namespace ut_range_ {
    
class Foo : capo::inherit_chain<Foo, capo::unequal     , capo::comparable
                                   , capo::addable     , capo::subtractable
                                   , capo::multipliable, capo::dividable>
{
public:
    double x_ = 0;

    Foo(void) = default;

#if defined(__clang__)
    Foo(double x) : x_(x) {}
#else/*!__clang__*/
    template <typename T, CAPO_REQUIRE_(std::is_arithmetic<T>::value)>
    Foo(T x) : x_(static_cast<double>(x)) {}
#endif/*__clang__*/

    explicit operator double(void) const { return x_; }
    explicit operator size_t(void) const { return static_cast<size_t>(x_); }

    bool operator==(const Foo& y) const { return (x_ == y.x_); }
    bool operator <(const Foo& y) const { return (x_  < y.x_); }

    Foo& operator+=(const Foo& y) { x_ += y.x_; return (*this); }
    Foo& operator-=(const Foo& y) { x_ -= y.x_; return (*this); }
    Foo& operator*=(const Foo& y) { x_ *= y.x_; return (*this); }
    Foo& operator/=(const Foo& y) { x_ /= y.x_; return (*this); }
};

} // namespace ut_range_
