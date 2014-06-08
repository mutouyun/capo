#ifndef TEST_MEMORY_H
#define TEST_MEMORY_H

////////////////////////////////////////////////////////////////

#include "../src/memory/construct.hpp"
#include <type_traits>

namespace np_test_construct
{
    class Foo
    {
        int x_ = 123;

    public:
        Foo(void)       { std::cout << "default" << std::endl; }
        Foo(int)        { std::cout << "int"     << std::endl; }
        Foo(const Foo&) { std::cout << "copy"    << std::endl; }
        Foo(Foo&&)      { std::cout << "move"    << std::endl; }
        ~Foo(void)      { std::cout << "destruct"<< std::endl; }
    };
}

void test_construct(void)
{
    TEST_CASE();
    using namespace np_test_construct;
    {
        std::aligned_storage<sizeof(Foo), alignof(Foo)>::type rr;
        capo::construct<Foo>(&rr);
        capo::construct<Foo>(&rr, 123);
        capo::construct<Foo>(&rr, Foo());
    }
    std::cout << std::endl;
    {
        std::aligned_storage<sizeof(Foo[3]), alignof(Foo[3])>::type rr;
        capo::construct<Foo[3]>(&rr, 321);
    }
    std::cout << std::endl;
    {
        capo::destruct<Foo>(0);
    }
    std::cout << std::endl;
    {
        capo::destruct<Foo[3]>(0);
    }
}

////////////////////////////////////////////////////////////////

//namespace np_test_alloc { struct alloc_malloc_2; }
//#define CAPO_DEFAULT_ALLOC_ np_test_alloc::alloc_malloc_2
#include "../src/memory/alloc.hpp"

namespace np_test_alloc
{
    struct alloc_malloc_2
    {
        static void* alloc(size_t /*size*/)
        {
            return nullptr;
        }

        static void free(void* /*p*/, size_t /*size*/)
        {
        }

        static void* realloc(void* /*p*/, size_t /*old_size*/, size_t /*new_size*/)
        {
            return nullptr;
        }
    };
}

void test_alloc(void)
{
    TEST_CASE();
    using namespace np_test_alloc;
    using namespace np_test_construct;

    auto p = capo_alloc<Foo>();
    capo_realloc<Foo[3]>(p, 123);
}

////////////////////////////////////////////////////////////////

#include "../src/memory/allocator.hpp"
#include <vector>

void test_allocator(void)
{
    TEST_CASE();

    std::vector<int, capo::allocator<int>> foo = { 10, 20, 30 };
    for (auto x : foo)
        std::cout << x << " ";
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////

void test_memory(void)
{
    test_construct();
    test_alloc();
    test_allocator();
}

////////////////////////////////////////////////////////////////

#endif // TEST_MEMORY_H

