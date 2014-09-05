#ifndef TEST_CHRONO_H
#define TEST_CHRONO_H

////////////////////////////////////////////////////////////////

#include "../include/chrono/stopwatch.hpp"
#include <thread>
#include <iomanip>

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

void test_stopwatch(void)
{
    TEST_CASE();

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

#undef STOPWATCH_CHECKER__

////////////////////////////////////////////////////////////////

void test_chrono(void)
{
    test_stopwatch();
}

////////////////////////////////////////////////////////////////

#endif // TEST_CHRONO_H
