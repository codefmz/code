#include "threadpool.h"
#include "log.h"

ThreadPool* ThreadPool::createNew(int num)
{
    return new ThreadPool(num);
}

ThreadPool::ThreadPool(int num) : mThreads(num), mQuit(false)
{
    mMutex = Mutex::createNew();
    mCondMutex = Mutex::createNew();
    mCondition = Condition::createNew();

    createThreads();
}

ThreadPool::~ThreadPool()
{
    cancelThreads();
    delete mMutex;
    delete mCondition;
}

void ThreadPool::addTask(Task& task)
{
    MutexLockGuard mutexLockGuard(mMutex);
    mTaskQueue.push(task);
    mCondition->signal();
}

void ThreadPool::handleTask()
{
    while(mQuit != true) {
        Task task;
        {
            MutexLockGuard mutexLockGuard(mMutex);
            while (mTaskQueue.empty() && mQuit == false) {
                mCondition->wait(mMutex);
            }

            if (mQuit == true) {
                break;
            }

            if (mTaskQueue.empty()) {
                continue;
            }

            task = mTaskQueue.front();
            mTaskQueue.pop();
        }
        task.handle();
    }
}

void ThreadPool::createThreads()
{
    for(std::vector<MThread>::iterator it = mThreads.begin(); it != mThreads.end(); ++it) {
        (*it).start(this);
    }
}

void ThreadPool::cancelThreads()
{
    while (true) {
        MutexLockGuard mutexLockGuard(mMutex);
        if (mTaskQueue.empty()) {
            mQuit = true;
            mCondition->broadcast();
            break;
        }
    }
    for(std::vector<MThread>::iterator it = mThreads.begin(); it != mThreads.end(); ++it) {
        (*it).join();
    }

    mThreads.clear();
}

void ThreadPool::MThread::run(void* arg)
{
    ThreadPool* threadPool = (ThreadPool*)arg;
    threadPool->handleTask();
}