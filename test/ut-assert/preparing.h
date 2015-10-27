#pragma once

#include "capo/assert.hpp"

namespace ut_assert_ {

bool b = true;
int i = 1, j = 2;
char what[] = "What's up";

void func_1(void)
{
    CAPO_ENSURE_(b = false)(i)(j)(what).msg("func_1 -> capo::assert::context\n").except();
}

void func_2(void)
{
    CAPO_ENSURE_(b = false)(i)(j)(what).msg("func_2 -> bool\n").except(b);
}

} // namespace ut_assert_
