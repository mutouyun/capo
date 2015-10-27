#pragma once

TEST_METHOD(alloc_malloc_case)
{
    using namespace ut_memory_;
    start<ut_memory_::alloc_malloc>();
}

TEST_METHOD(fixed_pool_case)
{
    using namespace ut_memory_;
    using namespace capo;

    start<fixed_pool<TestSMax, use::arithmetic<>, ut_memory_::alloc_malloc>>();
    start<fixed_pool<TestSMax, use::geometric<> , ut_memory_::alloc_malloc>>();
    start<fixed_pool<TestSMax, use::fibonacci   , ut_memory_::alloc_malloc>>();

    start<fixed_pool<TestSMax, use::arithmetic<>, scope_alloc<ut_memory_::alloc_malloc>>>();
    start<fixed_pool<TestSMax, use::geometric<> , scope_alloc<ut_memory_::alloc_malloc>>>();
    start<fixed_pool<TestSMax, use::fibonacci   , scope_alloc<ut_memory_::alloc_malloc>>>();

    start<fixed_pool<TestSMax, use::arithmetic<>, scope_alloc<ut_memory_::alloc_malloc, use::block_normal>>>();
    start<fixed_pool<TestSMax, use::geometric<> , scope_alloc<ut_memory_::alloc_malloc, use::block_normal>>>();
    start<fixed_pool<TestSMax, use::fibonacci   , scope_alloc<ut_memory_::alloc_malloc, use::block_normal>>>();
}

TEST_METHOD(variable_pool_case)
{
    ut_memory_::start<capo::variable_pool<CAPO_VARIABLE_POOL_CHUNKSIZE_, ut_memory_::alloc_malloc>>();
}
