
#include <tuple>

#include "gtest/gtest.h"

#include "capo/type_list.hpp"
#include "capo/type_name.hpp"

////////////////////////////////////////////////////////////////

using namespace capo;
using types_t = types<short, int, unsigned char, long long, float&, const double, long*>;

template <typename T, typename U>
struct is_large : std::integral_constant<bool, (sizeof(T) > sizeof(U))> {};

#define TEST_METHOD(TEST_NAME) TEST(UT_type_list, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(types_at)
{
    {
        using t_t = types_front_t<types_t>;
        EXPECT_EQ(type_name<short>(), type_name<t_t>());
    }
    {
        using t_t = types_back_t<types_t>;
        EXPECT_EQ(type_name<long*>(), type_name<t_t>());
    }
    {
        using t_t = types_at_t<types_t, 3>;
        EXPECT_EQ(type_name<long long>(), type_name<t_t>());
    }
    {
        //using t_t = types_at_t<types_t, types_size<types_t>::value>;
    }
    {
        //using t_t = types_at_t<int, 3>;
    }
}

TEST_METHOD(types_link)
{
    {
        using t_t = types_link_t<void, types_t>;
        EXPECT_EQ((type_name<types<void, short, int, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_link_t<types_t, void>;
        EXPECT_EQ((type_name<types<short, int, unsigned char, long long, float&, const double, long*, void>>()), type_name<t_t>());
    }
    {
        using t_t = types_link_t<types_t, types_t>;
        EXPECT_EQ((type_name<types<short, int, unsigned char, long long, float&, const double, long*, 
                                   short, int, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_link_t<std::tuple<void*, void**, void***>, types_t>;
        EXPECT_EQ((type_name<std::tuple<void*, void**, void***, short, int, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
}

TEST_METHOD(types_assign)
{
    {
        using t_t = types_assign_t<3, int>;
        EXPECT_EQ((type_name<types<int, int, int>>()), type_name<t_t>());
    }
    {
        using t_t = types_assign_t<3, types_t>;
        EXPECT_EQ((type_name<types<short, int, unsigned char, long long, float&, const double, long*, 
                                   short, int, unsigned char, long long, float&, const double, long*, 
                                   short, int, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_assign_t<0, types_t>;
        EXPECT_EQ((type_name<types<>>()), type_name<t_t>());
    }
    {
        using t_t = types_assign_t<0, std::tuple<void*>>;
        EXPECT_EQ((type_name<std::tuple<>>()), type_name<t_t>());
    }
}

TEST_METHOD(types_insert)
{
    {
        using t_t = types_insert_t<types_t, 0, void>;
        EXPECT_EQ((type_name<types<void, short, int, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_insert_t<types_t, types_size<types_t>::value, void>;
        EXPECT_EQ((type_name<types<short, int, unsigned char, long long, float&, const double, long*, void>>()), type_name<t_t>());
    }
    {
        using t_t = types_insert_t<types_t, 3, void>;
        EXPECT_EQ((type_name<types<short, int, unsigned char, void, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_insert_t<types_t, 2, types<void, void*, void**>>;
        EXPECT_EQ((type_name<types<short, int, void, void*, void**, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        //using t_t = types_insert_t<types_t, types_size<types_t>::value + 1, void>;
    }
    {
        //using t_t = types_insert_t<int, 1, void>;
    }
}

TEST_METHOD(types_erase)
{
    {
        using t_t = types_erase_t<types_t, 0>;
        EXPECT_EQ((type_name<types<int, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_erase_t<types_t, 2, 3>;
        EXPECT_EQ((type_name<types<short, int, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_erase_t<types_t, types_size<types_t>::value - 1>;
        EXPECT_EQ((type_name<types<short, int, unsigned char, long long, float&, const double>>()), type_name<t_t>());
    }
    {
        //using t_t = types_erase_t<types_t, -1, 0>;
    }
    {
        //using t_t = types_erase_t<types_t, types_size<types_t>::value>;
    }
    {
        //using t_t = types_erase_t<types<void>, 0, 3>;
    }
    {
        //using t_t = types_erase_t<types<void>, 0, 0>;
    }
    {
        //using t_t = types_erase_t<types<>, 0, 3>;
    }
    {
        //using t_t = types_erase_t<int, 0>;
    }
}

TEST_METHOD(types_find)
{
    EXPECT_EQ(3 , (types_find<types_t, long long>::value));
    EXPECT_EQ(-1, (types_find<types_t, void>::value));
}

TEST_METHOD(types_replace)
{
    {
        using t_t = types_replace_t<types_t, int, void*&&>;
        EXPECT_EQ((type_name<types<short, void*&&, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_replace_t<types<int, long, int>, int, void>;
        EXPECT_EQ((type_name<types<void, long, void>>()), type_name<t_t>());
    }
    {
        using t_t = types_replace_t<types<int, long, int>, int, types<void, void>>;
        EXPECT_EQ((type_name<types<void, void, long, void, void>>()), type_name<t_t>());
    }
}

TEST_METHOD(types_remove)
{
#if !defined(_MSC_VER)
    /*
        <MSVC 2013> **STRANGE**
        These tests will make TEST_METHOD(types_compact) FAIL.
    */
    {
        using t_t = types_remove_t<types_link_t<types_t, types_t>, int>;
        EXPECT_EQ((type_name<types<short, unsigned char, long long, float&, const double, long*, 
                                   short, unsigned char, long long, float&, const double, long*>>()), type_name<t_t>());
    }
    {
        using t_t = types_remove_t<types_t, types<int, long*, void>>;
        EXPECT_EQ((type_name<types<short, unsigned char, long long, float&, const double>>()), type_name<t_t>());
    }
#endif
    {
        using t_t = types_remove_t<types<int, int, int>, int>;
        EXPECT_EQ((type_name<types<>>()), type_name<t_t>());
    }
    {
        using t_t = types_remove_t<types<void, void, void>, int>;
        EXPECT_EQ((type_name<types<void, void, void>>()), type_name<t_t>());
    }
    {
        using t_t = types_remove_t<types_t, types_t>;
        EXPECT_EQ((type_name<types<>>()), type_name<t_t>());
    }
    {
        using t_t = types_remove_t<types<types<>>, types<>>;
        EXPECT_EQ((type_name<types<>>()), type_name<t_t>());
    }
    {
        //using t_t = types_remove_t<int, void>;
    }
}

TEST_METHOD(types_compact)
{
    {
        using t_t = types_compact_t<types_link_t<types_t, types_t>>;
        EXPECT_EQ((type_name<types_t>()), type_name<t_t>());
    }
    {
        using t_t = types_compact_t<types<int, int, void, void*, void>>;
        EXPECT_EQ((type_name<types<int, void, void*>>()), type_name<t_t>());
    }
    {
        using t_t = types_compact_t<types<int>>;
        EXPECT_EQ((type_name<types<int>>()), type_name<t_t>());
    }
    {
        //using t_t = types_compact_t<int>;
    }
}

TEST_METHOD(types_reverse)
{
    using t_t = types_reverse_t<types_t>;
    EXPECT_EQ((type_name<types<long*, const double, float&, long long, unsigned char, int, short>>()), type_name<t_t>());
}

TEST_METHOD(types_select_if)
{
    using t_t = types_select_if_t<types_t, is_large>;
    EXPECT_EQ(type_name<const double>(), type_name<t_t>());
}

TEST_METHOD(types_sort_if)
{
    {
        using t_t = types_sort_if_t<types_t, is_large>;
        EXPECT_EQ((type_name<types<const double, long long, long*, float&, int, short, unsigned char>>()), type_name<t_t>());
    }
    {
        using t_t = types_sort_if_t<types<int>, is_large>;
        EXPECT_EQ((type_name<types<int>>()), type_name<t_t>());
    }
}