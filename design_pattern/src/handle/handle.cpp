#include "handle.h"
#include "gtest/gtest.h"

class handleTest : public ::testing::Test {
protected:
    handleTest() {
    }
    ~handleTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(handleTest, test1) {
    std::shared_ptr<Handle> zonjingli = std::make_shared<ZongJingLi>();
    std::shared_ptr<Handle> zonjian = std::make_shared<ZonJian>();
    std::shared_ptr<Handle> jingli = std::make_shared<JingLi>();

    Request re(" 小菜请假 ", RequestType::Type1);
    jingli->setNextHandle(zonjian);
    zonjian->setNextHandle(zonjingli);

    EXPECT_TRUE(jingli->handle(re));
    EXPECT_TRUE(zonjian->handle(re));

    Request re2(" 小菜加薪 ", RequestType::Type3);
    EXPECT_TRUE(jingli->handle(re2));

}