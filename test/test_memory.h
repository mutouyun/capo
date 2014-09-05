#ifndef TEST_MEMORY_H
#define TEST_MEMORY_H

////////////////////////////////////////////////////////////////

#include "../include/memory/construct.hpp"
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

#include "../include/memory/allocator_wrapper.hpp"
#include "../include/memory/standard_alloc.hpp"
#include <vector>

void test_allocator_wrapper(void)
{
    TEST_CASE();

    std::vector<int, capo::allocator_wrapper<int, capo::use::alloc_std>> foo = { 10, 20, 30 };
    for (auto x : foo)
        std::cout << x << " ";
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////

#include "../include/proofing/assert.hpp"
#include "../include/type/type_name.hpp"
#include "../include/random/random.hpp"
#include "../include/chrono/stopwatch.hpp"

#include <cstring>  // memset
#include <cstdio>   // printf
#include <utility>  // std::forward
#include <thread>   // std::thread

namespace np_test_memory {
namespace simple {

////////////////////////////////////////////////////////////////
/// Simple array wrapper
////////////////////////////////////////////////////////////////

template <typename T, size_t Bound = 0>
struct array
{
    T* c_arr_;

    array(size_t bound = Bound)
        : c_arr_(new T[bound])
    {}

    ~array(void) { delete [] c_arr_; }

    T&       operator[](size_t n)       { return c_arr_[n]; }
    const T& operator[](size_t n) const { return c_arr_[n]; }

    void clear(void)
    {
        capo_assert(Bound);
        capo_assert(c_arr_ != nullptr);
        ::memset(c_arr_, 0, sizeof(T[Bound]));
    }

    size_t count(void) const { return Bound; }
};

} // namespace simple

////////////////////////////////////////////////////////////////
/// Prepare to test
////////////////////////////////////////////////////////////////

#ifdef NDEBUG
const int TestCycl = 1000;
const int TestCont = 1000;
#else
const int TestCycl = 100;
const int TestCont = 100;
#endif
const int TestSMin = 4;
const int TestSMax = 128;

simple::array<size_t, TestCont> sizes;
simple::array<size_t, TestCont> index[2];

void init(void)
{
    ::printf("Cycles:\t\t%d\nContinuous:\t%d\nAlloc Size:\t%d bytes-%d bytes\n",
             TestCycl, TestCont, TestSMin * sizeof(char), TestSMax * sizeof(char));

    capo::random<std::mt19937> rdm(TestSMin, TestSMax);
    for (int i = 0; i < TestCont; ++i) sizes[i] = rdm();
}

volatile bool is_not_started;

template <class AllocT, int = AllocT::AllocType>
struct test_alloc
{
    static AllocT pool_;

    static size_t remain(void) { return pool_.remain(); }

    static void clear(void) { pool_.clear(); }

    static void* alloc(size_t size)
    {
        return pool_.alloc(size);
    }

    static void free(void* p, size_t size)
    {
        pool_.free(p, size);
    }
};

template <class AllocT, int Tp_>
AllocT test_alloc<AllocT, Tp_>::pool_;

template <class AllocT>
struct test_alloc<AllocT, capo::alloc_concept::RegionAlloc> : AllocT
{
    static size_t remain(void) { return 0; }
    static void   clear(void)  {}
};

template <typename A>
void thread_proc(size_t test_cycl)
{
    test_cycl /= 2;
    simple::array<void*, TestCont> ptrs; ptrs.clear();
    while (is_not_started) std::this_thread::yield();
    for(size_t i = 0; i < test_cycl; ++i)
    {
        A alc; (void)alc;
        for(int x = 0; x < 2; ++x)
        for(int n = 0; n < TestCont; ++n)
        {
            size_t m   = index[x][n];
            void*(& p) = ptrs [m];
            size_t s   = sizes[m];
            if (p == NULL)
                p = alc.alloc(s);
            else
            {
                alc.free(p, s);
                p = NULL;
            }
        }
    }
}

#define TEST_MEMPOOL__(TEST_NAME) do                                \
{                                                                   \
    ::printf("%s test: \t", #TEST_NAME);                            \
    is_not_started = true;                                          \
    /* Create threads */                                            \
    simple::array<std::thread> h(threads_n);                        \
    for(size_t i = 0; i < threads_n; ++i)                           \
        h[i] = std::thread(thread_proc<test_alloc<A>>, (TestCycl / threads_n)); \
    is_not_started = false;                                         \
    /* Start timer */                                               \
    capo::stopwatch<> sw(true);                                     \
    for(size_t i = 0; i < threads_n; ++i)                           \
        h[i].join();                                                \
    /* Record time */                                               \
    size_t value = (size_t)sw.elapsed<std::chrono::milliseconds>(); \
    size_t r = test_alloc<A>::remain();                             \
    ::printf("%d ms, remain: %d bytes\n", value, r);                \
    test_alloc<A>::clear();                                         \
} while(0)

template <typename A>
void start(unsigned threads_n = 1)
{
    capo_assert(threads_n);
    ::printf("\nStart for %s...(threads: %d)\n", 
             capo::type_name<A>().c_str(), threads_n);
    {
        for (int i = 0, n = TestCont - 1; i < TestCont; ++i, --n)
            index[1][n] = index[0][i] = i;
        TEST_MEMPOOL__(LIFO);
    }
    {
        for (int i = 0; i < TestCont; ++i)
            index[1][i] = index[0][i] = i;
        TEST_MEMPOOL__(FIFO);
    }
    {
        capo::random<std::mt19937> rdm(0, TestCont - 1);
        for (int i = 0; i < TestCont; ++i)
            index[1][i] = index[0][i] = rdm();
        TEST_MEMPOOL__(Random);
    }
}

#undef TEST_MEMPOOL__

} // namespace np_test_memory

////////////////////////////////////////////////////////////////

void test_memory(void)
{
    test_construct();
    test_allocator_wrapper();

    TEST_CASE();
    using namespace np_test_memory;
    
    init();

    start<capo::use::alloc_malloc>(1);
}

////////////////////////////////////////////////////////////////

#endif // TEST_MEMORY_H

