#pragma once

#include "capo/output.hpp"
#include "capo/random.hpp"
#include "capo/stopwatch.hpp"
#include "capo/assert.hpp"
#include "capo/unused.hpp"
#include "capo/type_name.hpp"
#include "capo/memory/standard_alloc.hpp"
#include "capo/memory/fixed_pool.hpp"
#include "capo/memory/variable_pool.hpp"
#include "capo/memory/scope_alloc.hpp"

#include <vector>
#include <thread>
#include <future>

namespace ut_memory_ {

#ifdef NDEBUG
const int TestCycl = 10000;
const int TestCont = 10000;
const int TestSMin = sizeof(void*);
const int TestSMax = sizeof(void*);
#else
const int TestCycl = 2;
const int TestCont = 10000;
const int TestSMin = 256;
const int TestSMax = 256;
#endif

std::vector<size_t> sizes(TestCont);
std::vector<int>    index[3][2];

void init(void)
{
    capo::output("Test Cycles:\t{0}\nContinuous:\t{1}\nAlloc Size:\t{2} - {3} bytes\n", 
                 TestCycl, TestCont, TestSMin * sizeof(char), TestSMax * sizeof(char));

    capo::random<> rdm_sizes(TestSMin, TestSMax);
    for (int i = 0; i < TestCont; ++i) sizes[i] = rdm_sizes();

    for (auto& ix : index)
    {
        ix[0].resize(TestCont);
        ix[1].resize(TestCont);
    }
    for (int i = 0, n = TestCont - 1; i < TestCont; ++i, --n)
        index[0][1][n] = index[0][0][i] = i;
    for (int i = 0; i < TestCont; ++i)
        index[1][1][i] = index[1][0][i] = i;
    capo::random<> rdm_index(0, TestCont - 1);
    for (int i = 0; i < TestCont; ++i)
        index[2][1][i] = index[2][0][i] = rdm_index();
}

struct RunOnce
{
    RunOnce(void) { init(); }
} init__;

struct alloc_malloc : capo::use::alloc_malloc
{
    enum { AllocType = capo::alloc_concept::StaticAlloc };

    static size_t alloced_;

    static size_t alloced(void) { return alloced_; }

    static void* alloc(size_t size)
    {
        alloced_ += size;
        return capo::use::alloc_malloc::alloc(size);
    }
    static void free(void* p, size_t size)
    {
        alloced_ -= size;
        capo::use::alloc_malloc::free(p, size);
    }
    using capo::use::alloc_malloc::free;
};
size_t alloc_malloc::alloced_ = 0;

template <class AllocT, int = AllocT::AllocType>
struct test_alloc
{
    static AllocT pool_;

    static size_t alloced(void) { return alloc_malloc::alloced(); }
    static void   clear  (void) { pool_.clear(); alloc_malloc::alloced_ = 0; }

    static void* alloc(size_t size)
    {
        return pool_.alloc(size);
    }
    static void free(void* p, size_t size)
    {
        pool_.free(p, size);
    }
};
template <class AllocT, int AC>
AllocT test_alloc<AllocT, AC>::pool_;

template <class AllocT>
struct test_alloc<AllocT, capo::alloc_concept::RegionAlloc>
{
    AllocT pool_;

    static size_t alloced_;
    ~test_alloc(void) { alloced_ = alloc_malloc::alloced(); }

    static size_t alloced(void) { return alloced_; }
    static void   clear(void)   { alloced_ = alloc_malloc::alloced_ = 0; }

    void* alloc(size_t size)          { return pool_.alloc(size); }
    void  free (void* p, size_t size) { pool_.free(p, size); }
};
template <class AllocT>
size_t test_alloc<AllocT, capo::alloc_concept::RegionAlloc>::alloced_ = 0;

volatile bool is_not_started;

#define TEST_CYCLES__(C, A1, A2) do      \
{                                        \
    CAPO_UNUSED_ AllocT alc;             \
    for(size_t x = 0; x < C; ++x)        \
    for(size_t n = 0; n < TestCont; ++n) \
    {                                    \
        size_t m   = ix[x][n];           \
        void*(& p) = ptrs [m];           \
        size_t s   = sizes[m];           \
        if (p == nullptr)                \
        {                                \
            A1;                          \
            p = alc.alloc(s);            \
        }                                \
        else                             \
        {                                \
            A2;                          \
            alc.free(p, s);              \
            p = nullptr;                 \
        }                                \
    }                                    \
} while(0)

template <typename AllocT, size_t ThreadN>
void working_proc(size_t& alloced_size, const std::vector<int>* ix)
{
    static const size_t TEST_CYCL = (TestCycl / ThreadN / 2) - 1;
    void** ptrs = new void*[TestCont];
    memset(ptrs, 0, sizeof(void*) * TestCont);
    while (is_not_started) std::this_thread::yield();
    TEST_CYCLES__(2, if (x == 0) alloced_size += s, 
                     if (x == 0) alloced_size -= s);
    for (size_t i = 0; i < TEST_CYCL; ++i)
    {
        TEST_CYCLES__(2,,);
    }
    delete [] ptrs;
}

#undef TEST_CYCLES__

template <typename AllocT, size_t ThreadN, size_t IndexN>
void test_memory_pool(const char* name)
{
    capo::output("{0} test: \t", name);
    is_not_started = true;

    struct {
        size_t      alloced_size_ = 0;
        std::thread working_ { &working_proc<test_alloc<AllocT>, ThreadN>, std::ref(alloced_size_), index[IndexN] };
    } threads[ThreadN];

    is_not_started = false;
    capo::stopwatch<> sw(true);
    for (auto& t : threads) t.working_.join();

    auto value = sw.elapsed<std::chrono::milliseconds>();
    size_t alloced_malc = test_alloc<AllocT>::alloced();
    size_t alloced_size = 0;
    for (auto& t : threads) alloced_size += t.alloced_size_;
    size_t fragment = alloced_malc ? alloced_malc - alloced_size : 0;

    capo::output("{0} ms, allocated: {1} bytes, Fragment: {2} bytes, {3:.2}%\n",
                 value, alloced_size, fragment, alloced_size ? (fragment * 100 / (double)alloced_size) : 0.0);
    test_alloc<AllocT>::clear();
}

template <typename AllocT, size_t ThreadN = 1>
void start(void)
{
    static_assert(ThreadN > 0, "Thread number is 0!");

    capo::output("Start for {0}...(threads: {1})\n", capo::type_name<AllocT>().c_str(), ThreadN);

    test_memory_pool<AllocT, ThreadN, 0>("LIFO");
    test_memory_pool<AllocT, ThreadN, 1>("FIFO");
    test_memory_pool<AllocT, ThreadN, 2>("Random");

    std::cout << std::endl;
}

} // namespace ut_memory_
