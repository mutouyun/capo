#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <utility>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

////////////////////////////////////////////////////////////////

struct out_t
{
    bool is_out_ = false;

    ~out_t(void)
    {
        if (is_out_)
            std::cout << std::endl;
    }

    out_t(void) = default;
    out_t(out_t&& rhs) : is_out_(rhs.is_out_)
    {
        rhs.is_out_ = false;
    }

    template <typename T>
    out_t& operator<<(T&& val)
    {
        is_out_ = true;
        std::cout << std::forward<T>(val);
        return (*this);
    }
};

out_t expect_out(void) { return {}; }

#define EXPECT_EQ(EXPECTED, ACTUAL)                 Assert::AreEqual(EXPECTED, ACTUAL); expect_out()
#define EXPECT_STREQ(EXPECTED, ACTUAL)              Assert::AreEqual(EXPECTED, ACTUAL); expect_out()
#define EXPECT_DOUBLE_EQ(EXPECTED, ACTUAL)          Assert::AreEqual((double)EXPECTED, (double)ACTUAL, 1e-8); expect_out()
#define EXPECT_THROW(STATEMENT, EXPECTED_EXCEPTION) Assert::ExpectException<EXPECTED_EXCEPTION>([&]{ STATEMENT; })
#define EXPECT_LT(VAL1, VAL2)                       Assert::IsTrue((VAL1) < (VAL2))

////////////////////////////////////////////////////////////////

#include "ut-concept/preparing.h"
TEST_CLASS(ut_concept)
{
public:
#include "ut-concept/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-constant_array/preparing.h"
TEST_CLASS(ut_constant_array)
{
public:
#include "ut-constant_array/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-construct/preparing.h"
TEST_CLASS(ut_construct)
{
public:
#include "ut-construct/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-countof/preparing.h"
TEST_CLASS(ut_countof)
{
public:
#include "ut-countof/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-gc/preparing.h"
TEST_CLASS(ut_gc)
{
public:
#include "ut-gc/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-max_min/preparing.h"
TEST_CLASS(ut_max_min)
{
public:
#include "ut-max_min/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-operator/preparing.h"
TEST_CLASS(ut_operator)
{
public:
#include "ut-operator/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-preprocessor/preparing.h"
TEST_CLASS(ut_preprocessor)
{
public:
#include "ut-preprocessor/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-printf/preparing.h"
TEST_CLASS(ut_printf)
{
public:
#include "ut-printf/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-range/preparing.h"
TEST_CLASS(ut_range)
{
public:
#include "ut-range/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-sequence/preparing.h"
TEST_CLASS(ut_sequence)
{
public:
#include "ut-sequence/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-signal/preparing.h"
TEST_CLASS(ut_signal)
{
public:
#include "ut-signal/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-spin_lock/preparing.h"
TEST_CLASS(ut_spin_lock)
{
public:
#include "ut-spin_lock/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-trackable/preparing.h"
TEST_CLASS(ut_trackable)
{
public:
#include "ut-trackable/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-type_list/preparing.h"
TEST_CLASS(ut_type_list)
{
public:
#include "ut-type_list/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-types_to_seq/preparing.h"
TEST_CLASS(ut_types_to_seq)
{
public:
#include "ut-types_to_seq/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-any/preparing.h"
TEST_CLASS(ut_any)
{
public:
#include "ut-any/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-cmdline/preparing.h"
TEST_CLASS(ut_cmdline)
{
public:
#include "ut-cmdline/cases.h"
};

////////////////////////////////////////////////////////////////

#include "ut-singleton/preparing.h"
TEST_CLASS(ut_singleton)
{
public:
#include "ut-singleton/cases.h"
};

////////////////////////////////////////////////////////////////
