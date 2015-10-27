#pragma once

TEST_METHOD(track)
{
    using namespace ut_trackable_;

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
