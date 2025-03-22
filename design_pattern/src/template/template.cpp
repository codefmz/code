#include "gtest/gtest.h"
#include "template.h"
#include <memory>
#include <iostream>

class templateTest : public ::testing::Test {
protected:
    templateTest() {
    }
    ~templateTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(templateTest, test1) {
    std::shared_ptr<AbstractTemplate> t1 = std::make_shared<Template>();
    t1->run();
}