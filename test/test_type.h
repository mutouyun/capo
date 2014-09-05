#ifndef TEST_TYPE_H
#define TEST_TYPE_H

////////////////////////////////////////////////////////////////

#include "../include/type/type_name.hpp"

namespace np_test_type_name
{
    class Foo {};

    template <typename T>
    auto func(T&&) -> T;
}

void test_type_name(void)
{
    TEST_CASE();
    using namespace np_test_type_name;

    std::cout << capo::type_name<const volatile void *>()
              << std::endl;
    std::cout << capo::type_name<const volatile void *(&)[10]>()
              << std::endl;
    std::cout << capo::type_name<int [1][2][3]>()
              << std::endl;
    std::cout << capo::type_name<const int [3][10]>()
              << std::endl << std::endl;

    std::cout << capo::type_name<int(unsigned)>()
              << std::endl;
    std::cout << capo::type_name<int(*)(const int *(*)[3][10], Foo&&, int, unsigned)>()
              << std::endl;
    std::cout << capo::type_name<char(*(*const)(const int(&)[10]))[10]>()
              << std::endl << std::endl;

    std::cout << capo::type_name<int Foo::* const(&)[]>()
              << std::endl << std::endl;

    std::cout << capo::type_name<void(Foo::*)(void)>()
              << std::endl;
    std::cout << capo::type_name<void(Foo::*)(void) volatile const>()
              << std::endl;
    std::cout << capo::type_name<void(Foo::*(*)[])(int) const>()
              << std::endl;
    std::cout << capo::type_name<int (Foo::* const)(int, Foo&&, int) volatile>()
              << std::endl << std::endl;

    std::cout << capo::type_name<decltype(func<Foo>)>() << std::endl;
    std::cout << capo::type_name<decltype(func<Foo&>)>() << std::endl;
    std::cout << capo::type_name<decltype(func<Foo&&>)>() << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../include/type/type_list.hpp"
#include <tuple>

namespace np_test_type_list
{
    using namespace capo;
    using types_t = types<short, int, unsigned char, long long, float&, const double, long*>;

    template <typename T, typename U>
    struct is_large
         : std::integral_constant<bool, (sizeof(T) > sizeof(U))>
    {};
}

void test_types_at(void)
{
    TEST_CASE();
    using namespace np_test_type_list;

    std::cout << type_name<types_t>() << std::endl;
    {
        using t_t = types_front_t<types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_back_t<types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_at_t<types_t, 3>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_at_t<types_t, types_size<types_t>::value>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_at_t<int, 3>;
        //std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_link(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_link_t<void, types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_link_t<types_t, void>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_link_t<types_t, types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_link_t<std::tuple<void*, void**, void***>, types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_assign(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_assign_t<3, int>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_assign_t<2, types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_assign_t<0, types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_assign_t<0, std::tuple<void*>>;
        std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_insert(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_insert_t<types_t, 0, void>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_insert_t<types_t, types_size<types_t>::value, void>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_insert_t<types_t, 3, void>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_insert_t<types_t, 2, types<void, void*, void**>>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_insert_t<types_t, types_size<types_t>::value + 1, void>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_insert_t<int, 1, void>;
        //std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_erase(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_erase_t<types_t, 0>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_erase_t<types_t, 2, 3>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_erase_t<types_t, types_size<types_t>::value - 1>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_erase_t<types_t, -1, 0>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_erase_t<types_t, types_size<types_t>::value>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_erase_t<types<void>, 0, 3>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_erase_t<types<void>, 0, 0>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_erase_t<types<>, 0, 3>;
        //std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_erase_t<int, 0>;
        //std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_find(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    std::cout << types_find<types_t, long long>::value << std::endl;
    std::cout << types_find<types_t, void>::value << std::endl;
}

void test_types_replace(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_replace_t<types_t, int, void*&&>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_replace_t<types<int, long, int>, int, void>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_replace_t<types<int, long, int>, int, types<void, void>>;
        std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_remove(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_remove_t<types_link_t<types_t, types_t>, int>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_remove_t<types<int, int, int>, int>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_remove_t<types<void, void, void>, int>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_remove_t<types_t, types<int, long*, void>>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_remove_t<types_t, types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_remove_t<types<types<>>, types<>>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_remove_t<int, void>;
        //std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_compact(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_compact_t<types_link_t<types_t, types_t>>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_compact_t<types<int, int, void, void*, void>>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        //using t_t = types_compact_t<int>;
        //std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_reverse(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_reverse_t<types_t>;
        std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_select_if(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_select_if_t<types_t, is_large>;
        std::cout << type_name<t_t>() << std::endl;
    }
}

void test_types_sort_if(void)
{
    TEST_CASE();
    using namespace np_test_type_list;
    {
        using t_t = types_sort_if_t<types_t, is_large>;
        std::cout << type_name<t_t>() << std::endl;
    }
    {
        using t_t = types_sort_if_t<types<int>, is_large>;
        std::cout << type_name<t_t>() << std::endl;
    }
}

void test_type_list(void)
{
    test_types_at();
    test_types_link();
    test_types_assign();
    test_types_insert();
    test_types_erase();
    test_types_find();
    test_types_replace();
    test_types_remove();
    test_types_compact();
    test_types_reverse();
    test_types_select_if();
    test_types_sort_if();
}

////////////////////////////////////////////////////////////////

#include "../include/type/type_concept.hpp"

namespace np_test_type_concept
{
    class Foo
    {
    public:
        using foo_type = void;
    };

    CAPO_CONCEPT_INNER_TYPE(foo_type);

    class Bar
    {
    public:
        void bar_func(int) const;
    };

    CAPO_CONCEPT_MEMBER_FUNCTION(bar_func, void (C::*)(int) const);
}

void test_type_concept(void)
{
    TEST_CASE();
    using namespace np_test_type_concept;
    std::cout << "has_foo_type<Foo> ->: " << has_foo_type<Foo>::value << std::endl;
    std::cout << "has_foo_type<Bar> ->: " << has_foo_type<Bar>::value << std::endl;
    std::cout << "has_bar_func<Foo> ->: " << has_bar_func<Foo>::value << std::endl;
    std::cout << "has_bar_func<Bar> ->: " << has_bar_func<Bar>::value << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../include/type/constant_array.hpp"

namespace np_test_constant_array
{
    template <int N>
    struct make_bigger
         : std::integral_constant<int, (N + 1)>
    {};
}

void test_constant_array(void)
{
    TEST_CASE();
    using namespace np_test_constant_array;

    using constant_array = capo::constant_array<int>;
    using arr1 = constant_array::type<1, 2, 3, 4>;
    using arr2 = constant_array::assign<4, 0>;
    using arr3 = constant_array::insert<arr2, 0, -1>;
    using arr4 = constant_array::foreach<arr3, make_bigger>;
    using arr5 = constant_array::replace<arr4, 1, 2>;
    using arr6 = constant_array::remove<arr5, 0>;
    using arr7 = constant_array::compact<arr6>;
    using arr8 = constant_array::link<arr1, arr7>;
    using arr9 = constant_array::reverse<arr8>;
    auto& xx = constant_array::sort_less<arr9>::value;
    for (auto i : xx)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../include/type/types_to_seq.hpp"

void test_types_to_seq(void)
{
    TEST_CASE();

    std::cout << capo::type_name<capo::types_to_seq<int, long, double>>() << std::endl;
}

////////////////////////////////////////////////////////////////

void test_type(void)
{
    //test_type_name();
    //test_type_list();
    //test_type_concept();
    //test_constant_array();
    test_types_to_seq();
}

////////////////////////////////////////////////////////////////

#endif // TEST_TYPE_H
