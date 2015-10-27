#pragma once

#include "capo/construct.hpp"
#include "capo/printf.hpp"

#include <string>
#include <type_traits>

namespace ut_construct_ {

std::string g_str;

struct Foo
{
    std::string* str_ = &g_str;

    Foo(int x)
    {
        str_->clear();
        capo::printf([this](const std::string& str)
        {
            *str_ += str;
        }, "int %d", x);
    }
    Foo(void)       { *str_ = "default"; }
    Foo(const Foo&) { *str_ = "copy"; }
    Foo(Foo&&)      { *str_ = "move"; }
    ~Foo(void)      { if (str_) *str_ = "destruct"; }
};

std::aligned_storage<sizeof(Foo), alignof(Foo)>::type rr;
Foo* pfoo = reinterpret_cast<Foo*>(&rr);

using farr_t = Foo[3];
std::aligned_storage<sizeof(farr_t), alignof(farr_t)>::type ra;
farr_t* farr = reinterpret_cast<farr_t*>(&ra);

} // namespace ut_construct_
