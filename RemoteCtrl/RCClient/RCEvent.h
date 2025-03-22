#ifndef RCEVENT_H
#define RCEVENT_H

#include <boost/pool/singleton_pool.hpp>
#include "Client.h"

typedef enum {
    RC_EVENT_CONNECT,
    RC_EVENT_RECV,
    RC_EVENT_SEND,
} RC_EVENT_TYPE;

class RCEvent
{
public:
    static RCEvent* Create(Client* client, RC_EVENT_TYPE type);
    static void Destroy(RCEvent* event);

public:
    OVERLAPPED overlapped;
    Client* client;
    RC_EVENT_TYPE type;
};

#endif // !RCEVENT_H
