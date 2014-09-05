#ifndef TEST_ALGORITHM_H
#define TEST_ALGORITHM_H

////////////////////////////////////////////////////////////////

#include "../include/algorithm/max_min.hpp"

void test_max_min(void)
{
    TEST_CASE();

    std::cout << capo::max(1, 3, 2) << std::endl;
    std::cout << capo::min(1, 3, 2) << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../include/algorithm/sequence.hpp"

void test_fibonacci(void)
{
    TEST_CASE();

    std::cout << "fibonacci ->:" << std::endl;
    for (size_t i = 0; i < 20; ++i)
    {
        unsigned long long x = 0, y = 1;
        capo::use::fibonacci::at(x, y, i);
        std::cout << i << ": " << x << std::endl;
    }
}

void test_sequence(void)
{
    TEST_CASE();

    test_fibonacci();

    auto xx = capo::sequence<capo::use::arithmetic<2>, int>(1, 21, 1);
    decltype(xx)::iterator it = xx.begin();

    std::cout << "arithmetic<2>(1, 21, 1) ->:" << std::endl;
    for (auto i : xx)
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../include/algorithm/range.hpp"
#include "../include/utility/operator.hpp"

namespace np_test_range
{
    using namespace capo;

    class Foo : inherit_chain<Foo, unequal, comparable
                                 , addable, subtractable, multipliable, dividable>
    {
    public:
        double x_;

        Foo(void)     : x_(0) {}
        Foo(double x) : x_(x) {}

        explicit operator size_t(void) const { return static_cast<size_t>(x_); }

        bool operator==(const Foo& y) const { return (x_ == y.x_); }
        bool operator <(const Foo& y) const { return (x_  < y.x_); }

        Foo& operator+=(const Foo& y) { x_ += y.x_; return (*this); }
        Foo& operator-=(const Foo& y) { x_ -= y.x_; return (*this); }
        Foo& operator*=(const Foo& y) { x_ *= y.x_; return (*this); }
        Foo& operator/=(const Foo& y) { x_ /= y.x_; return (*this); }
    };
}

void test_range(void)
{
    TEST_CASE();
    using namespace np_test_range;

    std::cout << "range(Foo(15), Foo(20.7), 0.8):";
    for (auto i : range(Foo(15), Foo(20.7), 0.8))
    {
        std::cout << " " << i.x_;
    }
    std::cout << std::endl;

    auto r = range(1, 2, 3);
    int a = 15;
    r = range(a);
    std::cout << "range(15):";
    for (int i : r)
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range(2, 6):";
    for (auto i : range(2, 6))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    const int x = 2, y = 6, z = 3;
    std::cout << "range(2, 6, 3):";
    for (auto i : range(x, y, z))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range(-2, -6, -3):";
    for (auto i : range(-2, -6, -3))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range(10.5, 15.5):";
    for (auto i : range(10.5, 15.5))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range(35, 27, -1):";
    for (int i : range(35, 27, -1))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range(2, 8, 0.5):";
    for (auto i : range(2, 8, 0.5))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range(8, 7, -0.1):";
    for (auto i : range(8, 7, -0.1))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "range('a', 'z'):";
    for (auto i : range('a', 'z'))
    {
        std::cout << " " << i;
    }
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////

void test_algorithm(void)
{
    //test_max_min();
    //test_sequence();
    test_range();
}

////////////////////////////////////////////////////////////////

#endif // TEST_ALGORITHM_H
