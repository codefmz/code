#include "gtest/gtest.h"
#include "threadpool.h"

class threadTest : public ::testing::Test {
public:
    threadTest() {
    }
    ~threadTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

void testThread1(void *arg) {
    std::cout << " void testThread1 = testThread1 arg = " << (uintptr_t)arg << std::endl;
}

TEST_F(threadTest, threadTest1)
{
    ThreadPool *pool = ThreadPool::createNew(100);
    Task task;
    for (int i = 0; i < 100; i++) {
        task.setTaskCallback(testThread1, (void *)(uintptr_t)i);
        pool->addTask(task);
    }

    delete pool;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}