#pragma once

#include "capo/signal.hpp"

namespace ut_signal_ {

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

float product   (float x, float y) { return x * y; }
float quotient  (float x, float y) { return x / y; }
float sum       (float x, float y) { return x + y; }
float difference(float x, float y) { return x - y; }

} // namespace ut_signal_
