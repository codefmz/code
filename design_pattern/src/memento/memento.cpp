#include "gtest/gtest.h"
#include "memento.h"
#include <memory>
#include <iostream>

class mementoTest : public ::testing::Test {
protected:
    mementoTest() {
    }
    ~mementoTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(mementoTest, test1) {
    std::shared_ptr<Caretaker> caretaker = std::make_shared<Caretaker>();
    std::shared_ptr<Role> role = std::make_shared<Role>(100, 10, 10);
    caretaker->save(role->save());
    role->show();
    role->fight();
    role->show();

    role->recovery(caretaker->getMemento());
    role->show();
}