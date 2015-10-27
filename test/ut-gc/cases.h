#pragma once

TEST_METHOD(scope_alloc)
{
    using namespace ut_gc_;
    capo::scope_alloc<> gc;
    gc.alloc(sizeof(XXX), [] { printf("XXX is deleting...\n"); });
    gc.alloc<XXX[3]>();
}
