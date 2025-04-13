#include "gtest/gtest.h"
#include "timer.h"
#include "epollpoller.h"

class timerTest : public ::testing::Test {
public:
    timerTest() {
    }
    ~timerTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

void testThread1(void *arg) {
    static int i = 0;
    std::cout << " void testThread1 = testThread1 curTimer = " << Timer::getCurTime() << " i = " << i++ << std::endl;
}

TEST_F(timerTest, timerTest1)
{
    Poller* poller = EPollPoller::createNew();
    TimerManager* timerManager = TimerManager::createNew(poller);
    TimerEvent *timerEvent = TimerEvent::createNew(nullptr);
    timerEvent->setTimeoutCallback(testThread1);

    timerManager->addTimer(timerEvent, Timer::getCurTime(), 500);
    while (true) {
        poller->handleEvent();
    }


    delete poller;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}