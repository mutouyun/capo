#pragma once

////////////////////////////////////////////////////////////////

TEST_METHOD(cmdline)
{
    using namespace ut_cmdline_;

    std::string buf;
    auto out = [&](const std::string& str)
    {
        buf = str;
        std::cout << buf << std::endl;
    };

    a.push(cmdline::options
    {
        {
            "-h", "--help", "Print usage.", false, "", 
            [&](auto& a, auto&) {
                a.print_usage(out);
            }
        },
        {
            "-t", "--test", "You must use this option.", true, "", 
            [](auto&, auto&) { /*Do Nothing.*/ }
        },
        {
            "-o", "--output", "Print text.", true, "Hello World!",
            [&](auto&, auto& str) {
                out(str);
            }
        }
    });

    {
        const char * const argv[] = { "cmdline" };
        a.exec(out, countof(argv), argv);
        EXPECT_STREQ("Usage: cmdline --test --output=Hello World! [OPTIONS]...\n"
                     "Options: \n"
                     "  -t, --test\tYou must use this option.\n"
                     "  -o, --output\tPrint text.[=Hello World!]\n"
                     "  -h, --help\tPrint usage.\n", buf.c_str());
    }
    {
        const char * const argv[] = { "cmdline", "-h" };
        a.exec(out, countof(argv), argv);
        EXPECT_STREQ("Usage: cmdline --test --output=Hello World! [OPTIONS]...\n"
                     "Options: \n"
                     "  -t, --test\tYou must use this option.\n"
                     "  -o, --output\tPrint text.[=Hello World!]\n"
                     "  -h, --help\tPrint usage.\n", buf.c_str());
    }
    {
        const char * const argv[] = { "cmdline", "-o=fdagdag" };
        a.exec(out, countof(argv), argv);
        EXPECT_STREQ("Usage: cmdline --test --output=Hello World! [OPTIONS]...\n"
                     "Options: \n"
                     "  -t, --test\tYou must use this option.\n"
                     "  -o, --output\tPrint text.[=Hello World!]\n"
                     "  -h, --help\tPrint usage.\n", buf.c_str());
    }
    {
        const char * const argv[] = { "cmdline", "-o=fdagdag", "-t" };
        a.exec(out, countof(argv), argv);
        EXPECT_STREQ("fdagdag", buf.c_str());
    }
    {
        const char * const argv[] = { "cmdline", "-t", "-o" };
        a.exec(out, countof(argv), argv);
        EXPECT_STREQ("Hello World!", buf.c_str());
    }
}
