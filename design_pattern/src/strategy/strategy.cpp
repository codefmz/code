#include "strategy.h"
#include "gtest/gtest.h"

class strategy : public ::testing::Test {
protected:
    strategy() {
    }
    ~strategy() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }

    void calcuate(StrategyType type) {
        content = new CashContent(type);
        std::cout << " type: " << type << " money: " << money
                  << " result: " << content->getResult(money) << std::endl;
    }

    CashContent *content;
    double money;
};
TEST_F(strategy, normal) {
    money = 100;
    calcuate(CASH_NORMAL);
}

TEST_F(strategy, rebate) {
    money = 100;
    calcuate(CASH_REBATE);
}

TEST_F(strategy, return) {
    money = 700;
    calcuate(CASH_RETURN);
}
