#include "gtest/gtest.h"
#include "observer.h"

class observerTest : public ::testing::Test {
protected:
    observerTest() {
    }
    ~observerTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(observerTest, test1) {
    std::shared_ptr<Subject> sub = std::make_shared<Subject>();
    std::shared_ptr<Observer> obs1 = std::make_shared<StockObserver>("liming", sub);
    sub->attach(obs1);
    std::shared_ptr<Observer> obs2 = std::make_shared<NBAObserver>("wanghua", sub);
    sub->attach(obs2);

    sub->set_state(" boss is coming!");
    sub->notify();
}

