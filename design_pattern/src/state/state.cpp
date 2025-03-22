#include "gtest/gtest.h"
#include <iostream>
#include "state.h"

Work::Work(): hour(0), state(nullptr), finished(false) {
    state = std::make_shared<ForenoonState>();
    hour = 9;
}

void Work::writeProgram()
{
    state->writeProgram(this);
}

class stateTest : public ::testing::Test {
protected:
    stateTest() {
    }
    ~stateTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(stateTest, test1) {
    std::shared_ptr<Work> work = std::make_shared<Work>();
    work->writeProgram();

    work->hour = 12;
    work->writeProgram();

    work->hour = 15;
    work->writeProgram();

    work->hour = 18;
    work->writeProgram();

    work->finished = true;
    work->hour = 20;
    work->writeProgram();
}