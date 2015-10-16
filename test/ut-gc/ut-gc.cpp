#include "gtest/gtest.h"

#include "capo/scope_alloc.hpp"

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

#define TEST_METHOD(TEST_NAME) TEST(gc, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(scope_alloc)
{
    capo::scope_alloc<> gc;
    gc.alloc(sizeof(XXX), []{ printf("XXX is deleting...\n"); });
    gc.alloc<XXX[3]>();
}
