#pragma once

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
