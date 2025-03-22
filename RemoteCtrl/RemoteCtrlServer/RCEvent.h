#ifndef _RC_EVENT_H_
#define _RC_EVENT_H_

#include <boost/pool/singleton_pool.hpp>
#include <winsock2.h>
#include "RCBase.h"

class Client;

typedef enum {
    RC_EVENT_CONNECT,
    RC_EVENT_RECV,
    RC_EVENT_SEND,
    RC_EVENT_ACCEPT,
    RC_EVENT_BUTT
} RC_EVENT_TYPE;

class RCEvent
{
public:
    OVERLAPPED overlapped;
    Client* client;
    RC_EVENT_TYPE type;
    char* packetData;

public:
    OVERLAPPED& GetOverlapped() {
        return overlapped;
    }
    static RCEvent* Create(Client* client, RC_EVENT_TYPE type);
    static void Destroy(RCEvent* event);
};
#endif /* _RC_EVENT_H_ */
