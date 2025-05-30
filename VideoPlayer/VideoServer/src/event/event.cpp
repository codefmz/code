#include <stdio.h>
#include "Event.h"

TriggerEvent* TriggerEvent::createNew(void* arg)
{
    return new TriggerEvent(arg);
}

TriggerEvent::TriggerEvent(void* arg) : mArg(arg) { }

void TriggerEvent::handleEvent()
{
    if (mTriggerCallback) {
        mTriggerCallback(mArg);
    }
}

TimerEvent* TimerEvent::createNew(void* arg)
{
    return new TimerEvent(arg);
}

TimerEvent::TimerEvent(void* arg) : mArg(arg) { }

void TimerEvent::handleEvent()
{
    if (mTimeoutCallback) {
        mTimeoutCallback(mArg);
    }
}

IOEvent* IOEvent::createNew(int fd, void* arg)
{
    if(fd < 0) {
        return nullptr;
    }

    return new IOEvent(fd, arg);
}

IOEvent::IOEvent(int fd, void* arg) : mFd(fd), mArg(arg),
    mEvent(EVENT_NONE), mREvent(EVENT_NONE), mReadCallback(NULL),
    mWriteCallback(NULL), mErrorCallback(NULL)
{

}

void IOEvent::handleEvent()
{
    if (mReadCallback && (mREvent & EVENT_READ)) {
        mReadCallback(mArg);
    }

    if (mWriteCallback && (mREvent & EVENT_WRITE)) {
        mWriteCallback(mArg);
    }

    if (mErrorCallback && (mREvent & EVENT_ERROR)) {
        mErrorCallback(mArg);
    }
};
