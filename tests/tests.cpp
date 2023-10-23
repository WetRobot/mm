#include "gtest/gtest.h"
#include "mm/mm.hpp"
#include <cmath>

TEST(MarkovModelTests, PositiveNumbers) {
    mm::MarkovModel<std::string> M(1.0, 1.0, true);
    M.update("<<", "a");
    M.update("<<", "b");
    EXPECT_EQ(M.lpmf("<<", "a"), M.lpmf("<<", "b"));
    EXPECT_EQ(M.lpmf("<<", "a"), std::log(1) - std::log(3));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
