#pragma once

#include "capo/memory/scope_alloc.hpp"

namespace ut_gc_ {

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


} // namespace ut_gc_