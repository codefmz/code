#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_
#include <queue>
#include <vector>

#include <atomic>
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

class Task
{
public:
    typedef void (*TaskCallback)(void*);
    Task() { };

    void setTaskCallback(TaskCallback cb, void* arg) {
        mTaskCallback = cb;
        mArg = arg;
    }

    void handle() {
        if(mTaskCallback) {
            mTaskCallback(mArg);
        }
    }

    bool operator=(const Task& task) {
        this->mTaskCallback = task.mTaskCallback;
        this->mArg = task.mArg;
    }

private:
    void (*mTaskCallback)(void*);
    void* mArg;
};

class ThreadPool
{
public:
    static ThreadPool* createNew(int num);

    ThreadPool(int num);
    ~ThreadPool();

    void addTask(Task& task);

    class MThread : public Thread
    {
    protected:
        virtual void run(void *arg);
    };
private:
    void createThreads();
    void cancelThreads();
    void handleTask();

private:
    std::queue<Task> mTaskQueue;
    Mutex* mMutex;
    Mutex* mCondMutex;
    Condition* mCondition;
    std::vector<MThread> mThreads;
    std::atomic<bool> mQuit;
};

#endif //_THREADPOOL_H_