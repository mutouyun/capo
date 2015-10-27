#pragma once

#include "capo/range.hpp"
#include "capo/operator.hpp"

namespace ut_range_ {
    
class Foo : capo::inherit_chain<Foo, capo::unequal     , capo::comparable
                                   , capo::addable     , capo::subtractable
                                   , capo::multipliable, capo::dividable>
{
public:
    double x_;

    Foo(void)     : x_(0) {}
    Foo(double x) : x_(x) {}

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
