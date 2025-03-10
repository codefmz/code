#include "gtest/gtest.h"
#include "factory.h"
#include <iostream>

class factoryTest : public ::testing::Test {
protected:
    factoryTest() : oper(nullptr), factory(nullptr) {
    }
    ~factoryTest() override {
        if (oper != nullptr) {
            delete oper;
            oper = nullptr;
        }

        if (factory != nullptr) {
            delete factory;
            factory = nullptr;
        }
    }
    void SetUp() override {
    }
    void TearDown() override {
    }

    int num1;
    int num2;
    IFactory *factory;
    Operator *oper;
};

TEST_F(factoryTest, calculate) {
    // 如果要改为减法，直接修改factory的实例即可，使用反射可以不用修改源码，实现开闭原则
    factory = new AddFactory();
    oper = factory->createOper();

    num1 = 10;
    num2 = 20;
    EXPECT_EQ(oper->calculate(num1, num2), 30);

    num1 = 40;
    num2 = 40;
    EXPECT_EQ(oper->calculate(num1, num2), 80);

    num1 = 8;
    num2 = 6;
    EXPECT_EQ(oper->calculate(num1, num2), 14);
}
