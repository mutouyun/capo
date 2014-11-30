// UT_main.cpp : Defines the entry point for the console application.
//

#include <cstdlib>

#include "gtest/gtest.h"

////////////////////////////////////////////////////////////////

class AllTestEnvironment : public testing::Environment
{
public:
    virtual void SetUp(void)
    {
        /*Do nothing*/
    }
    virtual void TearDown(void)
    {
#if defined(_MSC_VER)
        std::system("pause");
#endif
    }
};

////////////////////////////////////////////////////////////////

GTEST_API_ int main(int argc, char* argv[])
{
    testing::AddGlobalTestEnvironment(new AllTestEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
