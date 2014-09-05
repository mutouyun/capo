#ifndef TEST_PROOFING_H
#define TEST_PROOFING_H

////////////////////////////////////////////////////////////////

#include "../include/proofing/printf.hpp"

void test_printf(void)
{
    TEST_CASE();

    char c = 'A';
    capo::printf("1234567%s%c\n", " ", c);
}

////////////////////////////////////////////////////////////////

#include "../include/proofing/assert.hpp"

void test_assert(void)
{
    TEST_CASE();

    bool b = true;
    int i = 1, j = 2;
    char what[] = "What's up";

    try
    {
        capo_assert(b = false)(i)(j)(what).msg("blabla").except(b);
        std::cout << b << std::endl;
    }
    catch(const capo::assert::context&)
    {
        std::cout << "catched assert::context!" << std::endl;
    }
    catch(bool b)
    {
        std::cout << "catched bool: " << b << std::endl;
    }
    catch(...)
    {
        std::cout << "What?!" << std::endl;
    }
}

////////////////////////////////////////////////////////////////

void test_proofing(void)
{
    //test_printf();
    test_assert();
}

////////////////////////////////////////////////////////////////

#endif // TEST_PROOFING_H
