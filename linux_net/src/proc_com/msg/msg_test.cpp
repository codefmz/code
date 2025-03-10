#include "gtest/gtest.h"

class msgTest : public ::testing::Test {
public:
    msgTest() {
        std::cout << "msgTest" << std::endl;
    }
    ~msgTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(msgTest, test1) {
    printf("hello world!");

    EXPECT_EQ(1, 1);
}