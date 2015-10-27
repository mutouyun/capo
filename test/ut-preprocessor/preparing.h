#pragma once

#include "capo/preprocessor.hpp"
#include "capo/func_decl.hpp"

namespace ut_preprocessor_ {
    
int foo(int a, int b)
{
    return a + b;
}

const char* bar_0(void)
{
    return __FUNCTION__;
}

const char* bar_3(int, int, int)
{
    return __FUNCTION__;
}

} // namespace ut_preprocessor_
