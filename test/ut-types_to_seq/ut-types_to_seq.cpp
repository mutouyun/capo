
#include "gtest/gtest.h"

#include "capo/type_name.hpp"
#include "capo/type_list.hpp"
#include "capo/types_to_seq.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////

#define TEST_METHOD(TEST_NAME) TEST(types_to_seq, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(types_to_seq)
{
    EXPECT_EQ((capo::type_name<capo::constant_seq<0, 1, 2>>()), 
              (capo::type_name<capo::types_to_seq<int, long, double>>()));
}

TEST_METHOD(list_to_seq)
{
    EXPECT_EQ((capo::type_name<capo::constant_seq<0, 1, 2>>()), 
              (capo::type_name<capo::list_to_seq<capo::types<int, long, double>>>()));
}
