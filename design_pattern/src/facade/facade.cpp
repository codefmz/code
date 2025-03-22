#include "gtest/gtest.h"
#include "facade.h"
#include <memory>
#include <iostream>

class facadeTest : public ::testing::Test {
protected:
    facadeTest() {
    }
    ~facadeTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(facadeTest, test1) {
    std::shared_ptr<Facade> t1 = std::make_shared<Facade>();

    t1->MethodA();
    std::cout<< "======== " << std::endl;
    t1->MethodB();
}