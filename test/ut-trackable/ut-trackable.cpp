
#include "gtest/gtest.h"

#include "capo/trackable.hpp"
#include "capo/output.hpp"

////////////////////////////////////////////////////////////////

class XXX : public capo::trackable<XXX>
{
public:
    const char* name_;

    XXX(const char* name) : name_(name)
    {
        printf("XXX create: %s\n", name_);
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
        printf("XXX destory: %s\n", name_);
    }
};

#define TEST_METHOD(TEST_NAME) TEST(trackable, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(track)
{
    std::string buf;
    auto out = [&buf](const std::string& str)
    {
        buf += str;
        std::cout << str;
    };

    XXX x1("x1");
    XXX x2("x2");

    for (auto x : XXX::list())
    {
        capo::output(out, "{0} ", x->name_);
    }
    std::cout << std::endl;
    EXPECT_STREQ("x2 x1 ", buf.c_str());
    buf = "";

    XXX* x3 = new XXX("x3");

    for (auto x : XXX::list())
    {
        capo::output(out, "{0} ", x->name_);
    }
    std::cout << std::endl;
    EXPECT_STREQ("x3 x2 x1 ", buf.c_str());
    buf = "";

    delete x3;

    for (auto x : XXX::list())
    {
        capo::output(out, "{0} ", x->name_);
    }
    std::cout << std::endl;
    EXPECT_STREQ("x2 x1 ", buf.c_str());
}
