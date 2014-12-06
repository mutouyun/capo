
#include "gtest/gtest.h"

#include "capo/type_name.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////

class Foo {};

template <typename T>
auto func(T&&)->T;

#define TEST_METHOD(TEST_NAME) TEST(type_name, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(cv_qualified_compound_types)
{
    std::cout << capo::type_name<const volatile void *>() << std::endl;
    std::cout << capo::type_name<const volatile void *(&)[10]>() << std::endl;
    std::cout << capo::type_name<int[1][2][3]>() << std::endl;
    std::cout << capo::type_name<const int[3][10]>() << std::endl;
    std::cout << capo::type_name<int(unsigned)>() << std::endl;
}

TEST_METHOD(function)
{
    std::cout << capo::type_name<int(*)(const int *(*)[3][10], Foo&&, int, unsigned)>() << std::endl;
    std::cout << capo::type_name<char(*(*const)(const int(&)[10]))[10]>() << std::endl;
}

TEST_METHOD(type_with_class)
{
    std::cout << capo::type_name<int Foo::* const(&)[]>() << std::endl;
    std::cout << capo::type_name<void(Foo::*)(void)>() << std::endl;
    std::cout << capo::type_name<void(Foo::*)(void) volatile const>() << std::endl;
    std::cout << capo::type_name<void(Foo::*(*)[])(int) const>() << std::endl;
    std::cout << capo::type_name<int (Foo::* const)(int, Foo&&, int) volatile>() << std::endl;
}

TEST_METHOD(reference)
{
    std::cout << capo::type_name<decltype(func<Foo>)>() << std::endl;
    std::cout << capo::type_name<decltype(func<Foo&>)>() << std::endl;
    std::cout << capo::type_name<decltype(func<Foo&&>)>() << std::endl;
}