#ifndef _TIMER_H_
#define _TIMER_H_
#include <map>
#include <stdint.h>

#include "Poller.h"
#include "Event.h"

typedef uint32_t TimerId;
typedef int64_t Timestamp; //ms
typedef uint32_t TimeInterval; //ms

class Timer
{
public:
    ~Timer();

    static Timestamp getCurTime();

private:
    friend class TimerManager;
    Timer(TimerEvent* event, Timestamp timestamp, TimeInterval timeInterval);
    void handleEvent();

private:
    TimerEvent* mTimerEvent;
    Timestamp mTimestamp;
    TimeInterval mTimeInterval;
    bool mRepeat;
};

class TimerManager
{
public:
    static TimerManager* createNew(Poller* poller);

    TimerManager(int timerFd, Poller* poller);
    ~TimerManager();

    TimerId addTimer(TimerEvent* event, Timestamp timestamp, TimeInterval timeInterval);
    bool removeTimer(TimerId timerId);

private:
    void modifyTimeout();
    static void handleRead(void*);
    void handleTimerEvent();

private:
    Poller* mPoller;
    int mTimerFd;
    uint32_t mLastTimerId;
    std::map<TimerId, Timer> mTimers;

    typedef std::pair<Timestamp, TimerId> TimerIndex;
    std::multimap<TimerIndex, Timer> mEvents;
    IOEvent* mTimerIOEvent;
};

#endif //_TIMER_H_