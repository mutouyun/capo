#pragma once

TEST_METHOD(any_type)
{
    capo::any any_int = 10;
    EXPECT_EQ(capo::any_cast<int>(any_int), 10);
    EXPECT_EQ(any_int.type().name(), typeid(int).name());

    capo::any any_double;
    EXPECT_EQ(any_double.empty(), true);
    any_double = 10.0;
    EXPECT_EQ(any_double.type().name(), typeid(double).name());
}

TEST_METHOD(any_cast)
{
    capo::any any_i = 123;
    EXPECT_THROW(capo::any_cast<double>(any_i), std::bad_cast);
    EXPECT_EQ(capo::any_cast<int>(any_i), 123);
    EXPECT_EQ(capo::any_cast<double>(&any_i), nullptr);
}
