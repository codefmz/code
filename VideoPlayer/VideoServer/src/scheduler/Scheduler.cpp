#include <sys/eventfd.h>
#include <unistd.h>
#include <stdint.h>

#include "Scheduler.h"
#include "SelectPoller.h"
#include "EpollPoller.h"
#include "Log.h"

static int createEventFd()
{
    int evtFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtFd < 0) {
        LOG_ERR("failed to create event fd\n");
        return -1;
    }

    return evtFd;
}

EventScheduler* EventScheduler::createNew(PollerType type)
{
    if (type >= POLLER_TYPE_BUT) {
        return NULL;
    }


    int evtFd = createEventFd();
    if (evtFd < 0) {
        return NULL;
    }

    return new EventScheduler(type, evtFd);
}

EventScheduler::EventScheduler(PollerType type, int fd) : mQuit(false), mWakeupFd(fd)
{
    switch (type) {
        case POLLER_SELECT:
            mPoller = SelectPoller::createNew();
            break;
        case POLLER_EPOLL:
            mPoller = EPollPoller::createNew();
            break;
    }

    mTimerManager = TimerManager::createNew(mPoller);
    mWakeIOEvent = IOEvent::createNew(mWakeupFd, this);
    mWakeIOEvent->setReadCallback(handleReadCallback);
    mWakeIOEvent->enableReadHandling();
    mPoller->addIOEvent(mWakeIOEvent);
    mMutex = Mutex::createNew();
}

EventScheduler::~EventScheduler()
{
    mPoller->removeIOEvent(mWakeIOEvent);
    ::close(mWakeupFd);

    delete mWakeIOEvent;
    delete mTimerManager;
    delete mPoller;
    delete mMutex;
}

bool EventScheduler::addTriggerEvent(TriggerEvent* event)
{
    mTriggerEvents.push_back(event);
    return true;
}

TimerId EventScheduler::addTimedEventRunAfater(TimerEvent* event, TimeInterval delay)
{
    Timestamp when = Timer::getCurTime();
    when += delay;

    return mTimerManager->addTimer(event, when, 0);
}

TimerId EventScheduler::addTimedEventRunAt(TimerEvent* event, Timestamp when)
{
    return mTimerManager->addTimer(event, when, 0);
}

TimerId EventScheduler::addTimedEventRunEvery(TimerEvent* event, TimeInterval interval)
{
    Timestamp when = Timer::getCurTime();
    when += interval;

    return mTimerManager->addTimer(event, when, interval);
}

bool EventScheduler::removeTimedEvent(TimerId timerId)
{
    return mTimerManager->removeTimer(timerId);
}

bool EventScheduler::addIOEvent(IOEvent* event)
{
    return mPoller->addIOEvent(event);
}

bool EventScheduler::updateIOEvent(IOEvent* event)
{
    return mPoller->updateIOEvent(event);
}

bool EventScheduler::removeIOEvent(IOEvent* event)
{
    return mPoller->removeIOEvent(event);
}

void EventScheduler::loop()
{
    while(mQuit != true) {
        this->handleTriggerEvents();
        mPoller->handleEvent();
        this->handleOtherEvent();
    }
}

void EventScheduler::wakeup()
{
    uint64_t one = 1;
    int ret;
    ret = ::write(mWakeupFd, &one, sizeof(one));
}

void EventScheduler::handleTriggerEvents()
{
    if (!mTriggerEvents.empty()) {
        for(std::vector<TriggerEvent*>::iterator it = mTriggerEvents.begin(); it != mTriggerEvents.end(); ++it) {
            (*it)->handleEvent();
        }

        mTriggerEvents.clear();
    }
}

void EventScheduler::handleReadCallback(void* arg)
{
    if(!arg) {
        return;
    }

    EventScheduler* scheduler = (EventScheduler*)arg;
    scheduler->handleRead();
}

void EventScheduler::handleRead()
{
    uint64_t one;
    while(::read(mWakeupFd, &one, sizeof(one)) > 0);
}

void EventScheduler::runInLocalThread(Callback callBack, void* arg)
{
    MutexLockGuard mutexLockGuard(mMutex);
    mCallBackQueue.push(std::make_pair(callBack, arg));
}

void EventScheduler::handleOtherEvent()
{
    MutexLockGuard mutexLockGuard(mMutex);
    while (!mCallBackQueue.empty()) {
        std::pair<Callback, void*> event = mCallBackQueue.front();
        event.first(event.second);
    }
}