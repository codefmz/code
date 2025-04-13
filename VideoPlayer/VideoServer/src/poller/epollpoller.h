#ifndef _EPOLLPOLLER_H_
#define _EPOLLPOLLER_H_

#include <sys/epoll.h>
#include <vector>
#include "poller.h"

class EPollPoller : public Poller
{
public:
    static EPollPoller* createNew();

    EPollPoller();
    virtual ~EPollPoller();

    virtual bool addIOEvent(IOEvent* event);
    virtual bool updateIOEvent(IOEvent* event);
    virtual bool removeIOEvent(IOEvent* event);
    virtual void handleEvent();

private:
    int mEPollFd;
    std::vector<struct epoll_event> mEPollEventList;
    std::vector<IOEvent*> mEvents;
};

#endif //_EPOLLPOLLER_H_