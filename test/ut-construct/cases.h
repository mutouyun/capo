#pragma once

TEST_METHOD(construct)
{
    using namespace ut_construct_;

    capo::construct<Foo>(pfoo);
    EXPECT_STREQ("default", g_str.c_str());

    capo::construct<Foo>(pfoo, 123);
    EXPECT_STREQ("int 123", g_str.c_str());

    capo::construct<Foo>(pfoo, Foo());
    EXPECT_STREQ("move", g_str.c_str());
}

TEST_METHOD(array)
{
    using namespace ut_construct_;

    capo::construct<farr_t>(farr, 321);
    for (Foo& ff : *farr)
    {
        EXPECT_STREQ("int 321", g_str.c_str()) << "At index: " << (&ff - *farr);
    }
}

TEST_METHOD(destruct)
{
    using namespace ut_construct_;

    capo::destruct<Foo>(pfoo);
    EXPECT_STREQ("destruct", g_str.c_str());

    capo::destruct<farr_t>(farr);
    for (Foo& ff : *farr)
    {
        EXPECT_STREQ("destruct", g_str.c_str()) << "At index: " << (&ff - *farr);
    }
}
