#include "gtest/gtest.h"
#include <iostream>
#include "prototype.h"

class prototypeTest : public ::testing::Test {
protected:
    prototypeTest() {
    }
    ~prototypeTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(prototypeTest, test1) {
    std::shared_ptr<Shape> shape1 = std::make_shared<Circle>();
    shape1->draw();

    std::shared_ptr<Shape> shape2 = shape1->clone();
    shape2->draw();
}