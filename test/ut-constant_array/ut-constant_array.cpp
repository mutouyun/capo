
#include <iostream>

#include "gtest/gtest.h"

#include "capo/constant_array.hpp"
#include "capo/type_name.hpp"
#include "capo/countof.hpp"

////////////////////////////////////////////////////////////////

template <int N>
struct make_bigger : std::integral_constant<int, (N + 1)> {};

#define TEST_METHOD(TEST_NAME) TEST(constant_array, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(constant_array)
{
    using constant_array = capo::constant_array<int>;

    using arr1 = constant_array::type<1, 2, 3, 4>;
    std::cout << capo::type_name<arr1>() << std::endl;

    using arr2 = constant_array::assign<4, 0>;
    EXPECT_EQ((capo::type_name<constant_array::type<0, 0, 0, 0>>()), capo::type_name<arr2>());

    using arr3 = constant_array::insert<arr2, 0, -1>;
    EXPECT_EQ((capo::type_name<constant_array::type<-1, 0, 0, 0, 0>>()), capo::type_name<arr3>());

    using arr4 = constant_array::foreach<arr3, make_bigger>;
    EXPECT_EQ((capo::type_name<constant_array::type<0, 1, 1, 1, 1>>()), capo::type_name<arr4>());

    using arr5 = constant_array::replace<arr4, 1, 2>;
    EXPECT_EQ((capo::type_name<constant_array::type<0, 2, 2, 2, 2>>()), capo::type_name<arr5>());

    using arr6 = constant_array::remove<arr5, 0>;
    EXPECT_EQ((capo::type_name<constant_array::type<2, 2, 2, 2>>()), capo::type_name<arr6>());

    using arr7 = constant_array::compact<arr6>;
    EXPECT_EQ((capo::type_name<constant_array::type<2>>()), capo::type_name<arr7>());

    using arr8 = constant_array::link<arr1, arr7>;
    EXPECT_EQ((capo::type_name<constant_array::type<1, 2, 3, 4, 2>>()), capo::type_name<arr8>());

    using arr9 = constant_array::reverse<arr8>;
    EXPECT_EQ((capo::type_name<constant_array::type<2, 4, 3, 2, 1>>()), capo::type_name<arr9>());

    auto& xx = constant_array::type<1, 2, 2, 3, 4>::value;
    auto& yy = constant_array::sort_less<arr9>::value;
    EXPECT_EQ(capo::countof(xx), capo::countof(yy));
    for (size_t i = 0; i < capo::countof(xx); ++i)
    {
        EXPECT_EQ(xx[i], yy[i]) << "At index: " << i;
    }
}
