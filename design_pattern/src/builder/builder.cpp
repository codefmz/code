#include "gtest/gtest.h"
#include "builder.h"
#include <memory>
#include <iostream>

class builderTest : public ::testing::Test {
protected:
    builderTest() {
    }
    ~builderTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(builderTest, test1) {
    Director director;

    std::shared_ptr<PersonBuilder> p1 = std::make_shared<ThinPersonBuilder>();
    director.setBuilder(p1);
    director.construct();

    std::shared_ptr<PersonBuilder> p2 = std::make_shared<FatPersonBuilder>();
    director.setBuilder(p2);
    director.construct();
}