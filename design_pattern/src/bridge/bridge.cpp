#include "gtest/gtest.h"
#include "bridge.h"

class bridgeTest : public ::testing::Test {
protected:
    bridgeTest() {
    }
    ~bridgeTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(bridgeTest, test1) {
    auto dang = std::make_shared<DangDang>();
    std::shared_ptr<Message> message = std::make_shared<MessageLite>(dang);

    message->normal();
    message->special();

    std::shared_ptr<Message> message1 = std::make_shared<MessagePrefect>(dang);

    message1->normal();
    message1->special();
}