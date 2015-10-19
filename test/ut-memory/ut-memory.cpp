#include "gtest/gtest.h"

#include "capo/output.hpp"
#include "capo/random.hpp"
#include "capo/stopwatch.hpp"
#include "capo/assert.hpp"
#include "capo/unused.hpp"
#include "capo/type_name.hpp"
#include "capo/standard_alloc.hpp"
#include "capo/fixed_pool.hpp"
#include "capo/variable_pool.hpp"

#include <vector>
#include <thread>
#include <future>

////////////////////////////////////////////////////////////////

namespace ut {

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
std::vector<size_t> index[3][2];

void init(void)
{
    capo::output(std::cout, "Cycles:\t\t{0}\nContinuous:\t{1}\nAlloc Size:\t{2} - {3} bytes\n", 
                 TestCycl, TestCont, TestSMin * sizeof(char), TestSMax * sizeof(char));

    capo::random<> rdm_sizes(TestSMin, TestSMax);
    for (size_t i = 0; i < TestCont; ++i) sizes[i] = rdm_sizes();

    for (auto& ix : index)
    {
        ix[0].resize(TestCont);
        ix[1].resize(TestCont);
    }
    for (size_t i = 0, n = TestCont - 1; i < TestCont; ++i, --n)
        index[0][1][n] = index[0][0][i] = i;
    for (size_t i = 0; i < TestCont; ++i)
        index[1][1][i] = index[1][0][i] = i;
    capo::random<> rdm_index(0, TestCont - 1);
    for (size_t i = 0; i < TestCont; ++i)
        index[2][1][i] = index[2][0][i] = rdm_index();
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
struct test_alloc<AllocT, capo::alloc_concept::RegionAlloc>
{
    AllocT pool_;

    static size_t remain(void) { return 0; }
    static void   clear(void)  {}

    void* alloc(size_t size)        { return pool_.alloc(size); }
    void free(void* p, size_t size) { pool_.free(p, size); }
};

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
void working_proc(size_t& alloced_size, const std::vector<size_t>* ix)
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
    capo::output(std::cout, "{0} test: \t", name);
    is_not_started = true;

    struct {
        size_t      alloced_size_ = 0;
        std::thread working_ { working_proc<test_alloc<AllocT>, ThreadN>, std::ref(alloced_size_), index[IndexN] };
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

    test_memory_pool<AllocT, ThreadN, 0>("LIFO");
    test_memory_pool<AllocT, ThreadN, 1>("FIFO");
    test_memory_pool<AllocT, ThreadN, 2>("Random");

    std::cout << std::endl;
}

} // namespace ut

#define TEST_METHOD(TEST_NAME) TEST(memory, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(alloc_malloc)
{
    //ut::start<capo::use::alloc_malloc>();
}

TEST_METHOD(fixed_pool)
{
    //ut::start<capo::fixed_pool<ut::TestSMax, capo::use::arithmetic<>, capo::use::alloc_malloc>>();
    //ut::start<capo::fixed_pool<ut::TestSMax, capo::use::geometric<> , capo::use::alloc_malloc>>();
    //ut::start<capo::fixed_pool<ut::TestSMax, capo::use::fibonacci   , capo::use::alloc_malloc>>();
}

TEST_METHOD(variable_pool)
{
    ut::start<capo::variable_pool<>>();
}
