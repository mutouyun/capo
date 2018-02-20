#pragma once

#include "capo/singleton.hpp"
#include "capo/countof.hpp"

#include <thread>
#include <chrono>
#include <future>

namespace ut_singleton_ {

class XXX
{
public:
    XXX(void)
    {
        printf("XXX create\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    XXX(int x)
    {
        printf("XXX create with int: %d\n", x);
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
        printf("XXX destory\n");
    }
};


} // namespace ut_singleton_