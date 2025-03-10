#include "gtest/gtest.h"
#include <iostream>
#include "mediator.h"

class mediatorTest : public ::testing::Test {
protected:
    mediatorTest() {
    }
    ~mediatorTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(mediatorTest, test1) {
    std::shared_ptr<UnitedNations> mediator = std::make_shared<UnitedNationsSecurityCouncil>();
    std::shared_ptr<Country> usa = std::make_shared<USA>(mediator);
    std::shared_ptr<Country> iraq = std::make_shared<Iraq>(mediator);

    mediator->setIraq(iraq);
    mediator->setUsa(usa);
    mediator->declare("不准研制核武器，否则要发动战争！", usa->getName());
    mediator->declare("我们没有核武器！", iraq->getName());
}