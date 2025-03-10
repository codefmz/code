#include "flyWight.h"
#include "gtest/gtest.h"

class flyWightTest : public ::testing::Test {
protected:
    flyWightTest() {
    }
    ~flyWightTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(flyWightTest, test1) {
    FlyWightFactory factory;

    auto webSite = factory.getWebSiteCategory("新闻");
    User user1("小明");
    webSite->Use(user1);

    User user2("小红");
    webSite->Use(user2);
}