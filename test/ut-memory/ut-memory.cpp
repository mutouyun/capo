#include "gtest/gtest.h"

#include "capo/output.hpp"
#include "capo/random.hpp"
#include "capo/stopwatch.hpp"
#include "capo/assert.hpp"
#include "capo/unused.hpp"
#include "capo/type_name.hpp"
#include "capo/standard_alloc.hpp"
#include "capo/fixed_pool.hpp"

#include <vector>
#include <array>
#include <thread>
#include <future>

////////////////////////////////////////////////////////////////

#ifdef NDEBUG
const size_t TestCycl = 10000;
const size_t TestCont = 10000;
const size_t TestSMin = 256;
const size_t TestSMax = 256;
#else
const size_t TestCycl = 2/*1000*/;
const size_t TestCont = 2/*100*/;
const size_t TestSMin = 256;
const size_t TestSMax = 256;
#endif

std::vector<size_t> sizes(TestCont);
std::vector<size_t> index[2];

void init(void)
{
    capo::output(std::cout, "Cycles:\t\t{0}\nContinuous:\t{1}\nAlloc Size:\t{2} - {3} bytes\n", 
                 TestCycl, TestCont, TestSMin * sizeof(char), TestSMax * sizeof(char));

    capo::random<> rdm(TestSMin, TestSMax);
    for (size_t i = 0; i < TestCont; ++i) sizes[i] = rdm();

    index[0].resize(TestCont);
    index[1].resize(TestCont);
}

struct RunOnce
{
    RunOnce(void) { init(); }
} init__;

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

template <class AllocT, int AC>
AllocT test_alloc<AllocT, AC>::pool_;

template <class AllocT>
struct test_alloc<AllocT, capo::alloc_concept::RegionAlloc> : AllocT
{
    static size_t remain(void) { return 0; }
    static void   clear(void)  {}
};

volatile bool is_not_started;

#define TEST_CYCLES__(C, A1, A2) do     \
{                                       \
    CAPO_UNUSED_ AllocT alc;            \
    for(int x = 0; x < C; ++x)          \
    for(int n = 0; n < TestCont; ++n)   \
    {                                   \
        size_t m   = index[x][n];       \
        void*(& p) = ptrs [m];          \
        size_t s   = sizes[m];          \
        if (p == nullptr)               \
        {                               \
            A1;                         \
            p = alc.alloc(s);           \
        }                               \
        else                            \
        {                               \
            A2;                         \
            alc.free(p, s);             \
            p = nullptr;                \
        }                               \
    }                                   \
} while(0)

template <typename AllocT, size_t ThreadN>
void working_proc(size_t& alloced_size)
{
    static const size_t TEST_CYCL = (TestCycl / ThreadN / 2) - 1;
    std::array<void*, TestCont> ptrs;
    ptrs.assign(nullptr);
    while (is_not_started) std::this_thread::yield();
    TEST_CYCLES__(2, if (x == 0) alloced_size += s, 
                     if (x == 0) alloced_size -= s);
    for (size_t i = 0; i < TEST_CYCL; ++i)
    {
        TEST_CYCLES__(2,,);
    }
}

#undef TEST_CYCLES__

template <typename AllocT, size_t ThreadN>
void test_memory_pool(const char* name)
{
    capo::output(std::cout, "{0} test: \t", name);
    is_not_started = true;

    struct {
        size_t      alloced_size_ = 0;
        std::thread working_ { working_proc<test_alloc<AllocT>, ThreadN>, std::ref(alloced_size_) };
    } threads[ThreadN];

    is_not_started = false;
    capo::stopwatch<> sw(true);
    for (auto& t : threads) t.working_.join();

    auto value  = sw.elapsed<std::chrono::milliseconds>();
    auto remain = test_alloc<AllocT>::remain();
    size_t alloced_size = 0;
    for (auto& t : threads) alloced_size += t.alloced_size_;
    size_t fragment = remain ? remain - alloced_size : 0;

    capo::output(std::cout, "{0} ms, allocated: {1} bytes, Fragment: {2} bytes, {3:.2}%\n",
                 value, alloced_size, fragment, alloced_size ? (fragment * 100 / (double)alloced_size) : 0.0);
    test_alloc<AllocT>::clear();
}

template <typename AllocT, size_t ThreadN = 1>
void start(void)
{
    static_assert(ThreadN > 0, "Thread number is 0!");
    capo::output(std::cout, "Start for {0}...(threads: {1})\n",
                 capo::type_name<AllocT>().c_str(), ThreadN);
    {
        for (size_t i = 0, n = TestCont - 1; i < TestCont; ++i, --n)
            index[1][n] = index[0][i] = i;
        test_memory_pool<AllocT, ThreadN>("LIFO");
    }
    {
        for (size_t i = 0; i < TestCont; ++i)
            index[1][i] = index[0][i] = i;
        test_memory_pool<AllocT, ThreadN>("FIFO");
    }
    {
        capo::random<> rdm(0, TestCont - 1);
        for (size_t i = 0; i < TestCont; ++i)
            index[1][i] = index[0][i] = rdm();
        test_memory_pool<AllocT, ThreadN>("Random");
    }
    std::cout << std::endl;
}

#define TEST_METHOD(TEST_NAME) TEST(memory, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(alloc_malloc)
{
    start<capo::use::alloc_malloc>();
}

TEST_METHOD(fixed_pool)
{
    start<capo::fixed_pool<TestSMax, capo::use::fibonacci   , capo::use::alloc_malloc>>();
    start<capo::fixed_pool<TestSMax, capo::use::geometric<> , capo::use::alloc_malloc>>();
    start<capo::fixed_pool<TestSMax, capo::use::arithmetic<>, capo::use::alloc_malloc>>();
}
