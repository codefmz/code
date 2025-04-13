#ifndef _POLLER_H_
#define _POLLER_H_

#include <map>
#include "event.h"

class Poller
{
public:
    virtual ~Poller();

    virtual bool addIOEvent(IOEvent* event) = 0;
    virtual bool updateIOEvent(IOEvent* event) = 0;
    virtual bool removeIOEvent(IOEvent* event) = 0;
    virtual void handleEvent() = 0;

protected:
    Poller();

protected:
    std::map<int, IOEvent*> mEventMap;
};

#endif //_POLLER_H_