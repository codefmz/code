#include "gtest/gtest.h"
#include <log.h>

class logTest : public ::testing::Test {
public:
    logTest() {
    }
    ~logTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST(logTest, logTest1)
{
    LOG_ERR("logTest1");
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}