#include "gtest/gtest.h"
#include "ngm/ngm.hpp"

TEST(NgramModelTests, PositiveNumbers) {
    ngram::NgramModel M(3, 1.0, 1.0, true);
    M.update(std::string("<<a>"));
    M.update(std::string("<<b>"));
    std::vector<double> lp(10);
    std::vector<std::string> x(10);
    for (int i = 0; i < x.size(); i++) {
        x[i] = "<<aaaaaaaa>";
    }
    M.lpmf(x, lp, 2);
    EXPECT_EQ(lp[0], lp[1]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
