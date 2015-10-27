#pragma once

TEST_METHOD(spin_lock_ensure)
{
    using namespace ut_spin_lock_;

    auto sec_1 = do_test<capo::spin_lock>();
    EXPECT_EQ(499950000ull, g_counter);

    auto sec_2 = do_test<std::mutex>();
    EXPECT_EQ(499950000ull, g_counter);

    std::cout << "capo::spin_lock: " << sec_1 << " ms" << std::endl;
    std::cout << "std::mutex:      " << sec_2 << " ms" << std::endl;
}
