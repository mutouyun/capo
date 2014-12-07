#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

#include "capo/construct.hpp"
#include "capo/printf.hpp"

#include <string>
#include <type_traits>

////////////////////////////////////////////////////////////////

std::string g_str;

struct Foo
{
    std::string* str_ = &g_str;

    Foo(void)
    {
        str_ = nullptr;
        g_str = "default";
    }

    Foo(int x)
    {
        str_->clear();
        capo::printf([this](const std::string& str)
        {
            *str_ += str;
        }, "int %d", x);
    }

    Foo(const Foo&) { *str_ = "copy"; }
    Foo(Foo&&)      { *str_ = "move"; }
    ~Foo(void)      { if (str_) *str_ = "destruct"; }
};

std::aligned_storage<sizeof(Foo), alignof(Foo)>::type rr;
Foo* pfoo = reinterpret_cast<Foo*>(&rr);

using farr_t = Foo[3];
std::aligned_storage<sizeof(farr_t), alignof(farr_t)>::type ra;
farr_t* farr = reinterpret_cast<farr_t*>(&ra);

#define TEST_METHOD(TEST_NAME) TEST(construct, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(construct)
{
    capo::construct<Foo>(pfoo);
    EXPECT_EQ("default", g_str);

    capo::construct<Foo>(pfoo, 123);
    EXPECT_EQ("int 123", g_str);

    capo::construct<Foo>(pfoo, Foo());
    EXPECT_EQ("move", g_str);
}

TEST_METHOD(array)
{
    capo::construct<farr_t>(farr, 321);
    for (Foo& ff : *farr)
    {
        EXPECT_EQ("int 321", g_str) << "At index: " << (&ff - *farr);
    }
}

TEST_METHOD(destruct)
{
    capo::destruct<Foo>(pfoo);
    EXPECT_EQ("destruct", g_str);

    capo::destruct<farr_t>(farr);
    for (Foo& ff : *farr)
    {
        EXPECT_EQ("destruct", g_str) << "At index: " << (&ff - *farr);
    }
}
