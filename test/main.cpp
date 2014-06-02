#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif/*_MSC_VER*/

#include <iostream>

////////////////////////////////////////////////////////////////

#include "../src/macro/macro_func.hpp"

#define TEST_CASE() \
    std::cout << std::endl \
              << "----------------------------------" << std::endl \
              << CAPO_FUNC_ << " ->: " << std::endl \
              << "----------------------------------" << std::endl

////////////////////////////////////////////////////////////////

#include "test_type.h"
#include "test_utility.h"
#include "test_algorithm.h"
#include "test_proofing.h"
#include "test_thread.h"

////////////////////////////////////////////////////////////////

int main(void)
{
    test_type();
    //test_utility();
    //test_algorithm();
    //test_proofing();
    //test_thread();
    system("pause");
    return 0;
}
