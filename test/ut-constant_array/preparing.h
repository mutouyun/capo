#pragma once

#include "capo/constant_array.hpp"
#include "capo/type_name.hpp"
#include "capo/countof.hpp"

#include <iostream>

namespace ut_constant_array_ {

template <int N>
struct make_bigger : std::integral_constant<int, (N + 1)> {};

} // namespace ut_constant_array_