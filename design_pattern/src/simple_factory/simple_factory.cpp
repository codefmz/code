#include "gtest/gtest.h"
#include <iostream>
#include "simple_factory.h"

class simple_factory : public ::testing::Test {
protected:
    simple_factory() {
        num1 = 100;
        num2 = 10;
        oper = nullptr;
    }
    ~simple_factory() override {
        if (oper != nullptr) {
            delete oper;
            oper = nullptr;
        }
    }
    void SetUp() override {
    }
    void TearDown() override {
    }

    int calcuate(char name) {
        oper = OperFactory::CreateOper(name);
        int result = oper->calculate(num1, num2);
        delete oper;
        oper = nullptr;
        return result;
    }

    int num1;
    int num2;
    Operator *oper;
};


TEST_F(simple_factory, add) {
    EXPECT_EQ(calcuate('+'), 110);
    num1 = 10;
    num2 = 20;
    EXPECT_EQ(calcuate('+'), 30);
    num1 = 40;
    num2 = 20;
    EXPECT_EQ(calcuate('+'), 60);
}

TEST_F(simple_factory, sub) {
    EXPECT_EQ(calcuate('-'), 90);
}

TEST_F(simple_factory, multi) {
    EXPECT_EQ(calcuate('*'), 1000);
}

TEST_F(simple_factory, div) {
    EXPECT_EQ(calcuate('/'), 10);
}

// 增加一个求余运算而不用改客户端，也就是上面计算函数的代码
TEST_F(simple_factory, mod) {
    EXPECT_EQ(calcuate('%'), 0);
}