
#include "gtest/gtest.h"

#include "capo/stopwatch.hpp"

#include <iostream>
#include <iomanip>
#include <thread>

////////////////////////////////////////////////////////////////

#ifndef STOPWATCH_CHECKER__
#define STOPWATCH_CHECKER__(OP) \
    std::cout << "stopwatch ->: " << #OP << std::endl; \
    sw.OP(); \
    for(int i = 0; i < 5; ++i) \
    { \
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); \
        std::cout << "stopwatch ->: " \
                  << sw.elapsed<std::chrono::milliseconds>() / 1000.0 \
                  << "  " \
                  << sw.elapsed<std::chrono::milliseconds, 1>() / 1000.0 \
                  << "  " \
                  << sw.elapsed<std::chrono::milliseconds, 2>() / 1000.0 \
                  << std::endl; \
    }
#endif/*STOPWATCH_CHECKER__*/

#define TEST_METHOD(TEST_NAME) TEST(stopwatch, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(stopwatch)
{
    std::cout.setf(std::ios::fixed);
    std::cout << std::setiosflags(std::ios_base::showpoint);

    capo::stopwatch<3> sw; // Record 3 groups of time

    STOPWATCH_CHECKER__(start)
    STOPWATCH_CHECKER__(pause)
    STOPWATCH_CHECKER__(start)
    STOPWATCH_CHECKER__(pause<1>)
    STOPWATCH_CHECKER__(restart<1>)
    STOPWATCH_CHECKER__(stop)
    STOPWATCH_CHECKER__(start)
    STOPWATCH_CHECKER__(start)

    std::cout << std::resetiosflags(std::ios_base::showpoint);
    std::cout.unsetf(std::ios::fixed);
}

////////////////////////////////////////////////////////////////

#undef STOPWATCH_CHECKER__
