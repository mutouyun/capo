#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

////////////////////////////////////////////////////////////////

#include "../src/utility/countof.hpp"

void test_countof(void)
{
    TEST_CASE();

    void* p[100];
    int arr[capo::countof(p)];
    std::cout << capo::countof(arr) << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../src/utility/operator.hpp"

namespace np_test_operator
{
    class Foo : capo::inherit_chain<Foo, capo::addable
                                       , capo::subtractable
                                       , capo::multipliable
                                       , capo::dividable>
    {
    public:
        Foo(void) {}
        Foo(int)  {}

        Foo& operator+=(int)        { return (*this); }
        Foo& operator+=(const Foo&) { return (*this); }
        Foo& operator-=(int)        { return (*this); }

        Foo& operator*=(const Foo&) { return (*this); }
        Foo& operator/=(const Foo&) { return (*this); }
    };
}

void test_operator(void)
{
    TEST_CASE();
    using namespace np_test_operator;

    Foo foo;
    foo = foo + foo + 1 - 2 + foo + 3 / foo * foo;
}

////////////////////////////////////////////////////////////////

void test_utility(void)
{
    test_countof();
    test_operator();
}

////////////////////////////////////////////////////////////////

#endif // TEST_UTILITY_H
