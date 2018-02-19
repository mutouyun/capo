#pragma once

#include "capo/singleton.hpp"

namespace ut_singleton_ {

class XXX
{
public:
    XXX(void)
    {
        printf("XXX create\n");
    }
    XXX(int x)
    {
        printf("XXX create with int: %d\n", x);
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


} // namespace ut_singleton_