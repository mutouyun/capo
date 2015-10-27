#pragma once

TEST_METHOD(foo_case)
{
    using namespace ut_range_;

    static const double arr[] =
    {
        15, 15.8, 16.6, 17.4, 18.2, 19, 19.8, 20.6
    };
    int i = 0;
    for (auto n : capo::range(Foo(15), Foo(20.7), 0.8))
    {
        EXPECT_DOUBLE_EQ(arr[i], (double)n) << "At index: " << i;
        ++i;
    }
}

TEST_METHOD(integer_case)
{
    auto r = capo::range(1, 2, 3);
    {
        static const int arr[] =
        {
            1, 2, 3
        };
        int i = 0;
        for (auto n : r)
        {
            EXPECT_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
    int a = 15;
    r = capo::range(a);
    {
        static const int arr[] =
        {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
        };
        int i = 0;
        for (auto n : r)
        {
            EXPECT_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
    {
        static const int arr[] =
        {
            2, 3, 4, 5
        };
        int i = 0;
        for (auto n : capo::range(2, 6))
        {
            EXPECT_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
}

TEST_METHOD(minus_step)
{
    {
        static const int arr[] =
        {
            2, 5
        };
        const int x = 2, y = 6, z = 3;
        int i = 0;
        for (auto n : capo::range(x, y, z))
        {
            EXPECT_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
    {
        static const int arr[] =
        {
            -2, -5
        };
        int i = 0;
        for (auto n : capo::range(-2, -6, -3))
        {
            EXPECT_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
    {
        static const int arr[] =
        {
            35, 34, 33, 32, 31, 30, 29, 28
        };
        int i = 0;
        for (int n : capo::range(35, 27, -1))
        {
            EXPECT_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
}

TEST_METHOD(double_case)
{
    {
        static const double arr[] =
        {
            10.5, 11.5, 12.5, 13.5, 14.5
        };
        int i = 0;
        for (auto n : capo::range(10.5, 15.5))
        {
            EXPECT_DOUBLE_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
    {
        static const double arr[] =
        {
            2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5
        };
        int i = 0;
        for (auto n : capo::range(2, 8, 0.5))
        {
            EXPECT_DOUBLE_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
    {
        static const double arr[] =
        {
            8, 7.9, 7.8, 7.7, 7.6, 7.5, 7.4, 7.3, 7.2, 7.1
        };
        int i = 0;
        for (auto n : capo::range(8, 7, -0.1))
        {
            EXPECT_DOUBLE_EQ(arr[i], n) << "At index: " << i;
            ++i;
        }
    }
}

TEST_METHOD(char_case)
{
    static const char arr[] =
    {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y'
    };
    int i = 0;
    for (auto n : capo::range('a', 'z'))
    {
        EXPECT_EQ(arr[i], n) << "At index: " << i;
        ++i;
    }
}
