#pragma once

#include "capo/type_list.hpp"
#include "capo/type_name.hpp"

#include <tuple>

namespace ut_type_list_ {

using namespace capo;
using types_t = types<short, int, unsigned char, long long, float&, const double, long*>;

template <typename T, typename U>
struct is_large : std::integral_constant<bool, (sizeof(T) > sizeof(U))> {};

} // namespace ut_type_list_
