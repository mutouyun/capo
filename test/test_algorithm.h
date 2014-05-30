#ifndef TEST_ALGORITHM_H
#define TEST_ALGORITHM_H

////////////////////////////////////////////////////////////////

#include "../src/algorithm/max_min.hpp"

void test_max_min(void)
{
    TEST_CASE();

    std::cout << capo::max(1, 3, 2) << std::endl;
    std::cout << capo::min(1, 3, 2) << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../src/algorithm/range.hpp"
#include "../src/utility/operator.hpp"

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

        explicit operator int(void) const { return static_cast<int>(x_); }

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

    std::cout << "range(15):";
    for (int i : range(15))
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

    std::cout << "range(2, 6, 3):";
    for (auto i : range(2, 6, 3))
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
    test_max_min();
    test_range();
}

////////////////////////////////////////////////////////////////

#endif // TEST_ALGORITHM_H
