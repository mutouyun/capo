#pragma once

#include "capo/printf.hpp"
#include "capo/output.hpp"

#include <string>
#include <string.h>

namespace ut_printf_ {

std::string buf;
void out(std::string&& str)
{
    buf = std::move(str);
    std::cout << buf << std::endl;
}

class Foo
{
public:
    template <typename T>
    void operator()(capo::follower<T>&& out) const
    {
        out("Foo address: 0x{:08x}", 0x12345678/*(size_t)this*/);
    }
};

class Bar1
{
public:
    explicit operator char*(void) const
    {
        return (char*)"I'm Bar1...";
    }
};

class Bar2
{
public:
    operator const char*(void) const
    {
        return "I'm Bar2...";
    }
};

} // namespace ut_printf_
