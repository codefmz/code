#include "gtest/gtest.h"
#include "command.h"

class commandTest : public ::testing::Test {
protected:
    commandTest() {
    }
    ~commandTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(commandTest, test1) {
    std::shared_ptr<Barbecuer> boy = std::make_shared<Barbecuer>();
    std::shared_ptr<Command> bakeMuttonCommand = std::make_shared<BakeMuttonCommand>(boy);
    std::shared_ptr<Command> bakeChickenWingCommand = std::make_shared<BakeChickenWingCommand>(boy);
    std::shared_ptr<Command> bakeChickenWingCommand2 = std::make_shared<BakeChickenWingCommand>(boy);

    Waiter waiter;
    waiter.setOrder(bakeChickenWingCommand);
    waiter.setOrder(bakeMuttonCommand);
    waiter.setOrder(bakeChickenWingCommand2);

    waiter.notify();
}
