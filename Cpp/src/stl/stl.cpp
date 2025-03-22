#include <string>
#include <algorithm>
#include <functional>
#include "gtest/gtest.h"

using namespace std;

class stlTest : public ::testing::Test {
protected:
    stlTest() {
    }
    ~stlTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }

    std::string str = "the more handsome";
};

TEST_F(stlTest, stlTest1) {
    // size_t sz = 4;

    // cout << wc << endl;
    // EXPECT_EQ(wc, 1);
}