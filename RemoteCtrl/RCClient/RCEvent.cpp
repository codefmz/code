#include "pch.h"
#include "RCEvent.h"

// use thread-safe memory pool
typedef boost::singleton_pool<RCEvent, sizeof(RCEvent)> RCEventPool;

RCEvent* RCEvent::Create(Client* client, RC_EVENT_TYPE type)
{
    RCEvent* event = static_cast<RCEvent*>(RCEventPool::malloc());
    ZeroMemory(event, sizeof(RCEvent));
    event->client = client;
    event->type = type;
    return event;
}

void RCEvent::Destroy(RCEvent* event)
{
    RCEventPool::free(event);
}
