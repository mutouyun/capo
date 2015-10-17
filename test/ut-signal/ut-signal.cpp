
#include "gtest/gtest.h"

#include "capo/signal.hpp"

////////////////////////////////////////////////////////////////

class XXX
{
public:
    XXX(void)
    {
        printf("XXX create\n");
    }
    XXX(const XXX&)
    {
        printf("XXX copy\n");
    }
    XXX(XXX&&)
    {
        printf("XXX move\n");
    }
    ~XXX(void)
    {
        printf("XXX destory\n");
    }
};

capo::signal<int(int, XXX, int)> SigTest;

class Foo
{
public:
    int func(int a, XXX)
    {
        printf("%d\n", a);
        return 333;
    }
};

#define TEST_METHOD(TEST_NAME) TEST(signal, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(signal)
{
    Foo foo;
    SigTest.connect(&foo, &Foo::func);
    XXX xx;
    int n = SigTest(123, xx/*std::move(xx)*/, 321);
    EXPECT_EQ(foo.func(123, xx), n);
}
