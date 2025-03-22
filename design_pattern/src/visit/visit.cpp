#include "gtest/gtest.h"
#include "visit.h"
#include <memory>
#include <iostream>

class visitTest : public ::testing::Test {
protected:
    visitTest() {
    }
    ~visitTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(visitTest, test1) {
    std::shared_ptr<Action> success = std::make_shared<Success>("成功");
    std::shared_ptr<Action> fail = std::make_shared<Failing>("失败");
    std::shared_ptr<Person> man = std::make_shared<Man>("男人");
    std::shared_ptr<Person> woman = std::make_shared<Woman>("女人");

    man->accept(success.get());
    woman->accept(success.get());
    man->accept(fail.get());
    woman->accept(fail.get());
}