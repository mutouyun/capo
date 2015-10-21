
#include "gtest/gtest.h"

#include "capo/signal.hpp"

////////////////////////////////////////////////////////////////

#define TEST_METHOD(TEST_NAME) TEST(signal, TEST_NAME)

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

capo::signal<void(int, XXX, int)> SigTest;

class Foo
{
public:
    int func(int a, XXX)
    {
        printf("%d\n", a);
        return 333;
    }
};

TEST_METHOD(object)
{
    Foo foo;
    SigTest.connect(&foo, &Foo::func);
    XXX xx;
    /*int n = */SigTest(123, xx/*std::move(xx)*/, 321);
}

////////////////////////////////////////////////////////////////

float product   (float x, float y) { return x * y; }
float quotient  (float x, float y) { return x / y; }
float sum       (float x, float y) { return x + y; }
float difference(float x, float y) { return x - y; }

TEST_METHOD(disconnect)
{
    capo::signal<float(float, float)> sig;
    sig.connect(&product);
    sig.connect(&quotient);
    sig.connect(&sum);
    sig.connect(&difference);
    EXPECT_EQ(sig(1.f, 3.f), -2.f);

    sig.disconnect(&difference);
    EXPECT_EQ(sig(1.f, 3.f), 4.f);

    sig.disconnect();
    EXPECT_EQ(sig(1.f, 3.f), 0.f);
}

TEST_METHOD(max)
{
    capo::signal<float(float, float)> sig, sig2;
    sig.connect(&product);
    sig.connect(&quotient);
    sig.connect(&sum);
    sig.connect(&difference);
    EXPECT_EQ(sig(5.f, 3.f), 2.f);

    sig2 = sig;
    sig2 = std::move(sig);
    EXPECT_EQ(sig(5.f, 3.f), 0.f);

    sig2.combiner([](auto& results)
    {
        using value_t = typename std::remove_reference<decltype(results)>::type::value_type;
        if (results.empty()) return value_t{};
        value_t* ret = nullptr;
        for (auto& r : results)
        {
            if (ret == nullptr || *ret < r)
            {
                ret = &r;
            }
        }
        return *ret;
    });
    EXPECT_EQ(sig2(5.f, 3.f), 15.f);

    sig2[0].block(true);
    EXPECT_EQ(sig2(5.f, 3.f), 8.f);
}

TEST_METHOD(foreach)
{
    capo::signal<float(float, float)> sig;
    sig.connect(&product);
    sig.connect(&quotient);
    sig.connect(&sum);
    sig.connect(&difference);

    float res[] = 
    {
        product   (5.f, 3.f),
        quotient  (5.f, 3.f),
        sum       (5.f, 3.f),
        difference(5.f, 3.f)
    };

    int i = 0;
    for (const auto& f : sig)
    {
        EXPECT_EQ(f(5.f, 3.f), res[i++]);
    }
}
