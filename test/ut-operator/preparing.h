#pragma once

#include "capo/operator.hpp"

namespace ut_operator_ {

struct Foo : capo::inherit_chain<Foo, capo::addable
                                    , capo::subtractable
                                    , capo::multipliable
                                    , capo::dividable>
{
    int x_;

    Foo(void)  : x_(0) {}
    Foo(int x) : x_(x) {}

    Foo& operator+=(const Foo& rhs) { x_ += rhs.x_; return (*this); }
    Foo& operator-=(const Foo& rhs) { x_ -= rhs.x_; return (*this); }

    Foo& operator*=(const Foo& rhs) { x_ *= rhs.x_; return (*this); }
    Foo& operator/=(const Foo& rhs) { x_ /= rhs.x_; return (*this); }
};

} // namespace ut_operator_
