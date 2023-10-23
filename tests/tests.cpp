#include "gtest/gtest.h"
#include "mm/mm.hpp"
#include <cmath>

TEST(MarkovModelTests, PositiveNumbers) {
    mm::MarkovModel<std::string> M(1.0, 0.5, true);
    M.update("<<", "a");
    M.update("<<", "b");
    EXPECT_EQ(M.lpmf("<<", "a"), M.lpmf("<<", "b"));
    EXPECT_EQ(M.lpmf("<<", "a"), std::log(2) - std::log(4.5));
    EXPECT_EQ(M.lpmf("<<", "x"), std::log(0.5) - std::log(4.5));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
